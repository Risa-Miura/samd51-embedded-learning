# Publish with GitHub Pages

## Repository and website

- Repository: [https://github.com/Risa-Miura/samd51-embedded-learning](https://github.com/Risa-Miura/samd51-embedded-learning)
- Configured website address: [risa-miura.github.io/samd51-embedded-learning](https://risa-miura.github.io/samd51-embedded-learning/)

A configured address alone does not establish deployment success. Check the Actions run and visit the URL after the initial deployment.

## First publication

If this repository has not yet been published, create a **public** repository named `samd51-embedded-learning` under `Risa-Miura`. Leave it empty (no generated README, license, or gitignore) to preserve this project's history. Do not replace an unrelated existing repository.

From the delivered repository folder:

```sh
git remote -v
git remote add origin https://github.com/Risa-Miura/samd51-embedded-learning.git
git push -u origin main
```

Add `origin` only if absent; if it already matches, just push. The delivered folder has its own Git history. For a fresh source ZIP without `.git`, run `git init -b main`, `git add .`, and `git commit -m "Create SAMD51 learning journal"` before adding the remote.

In GitHub, open **Settings → Pages → Build and deployment → Source → GitHub Actions**. In **Actions**, run **Build and deploy documentation** using **Run workflow** if the initial push ran before Pages was enabled. The `github-pages` environment may require approval if repository rules demand it.

## What the workflow does

Every push to `main` and every pull request targeting `main` builds with locked documentation dependencies on Python 3.13, treats MkDocs warnings as errors, and checks all rendered local links and anchor targets. Pull requests build only; main pushes and manual runs upload a Pages artifact and deploy with `actions/deploy-pages`.

The deployment job has `pages: write` and `id-token: write`. Other steps have read-only repository access. No personal access token is required in repository secrets. The implementation follows [GitHub's custom Pages workflow guidance](https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages).

## Verify and troubleshoot

1. In **Actions**, open the newest run and confirm both `build` and `deploy` succeed.
2. Open the URL shown by the deployment; test navigation, search, a code copy button, and light/dark mode.
3. For build failures, inspect the named file/line in the strict-build or link-check output. Reproduce the same commands locally.
4. For Pages configuration errors, confirm **Source: GitHub Actions**, repository visibility, environment rules, and Actions permissions.

Public repositories can use GitHub Pages with GitHub Free, subject to [GitHub's current plan and usage limits](https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages). This project needs no paid domain or external hosting service.

## Move or rename the repository

Update `site_url`, `repo_url`, `repo_name`, README links, and source links in the documentation together. Keep the path suffix in `site_url` for a project Pages site. Rebuild and check links before deploying.
