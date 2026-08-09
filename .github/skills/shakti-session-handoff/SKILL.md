# shakti-session-handoff

Continuity card for the next session working on Shakti (`TylerALofall/shakti_eden-`).
Read this first, then `SHAKTI_LOCK_V1_1.md` (the governing contract), then
`.github/copilot-instructions.md` (the safety gate). Tyler's latest direct
instruction always overrides everything below.

## Current state (as of 2026-08-09)

- **Branch of record:** `copilot/binary-sound-implementation-scout`. Never
  touch `main` / do not treat default `Shakti-main` uploads as auto-merge.
  Stop and report if the working branch is `main`.
- **Purity:** The tree is **100% C99**. No `.sh`, no Python, no JS, no other
  languages. This is a hard, non-negotiable requirement (see the `NOTICE` in
  `.github/copilot-instructions.md`).
- **Build:** `make` (CFLAGS `-std=c99 -Wall -Wextra -Wpedantic -Werror -O2`).
  Includes `hearing/hearing` and `pad_wav` (Lock §10 lead/tail tool).
- **Test:** `make test` → runs `tests/test_shakti` (unit),
  `tests/test_integration`, and `tests/test_roundtrip`.
- **Boundary (verified):** `src/`, `include/`, `tools/` contain **no**
  `system`/`popen`/`fork`/`exec`/`spawn`, no threads, no daemon, **no runner**.
  Shakti is one self-contained C99 process holding one `shakti_runtime_t`.

## Frozen layout decisions (Tyler lock paste + judgment, 2026-08-09)

Correct anytime; these are the working defaults:

1. **`sound_art`** is the audio channel. Same case-exact basename as other
   modalities: `KEY.wav` + `KEY.8x8.txt` + `KEY.png` (svg is migration input).
2. **Audio timing (Lock §10):** 16 kHz WAV; **0.2 s zero lead and 0.2 s zero
   tail inside each spoken file**. Playback completion advances the lesson.
   Between-word gap in a pipe sequence comes from each clip’s own pad, not a
   second invented timer.
3. **Asset home now:** keep using `eden_out/Sound_art|Visual_text|Visual_art`
   as the live training pile (73 sound keys all match a visual_text key).
   Lock target dirs `Tokens/sound_art|written_art|visual_art|text` stay the
   long-term shape; no bulk move until a named work packet.
4. **Four-panel lesson** (Lock §8): TL visual, TR voice_text, BL written, BR
   text. `sound_art` times the unit; not a fifth panel. Dedicated monitor =
   host UI later (Swift), not a C99 core block.
5. **Memory (Lock §13 + addendum §4–5):**
   - short-term = assembled working material
   - complete blocks / reflection capsules → append-only **long-term**
   - whole-capsule retrieve through **MCP memory call** (= the RAG-style call)
   - Eden + School stay resident; notes are scratch unless she links them
6. **Binary + prenatal:** binary eyes/deposit stays pixel-exact rebuild path;
   `hearing/` prenatal heartbeat+light is pre-Level-0 grounding scout, not a
   replacement for Pass 1 stones.
7. **Zips on Shakti-main** (`For Groc 2.zip`, baseline zips, lock upload) =
   evidence only until Tyler orders a read-only open. Do not bulk import.

## Tyler's architecture (do not violate)

1. **Shakti lives in her own shell (her house).** Her code, Eden, memory,
   lessons, and state stay inside. Everything inside is hers and stays together.
2. **No runner, no subprocess, no daemon, no second core.** If a change would
   introduce one, the change is wrong by definition.
3. **The MCP is the only gated route out** — the gate and the fingers to the
   outside. It is a router / dispatcher / receipt-recorder / memory-call route.
   It does not think, iterate, or run a second core.
4. **Every message goes through the MCP.** The MCP records chats and contacts to
   the outside and connects Shakti to long-term memory. She uses it for
   everything that crosses the shell boundary.
5. **If she ever needs a CORE/ML to train her, that training happens outside her
   house** and its artifacts wait elsewhere until deliberately admitted.
6. **Inside Eden is 100% deterministic.** Fixed order; no probability, no
   options, no randomness. Anything optional/probabilistic lives outside Eden.
7. **Child-learning order:** solo lesson first → then crossing (two/three/four
   way) only after every included lesson was learned solo. Mastery = a 10-correct
   consecutive streak (100% or not mastered).

## Lock requirements still `UNKNOWN` (candidates for build-out)

From `SHAKTI_LOCK_V1_1.md` §16 — resolve only with Tyler's approval, one module
at a time, each with named paths + validation before editing:

- Exact scheduler behavior for the nine loop routes (Ga–Gi).
- Exact C99 in-memory structures for the XML dictionary.
- Final approved token set beyond ASCII 32–126 + named math/control symbols.
- Final dynamic-math transition after resident foundational tables.
- Memory-game self-practice / twin-view behavior.
- Exact future Swift APIs and entitlements.
- Background music outside the Level-2 victory event.
- Exact controlled repository deletion list after validation.

## Approved build-out order (from Tyler, child-learning order)

1. **Nine-route loop scheduler** — deterministic fixed-order route selection
   (resolves the lock `UNKNOWN`). Allowed: `src/shakti_loop.c`,
   `include/shakti_loop.h`, `tests/` + `make`/`make test`.
2. **Tri-sense convergence** — the four-panel lesson unit (visual_art /
   voice_text / written_text / text) with `sound_art` timing (Lock §8).
3. **Three-stage memory hardening** — append-only capsules + exact replay
   (Lock §13, addendum §5).
4. **Deterministic senses / pre-born tables** — extend hard-coded validation
   tables level by level (Glyphs → Counting → ABC → Colors → Shapes → Basic
   math), compositional crossing rather than flat mass example generation.

## What was done in the session that created this card

- PR #7 review fix (`discussion_r3743595332`): prenatal flash channel now
  enforces absolute dark for stream elapsed time
  `[0, SHAKTI_HEARING_DARK_PHASE_SECONDS)` (20 s), then ambient+pulse
  entrainment; `SHAKTI_HEARING_MAX_SAMPLES` raised to 25 s so Phase 2 is
  reachable (`hearing/hearing.h`, `hearing/hearing_synth.c`).
- PR #7 review `discussion_r3743595347` only: hearing public stream is a
  **scalar light channel** (one intensity per 10 ms frame), not an 8×8 grid.
  Removed unused `SHAKTI_HEARING_FLASH_GRID_SIZE` / `SHAKTI_HEARING_FLASH_PIXELS`
  and aligned `hearing/hearing.h` + `hearing/README.md` with the existing
  `flash_intensities[]` + `INPUT_DIM=2` model. Validated with `make hearing`.
- Prior: Inventoried the `TylerALofall-patch-1` zips (read-only). Current tree
  is newer; do not overwrite from zips. Python inside zips was not imported.
- Prior: POSIX shell integration tests → pure-C99 `tests/test_integration.c`.
- Prior: C-callable tool entry points; `SHAKTI_APP_NO_MAIN` for `shakti_app_main`.

## How to work here (hard rules)

- **C99 only.** No Python/JS/shell/subprocess/daemon — and no wording that could
  be read as adding them.
- **Never edit files with Python.** Use transparent direct editing tools.
- **No agents unless Tyler directly commands them.**
- **Think first; smallest change; never rewrite Tyler's documents or structure.**
- Read-only for inventory/status unless Tyler approves a plan naming every
  allowed path and its validation. State allowed paths + validation before any
  edit.
- One approved section/module at a time. Never bulk delete/overwrite/rename.
- Keep Eden status OPEN. Preserve every correct and incorrect School attempt in
  append-only readable logs.
