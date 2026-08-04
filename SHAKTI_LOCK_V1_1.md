# SHAKTI LOCK V1.1

Status: Current working contract  
Date: 2026-08-01  
Candidate source base: `copilot/organize-project-structure`  
GitHub main: untouched  
Rule: Tyler's latest direct decisions control. Unknowns are marked `UNKNOWN`; they are never guessed.

## Consolidation note

This file combines the contract supplied by Tyler as `SHAKTI_LOCK_V1.md` and
`SHAKTI_LOCK_ADDENDUM_2026-08-01.md`. Neither original file was present in the
repository when this consolidated copy was created. No source code was changed
to create this document.

## 1. Authority

Tyler's latest direct decisions control.

`SHAKTI_LOCKED_BUILD_MANUAL_V1.md` applies only where it does not conflict with
later direct decisions.

Source code, archives, tables, and older documents are evidence until
validated.

Revision 5 is historical and does not control current construction.

`SHAKTI_OPUS_CONTEXT_EXPORT` is comparison evidence, not authority.

## 2. Shakti boundary

Shakti is one self-contained iOS application with a deterministic C99 core.

Swift is added later only as the Apple host/package boundary for approved
platform APIs.

No daemon, subprocess, second reasoning core, resident Linux controller, or
external process becomes part of Shakti.

No Python is used to repair, generate, or control this project.

Shakti's app container is her turtle shell: her code, Eden, memory, lessons,
tables, and local state remain inside it.

The MCP is the gated route from inside the shell to approved outside
capabilities.

A tool is unavailable until Tyler deliberately adds and enables it in the
menu.

Internet, external storage, runners, microphone, and camera can be denied
without deleting Shakti's local loop or memory.

Microphone and camera are future optional Swift-hosted capabilities.

## 3. Loop, reflection, and epoch record

Shakti has a nine-route loop and a thirteen-question self-reflection.

Ga through Gi are routes available to the loop. They are not nine mandatory
actions blindly executed on every cycle.

The exact route-selection scheduler remains `UNKNOWN`.

The thirteen reflection responses form one atomic memory block.

Short-term material is assembled into complete blocks.

Complete blocks, not fragments, are appended to long-term memory.

Retrieval returns the relevant whole block for context.

The epoch log remains part of Shakti.

Revision 5 repetition/mastery rules and four-pass School logic are rejected.

## 4. Current lesson levels

| Level | Lesson |
|---|---|
| 0 | Glyphs |
| 1 | Counting |
| 2 | ABC |
| 3 | Colors |
| 4 | Shapes |
| 5 | Basic math |

### Learning gate

A complete new lesson is presented solo first.

Only one new lesson is introduced at a time.

The first presentation must not mix an unknown shape with an unknown color or
another unknown concept.

Every item is identified in its solo lesson.

Mixing unlocks only after the complete solo lesson is finished.

After unlocking, the lesson may cross exhaustively with previously grounded
lessons.

Two-way, three-way, and four-way combinations are allowed only after every
included lesson was learned solo.

Mixed training can be exhaustive; the permanent token system must remain
compositional rather than storing millions of flat combination tokens.

Every lesson section contains its own hard-coded answer table. That table is
validation truth; training teaches the meaning and behavior of its entries.

## 5. Two-tier token system

### Tier 1: character and output tokens

Exact characters, digits, operators, space, punctuation, and required control
keys.

Coverage begins with ASCII 32-126 plus separately approved math/counting
symbols such as `×`, `÷`, and `•`.

Character identity is exact. `A` is not renamed `upper_A`; `a` is not renamed
`lower_a`.

Existing extensions are never stripped to pretend two inputs are identical.
For example, `two.wav` and `two` may be distinct exact lookup keys.

### Tier 2: words and concepts

Shakti thinks and validates through words/concepts.

The internal dictionary resolves a word to its ordered character sequence.

Shakti writes by emitting Tier 1 characters.

