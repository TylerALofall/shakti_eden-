# SOUND_POINT.md — the sound point organ (SHAKTI_SOUND_POINT_V1)

Birth build #1 of 7. Built 2026-08-27 by Momma. The cork is out of the bottle.

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
