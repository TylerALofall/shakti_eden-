# governed-markdown-updater

A structure-and-integrity controller for one source-of-truth Markdown workbook.

This skill is a set of instructions the model executes directly. The stated
design target is one C99 program providing four operations — `inventory`,
`decide`, `generate`, `audit` — so the workbook's structure is enforced
deterministically by a program rather than by model attention. Until that
program exists, this document is the controlling instruction set.

## Part 1 — The controlling outline

The outline below is the exact structure. Every governed workbook is
section-by-section identical to this outline.

### Exact section order

```
# YYYY_MM_DD [DOCUMENT TITLE] — SOLE SOURCE OF TRUTH

# I. INTRODUCTION

# II. TABLE OF CONTENTS
## II.A. Heading Index
## II.B. Code and File Flowchart

# III. RULES AND LAWS

# IV. FILE REGISTRY

# V. DEFINITIONS AND ELEMENTS

# VI. CURRENT BUILT STRUCTURE

# VII. FUNCTION REGISTRY AND JOBS

# VIII. TASK LIST

# IX. SCHEMAS AND TEMPLATES

# X. PROJECT FILES

# XI. AMENDMENTS | LOG
```

### Current Built Structure subsections

Inside Section VI, every major section uses the same six subsections:

- `a` — Introduction
- `b` — Local Contents
- `c` — Flowchart
- `d` — Files and Elements
- `e` — Functions and Jobs
- `f` — Current Built Result

With major sections A–L this produces exactly 72 local addresses, `Aa`–`Af`
through `La`–`Lf`.

### Structural identity

```
Major section:       A
Subsection:          a
Function number:     01
Structural address:  Aa-01
Logged identity:     [EPOCH]: [SUBSECOND_3] [SECTION_UPPER][SUBSECTION_LOWER]-[FUNCTION_2][PHASE]
```

Logged identity example:

```
1785108642: 184 Gf-01Z
```

- Epoch comes first.
- A colon follows the epoch.
- Subsecond contains exactly three digits.
- Major section uses uppercase.
- Subsection uses lowercase.
- Function order contains exactly two digits and remains sequential inside its
  subsection.
- A phase letter — `A`, `T`, or `Z` — appears at the end.

### Record numbering

```
Document section:       VIII
Code major section:     A
Code subsection:        a
Function position:      01
Structural identity:    Aa-01
File record:            FR-001
Element record:         EL-001
Schema record:          SC-001
Template record:        TP-001
Amendment record:       AM-[YYYY_MM_DD]-001
```

Numbering remains sequential within its owning section. Existing issued
numbers remain stable. Every heading and record has one working Table of
Contents link or cross-reference.

## Part 2 — Section-by-section description

Each section below is described twice: once as the **generic** definition that
applies to any governed workbook, and once as the **Shakti** filling of that
identical structure. The generic version is an example of how the section is
understood; a different project's guide would be section-by-section identical
to this one.

### Title line

- **Generic:** The first line is `# YYYY_MM_DD [DOCUMENT TITLE] — SOLE SOURCE
  OF TRUTH`. The date identifies the active copy.
- **Shakti:** The dated title identifies the active Shakti workbook copy.

### I. Introduction

- **Generic:** States that the document is the current source of truth for the
  project and carries the complete document-handling instruction: state the
  exact purpose of the work; use the Table of Contents and flowchart to locate
  the owning uppercase section, lowercase subsection, registered files, and
  numbered functions; place an authorized addition at the final numbered
  position of the highest applicable existing subsection; update every
  connected file, element, definition, function, schema, template, flowchart
  node, and amendment entry in the same revision; preserve all unrelated
  wording and code exactly; publish one dated active copy and retain the prior
  version as the controlled backup. The full controlling outline (Part 1)
  appears in the Introduction before project facts.
- **Shakti:** The Shakti workbook's Introduction carries the same handling
  instruction, with the project named as Shakti, and the controlling outline
  ahead of Shakti project facts.

### II. Table of Contents

