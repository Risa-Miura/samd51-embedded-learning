"""Check built HTML links, assets, and fragments without network access.

Run from the repository root after `python -m mkdocs build --strict --clean`.
"""
from html.parser import HTMLParser
from pathlib import Path
import sys
from urllib.parse import unquote, urlsplit

ROOT = Path(__file__).resolve().parents[1]
SITE = ROOT / "site"
BASE = "/samd51-embedded-learning/"


class Page(HTMLParser):
    def __init__(self, source):
        super().__init__(convert_charrefs=True)
        self.ids = set()
        self.links = []
        self.feed(source)

    def handle_starttag(self, tag, attributes):
        attrs = dict(attributes)
        if "id" in attrs:
            self.ids.add(attrs["id"])
        if tag == "a" and attrs.get("name"):
            self.ids.add(attrs["name"])
        key = "href" if tag in ("a", "link") else "src" if tag in ("img", "script", "iframe") else None
        if key and attrs.get(key):
            self.links.append(attrs[key])


def main():
    if not (SITE / "index.html").is_file():
        sys.exit("Build the site first: python -m mkdocs build --strict --clean")
    pages = {p.resolve(): Page(p.read_text(encoding="utf-8")) for p in SITE.rglob("*.html")}
    failures = []
    checked = 0
    for source, page in pages.items():
        for link in page.links:
            url = urlsplit(link)
            if url.scheme or url.netloc:
                continue
            path = unquote(url.path)
            if path.startswith(BASE):
                target = SITE / path[len(BASE):]
            elif path.startswith("/"):
                target = SITE / path.lstrip("/")
            else:
                target = source.parent / path if path else source
            target = target.resolve()
            if not target.is_relative_to(SITE.resolve()):
                failures.append((source, link, "escapes site directory"))
                continue
            if target.is_dir():
                target /= "index.html"
            # Material's 404 page uses location-dependent relative assets; the
            # template renders them against the site's base URL. Normal pages
            # and their fragment links are still checked here.
            if not target.is_file():
                failures.append((source, link, "missing file"))
            elif url.fragment and target in pages and unquote(url.fragment) not in pages[target].ids:
                failures.append((source, link, "missing anchor"))
            checked += 1
    for source, link, reason in failures:
        print(f"{source.relative_to(SITE)}: {reason}: {link}")
    if failures:
        sys.exit(f"FAIL: {len(failures)} broken internal links")
    print(f"PASS: {len(pages)} HTML pages; {checked} local links/assets/fragments checked.")


if __name__ == "__main__":
    main()
