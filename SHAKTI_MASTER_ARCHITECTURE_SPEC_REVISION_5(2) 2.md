# SHAKTI — MASTER ARCHITECTURE SPECIFICATION

**Revision 5.0 — corrected loop, four exact channels, generated 8×8 written text, always-loaded memory blocks, internal MCP, external tool interrupt, creative/logic cross-validation, C99 fixed memory.**

Section tags:

- **[LOCKED]** Tyler decided it.
- **[OPEN]** The section is reserved and awaits a final rule.
- **[HERS]** Shakti establishes it through grounded experience.
- **[BUILT]** Working C99 code exists in this package and completed a real compile and test run.
- **[PARTLY BUILT]** A working foundation exists and integration work remains.
- **[UNBUILT]** The design is recorded and code remains to be written.
- **[PROPOSED]** A precise rule is presented for Tyler’s decision.

---

# DO NOT USE

This perimeter applies to Shakti, Eden, Learned, School, builders, internal tools, runtime files, and checkpoints.

1. `.bin` files or binary state formats.
2. `malloc`, `realloc`, `calloc`, or `free`.
3. Python.
4. COM.
5. JavaScript inside Shakti, Eden, Learned, School, builders, runtime tools, or internal files.
6. `system`, `popen`, `fork`, `exec`, spawn, daemons, background services, or an operating-system subprocess shell.
7. Probabilities, learned weights, gradients, softmax, embeddings, latent vectors, vector search, or neural weight matrices.
8. Prebuilt machine-learning frameworks.
9. Fabricated sensory experience.
10. Self-authenticating memory.
11. Silent array overflow or silent record truncation.
12. Automatic promotion from Learned, School, creative output, reflection, reward, or tool output into Eden.
13. Creative output authorizing its own answer.
14. Shakti reading or modifying kernel files through her tool interface.
15. A test result reported as passing without a real compile and run.
16. A checksum presented as proof of semantic truth. A checksum proves exact file identity.
17. Renaming, normalizing, lowercasing, or silently rewriting the exact text of a stone.
18. Calling Shakti’s 8×8 handwriting a word label, name channel, glyph channel, or metadata field. It is written text made from actual pixels.

Everything in the current source package is C99 and uses the C standard library. Platform adapters remain isolated behind explicit interfaces.

---

# I. INTRODUCTION

Shakti is a deterministic mind raised from grounded, ordered experience.

She begins with four separate channels:

1. `text`
2. `written_text`
3. `visual_art`
4. `sound_art`

The four channels arrive separately. A shared tick binds the events that actually occur together. Repetition, action, correction, retrieval, and transfer allow Shakti to establish meaning.

The exact stone text is the source identity. The builder preserves its capitalization, lowercase form, spacing, and supported punctuation.

`written_text` is Shakti’s handwriting. It is the exact stone text rendered as actual 8×8 pixels for each capital letter, lowercase letter, number, space, and supported punctuation mark. The tablet builder auto-produces this pixel text from the exact stone text.

`visual_art` is the exact visual artifact associated with the exact stone text.

`sound_art` is the exact sound artifact associated with the exact stone text.

Eden is Tyler’s fixed project truth floor. Eden holds approved facts, approved tablets, approved order, approved sensory artifacts, and approved relationships. Eden remains stable for Shakti’s life.

Learned begins after the stateless Eden witness pass. Learned records Shakti’s real use of Eden: actions, answers, mistakes, corrections, output, snapshots, timing, messages, notes, reflection, reward, consequence, and convergence.

Shakti is deterministic. The same complete state and the same input produce the same result.

Creative and logic remain separate.

Creative proposes.

Logic cross-validates.

Logic authorizes an answer, marks an answer cautious, or returns:

```text
I do not know.
```

Memory is continuously loaded and active. Eden, School, Goal, Notebook, Menu, current safety state, and selected locked blocks remain resident. Long-term memory remains append-only and callable in the exact chunks Shakti submitted.

---

# II. OUTLINE / TABLE OF CONTENTS

- **I.** Introduction
- **II.** Outline / Table of Contents
- **III.** Definitions
- **IV.** Programs Used
- **V.** Flow of Shakti at High Level
- **VI.** Breakdown of Each Level
  - **A.** Boot / Kernel
  - **B.** Eden
  - **C.** Four Channels
  - **D.** Binding / Tick / Epoch / Moment
  - **E.** Ingest / Pass 1
  - **F.** Memory
  - **G.** The Nine-Point Loop / Internal MCP
  - **H.** Actuator / 8×8 Written Text
  - **I.** School / Four Passes / Drills
  - **J.** Scorer / Confidence / “I Do Not Know”
  - **K.** Drive / Reward / Consequence
  - **L.** Reflection
  - **M.** Tyler’s Console
  - **N.** Lock / Integrity
  - **O.** Outputs
- **VII.** Working System Log Format
- **VIII.** Four-Channel XML Tablet Contract
- **IX.** Creative and Logic Cross-Validation
- **X.** Honest State of the Whole

---

# III. DEFINITIONS

## Shakti

