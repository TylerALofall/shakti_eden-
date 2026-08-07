# SHAKTI COMPLETE BLUEPRINT — CANONICAL VERBATIM

**Status:** Single current truth for this handoff.  
**Authority:** Tyler's direct statements.  
**Wording:** Requirement text is preserved verbatim. Editorial text is limited to headings and source labels.  
**Archive rule:** Everything under `archive/` is preserved evidence only and cannot override this file.

---

## Current loop and log map — Tyler verbatim

```text
1. Epoch time, that’s it.
2. A reprompt on a delay optionally and a countdown, Tyler can change.
3. Goal = system message or multistage system message, Tyler can change.
Shakti can write in 4, 6, 7, 9; read all.
4. Scratchpad and reminders.
5. Two-tier menu.
6. Tool call.
7. Message to Tyler.
8. Message to Shakti.
9. Reflections.
Same schema numbers throughout log, everything tied to epoch, numbers from blueprint.
Capital = section, lowercase = subsection.
After epoch: 3-digit subsecond timing, then function 00–99 per subsection/start order.
Use exact 13 self-reflection questions unchanged.
```

## Internal lookup and interrupt — preserved exact text

> The MCP is internal and is Shakti’s lookup.  
> Tyler can throw an interrupt from outside the MCP. The interrupt stops Shakti’s tools and leaves Shakti awake.  
> Shakti calls her memory by internal tool call automatically. The memory call pulls in the resident blocks and selected long-term chunks.

---

## Phase — A, T, Z

Every action carries exactly one phase.

| Phase | Is | Time it carries |
|---|---|---|
| `A` | Opens | Start time |
| `T` | The doing | Time of that step |
| `Z` | Closes | **Finish time** |

**`Z` lands later on the clock than its `A`.** They are two records with two
timestamps, not one record with a suffix. `Z` carries a reference back to the
opening line.


Duration is `Z` minus `A`, taken from monotonic time.

An `A` with no `Z` stays incomplete across restart. It is never quietly closed.

---

## Message envelope

```xml
<epoch>

  <heartbeat>
    <interval></interval>
    <beats_left></beats_left>
  </heartbeat>

  <goal></goal>

  <notebook>
    <note></note>
    <reminder></reminder>
  </notebook>

  <menu></menu>

  <to_Tyler>
    <message></message>
    <note></note>
  </to_Tyler>

  <to_Shakti>
    <message></message>
    <note></note>
  </to_Shakti>

  <reflection></reflection>

</epoch>
```

---

## Thirteen-point reflection

Verified byte-exact against `src/shakti_loop.c:13-27`.

```text
1   Title the last 10 cycles
2   Summary
3   Did I finish — if no, the reason
4   Was it successful
5   If I was to redo it, what would I do different
6   Tools used
7   What tools would have helped
8   What could Tyler have done different to help
9   What files did I save, and their paths
10  Memories recalled that helped, by the epoch of the first memory in the batch
11  Notes
12  Meta tags for auto-search — shoot for 15
13  Cross reference — override all additional memory sets to link this to:
```

One fixed versioned set. A new version is a deliberate act, not a drift.

**Commit law.** The thirteen are fields inside one candidate and one final
immutable block. Not thirteen files. Response 1 opens with `A`, responses 2
through 12 stay inside the same candidate, response 13 closes with `Z` and the
whole block commits atomically. Whole or not at all.

Cadence: due at 10 turns, three deferrals, tools block at 13.

---

## 1. Core boundary

- Shakti is one self-contained iOS application with a deterministic C99 core.
- Swift is added later only as the Apple-facing wrapper for approved platform APIs.
- No daemon, subprocess, second core, resident Linux controller, or external process is part of Shakti.
- Shakti's app container is her turtle shell: Eden, School, memory, lessons, tables, and local state remain inside it.
- MCP is her gated reach outside the shell.
- External tools are unavailable until Tyler adds and enables them in the menu.
- Internet, external storage, microphone, camera, runners, and other tools are detachable capabilities.
- Cutting external capabilities must not delete Shakti's local state or memory.

---

## 5. Reflection capsule

After a completed reflection, one immutable atomic capsule is appended to long-term memory.

The capsule contains:

- every message since the previous completed reflection;
- Shakti's chosen title;
- all thirteen reflection answers;
- Shakti's chosen tags;
- optional links to notes or reminders she decides are relevant.

The current Goal/system message remains resident above reflection and is not replaced by it.

Reflection exists to learn honestly from successes and failures. It is not punishment and does not hide mistakes.

---

## 6. Memory

- Eden and School remain resident.
- Shakti may keep personally selected memory capsules resident.
- Short-term memory may hold whole retrieved capsules.
- Long-term memory is append-only and immutable.
- Reflection capsules are retrieved whole for context.
- Notes are scratch-pad records and are not copied into every capsule automatically.
- Shakti decides which notes are worth linking.
- A reminder is an immutable future-point record stored in the notes area.
- Completing a reminder appends a linked completion record; it does not edit the original.

---

## 7. Lesson levels

```text
Level 0  Glyphs
Level 1  Counting
Level 2  ABC
Level 3  Colors
Level 4  Shapes
Level 5  Basic math
```

---

## 8. Learning order

