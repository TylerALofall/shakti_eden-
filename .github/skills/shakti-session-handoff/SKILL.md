# shakti-session-handoff

Continuity card for the next session working on Shakti (`TylerALofall/shakti_eden-`).
Read this first, then `SHAKTI_LOCK_V1_1.md` (the governing contract), then
`.github/copilot-instructions.md` (the safety gate). Tyler's latest direct
instruction always overrides everything below.

## Current state (as of 2026-08-09)

- **Branch of record:** `copilot/binary-sound-implementation-scout` (hearing +
  sense + MCP phase 1). Never touch `main` / do not treat default `Shakti-main`
  uploads as auto-merge. Stop and report if the working branch is `main`.
- **Purity:** The tree is **100% C99**. No `.sh`, no Python, no JS, no other
  languages. This is a hard, non-negotiable requirement (see the `NOTICE` in
  `.github/copilot-instructions.md`).
- **Build:** `make` (CFLAGS `-std=c99 -Wall -Wextra -Wpedantic -Werror -O2`).
  Includes `hearing/hearing`, `pad_wav` (Lock §10 lead/tail tool), `make sense`.
- **Test:** `make test` → `tests/test_shakti`, `tests/test_mcp`,
  `tests/test_integration`, `tests/test_roundtrip`, `tests/test_sense`. Green.
- **MCP phase 1 (landed):** `mcp/mcp.c` + `mcp/mcp.h` + `mcp/README.md`.
  `/shakti_run/` admits only when registered ∧ menu section present ∧
  Tyler-enabled ∧ permitted; else `DENIED` (no handler side effect). Receipt
  count on approve. Interrupt/resume unchanged. Handlers remain in `src/main.c`.
  Host HTML stays reference-only.
- **Boundary (verified):** `src/`, `include/`, `tools/`, `mcp/` contain **no**
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
7. **Senses always on:** born with sight/hearing working — never off, not grown
   at lesson age. Inside Eden: **no probability, no vector embeddings, no
   guessing model** on the sense path. `sense/` links `hearing_synth` only
   (not `hearing_model` GRU).
8. **All senses converge at one point** — `sense_converge` writes one
   `sense_point_t` (binary then render of the same event). No half-points.
9. **Zips on Shakti-main** (`For Groc 2.zip`, baseline zips, lock upload) =
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

- **sense module (Tyler plan implement, 2026-08-09):** New section dir `sense/`
  only — **all senses converge at one point** (`sense_point_t` / `sense_converge`
  / `sense_now`). Not parallel tracks. Files: `sense/README.md`, `sense/sense.h`,
  `sense/sense.c`, `sense/sense_map.c`, `tests/test_sense.c`. Makefile:
  `make sense`, `tests/test_sense` on `make test`. Links eyes + screen +
  **hearing_synth only** (no GRU/embeddings). Ring of 8 points; 64×64 mono
  vision + 10 ms @ 16 kHz PCM bits + light on the same `seq`. Pre-Eden 21 s
  prenatal (dark 0–20 s then flashes). Tyler: senses always on; no probability
  inside Eden; all senses meet at 1 point. Did **not** touch `src/**`, MCP,
  Eden tables, or Swift. Validated: `make sense` PASS, `make test` PASS.
  No merge until Tyler says.
- Prior: screen 640×360 RGBA; early optional self-reflection + MCP gate phase 1
  + hearing PR #7 + C99-only purity. See git history on this branch.

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