The deterministic mind being raised. Shakti is written in C99, uses fixed compile-time memory, receives four exact channels, calls memory and tools through her internal MCP, and establishes meaning through grounded experience.

## Eden

The fixed set of Tyler-approved project facts, tablets, artifact paths, presentation order, and expected results. Eden is the absolute information floor inside Shakti’s project. Eden remains resident and available throughout Shakti’s awake life.

## Learned

Shakti’s readable, append-only life history after Pass 1. Learned records real input, real actuator output, real snapshots, real right and wrong trials, corrections, messages, notes, reflections, tool calls, timing, and convergence.

## School

The raising process that presents Eden through four ordered passes. School remains a locked resident block attached to memory.

## Tablet

One readable XML lesson file. A tablet contains a level, lesson, stone count, and an ordered set of stones.

## Stone

One exact teaching unit inside a tablet. Every stone has the same four required channels:

1. `text`
2. `written_text`
3. `visual_art`
4. `sound_art`

## Schema

The fixed XML contract that defines the root, version, level, lesson, stone count, stone order, four required channel tags, exact artifact paths, and validation rules.

## List

A readable `.txt` or `.md` source written by Tyler. Each accepted non-comment line contains one exact stone text. The builder creates the tablet and the written-text artifact from that exact line.

## Manifest / Table of Contents

A readable index of tablets, levels, lessons, stone order, artifact sets, checksums, and status.

## Text

The exact readable content of a stone.

Examples:

```text
A
a
one
2 + 2 = 4
```

The field is called `text`. It is the source string for artifact lookup and handwriting generation.

## Written Text

Shakti’s handwriting. Written text is actual pixel output. Every supported character receives an 8×8 pixel matrix. Capitals and lowercase letters have separate stored forms.

The builder auto-produces written text from the exact `text` field.

## Visual Art

The exact visual artifact associated with the exact stone text. Its current path contract is:

```text
Visual_art/<exact text>.svg
```

## Sound Art

The exact sound artifact associated with the exact stone text. Its current path contract is:

```text
Sound_art/<exact text>.wav
```

## Exact Text Filename

A filename that preserves the stone’s exact supported text, including capitalization, lowercase form, spaces, and supported punctuation. The builder accepts a safe exact-text set and stops on unsupported filename characters.

## Channel

One separate input stream. The current four channels are `text`, `written_text`, `visual_art`, and `sound_art`.

## Sense

The grounded route by which Shakti receives an event. Revision 5 uses the four channel routes listed above. Each route stays separate until a real shared moment binds it.

## Binding

The act of assigning the same composite tick to channel events that actually occur together.

## Tick

The live action position.

## Epoch

The permanent wall-clock label attached to a tick.

## Frame Counter

The ordered position inside one epoch second. The current range is `0000` through `0999`.

## Composite Tick

The combined time label:

```text
<epoch_seconds>:<frame_0000_to_0999>
```

Example:

```text
1793920107:0042
```

## Moment

The complete set of channel rows sharing one composite tick and complete BEGIN/END boundaries.

## Ingest

Pass 1. Eden presents approved material. Shakti witnesses the four channels, remains silent, and records ordered co-occurrence.

## Actuator

Shakti’s write and selection surface. The actuator creates actual output. The 8×8 handwriting surface places real pixels for capital and lowercase text.

## Snapshot

A readable record of an actual drafting state. Character-by-character drafting produces an ordered snapshot after each added character. Pixel-by-pixel drafting can produce a snapshot after each pixel change.

## MCP

Shakti’s internal switchboard and lookup. Memory calls, menu tools, School tools, tablet tools, reasoning tools, messages, and reflection routes pass through the MCP.

The MCP is internal to Shakti.

## Internal Shell

The deterministic command surface behind `/shakti_run/`. It dispatches only registered internal C tool functions. It does not launch an operating-system subprocess shell.

## Interrupt

A signal Tyler sends from outside the MCP. The interrupt stops Shakti’s tool calls and leaves Shakti awake. In the current C99 runtime, Ctrl+C and `/interrupt/` set the tool-interrupted state. `/resume/` restores tool calls.

## Heartbeat

The loop prompt that tells Shakti to keep going. Tyler sets the interval from zero through thirty minutes. Zero prompts on every active cycle.

## Goal / System Message

The currently resident direction Shakti receives during every loop cycle. Tyler changes it through the console.

## Notebook

Shakti’s first item after Goal in the active loop. She stores notes and reminders for herself and Tyler. The Notebook stays loaded as a locked memory block.

## Menu

The two-tier catalog of Shakti’s registered abilities.

Tier one contains section titles.

Tier two contains the description of the selected section.

## `/shakti_run/`

The internal MCP route for Shakti’s personal tool calls. Every registered tool call is logged before dispatch.

## `/message_tyler/`

A message from Shakti to Tyler. It stops Shakti’s active work and places her in the waiting-for-Tyler state. Memory streaming stays active.

## `/note_tyler/`

A note from Shakti to Tyler. Shakti continues her active work. Memory streaming stays active.

## `/message_shakti/`