- A complete new lesson is presented solo first.
- Only one new lesson is introduced at a time.
- The first presentation must not mix two unknown concepts.
- The complete solo lesson must finish before crossing is allowed.
- After grounding, the lesson may cross exhaustively with previously learned lessons.
- Two-way, three-way, and four-way combinations are allowed only after every included lesson was learned solo.
- The first impression is the most important.
- Each lesson section contains its own hard-coded answer table.
- The answer table is validation truth; training teaches meaning and behavior.

---

## 9. Two-tier token system

### Tier 1 — characters and output tokens

- Exact characters, digits, operators, punctuation, space, and required keys.
- Coverage begins with ASCII 32-126 plus separately approved math/counting symbols such as `×`, `÷`, and `•`.
- Character identity is exact.
- `A` is `A`; `a` is `a`.
- Do not rename them `upper_A` or `lower_a`.
- File extensions are not stripped to pretend different sensory inputs are identical.

### Tier 2 — words and concepts

- Shakti thinks and validates through words and concepts.
- Her internal dictionary resolves a word to an ordered character sequence.
- She writes by emitting Tier 1 characters.
- A completed known word may trigger its whole-word audio.
- Crossed lessons are compositions of grounded roots, not millions of permanent flat tokens.

---

## 10. Canonical naming

- Use `snake_case`.
- Every modality for a token uses the same canonical basename.
- Capitalization remains significant.
- Names follow spoken order.
- Simple approved example:

```text
red_triangle.*
```

- Do not force old aliases or descriptive internal names into human-facing token identity.

---

## 12. Four-panel lesson

```text
top-left      visual_art
top-right     voice_text
bottom-left   written_text
bottom-right  text
```

- `sound_art` controls timing; it is not a fifth visual panel.
- Top-right may show the spoken word in quotes.
- Bottom-left reveals letters one at a time.
- Revealed letters remain visible until the next word starts.
- Bottom-right shows the complete word.
- The four panels form one lesson unit.

---

## 13. Written text and keyboard action

- Written glyphs use existing `uint64_t` 8x8 data.
- 8x10 glyph sources are rejected.
- Glyphs may be scaled for display without changing their 8x8 identity.
- Black is the default glyph color until a color has been taught solo.
- After grounding, taught colors may appear in crossed written-art lessons.

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

Future voice input may call the same key-event bundle through the Swift wrapper.

---

## 14. Audio timing

- Core lesson audio is 16 kHz WAV.
- Add 0.2 seconds before and 0.2 seconds after each spoken WAV.
- Actual playback completion controls progression.
- File-load speed never controls lesson speed.
- Letter reveal follows natural speaking speed.
- No lesson advances while required audio is still playing.

---

## 15. Counting visual

- A number lesson shows the integer and its count.
- A 32x32 ball may be an interactive counting button.
- Balls align in rows of ten.
- When needed, ball size may reduce to 16x16 and then 8x8.
- Rows of ten prepare later decimal and fraction relationships.

---

## 16. Color, shape, number, and alphabet crossing

- Every approved color may cross with every approved shape.
- Colors, shapes, numbers, and alphabet may later cross exhaustively.
- Crossed artifacts are generated from grounded roots.
- Some grounding rules may have explicit local exceptions when that makes a lesson clearer.
- Exceptions must be deliberate and must not silently rewrite the global rules.

---

## 17. Memory continuity

- Required roots, core tables, glyphs, active lesson state, and small essential audio remain available to the working runtime.
- Full songs and large media remain on disk and stream only when triggered.
- Continuity uses:
  - append-only local journal records;
  - sequence numbers;
  - checksums;
  - atomic checkpoints;
  - exact replay after suspension or relaunch.
- Recovery must not depend on pretending RAM can never stop.
- No external live service is required for recovery.

---

## 18. Victory song

- `Eden_Grows.wav` plays after every successful completion of Level 2.
- It is a victory dance.
- It stays in long-term/on-disk media storage and streams when triggered.

---

## Current training, frame, and sound instructions — Tyler verbatim

> Lessons 0-6 are ready waiting on the tables to get hooked u

> Do not mock code her

> Go to Eden out in the repo

> Add .2 sec to each side of the wav files and auto put it out there.  There’s a script that does all this

> Bottom left is letters top right is sound so word  them spelling the word then word again

> Yep and let sound lead you have the wav for every thing that’s taught

> I mean let sound control the time

```text
Hold it till the frame is done with all four but start it when it’s ready so the timing  will be

“One”
O
N
E
One

And keep like this
“O”
O “N”
O  N. “E”
```

> Each sound converges same time as frames you have all the keys to spell the words thoae are building blocks there’s letter tokens go ahead and finish it with out me! You can do it

---

## Repository and handoff instructions — Tyler verbatim

> And yhen push all the repos to 1 repo and make sure to keep the training files and 3 files same name in th Ed b bc out dir

> Don’t worry about it gett the copy out of there’s I’ll merge them all at the end

> Just let’s get a Shakti launched this turn

> I mean session

> I mean by the end of the session hopefully we can test her live

> Hold it all to give me in one zip with the entire complete hand off and 1 complete blue print not rewordified and ready to pass to the next with explaination of what’s going on amd wherr they are needing to go with this

---

## Change law

Anything in this file changes only by Tyler's direct statement. A model does not
edit it to match its own code. If code and this file disagree, the file is right
and the code is reported.
