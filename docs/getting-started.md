# Set up the workbench

## Clone and open in VS Code

Install [Python 3.13](https://www.python.org/downloads/), [Git](https://git-scm.com/downloads), and [VS Code](https://code.visualstudio.com/). Run from a terminal:

```sh
git clone https://github.com/Risa-Miura/samd51-embedded-learning.git
cd samd51-embedded-learning
code .
python -m venv .venv
```

On Windows, `py -3.13 -m venv .venv` selects Python 3.13 explicitly. If working from the delivered local folder, open that folder and skip cloning.

## Install and preview

On Windows PowerShell, activation is optional:

```powershell
.\.venv\Scripts\python.exe -m pip install -r requirements-lock.txt
.\.venv\Scripts\python.exe -m mkdocs serve
```

On macOS or Linux:

```sh
.venv/bin/python -m pip install -r requirements-lock.txt
.venv/bin/python -m mkdocs serve
```

Open [http://127.0.0.1:8000](http://127.0.0.1:8000). Edit Markdown under `docs/`; the preview refreshes when saved. Stop with Ctrl+C. From VS Code, choose **Python: Select Interpreter**, select this environment, then use **Terminal → Run Task → Docs: preview**.

## Validate before pushing

From the repository root, use the local environment explicitly. On Windows PowerShell:

```powershell
.\.venv\Scripts\python.exe -m mkdocs build --strict --clean
.\.venv\Scripts\python.exe scripts/check_links.py
```

On macOS or Linux:

```sh
.venv/bin/python -m mkdocs build --strict --clean
.venv/bin/python scripts/check_links.py
```

Then review and commit the intended changes:

```sh
git status
git add docs/
git commit -m "Document my first Microchip University lesson"
git push origin main
```

These commands work without activating the terminal. The build fails on documentation warnings. The link check inspects generated local files and fragment targets; it does not check external sites.

## If Python says "No module named mkdocs"

That error means the Python running the command does not have the documentation dependencies installed. Selecting an interpreter in VS Code does not necessarily change an already-open terminal. An Anaconda terminal may still run Anaconda's Python when you type `python`.

From the repository root in Windows PowerShell:

```powershell
python -m venv .venv
.\.venv\Scripts\python.exe -m pip install -r requirements-lock.txt
.\.venv\Scripts\python.exe -m mkdocs build --strict --clean
```

If `.venv` already exists, skip the first command. Use that same explicit Python path for installation and the build. You do not need to change PowerShell's execution policy or install MkDocs into your global/Anaconda environment. For the VS Code editor, select `.venv\Scripts\python.exe` with **Python: Select Interpreter**. The repository's **Docs** tasks invoke the local environment directly.

## Next steps

- [Write the first course entry](contributing.md#add-the-first-microchip-university-lesson).
- [Compile and upload LED blink](projects/led-blink.md).
- [Add another project or image](contributing.md).
- [Open the optional analysis notebook](projects/dma-benchmark.md#optional-python-analysis).
- [Configure Pages and inspect deployments](publishing.md).