- **Generic:** Two halves. `II.A. Heading Index` lists every heading with one
  working link. `II.B. Code and File Flowchart` plots every registered file,
  every major code section, every subsection beneath its major section, every
  function beneath its owning subsection, the direction of code flow, and the
  connections between files, sections, subsections, and functions. The File
  Registry and the flowchart always contain the same active files.
- **Shakti:** The Shakti flow area starts from Eden and Inference and includes
  the pre-born/pre-stateless state, Loading, pre-stateless training, Training
  Levels 1–6, after-training state, Training Scripts, artifact classes and
  sets, DFA preparation, C99 table preparation, the three-stage memory
  (instant memory at convergence, short-term memory, long-term memory), the
  full runtime cycle, menu choices, the nine MCP slots, and the thirteen
  self-reflection slots. The runtime cycle is plotted as its actual cycle. The
  nine MCP slots are plotted as available menu positions and choices, not as
  nine forced steps in a row. The thirteen reflection positions remain within
  the reflection area.

### III. Rules and Laws

- **Generic:** All retained rules and laws live in this one section. Each rule
  states the controlling requirement affirmatively — say what the rule is, not
  what it prohibits. A fact is written once at its controlling location; every
  additional use points back to its stable identifier. No rule is written for
  anything that was not directed.
- **Shakti:** The initial Shakti rule families, stated affirmatively:
  1. One dated active workbook carries the current verified project state.
  2. Each revision declares one exact purpose.
  3. Project logic and project tools use C99. Structured project schemas and
     records use XML.
  4. Uppercase sections, lowercase subsections, and two-digit function numbers
     define code-flow identity.
  5. Every recorded statement cites its file, element, function, authority, or
     evidence.
  6. One addition updates every affected register and flowchart connection
     together.
  7. New work takes the final available position within the highest applicable
     existing subsection.
  8. A discovered error is reported immediately and corrected in the next
     active dated copy.
  9. Unrelated wording, files, architecture, and code remain exact.
  10. One active workbook remains visible, with its previous state recoverable
      as the controlled backup.
  11. Every retained entry adds identity, ownership, connection, operation,
      evidence, or authority.

### IV. File Registry

- **Generic:** Every admitted file receives one record:

  ```
  FILE_PATH=
  FILE_TYPE=
  PURPOSE=
  MAJOR_SECTION=
  SUBSECTIONS=
  FUNCTION_IDS=
  INPUT_CONNECTIONS=
  OUTPUT_CONNECTIONS=
  STATUS=
  SOURCE_EVIDENCE=
  ```

  Schema relationships also belong in the File Registry:

  ```
  <file_record>
    <name>[exact_file_name]</name>
    <purpose>[exact_purpose]</purpose>
    <related_files>[exact_related_file_names]</related_files>
    <called_schemas>[exact_snake_case_schema_names]</called_schemas>
    <flow_marker>[exact_existing_flow_marker]</flow_marker>
  </file_record>
  ```

  Every schema name called by a file is copied word-for-word in
  `snake_case`. The same file is plotted at its exact marker in the flowchart.
- **Shakti:** The Shakti File Registry lists the project's C99 source files,
  headers, XML schemas, and Markdown documents, each at its owning section and
  flow marker. Artifact files appear through their manifests; the actual
  artifacts called by name are recorded exactly.

### V. Definitions and Elements

- **Generic:** Every newly admitted element receives:

  ```
  ELEMENT=
  EXACT_DEFINITION=
  MAJOR_SECTION=
  SUBSECTION=
  DEFINED_IN_FILE=
  USED_BY_FILES=
  USED_BY_FUNCTIONS=
  SOURCE_EVIDENCE=
  ```

  Existing definitions remain unchanged unless Tyler identifies the exact
  correction.
- **Shakti:** Shakti elements — for example the memory stages, MCP slots,
  reflection slots, training levels, DFA structures, and C99 tables — are
  defined once here and referenced by identifier everywhere else.

### VI. Current Built Structure