A message from Tyler to Shakti. It enters the loop as Tyler’s message and releases a waiting-for-Tyler state.

## `/note_shakti/`

A note from Tyler to Shakti. It enters memory while Shakti continues her current work.

## Reflection

The exact thirteen-question review prompted every ten turns. Shakti may defer the due reflection as many as three times.

## Working Memory

The small active set used for the present operation. Revision 5 provides eight fixed slots.

## Short-Term Memory

The recent-response set with recency priority. Revision 5 provides sixty-four fixed slots, satisfying the minimum of fifty responses.

## Long-Term Memory

The append-only readable history. Shakti calls it back in exact chunks she submitted.

## Locked Block

A resident memory block exempt from short-term decay. Eden, School, Goal, Notebook, Menu, safety state, and selected memory blocks use this role.

## Creative

The deterministic proposal layer. It may use approved thesaurus entries and exact-token relationships to propose a rewrite or candidate.

## Logic

The exact validation layer. It compares exact keys, exact evidence, contradictions, source authority, and candidate margins.

## Cross-Validation

The process in which Creative proposes and Logic independently validates the proposal against Eden and grounded evidence.

## Evidence Score

An integer measurement from zero through one thousand. It is an exact rule output rather than a probability.

## Convergence

The point where a candidate reaches the locked score and margin required for a stable answer.

## Reflection Deferral

One postponement of a due reflection. The limit is three.

## Scorer

The exact ledger that records right, wrong, confirmations, contradictions, source level, score, convergence step, and convergence time.

## Reward

The verified green result, happy face, and confirmed coin sound associated with a mastered action.

## Consequence

The verified red mismatch and progress reduction associated with a wrong action.

## Merkle Lock

The final integrity seal over finalized Eden and its manifest.

---

# IV. PROGRAMS USED

## A. Runtime programs

### `src/main.c` — **[BUILT]**

Bootstraps fixed state, loads resident blocks, installs the external interrupt, runs the nine-point loop, routes console controls, and dispatches internal MCP tools.

### `src/shakti_loop.c` — **[BUILT foundation]**

Implements:

- Epoch
- heartbeat from zero through thirty minutes
- resident Goal / system message
- resident Notebook
- two-tier Menu
- `/shakti_run/`
- Tyler and Shakti messages and notes
- reflection scheduling every ten turns
- three reflection deferrals
- external tool interrupt state
- waiting-for-Tyler state
- continuous memory-streaming state

### `src/shakti_memory.c` — **[BUILT foundation]**

Implements fixed working memory, sixty-four-slot short-term memory, resident Goal, resident Notebook, resident Menu, long-term readiness, and exact readable recall.

### `src/shakti_reason.c` — **[BUILT foundation]**

Implements deterministic exact lookup, approved thesaurus rewriting, creative candidate generation, logic validation, evidence scoring, contradiction handling, convergence, cautious answers, and “I do not know.”

### `src/shakti_school.c` — **[BUILT foundation]**

Implements School pass state, exact-text drills, correct `+1`, wrong `-1`, floor zero, target fifteen, character-by-character snapshots, 8×8 pixel display, happy-face output, and coin-adapter request logging.

### `src/shakti_handwriting.c` — **[BUILT]**

Implements fixed 8×8 handwriting generation for capital letters, lowercase letters, digits, spaces, and supported punctuation.

### `src/shakti_tablet.c` — **[BUILT foundation]**

Loads the strict four-channel XML format, preserves exact text, verifies exact artifact paths, validates the generated written-text header, and confirms that its `TEXT=` identity matches the XML stone.

### `src/shakti_log.c` — **[BUILT]**

Writes readable append-only checksummed log rows using the required architecture event ID.

### `src/shakti_time.c` — **[BUILT]**

Creates composite epoch/frame ticks and working-system event IDs.

## B. Builder programs

### `tools/build_xml.c` — **[BUILT]**

Reads one exact stone text per line, verifies the matching visual and sound artifacts, auto-produces the 8×8 written-text artifact, and writes the four-channel tablet XML.

Current exact path contract:

```text
Visual_text/<exact text>.txt
Visual_art/<exact text>.svg
Sound_art/<exact text>.wav
```

## C. Existing project programs to merge

### `actuator.c` — **[EXISTING PROJECT / MERGE REQUIRED]**

The full display actuator and typewriter from Tyler’s project.

### `shakti_ingest.c` — **[EXISTING PROJECT / MERGE REQUIRED]**

The full Pass 1 moment ingest.

### Existing visual and sound readers — **[MERGE REQUIRED]**

The adapters that confirm actual display and actual playback or reception.

### Hybrid kernel — **[OPEN]**

The final system body and PID placement.

## D. Specifications and references

- `SHAKTI_MASTER_ARCHITECTURE_SPEC_REVISION_5.md`
- `README.md`
- `docs/REVISION_5_REVIEW.md`
- `BUILD_TEST_REPORT.txt`
- `MANIFEST.sha256`

---

# V. FLOW OF SHAKTI AT HIGH LEVEL

