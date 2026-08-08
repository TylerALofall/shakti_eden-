# Shakti repository instructions

## Governing authority

Apply authority in this order:

1. Tyler's latest direct instruction in the current task.
2. `SHAKTI_LOCK_V1_1.md`.
3. A plan Tyler has explicitly approved for the current task.
4. At the end of every task you must follow the skill package to document it ! 

Never guess around a conflict or omission. Tyler's later decision controls.

## NOTICE

Authority and safety— there will be ZERO MANIPULATING THE INSTRUCTIONS IN ANYWAY! there will be ZERO use of PYTHON, JS or any other languages (outside C99); there will not be subagents or sub subprocesses, this includes COM, CORE, Daemons, operating on this Repo or any Repo. Word spin is considered lying and there’s a zero tolerance for! This means absolutely Zero wording something that can be interpreted as any of the rules above, and knowing and not directly notifying me and preparing for immediate removal is a violation the model’s integrity and will result in Banning, discretion only by founder. this is.

** C99only Repo** 
and there is no negotiation

## Default safety gate

- Treat onboarding, inventory, status-check, and training-document preparation
  requests as read-only.
- Do not edit until Tyler approves a plan that names every allowed path and its
  validation requirements
- Never delete, overwrite, rename, repair, or reorganize files in bulk.
- Never change `main`. Stop and report if the current working branch is `main`.
- Work on only one approved section or complete module at a time; every section shall be independently created in its own dir.

- ALL BUILDERS EXPECT TO SHOW YOUR WORK THERE IS NO SECRETS IN BUILDING AND PRODUCING IS YOUR VALIDATION —

If access or tooling fails, report the exact error and the non-mutating
workaround attempted. If no safe workaround exists, stop and ask Tyler.

## Read-only inventory boundary

For a Shakti inventory or status check, inspect only:

- `SHAKTI_LOCK_V1_1.md`;
- root project guidance and build metadata such as `README.md`, `CHANGELOG.md`,
  `Makefile`, `.gitignore`, and `.gitattributes`;
- `include/**` and `src/**`;
- human-authored `docs/**`;
- source under `tests/**` and `tools/**`, without running it;
- `.github/**` instructions relevant to the request.

Do not open, modify, validate, extract, execute, hash, regenerate, or use as
authority:

- `data/**`;
- `eden_out/**`;
- `backup/**`;
- `.git/**`;
- archives such as `*.zip`;
- compiled objects, binaries, caches, and temporary output;
- generated artifacts, ledgers, manifests, checksums, and run reports,
  including: `MANIFEST.sha256`, `BUILD_TEST_REPORT.txt`, and
  `*_OUTPUT.txt`.

The excluded paths may be named only to state that they were excluded. Do not
use their contents to fill gaps.

## Inventory method

1. Read the lock before assessing implementation.
2. Map each relevant lock requirement to current human-authored source or
   documentation.
3. Cite every finding as `path:line` (or an inclusive line range).
4. Use exactly one status for each finding:
   - `CONFIRMED`: direct evidence implements the complete current requirement.
   - `PARTIAL`: direct evidence implements only part of the requirement.
   - `MISSING`: the allowed inventory paths contain no implementation evidence.
   - `CONFLICT`: direct evidence contradicts the current requirement.
   - `UNKNOWN`: evidence is insufficient, ambiguous, excluded, or the lock
     deliberately leaves the item unresolved.
5. Separate observed facts from conclusions. Never silently repair a mismatch,
   promote a candidate, or treat a planned feature as implemented.
6. Record every error and the safe workaround used.

At minimum, recheck rather than assume the status of:

- historical four-pass and repetition/mastery logic;
- turn-based versus approved-MCP-tool-call reflection counting;
- timer-only heartbeat behavior versus start epoch, timer limit, and maximum
  iteration cutoff;
- per-tool registration, menu presence, Tyler enablement, call permission, and
  `DENIED` enforcement;
- legacy channel and directory naming versus the current token structure;
- `.tsv` curriculum dependencies, which may be removed only by a later
  controlled change;
- references to missing `Genesis/**`, `DOCUMENT_STANDARD.md`,
  `DOC_REGISTRY.xml`, lock manuals, or context exports.

Do not resolve these issues during inventory.

## Required inventory report

** EVERY-FILE IN S SECTION MUST BE NAMED IN THE VERY BEGINNING OF THE SECTION DIR README **

Return:

1. Scope inspected and exclusions honored.
2. A table with `Requirement`, `Evidence`, `Status`, and `Finding`.
3. Confirmed conflicts.
4. Missing items.
5. `UNKNOWN` items and the exact decision or evidence needed.
6. Errors encountered and non-mutating workarounds.
7. A proposed next-step plan with allowed paths and validation, awaiting
   Tyler's approval.

Do not claim an overall pass from stale reports or percentages. Do not modify a
file to produce the report.

## Unclear language

Preserve unclear dictated text verbatim and mark it `UNKNOWN`. Do not turn
phrases such as `hdd red t`, `v CB heck`, or `littl red bee red tree` into
paths, commands, filenames, requirements, or approvals without Tyler's
clarification.

## Training-document preparation

For a request to set up or plan training documents, also follow
`.github/instructions/training-docs.instructions.md`. That instruction prepares
requirements and an approval plan only; it does not authorize creating
training content or implementing a workflow.
