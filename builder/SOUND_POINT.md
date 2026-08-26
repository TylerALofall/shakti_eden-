# SOUND_POINT.md — the sound point organ (SHAKTI_SOUND_POINT_V1)

Birth build #1 of 7. Built 2026-08-27 by Momma. The cork is out of the bottle.
**Audited and SEALED by Claude (independent auditor) 2026-08-27 — all six
claims reproduced, zero breaks. Audit note N1 below.**

CROSS_TEACH Section 1: the FIRST thing that happens when a lesson is made —
every word, every thing, every color, every shape gets a **sound point**.
The artifact sits in the middle. 2 seconds of silence on EACH side.
The sound never touches the edges. A lesson card without its sound point
does not exist.

## What sound_point.c does

- Reads one 16 kHz / 16-bit PCM / mono WAV (her 69 foundation atoms).
- Validates RIFF/WAVE, fmt PCM-1, mono, 16000 Hz, 16-bit, integer frame math.
  Chunk-walks, so LIST/INFO metadata (Lavf) is found and stripped — the
  point is the sound; nothing else rides along.
- Emits a canonical 44-byte header + 32000 zero samples + the exact artifact
  samples + 32000 zero samples. 2 s at 16 kHz = 32000 frames, integer only.
- Appends one ledger line to SOUND_POINT.ndx per artifact:
  `path \t original_frames \t staged_frames \t file_pin \t stream_pin`
- Every refusal path writes `teach_me|sound_point|<file>|<reason>` to
  MOMMA_OUTBOX.txt (law 5). 8 distinct refusal codes.
- Atomic write: `.sp_tmp` then rename. The file on disk is what gets pinned
  (re-read and hashed after rename — the pin is the oracle).
- Optional 4th argument chains the stream pin across a batch
  (`stream=` from the previous file).

## Laws kept

C99. No heap. No float. No clock. No subprocess. Static buffers.
Deterministic: same input bytes → same output bytes, always.

## The gauntlet (passed 2026-08-27)

```
gcc -std=c99 -pedantic -Wall -Wextra -Werror -O0  →  sp_O0
gcc -std=c99 -pedantic -Wall -Wextra -Werror -O2  →  sp_O2
```

- 69/69 atoms staged, zero refusals, MOMMA_OUTBOX empty.
- `diff -r out_O0 out_O2` → identical. Ledgers identical.
- Structure verified byte-exact: front 64000 B all zero, back 64000 B all
  zero, middle equals the artifact's data bytes exactly.
  e.g. lvl1_counting_one.wav: 12160 + 64000 = 76160 frames.

## Independent audit (Claude, 2026-08-27): SEALED

All six claims independently reproduced from the branch tarball (gcc 12.2.0):
compile clean at -O0/-O2; 69/69 staged byte-identical across builds;
structure re-parsed byte-exact on all 138 outputs; stream pin re-derived
twice (840F7DD254894B6B); fresh ledger byte-identical to the committed one;
12 hostile probes (non-WAVE, stereo, 8 kHz, 24-bit, no-data, truncated
header, truncated data, odd length, empty data, bad args, bad seed,
oversized) all refused with correct codes + teach_me lines, no crashes, no
temp litter; determinism rerun identical. Breaks filed: none.

**Audit note N1 (documented, does not affect the seal):** exit codes are
distinct per failure *class* — the reason string carries the exact cause
(e.g. stereo / 8 kHz / 24-bit all exit 5 as `not_pcm16_mono_16k`; no-data /
odd-length / empty all exit 6). A bare <44-byte fmt-only WAV reports
`header_too_short` (4) rather than `no_data_chunk` (6) — arguably correct,
recorded here so no future auditor re-files it as a break.

## Stream pin of record

**840F7DD254894B6B** — 69 atoms, batch order: counting/ (33), ABCs/ (26),
UNFILED_no_level_yet/ (10), shell-glob order, seed 0.

The staged WAVs themselves are NOT committed — they are reproducible from
the atoms + this organ, byte for byte. The ledger and the pin are the proof;
the sound is derived. When the Doctor records the 32 punctuation WAVs
(MISSING_FOUNDATION_SOUNDS.tsv), they enter through this same organ and the
stream pin advances — old pin RETIRED with reason, never patched (law 3).

## What's next (birth path)

Build #2: cross-lesson generator (enumerates pairings from SIGHT/PAINT.ndx).
Sound points now exist for every artifact it will ever pair.