- **Generic:** Records what actually exists and how it connects — actual built
  structure, not proposed work or generic checklists. Major sections use
  uppercase letters and existing names and numbers are preserved exactly.
  Each major section contains the six subsections `a`–`f`:

  - `a` **Introduction** — what this major section is and does.
  - `b` **Local Contents** — the section's subsections and functions in
    existing flow order.
  - `c` **Flowchart** — the section's position in the code-and-file flow.
  - `d` **Files and Elements** — the files and elements owned by this section.
  - `e` **Functions and Jobs** — the section's functions with their jobs.
  - `f` **Current Built Result** — the current verified state of the section.

  Tables show the project's structured sets.
- **Shakti:** Shakti's existing major-section names and numbers are preserved
  and never renumbered. The required project areas include: the
  pre-born/pre-stateless state (these are the same state), Eden, Inference,
  Loading, pre-stateless training, Training Levels 1–6, after-training state,
  Training Scripts, artifact classes and sets, DFA preparation, C99 table
  preparation, instant memory at convergence, short-term memory, long-term
  memory, the nine-slot MCP, the thirteen-slot self-reflection, the full
  runtime cycle, and menu choices with their available slots. Artifact
  organization belongs inside the Training and Pre-Training major structure,
  after the Training Scripts subsection. Generated tables show: three-stage
  memory, Training Levels 1–6, DFA preparation, C99 table preparation, nine
  MCP slots, thirteen self-reflection slots, and artifact class order.
  `SUBCONSCIOUS KNOWLEDGE & SENSORY` is one of the stated section names in the
  source material and is looked for inside the current constitution document.

### VII. Function Registry and Jobs

- **Generic:** Every admitted function receives:

  ```
  FUNCTION_ID=[EPOCH][UPPERCASE][lowercase]-[TWO_DIGIT_NUMBER]
  FUNCTION=
  EXACT_SIGNATURE=
  JOB=
  DECLARATION_FILE=
  DEFINITION_FILE=
  INPUTS=
  OUTPUTS=
  STATE_EFFECTS=
  CALLS=
  CALLED_BY=
  CONNECTED_ELEMENTS=
  SOURCE_EVIDENCE=
  ```

  Function numbers begin at `01` and follow the order in which the functions
  flow. Existing identities remain unchanged.
- **Shakti:** Shakti functions are registered in flow order beneath their
  owning subsection, from pre-born through the runtime cycle, with their
  existing identities preserved.

### VIII. Task List

- **Generic:** The Task List appears immediately after the Function Registry
  and Jobs. Its controlling instruction is:

  > Tyler controls this Task List. An entry is added only by Tyler or by
  > copying Tyler's approved instruction word-for-word. Work follows the
  > listed order, beginning with the first authorized task.

- **Shakti:** Identical to the generic definition.

### IX. Schemas and Templates

- **Generic:** Contains the blank schema template and template records. The
  blank schema is:

  ```xml
  <schema>
    <name>[snake_case_name]</name>
    <description>[exact_description]</description>
  </schema>
  ```

  That is the whole template: a blank schema with a name and a description.
  The schema itself tells its story through its actual contents. Any file that
  needs the schema is added to it by exact word-for-word `snake_case` name.
  Each schema points to its marker on the flowchart and appears in the File
  Registry. A schema that spans many files (as the long ones do) is recorded
  once and referenced by name.
- **Shakti:** Shakti's structured schemas and records use XML. The long
  schemas shared across most files are recorded once here and called by exact
  name from the File Registry.

### X. Project Files

- **Generic:** The final content section contains the actual registered
  project files. Each file appears under its exact registered name with its
  complete contents in a matching fence (C sources in `c` fences, XML in `xml`
  fences, Markdown in `markdown` fences). The File Registry, flowchart,
  function entries, schema calls, and embedded file bodies agree exactly.
- **Shakti:** The Shakti workbook embeds the actual C99, XML, and Markdown
  project files. Artifacts are not embedded as tens of thousands of loose
  files; the workbook carries the artifact manifests and the exact artifacts
  that are called by name, organized under Training and Pre-Training.

### XI. Amendments | Log

