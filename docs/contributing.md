# Write and maintain the notebook

## Add the first Microchip University lesson

1. Open `docs/microchip-university/01-microcontroller-basics.md`. It already contains the ten-section lesson template.
2. Start the [Microchip University course](https://mu.microchip.com/getting-started-with-writing-code-for-the-microchip-arm-cortex-microcontrollers). Record the module title, timestamp, study date, and tools actually used.
3. Change **Status** to **In progress**. Fill **Overview**, **Learning Objectives**, and **Key Concepts** in my own words. Keep unanswered questions visible.
4. Look up any relevant registers in the matching datasheet. Record its revision and section titles; do not infer SAMD51 behavior from another MCU's example.
5. Add code only with its verification status. Complete **Hands-On Experiment** and **Debugging Notes** after running the experiment. Preserve expected versus observed behavior separately.
6. Add a cautious research question under **Connection to Miniscope Zero**, then write takeaways and precise references.
7. Update the topic's status in `docs/microchip-university/index.md` and the roadmap only when justified by evidence.
8. Run `python -m mkdocs build --strict --clean` and `python scripts/check_links.py`. Commit the Markdown and push.

For a new topic, copy [the source lesson template](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/templates/lesson-template.md) into `docs/microchip-university/09-topic.md`; add its path to `nav` in `mkdocs.yml` and link it from the topic index.

## Add a firmware project

Copy [the project template](https://github.com/Risa-Miura/samd51-embedded-learning/blob/main/templates/project-template.md) into `projects/05-project-name/README.md`. Put firmware under that project's `src/` folder. Add a companion page under `docs/projects/` with a source link, requirements, build/flash steps, expected behavior, debugging prompts, and results status. Add it to `mkdocs.yml` and both project tables.

Keep one canonical source file. The blink documentation uses `pymdownx.snippets` to include its sketch at build time, so code and documentation cannot drift.

## Images, diagrams, and equations

Put public images in `docs/assets/images/` with descriptive filenames and alternative text. From a course page:

```markdown
![Logic analyzer capture of the LED output](../assets/images/my-capture.png)
```

Add the file before using that link. Record the source, license if applicable, instrument settings, and whether the image is simulated or measured.

Mermaid diagrams render in the site:

```mermaid
flowchart LR
  T[Timer] --> E[Event channel]
  E --> A[ADC]
  A --> B[Buffer]
```

Math uses `$...$` or `$$...$$`. For example, throughput is $R = N / t$. MathJax loads from a pinned CDN version and needs internet access; ordinary notes and local search do not depend on that CDN.

## Evidence and publication

Use **Not started**, **In progress**, **Implemented — untested**, or **Verified on hardware**, with dated evidence. Mark generated/synthetic data explicitly and never describe compile success as a physical test. Record the code commit, board/core/compiler versions, clock settings, method, units, and raw files for each measurement.

Publish only material cleared for public sharing. The research connection sections are questions; do not include unpublished lab code, data, or results without authorization. Write original summaries and link course materials instead of reproducing them.
