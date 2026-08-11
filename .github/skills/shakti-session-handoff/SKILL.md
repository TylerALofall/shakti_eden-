# shakti-session-handoff

Continuity card for the next session working on Shakti (`TylerALofall/shakti_eden-`).
Read this first, then `SHAKTI_LOCK_V1_1.md` (the governing contract), then
`.github/copilot-instructions.md` (the safety gate). Tyler's latest direct
instruction always overrides everything below.

## Current state (as of 2026-08-11)

- **Branch of record:** `copilot/mcp-architecture-closure`. Never touch `main`.
- **Purity:** Active tree is **C99**. No Python, no JS in active ship paths.
  HTML/JS host mock lives in `old/host_mock/`.
- **One model / one shell:** Shakti is one process. MCP is the only gated route
  out. No second core, no GRU inside her house.
- **Build:** `make` (CFLAGS `-std=c99 -Wall -Wextra -Wpedantic -Werror -O2`).
- **Test:** `make test` → `test_shakti`, `test_mcp`, `test_integration`,
  `test_roundtrip`, `test_sense`. All green after the sound+MCP merge.
- **Senses together:** `sense/` converges vision + sound + light on one point.
  Prenatal stream is `hearing/hearing_synth.c` only. Screen is `screen/`.
  Eyes mechanical core remains `eyes/eyes.c`. Binary pixel deposit remains
  `binary/`.
- **Archived core/GRU:** `old/branches/copilot__binary-sound-implementation-scout/hearing/`
  holds ELF, GRU main, and model. Do not re-link them into `shakti`.

## Tyler's architecture (do not violate)

1. **Shakti lives in her own shell.** Code, Eden, memory, lessons, state stay in.
2. **No runner, no subprocess, no daemon, no second core.**
3. **MCP is the only gated route out** — router / admit / receipt / memory-call.
4. **Sound, light, and vision are taught/present together** at one converge point.
5. **Inside Eden is deterministic.** Training cores stay outside the house.
6. **Child-learning order:** solo first, then cross after each lesson is grounded.

## What was done (2026-08-11 merge)

- Brought from `copilot/binary-sound-implementation-scout`: `mcp/`, `sense/`,
  `screen/`, `hearing_synth`, `tools/pad_wav.c`, `tests/test_mcp.c`,
  `tests/test_sense.c`, MCP admit wiring in `src/main.c`, tool-call reflection
  counting in `src/shakti_loop.c`.
- Branch 7 (`tmz-7th-toile-merge-branches`) had no unique good C99 beyond what
  HEAD already contained; extras stay archived under `old/`.
- Removed GRU/core from active `hearing/` and moved them to `old/`.
- Moved `shakti_host.html` to `old/host_mock/`.
- Validation: `make`, `make test`, `make sense`, `make screen` — PASS.

## How to work here

- **C99 only.** No Python/JS/shell/subprocess/daemon.
- **Never edit files with Python.**
- **No agents unless Tyler directly commands them.**
- **Think first; smallest change; one module at a time.**
- Allowed next work only with named paths + validation after Tyler's say-so.