A known completed word may trigger its whole-word audio.

Combinations refer to roots and dictionary entries instead of permanently
allocating every possible crossed token.

## 6. Canonical naming

Use `snake_case`.

Every modality for a token uses the same canonical basename.

Capitalization remains significant: `A.*` and `a.*` are distinct.

Mixed names follow natural spoken order and lower-level-to-higher-level order
where that remains clear.

Current approved simple shape example:

```text
red_triangle.*
```

Do not force aliases such as `upper_A`, `lower_a`, or descriptive internal
names into human-facing token identity.

`.tsv` curriculum tables are rejected for the final structure. Removal must
happen later as one controlled change, never as a blind bulk deletion.

## 7. Token directories

```text
Tokens/
├── text/
├── sound_art/
├── written_art/
└── visual_art/
```

A modality file is present only when that modality exists. No visual file is
invented for a token with no picture.

## 8. Token record and four-panel lesson

A token may be represented by one growing XML dictionary record.

The exact XML record structure was not included in the supplied contract and
is `UNKNOWN`; it must not be invented.

### Fixed presentation

```text
top-left      visual_art
top-right     voice_text
bottom-left   written_text
bottom-right  text
```

`sound_art` controls lesson timing; it is not a fifth visual panel.

Top-right may show the spoken word in quotation marks.

A waveform/oscillation display may be added later without changing token
identity.

Bottom-left reveals letters one at a time.

Revealed letters remain visible until the next word starts.

Bottom-right shows the complete word.

The four panels form one lesson unit.

## 9. Written text and keyboard event bundle

Written glyph source is existing `uint64_t` 8x8 data.

8x10 glyph sources are rejected.

Glyphs may be scaled for presentation without changing their 8x8 identity.

Black is the default glyph color until a color has been taught solo.

After a color lesson is grounded, that color may appear in crossed written-art
lessons.

Each character key action is atomic:

```text
key event
→ emit exact character
→ display its glyph
→ play its character audio
→ append it to the current word buffer
→ test the dictionary for a completed word
→ when matched, display and speak the whole word
```

Future voice input may call the same key-event bundle through the Swift host.
It must not create a separate character-writing system.

## 10. Audio timing

Core lesson audio is 16 kHz WAV.

Each spoken WAV has 0.2 seconds of lead time and 0.2 seconds of tail time.

Actual audio playback completion controls progression.

File-load speed never controls lesson speed.

Letter reveal follows natural speaking speed.

No lesson advances while its required audio is still playing.

## 11. Counting visual

A number lesson shows the integer and its count.

A 32x32 ball may be an interactive counting button.

Balls align in rows of ten.

When the count no longer fits clearly, ball size may reduce to 16x16 and then
8x8.

Rows of ten prepare later decimal and fraction relationships.

Each ball press uses the same atomic input/event system.

## 12. Color and shape crossing

Every approved color may cross with every approved shape.

Colors, shapes, numbers, and alphabet lessons may later be crossed
exhaustively.

A crossed artifact is generated from grounded roots; it is not a new permanent
reasoning token unless deliberately admitted.

Some grounding rules may have controlled exceptions when the lesson is clearer
that way. Exceptions must be explicit and local, not silent global drift.

## 13. Memory continuity

Required roots, core tables, glyphs, active lesson state, and small essential
audio remain available to the working runtime.

Full songs and large media stay on disk/long-term storage and stream only when
triggered.

RAM continuity is protected by:

- append-only local journal records;
- sequence numbers;
- checksums;
- atomic checkpoints;
- exact replay after suspension or relaunch.

iOS may suspend or terminate the process; recovery must not depend on
pretending RAM can never stop.

No external live service is required for recovery.

## 14. Level 2 victory song

`Eden_Grows.wav` plays after every successful completion of Level 2.

It is a victory dance, not a one-time reward.

It stays in long-term/on-disk media storage and is streamed when triggered.