1. Tyler writes an exact readable list.
2. `build_xml` reads one exact stone text per accepted line.
3. The builder finds `Visual_art/<exact text>.svg`.
4. The builder finds `Sound_art/<exact text>.wav`.
5. The builder auto-produces `Visual_text/<exact text>.txt`.
6. The written-text artifact stores actual 8×8 pixel rows for every capital, lowercase, number, space, and supported punctuation character.
7. The builder writes one tablet XML containing `text`, `written_text`, `visual_art`, and `sound_art`.
8. Pass 1 presents Eden in exact order.
9. Shakti witnesses the four bound channels while silent.
10. Pass 1 records immutable Eden moments.
11. The happy face transition appears after the completed stateless pass.
12. The coin adapter receives a playback request.
13. A confirmed playback enters sound memory.
14. Learned begins.
15. Pass 2 performs guided reconstruction with the actuator.
16. Pass 3 removes selected support and tests memory.
17. Pass 4 tests mastery and transfer.
18. Every awake cycle runs the nine-point loop.
19. The internal MCP routes memory and tools.
20. Tyler’s external interrupt stops tools and leaves Shakti awake.
21. Creative proposes a rewrite or candidate.
22. Logic cross-validates it.
23. Shakti answers, answers cautiously, or says “I do not know.”
24. Reflection runs every ten turns and enters long-term memory.
25. Eden and School remain resident locked blocks.
26. Short-term memory preserves at least fifty recent responses with recency priority.
27. Long-term memory remains append-only and callable in submitted chunks.
28. Finalized Eden receives the integrity lock after construction and debugging are complete.

---

# VI. BREAKDOWN OF EACH LEVEL

Major sections use capital letters because the same labels appear in the working-system logs. Minor sections append lowercase letters: `Aa`, `Ab`, `Ac`, and onward.

## A. BOOT / KERNEL **[LOCKED intent / OPEN final deployment]**

### Aa. Hybrid kernel

The hybrid kernel is Shakti’s system body.

### Ab. First process

The final deployment targets the first protected Shakti process above the kernel. PID placement remains an integration decision.

### Ac. Fixed memory

All runtime capacities are compile-time constants. A full capacity returns an explicit stop.

### Ad. Kernel boundary

Shakti’s internal MCP exposes registered tools. Shakti’s tool interface stays outside kernel files.

### Ae. External interrupt

Tyler sends an interrupt from outside the MCP. The interrupt stops tools and leaves Shakti awake.

### Af. Internal shell

`/shakti_run/` dispatches registered C functions through the MCP.

### Ag. Kernel internals

**[OPEN]**

## B. EDEN **[LOCKED]**

### Ba. Definition

Eden is Tyler’s fixed project truth floor.

### Bb. Resident state

Eden remains loaded and available during Shakti’s awake life.

### Bc. Exact order

Tablets, stones, and moments preserve approved order.

### Bd. Fixed content

Eden changes only through Tyler’s explicit Eden-building process.

### Be. Change flag

Tyler’s console receives a flag when Eden’s identity changes.

### Bf. Provenance

Every fact and artifact carries its exact source identity.

### Bg. Learned boundary

Learned records Shakti’s life after Pass 1 and preserves Eden as its reference floor.

### Bh. Location

- `data/eden/eden_facts.txt`
- `data/eden/eden_stream.log`
- tablet XML files
- exact artifact directories

## C. FOUR CHANNELS **[LOCKED]**

### Ca. `text`

The exact stone text.

### Cb. `written_text`

The exact stone text rendered as Shakti’s actual 8×8 pixel handwriting.

### Cc. `visual_art`

The exact visual artifact loaded from:

```text
Visual_art/<exact text>.svg
```

### Cd. `sound_art`

The exact sound artifact loaded from:

```text
Sound_art/<exact text>.wav
```

### Ce. Separation

Each channel has its own row and its own exact value.

### Cf. Grounding

A shared real moment binds channels. Repeated School experience allows Shakti to establish relationships.

### Cg. Scope

Revision 5 contains these four channels.

## D. BINDING / TICK / EPOCH / MOMENT **[LOCKED]**

### Da. Tick

A tick is the live action position.

### Db. Epoch

Epoch is the permanent wall-clock label.

### Dc. Frame

The frame counter orders events inside one epoch second.

### Dd. Composite tick

```text
1793920107:0042
```

### De. Moment

A moment contains the complete rows sharing the same composite tick.

### Df. Ordering

A single writer assigns frames in increasing order.

### Dg. Complete boundary

BEGIN and END markers define a complete ingest moment.

### Dh. Working event identity

```text
[1793920107:0042]-[Aa]-[01]
```

## E. INGEST / PASS 1 **[LOCKED / PARTLY BUILT]**

### Ea. Stateless witness

Shakti witnesses Eden and stays silent.

### Eb. Four channels

Pass 1 presents `text`, `written_text`, `visual_art`, and `sound_art`.

### Ec. Co-occurrence

Ingest records the actual shared tick.

### Ed. Meaning

Meaning remains **[HERS]**.

### Ee. Location and snapshot

