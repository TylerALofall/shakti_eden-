# sense

Files in this section:

- `sense/README.md` (this file)
- `sense/sense.h` (C99 interface: frame capsule + RAM ring)
- `sense/sense.c` (ingest binder, PCM bits, present-to-screen)
- `sense/sense_map.c` (harness: fixture vision+sound + prenatal fallback)

## What this is

Thin binder so vision and sound are **one experience**, not two systems.

**Senses are always on.** She is born with them working; they are not grown
later and not switched off. The ring keeps flowing every tick the harness (and
later the awake loop) feeds — no “sense disabled” path in this module.

**No probability inside Eden.** This section is 100% deterministic: same RGBA +
PCM bytes always yield the same binary, the same render, the same `seq` binding.
There is **no** vector embedding, no guessing model, and no GRU/ML path here.
Prenatal sound/light uses only `hearing/hearing_synth.c` (fixed equations).

Each tick writes one `sense_frame_t` under one `seq`:

1. Vision binary — mono bits (`eyes_pull_mono`)
2. Vision rendered — rebuild pixels (`eyes_reconstruct_mono`)
3. Sound binary — int16 PCM bit string (16 kHz, 10 ms block)
4. Sound context — waveform samples + peak envelope + light flash

Error on vision = any pixel that cannot rebuild (`drift_v`).  
Error on sound = any sample whose int16 identity fails bit round-trip (`drift_s`).

Fixed ring of **8** slots. Constant flow = overwrite oldest. No daemon, no
threads, no dynamic allocation, no Python/JS.

## Ingest seam (Swift later)

Host or harness supplies:

- RGBA page `64 x 64` (camera later fills this; C99 never sees Swift)
- PCM float block of `160` samples (10 ms at 16 kHz; mic later)

Same `sense_ingest` call binds both sides.

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