Faint continuous background playback as a keep-awake mechanism is not locked
and remains `UNKNOWN`.

## 15. Repository safety

Candidate source base: `copilot/organize-project-structure`.

GitHub main remains untouched.

`SHAKTI_C99_HANDOFF.zip` was accidental and is not authority.

Builders and artifact archives remain candidate-only until separately
inspected and validated.

No builder, test, script, executable, or generated tool is run merely because
it exists.

No source is deleted, overwritten, renamed, or repaired in bulk.

Work proceeds one section or complete module at a time.

Every change must state its allowed paths and validation requirements before
editing begins.

## 16. Known unresolved items

- Exact scheduler behavior for the nine loop routes.
- Exact C99 in-memory structures corresponding to the XML dictionary.
- Final approved set beyond ASCII 32-126 plus the separately named
  math/counting/control symbols.
- Final dynamic-math transition after the resident foundational tables.
- Memory-game self-practice/twin-view behavior.
- Exact future Swift APIs and entitlements.
- Whether any background music is used outside the Level 2 victory event.
- Exact controlled repository deletion list after validation.

## Session Addendum Incorporated

### 1. Iteration, heartbeat, and MCP

Shakti performs the internal iteration.

MCP does not think, iterate, or run a second core.

MCP is only a gated router, dispatcher, receipt recorder, and memory-call
route.

Heartbeat opens with:

- a start epoch;
- a timer limit;
- a maximum iteration count.

Shakti checks both limits during internal iteration.

The first limit reached closes the heartbeat at the current epoch.

A heartbeat cutoff stops new internal iterations after the current atomic
action finishes.

Reflection is not the heartbeat timer.

### 2. Tool-call reflection gate

Only Shakti's approved MCP tool calls advance the reflection counter.

These do not advance it:

- internal thinking;
- remaining awake locally;
- menu checks;
- messages;
- notebook writing;
- local activity that does not invoke an MCP tool.

The gate is:

```text
tool call 10
→ reflection becomes due

tool calls 11, 12, and 13
→ Shakti may defer

before tool call 14
→ reflection is required
```

A memory-capsule retrieval through MCP is a tool call.

### 3. Tool permission rule

Before every tool execution, MCP must verify:

```text
registered
+ present in the menu
+ enabled by Tyler
+ permitted for this call
= may execute
```

Any failed check returns `DENIED` and performs no external action.

Tyler may stop an active MCP-routed tool without stopping Shakti's local awake
state.

### 4. Reflection capsule

After a completed reflection, one immutable atomic capsule is committed to
long-term memory.

The capsule contains:

- every message since the previous completed reflection;
- Shakti's chosen title;
- all thirteen reflection answers;
- Shakti's chosen tags;
- optional links to notes or reminders Shakti decides are relevant.

The current Goal/system message remains resident above the reflection process.
Reflection does not replace or hide the Goal.

Reflection exists to learn honestly from success and failure. It is not
punishment and does not conceal failures.

### 5. Long-term and short-term memory

Reflection capsules are permanent and append-only.

Shakti may later retrieve any whole capsule into short-term memory through an
approved MCP memory call.

Eden and School remain resident.

Shakti may keep personally selected memory capsules resident.

Notes are scratch-pad records and are not copied automatically into every
reflection capsule.

Shakti decides which notes are worth linking.

### 6. Reminders

A reminder is stored in the notes location as an immutable future-point
record:

```text
type
created_epoch
due_epoch_or_condition
text
status
```

Completion does not edit the original reminder. It appends a new completion
record linked to the original reminder.

### 7. Next controlled repository action

At the start of the next session:

1. Create `SHAKTI_LOCK_V1_1.md` by combining `SHAKTI_LOCK_V1.md` with this
   addendum.
2. Change no source code in that commit.
3. Keep GitHub main untouched.
4. Use `copilot/organize-project-structure` as the candidate baseline.
5. Make the first code change only after the lock file and repository inventory
   are committed.