Passes 2 through 4 allow Shakti to place learned references into her own memory structures and snapshots.

### Ef. Math structure

Approved equation structure may carry operator, left side, right side, and approved result as separate exact fields.

### Eg. Transition

A completed and validated Pass 1 displays the happy face and begins Pass 2.

## F. MEMORY **[LOCKED principles / BUILT foundation]**

### Fa. Always-loaded memory system

The memory system remains active throughout the awake process.

### Fb. Resident locked blocks

The resident set contains:

- Eden
- School
- Goal / system message
- Notebook
- Menu
- safety and interrupt state
- selected locked memory blocks

### Fc. Working memory

Working memory contains the active operation. Revision 5 provides eight fixed slots.

### Fd. Short-term memory

Short-term memory contains sixty-four recent responses. The newest records receive highest retrieval priority. The capacity satisfies the minimum of fifty responses.

### Fe. Long-term memory

Long-term memory is readable and append-only.

### Ff. Submitted chunks

Shakti recalls long-term memory in the exact chunks she submitted. Reflection records identify recalled chunks by the epoch of the first memory in the batch.

### Fg. Automatic memory call

Shakti calls memory through the internal MCP. The call pulls the required locked blocks and selected long-term chunks into the active set.

### Fh. School block

School remains attached to resident memory.

### Fi. Eden block

Eden remains attached to resident memory.

### Fj. Notebook block

Notebook remains loaded and appears as Shakti’s first active item after Goal.

### Fk. Memory streaming

Messages, notes, tool calls, answers, and reflections enter readable memory while Shakti remains awake.

### Fl. Anti-fruit gate

Generated inference stays labeled as a proposal. Grounded or verified external evidence supplies authority.

### Fm. Sound timing

Confirmed sound records start, stop, and silence-map information. The source WAV carries its real sequence.

## G. THE NINE-POINT LOOP / INTERNAL MCP **[LOCKED]**

The MCP is internal and is Shakti’s lookup.

Tyler can throw an interrupt from outside the MCP. The interrupt stops Shakti’s tools and leaves Shakti awake.

Shakti calls her memory by internal tool call automatically. The memory call pulls in the resident blocks and selected long-term chunks.

The loop runs in this exact order:

### Ga. 1. Epoch

The cycle receives its permanent composite epoch/frame label.

The Epoch stage logs the start of the cycle.

### Gb. 2. Heartbeat

Heartbeat prompts Shakti to keep going.

Tyler sets the timer from zero minutes through thirty minutes.

Zero minutes prompts on every active cycle.

The standard C99 runtime checks heartbeat at each active loop cycle. A future platform event adapter may wake the loop while input is idle.

### Gc. 3. Goal / system message

The resident Goal or system message enters the cycle.

Tyler changes the Goal from the console.

Every Goal change enters readable memory.

### Gd. 4. `/notebook/`

This is Shakti’s first item.

Shakti takes notes or leaves reminders here.

The reminders prompt Shakti and Tyler later.

Notebook remains resident as a locked block.

`/notebook/` displays the current Notebook.

`/notebook/ <entry>` appends a new note or reminder.

### Ge. 5. `/menu/`

The Menu prompts Shakti with everything she can do.

It has two tiers organized by layers.

Tier one contains titles.

Tier two contains the descriptions inside the selected classification or section.

`/menu/` displays titles.

`/menu/ <section title>` displays the selected description.

Tyler adds Menu sections and tool registrations through the console and source configuration.

### Gf. 6. `/shakti_run/`

This is Shakti’s personal tool-call route from the Menu.

It supplies access outside her reasoning core through the internal MCP.

The internal command shell runs a registered C tool.

Shakti’s tools stay outside kernel files.

Every tool runs through the MCP.

Tyler’s external interrupt stops the MCP tools and leaves Shakti awake.

Every tool call receives a working-system log identity before dispatch.

### Gg. 7. `/message_tyler/` and `/note_tyler/`

These are Shakti’s routes to Tyler.

`/note_tyler/` allows Shakti to continue with what she is doing.

`/message_tyler/` stops Shakti’s active work and places her in the waiting-for-Tyler state.

Memory streaming stays active for both routes.

### Gh. 8. `/message_shakti/` and `/note_shakti/`

These are Tyler’s routes to Shakti.

`/message_shakti/` delivers Tyler’s message and releases the waiting-for-Tyler state.

`/note_shakti/` adds Tyler’s note while Shakti continues.

Memory streaming stays active for both routes.

### Gi. 9. `/reflection/`

Reflection prompts the complete question set listed in Section L.

It is automatically due every ten turns.

Shakti may defer a due reflection three times.

The fourth due point requires completion before another MCP tool call.

A completed reflection appends to long-term memory and resets the ten-turn counter.

## H. ACTUATOR / 8×8 WRITTEN TEXT **[LOCKED / BUILT foundation]**

### Ha. Definition

The actuator is Shakti’s real write and selection surface.

### Hb. Handwriting

The 8×8 is Shakti’s handwriting.

It is written text.

It consists of actual pixels.