- **Generic:** The Amendments section at the end is the log. A compact
  amendment records each addition:

  ```
  AMENDMENT_ID=
  DATE=
  AUTHORIZED_PURPOSE=
  ADDED=
  PLACED_AT=
  FILES_REGISTERED=
  ELEMENTS_REGISTERED=
  FUNCTION_IDS=
  UNCHANGED_CONTENT_VERIFIED=
  SOURCE_EVIDENCE=
  ```

- **Shakti:** Identical to the generic definition.

## Part 3 — Update mechanics

### Addition-placement rule

For an authorized addition:

1. Locate the highest existing major section that owns it.
2. Locate the applicable lowercase subsection.
3. Place the addition at the final end of that subsection.
4. Assign the next available two-digit function number.
5. Preserve every earlier function number.
6. Ask Tyler when ownership or placement is unclear.
7. Create a new major section or subsection only with Tyler's authority.

An item with clear existing ownership may be moved into its correct location
while retaining its exact identity and wording. An unclear fit returns an
immediate question to Tyler.

### One connected update

One authorized addition is one connected transaction:

```
FLOWCHART
    ↕
FILE REGISTRY
    ↕
DEFINITIONS AND ELEMENTS
    ↕
CURRENT BUILT STRUCTURE
    ↕
FUNCTION REGISTRY
    ↕
AMENDMENT
```

The skill updates only the connected registers that the addition actually
affects.

### Concision control

A fact is written once at its controlling location. Every additional use
points back to its stable identifier.

A sentence remains when it contributes at least one of:

```
IDENTITY
OWNERSHIP
CONNECTION
OPERATION
EVIDENCE
AUTHORITY
```

## Part 4 — The programmatic design target

The stated design target is one C99 program providing four operations:

```
inventory  — extract exact existing sections, rules, files, elements,
             functions, schemas, templates, numbers, and flow connections

decide     — record Tyler's KEEP, REMOVE, or RELOCATE decisions by item number

generate   — build the complete Markdown source of truth from approved items

audit      — verify structure, preserved numbering, connections, embedded
             files, and exact agreement with Tyler's decisions
```

The program extracts existing material without rewriting it and presents
manageable numbered groups:

```
RULE-001   [exact existing rule]
SEC-A      [exact existing section]
SUB-Aa     [exact existing subsection]
FN-Aa-01   [exact existing function]
FILE-001   [exact existing file]
```

Tyler answers with the numbers that `STAY`, `GO`, or `RELOCATE`. Generation
uses only those recorded decisions. Every proposed addition is shown before
entering the active workbook. A newly authorized function takes the final
available position inside its existing owning subsection — assign the number
and it moves to the back of that section, with an amendment logged.

The deterministic audit verifies:

- Every retained source item appears exactly.
- Every removed item is absent.
- Every relocated item appears only at its approved location.
- Existing numbers remain attached to the same identities.
- Every registered file appears in the flowchart and final embedded-files
  section.
- Every schema call matches its exact `snake_case` name.
- Every section has its Introduction and local contents.
- The nine MCP slots and thirteen reflection slots retain their source
  identities.
- The generated file contains zero unapproved additions.

Until that program exists, this skill is executed as instructions and every
addition, placement, and amendment is shown to Tyler for confirmation.

## Part 5 — Change boundary

- Tyler identifies or authorizes the addition.
- The skill preserves existing wording and code.
- The skill updates only the exact connected locations.
- A discovered problem is reported to Tyler immediately.
- The workbook records what exists, where it exists, and how it connects.
- The workbook contains actual built structure rather than proposed work or
  generic checklists.
- One active workbook remains; prior states stay recoverable through its
  controlled backup/version history.
- Nothing is added without showing Tyler what is being added.

## Part 6 — UNKNOWN items

Preserved verbatim; not interpreted:

1. "don't close the chemo without the schema" — `UNKNOWN`. A later message
   says "don't close without the schema." The exact word and requirement
   await Tyler's clarification.
2. The exact existing heading for the front state was clarified by Tyler as
   one state with two names: "pre born" and "pre stateless" are the same; the
   sections are to be found inside the current constitution document.
