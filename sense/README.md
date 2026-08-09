# sense

Files in this section:

- `sense/README.md` (this file)
- `sense/sense.h` (C99 interface: one convergence point + RAM ring)
- `sense/sense.c` (converge, PCM bits, present-to-screen)
- `sense/sense_map.c` (harness: fixture + prenatal converge flow)

## What this is

**All senses converge at one point.** Not parallel systems. Sight, hearing,
light (and later senses) meet in a single `sense_point_t` under one `seq`.
There is no vision-only or sound-only commit.

**Senses are always on.** Born with them working — not grown at lesson age,
not switched off.

**No probability inside Eden.** Deterministic only: same RGBA + PCM → same
binary → same render → same point. No vector embedding, no guessing model,
no GRU on this path. Prenatal uses `hearing/hearing_synth.c` only.

At the one point, two stages of the **same** event:

1. Binary first — mono vision bits + sound int16 bits + light scalar
2. Rendered — rebuild vision pixels + waveform context + envelope

Error = cannot rebuild (`drift_v` pixel, `drift_s` sample).

API: `sense_converge(...)` is the sole meet point. `sense_now(ring)` is the
live point. Ring of **8** points, overwrite oldest. No daemon/threads/alloc.

## Pre-Eden path

When no external files are supplied, the harness synthesizes maternal
heartbeat + light via `hearing/hearing_synth.c`:

- Phase 1 dark (0–20 s): light channel is 0
- Phase 2 entrainment (20 s+): light flashes with the heartbeat
- Audio bits and light share the same `seq` every tick

## Build and run

```sh
make sense
make test
```

`make sense` builds `sense/sense_map` and runs it.  
`make test` includes `tests/test_sense`.

Artifacts (if any) go under `sense/output/` only.

## Out of scope here

- Wiring into `src/**`, MCP, Eden School tables
- Swift camera/mic host
- GRU training rewrite
- Full 640x360 binary stored per ring slot (vision capsule is 64x64; screen
  still receives a top-left present of that panel)