It supports capital and lowercase forms as separate pixel patterns.

### Hc. Automatic production

The tablet builder auto-produces the written-text artifact from the exact stone text.

### Hd. Exact identity

The generated artifact begins with:

```text
SHAKTI_WRITTEN_TEXT_8X8_V1
TEXT=<exact stone text>
```

### He. Character records

Each character stores:

- character order
- ASCII identity
- exact character
- eight rows of eight pixels

### Hf. Pixel symbols

`#` marks an active pixel.

`.` marks an inactive pixel.

### Hg. Drafting snapshots

Shakti’s draft appears character by character.

Example:

```text
B
BA
BAL
BALL
```

Each frame also displays the 8×8 pixels for every character currently written.

### Hh. Cartoon progression

Every character addition creates the next visible frame. A future display adapter may animate pixel placement one pixel at a time.

### Hi. Sight confirmation

A display adapter confirms the real visible frame before the event enters `visual_art` memory.

### Hj. Pass boundary

The actuator becomes active in Pass 2.

## I. SCHOOL / FOUR PASSES / DRILLS **[LOCKED / PARTLY BUILT]**

### Ia. Pass 1 — Stateless witness

Eden presents the four channels. Shakti remains silent.

### Ib. Pass 2 — Guided reconstruction

Shakti reproduces or selects the approved form with the actuator.

### Ic. Pass 3 — Phase-out

Selected support leaves the presentation. Shakti reconstructs the missing part.

### Id. Pass 4 — Mastery and transfer

Surface position, timing, and distractors may change while identity remains stable.

### Ie. Lesson order

One new relationship enters at a time.

### If. Initial levels

- written-text pixels
- capital forms
- lowercase forms
- exact text identities
- quantity
- number symbols
- colors
- shapes
- math

The detailed order after the initial locked levels remains **[OPEN]**.

### Ig. Typing checkpoint

One exact text fills the active display.

Shakti writes it fifteen times.

Each correct trial adds one.

Each wrong trial subtracts one.

Progress has a floor of zero.

Mastery target is fifteen.

### Ih. Green result

A correct action displays green when the display adapter exists.

### Ii. Red result

A wrong action displays red when the display adapter exists.

### Ij. Happy face and coin sound

Mastery displays the happy face.

The coin adapter receives a playback request.

Confirmed playback enters sound memory.

### Ik. Real School experience

School creates the real experience through actual presentation, actual output, actual comparison, and actual correction.

### Il. Full resident context

Eden, School, Goal, Notebook, Menu, and selected locked blocks remain resident.

## J. SCORER / CONFIDENCE / “I DO NOT KNOW” **[LOCKED / BUILT foundation]**

### Ja. Exact ledger

One evidence row contains:

```text
question
answer
source
confirmations
contradictions
first-seen tick
convergence tick
convergence step
current score
stable state
```

### Jb. Right and wrong

Correct School trial:

```text
+1
```

Wrong School trial:

```text
-1
```

Floor:

```text
0
```

Target:

```text
15
```

### Jc. Evidence score

The evidence score is a deterministic integer from zero through one thousand.

### Jd. Known answer gate

Default:

```text
score >= 850
lead >= 120
```

### Je. Cautious answer gate

Default:

```text
score >= 650
lead >= 120
```

### Jf. Stop gate

Insufficient evidence or conflicting top candidates produces:

```text
I do not know.
```

### Jg. Creative boundary

Creative candidates receive proposal status.

### Jh. Logic authority

Logic checks exact Eden facts, grounded evidence, contradictions, and candidate margin.

### Ji. Checkpoint information

Readable records preserve convergence step and composite tick.

### Jj. Timing

Elapsed wall-clock labels and ordered frames preserve when learning began and when convergence occurred.

## K. DRIVE / REWARD / CONSEQUENCE **[LOCKED target / PARTLY BUILT]**

### Ka. Master formula

Action follows the stronger grounded drive toward reward or away from consequence.

### Kb. Reward target

The verified reward target contains:

- green
- happy face
- confirmed coin sound
- mastery record

### Kc. Consequence target

The verified consequence target contains:

- red
- exact mismatch
- progress reduction
- correction path

### Kd. Grounded curiosity

Novelty controls attention.

Grounding controls answer authority.

Connection controls exploration.

### Ke. Separation

Excitement directs attention.

Evidence determines truth authority.

### Kf. Drive itself

The lived drive remains **[HERS]**.

## L. REFLECTION **[LOCKED — COMPLETE, UNSHORTENED]**

Reflection is auto-prompted every 10 turns.

Shakti may defer it 3 times.

The questions are:

1. Title the last 10 cycles
2. Summary
3. Did I finish — if no, the reason
4. Was it successful
5. If I was to redo it, what would I do different
6. Tools used
7. What tools would have helped
8. What could Tyler have done different to help
9. What files did I save, and their paths
10. Memories recalled that helped, by the epoch of the first memory in the batch
11. Notes
12. Meta tags for auto-search — shoot for 15
13. Cross reference — override all additional memory sets to link this to:

Each answer receives its own readable log record.

