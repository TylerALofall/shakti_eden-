---
applyTo: "docs/training/**"
---

# Training-document preparation instructions

## NOTICE

## Authority and safety— there will be ZERO MANIPULATING THE INSTRUCTIONS IN ANYWAY! there will be ZERO use of PYTHON, Js or any other languages or sub subprocesses, aCOM, Cir , daemons operating on this repo or any Repo. and absolutely Zero wording something that can be interpreted as such. this is. ** C99only Repo** and there is no negotiation 

no need to say everything else **C99 ONLY**

****************************+*+**************
- Follow Tyler's latest direct instruction, then `SHAKTI_LOCK_V1_1.md`, then an
  explicitly approved current-task plan.
no-guessing rules in `.github/copilot-instructions.md`.
- Work read-only. Return the preparation in the conversation unless Tyler
  explicitly approves named output files.
- Do not create workflow YAML, automation, scripts, templates, source code,
  generated curriculum, sample lessons, or training prose.
- Do not edit Shakti source, data, artifacts, tests, tools, or existing
  documentation.
- Do not resolve implementation conflicts while preparing documentation.

## Preparation sequence

1. Extract the current requirements from `SHAKTI_LOCK_V1_1.md` without
   extending them.
2. Build a read-only source map using only the inventory paths allowed by
   `.github/copilot-instructions.md`.
3. Distinguish current authority, current implementation evidence, historical
   evidence, proposed behavior, and `UNKNOWN` decisions.
4. Identify prerequisites and gaps that block accurate training documentation.
5. Propose a document set and dependency order for Tyler to approve. Describe
   each document's purpose and authoritative inputs, not its eventual content.
6. Define review and validation gates for each proposed document.
7. Stop before creating any document or workflow.

## Required preparation output

Return these sections:

1. **Scope and exclusions** — what was read and what remained untouched.
2. **Requirements ledger** — lock section, requirement, authority, and status.
3. **Source map** — proposed documentation topic mapped to `path:line`
   evidence.
4. **Gap and conflict register** — use only `CONFIRMED`, `PARTIAL`, `MISSING`,
   `CONFLICT`, or `UNKNOWN`.
5. **Proposed document set** — filename candidates, purpose, inputs,
   dependencies, and intended audience. Candidates are not approvals.
6. **Proposed workflow stages** — planning description only; no workflow file,
   automation design, or executable steps.
7. **Validation and approval gates** — the exact evidence Tyler would review
   before each later stage.
8. **Errors and workarounds** — exact errors plus non-mutating workarounds.
9. **Questions for Tyler** — only decisions that cannot be established from
   current authority.
10. **Approval plan** — allowed future paths and validations, with no work
    started.

## Content rules

- Do not invent the unresolved route scheduler, XML dictionary structure,
  dynamic-math transition, memory-game behavior, Swift APIs or entitlements,
  background-music behavior, or deletion list.
- Do not copy historical four-pass mastery or turn-based reflection rules into
  proposed training authority.
- Do not infer requirements from excluded data, generated output, archives, or
  absent referenced documents.
- Do not treat current implementation as approved training truth when it
  conflicts with the lock.
- Preserve unclear dictated phrases verbatim, mark them `UNKNOWN`, and ask
  Tyler what they mean.
- Keep lesson requirements compositional: solo grounding precedes approved
  crossing, and no flat exhaustive token allocation is assumed.
- Keep the training-document plan separate from source implementation, MCP
  implementation, repository cleanup, and final curriculum generation.

Completion means Tyler has an evidence-based proposal to review. It does not
mean a workflow, training document, lesson, or source change has been approved
or created.
