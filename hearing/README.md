# hearing

## Every file in this section

- `README.md` — this file
- `hearing.h` — C99 prenatal stream interface (PCM + light only)
- `hearing_synth.c` — deterministic maternal heartbeat and light flashes

## Role

Prenatal subconscious grounding for sound and light **together**. One mechanical
stream: 16 kHz PCM heartbeat plus a light scalar on the same 10 ms frames.
Phase 1 (0–20 s) is dark acoustic. Phase 2 (20 s+) entrains light with the
heartbeat peak.

This section does **not** hold a second core, GRU, trainer, or probability
model. Those were removed from the active tree. Archived GRU scout sources live
under `old/branches/copilot__binary-sound-implementation-scout/hearing/`.

Shakti stays in her shell. Hearing only fills buffers that `sense/` converges
with vision on one point. MCP remains the only gated route out.

## Build

```text
make sense
make test
```

`hearing_synth.c` links into `sense/sense_map` and `tests/test_sense` only.
No standalone hearing binary is shipped in the active tree.