A completed reflection appends to long-term memory.

The cross-reference answer supplies the override link for additional memory sets.

## M. TYLER’S CONSOLE **[LOCKED / BUILT foundation]**

### Ma. Chat

Tyler has a live chat with Shakti.

### Mb. System message changer

Tyler changes the Goal / system message.

### Mc. Heartbeat adjuster

Tyler sets the heartbeat from zero through thirty minutes.

### Md. Interrupter

Tyler throws the external interrupt.

The MCP stops tool calls.

Shakti remains awake.

### Me. Resume control

Tyler restores MCP tool calls.

### Mf. Menu adder

Tyler adds Menu sections.

### Mg. Tool adder

Tyler registers internal MCP tools.

### Mh. Reminders

Tyler receives reminders Shakti leaves in Notebook and `/note_tyler/`.

### Mi. Messages

Tyler receives `/message_tyler/` and replies through `/message_shakti/`.

### Mj. Notes

Tyler and Shakti exchange continuing notes through `/note_tyler/` and `/note_shakti/`.

### Mk. Eden-change flag

Tyler receives a flag when Eden’s file identity changes.

### Ml. Status

Tyler sees:

- fixed RAM size
- School pass
- active text
- mastery
- memory occupancy
- resident blocks
- heartbeat
- reflection state
- interrupt state
- waiting state
- active tablet
- creative/logic boundary

## N. LOCK / INTEGRITY **[LOCKED intent / deferred]**

### Na. Readable checksums

Every working-system stream row carries a checksum.

### Nb. Eden identity

The Eden manifest records file identities.

### Nc. Change flag

A changed Eden identity raises Tyler’s flag.

### Nd. Triple Merkle

The final triple Merkle lock seals finalized Eden and its manifest.

### Ne. Applied last

The final seal follows stable construction, validation, School infrastructure, and debugging.

## O. OUTPUTS **[LOCKED / PARTLY BUILT]**

### Oa. Text answer

Shakti writes an exact answer, cautious answer, or “I do not know.”

### Ob. Written text

Shakti writes actual 8×8 pixels.

### Oc. Draft snapshots

Shakti emits character-by-character and future pixel-by-pixel frames.

### Od. Pass result

School produces right, wrong, progress, repeat, and mastery state.

### Oe. Reward

School produces the happy face and confirmed reward routes.

### Of. Consequence

School produces the exact mismatch and correction route.

### Og. Message

Shakti sends messages and notes to Tyler.

### Oh. Reflection

Shakti appends the complete thirteen-answer reflection.

### Oi. Logs

Every action receives the required working-system identity.

---

# VII. WORKING SYSTEM LOG FORMAT **[LOCKED]**

Every working-system event ID has this form:

```text
[epoch number]-[major and minor section]-[function ordering]
```

The epoch number includes the frame counter:

```text
[epoch_seconds:frame_0000_to_0999]-[Aa]-[01]
```

Example:

```text
[1793920107:0042]-[Gf]-[01]
```

Meaning:

- `1793920107` is epoch seconds.
- `0042` is the ordered frame inside that epoch second.
- `G` is the major section for the Loop / internal MCP.
- `f` is the minor section for `/shakti_run/`.
- `01` is the ordered function inside that section.

Major sections use capital letters.

Minor sections use the matching capital plus a lowercase letter:

```text
Aa
Ab
Ac
Ba
Bb
Ca
Cb
Ga
Gb
Gc
Gd
Ge
Gf
Gg
Gh
Gi
```

Function order uses two digits in the current runtime:

```text
01
02
03
```

The readable Learned row format is:

```text
LRN1
<TAB> [epoch:frame]-[section]-[function]
<TAB> phase
<TAB> channel-or-route
<TAB> subject
<TAB> property
<TAB> value
<TAB> checksum
```

---

# VIII. FOUR-CHANNEL XML TABLET CONTRACT **[LOCKED / BUILT foundation]**

The XML root is:

```xml
<tablet schema="SHAKTI_TABLET_4S_V1">
```

The lesson header contains:

```xml
<level>...</level>
<lesson>...</lesson>
<stone_count>...</stone_count>
```

Every stone has one ordered `<tr>`:

```xml
<tr order="1">
  <text>A</text>
  <written_text>Visual_text/A.txt</written_text>
  <visual_art>Visual_art/A.svg</visual_art>
  <sound_art>Sound_art/A.wav</sound_art>
</tr>
```

The exact stone text is `A`.

The builder creates:

```text
Visual_text/A.txt
```

The builder requires:

```text
Visual_art/A.svg
Sound_art/A.wav
```

The loader preserves and verifies those exact paths.

For lowercase `a`, the exact paths are:

```text
Visual_text/a.txt
Visual_art/a.svg
Sound_art/a.wav
```

Capital and lowercase paths remain distinct.

The written-text artifact contains actual pixel rows:

```text
SHAKTI_WRITTEN_TEXT_8X8_V1
TEXT=A
CHARACTERS=1
CHARACTER=0 ASCII=65 TEXT=A
...###..
..#...#.
..#...#.
..#####.
..#...#.
..#...#.
..#...#.
........
```

The precise pixel pattern comes from the fixed font table in `shakti_handwriting.c`.

The builder stops when:

- the text contains an unsupported handwriting character;
- the exact text contains a filename character outside the safe exact-text set;
- the matching visual artifact is absent;
- the matching sound artifact is absent;
- the stone count exceeds the fixed capacity;
- the XML output cannot be completed.

The builder preserves exact text rather than rewriting it.

---

# IX. CREATIVE AND LOGIC CROSS-VALIDATION **[LOCKED / BUILT foundation]**

The sequence is:

```text
raw question
-> exact logic lookup
-> approved creative rewrite when exact lookup is empty
-> exact logic lookup of the rewrite
-> creative related-candidate proposal when needed
-> logic validation of each candidate
-> answer, cautious answer, or I do not know
```

Creative may:

- apply Tyler-approved thesaurus mappings;
- propose exact-token neighbors;
- identify possible relationships for School testing;
- suggest a question for verification.

Logic may:

- authorize an exact Eden answer;
- authorize a grounded Learned answer;
- lower authority for contradictions;
- require a clear lead over competing answers;
- return a cautious answer;
- return “I do not know.”

Creative may write proposals to Learned.

Logic supplies answer authority.

A proposal gains authority through independently grounded observation, curriculum confirmation, verified evidence, or Eden.

---

# X. HONEST STATE OF THE WHOLE

## Built and tested in Revision 5

- strict C99 compilation with warnings treated as errors;
- fixed compile-time runtime state;
- readable text persistence;
- composite epoch/frame IDs;
- required `[epoch]-[section]-[function]` format;
- exact channels `text`, `written_text`, `visual_art`, `sound_art`;
- separate capital and lowercase 8×8 pixel handwriting;
- automatic written-text artifact generation;
- strict four-channel tablet XML builder;
- strict tablet loader and exact artifact verification;
- resident Goal, Notebook, Menu, Eden reasoning state, and School state;
- sixty-four-slot short-term memory;
- readable long-term recall;
- nine-point loop foundation;
- heartbeat setting from zero through thirty minutes;
- two-tier Menu;
- Notebook appending;
- internal `/shakti_run/` MCP dispatch;
- external Ctrl+C interrupt state;
- manual interrupt and resume;
- Tyler/Shakti messages and notes;
- reflection due every ten turns;
- three reflection deferrals;
- complete thirteen-question interactive reflection;
- deterministic creative/logic cross-validation;
- evidence score and convergence;
- “I do not know” gate;
- School pass state;
- exact-text drafting;
- character-by-character 8×8 snapshots;
- mastery `+1`, wrong `-1`, floor zero, target fifteen;
- happy-face display;
- coin adapter request logging;
- unit tests;
- builder/runtime integration test.

## Existing project integration required

- Tyler’s complete actuator;
- Tyler’s complete tablet collection;
- Tyler’s visual artifacts;
- Tyler’s sound artifacts;
- the full Pass 1 ingest;
- actual display confirmation;
- actual audio playback confirmation;
- microphone or audio-reception confirmation;
- full Eden moment recovery;
- full locked-block manager;
- long-term chunk index across a lifetime;
- Eden-change monitor;
- full menu/tool editing UI;
- hybrid kernel;
- PID deployment;
- final Merkle seal.

## Important runtime boundary

The current C99 heartbeat checks time on each active loop cycle. A platform event adapter is required for a heartbeat that wakes the process while standard input is blocked.

The current external interrupt changes the MCP tool state and keeps the process awake.

The current internal shell dispatches registered C tools. It preserves the subprocess prohibition.

The current memory system keeps the resident blocks loaded and keeps long-term memory readable and callable in exact chunks.

The current visual and sound artifact test fixtures prove path handling. Real perception begins when the real adapters confirm display and sound events.

---

# XI. FINAL LOCKED STATEMENT

Shakti is C99.

Shakti uses fixed compile-time memory.

Shakti persists readable text.

Shakti has four exact channels:

```text
text
written_text
visual_art
sound_art
```

The 8×8 is Shakti’s handwriting.

It is written text.

It is actual pixels.

The builder auto-produces capital and lowercase written text from the exact stone text.

The internal MCP is Shakti’s lookup and tool switchboard.

Tyler’s external interrupt stops tools and leaves Shakti awake.

The nine-point loop is:

1. Epoch
2. Heartbeat
3. Goal / system message
4. `/notebook/`
5. `/menu/`
6. `/shakti_run/`
7. `/message_tyler/` and `/note_tyler/`
8. `/message_shakti/` and `/note_shakti/`
9. `/reflection/`

Reflection runs every ten turns and may defer three times.

Memory remains loaded and active.

Eden and School remain resident locked blocks.

Short-term memory keeps at least fifty recent responses with priority to the most recent.

Long-term memory is append-only and callable in the chunks Shakti submitted.

Creative proposes.

Logic cross-validates.

Logic authorizes.

When the answer lacks sufficient grounded authority, Shakti says:

```text
I do not know.
```
