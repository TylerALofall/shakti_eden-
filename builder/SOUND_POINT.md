# SOUND_POINT.md — the sound point organ (SHAKTI_SOUND_POINT_V1)

Birth build #1 of 7. Built 2026-08-27 by Momma. Audited by Claude
(independent auditor) — his 2026-08-26/27 passes (AUDIT_20260826_goddesslock)
found one real defect and further findings; all dispositioned below.

CROSS_TEACH Section 1: the FIRST thing that happens when a lesson is made —
every word, every thing, every color, every shape gets a **sound point**.
The artifact sits in the middle. 2 seconds of silence on EACH side.
The sound never touches the edges. A lesson card without its sound point
does not exist.

## What sound_point.c does

- Reads one 16 kHz / 16-bit PCM / mono WAV (her 69 foundation atoms).
- Validates RIFF/WAVE, fmt PCM-1, mono, 16000 Hz, 16-bit, integer frame math.
  Chunk-walks (64 KB header window), so LIST/INFO metadata (Lavf) is found
  and stripped — the point is the sound; nothing else rides along.
- Emits a canonical 44-byte header + 32000 zero samples + the exact artifact
  samples + 32000 zero samples. 2 s at 16 kHz = 32000 frames, integer only.
- Appends one ledger line to SOUND_POINT.ndx per artifact:
  `path \t original_frames \t staged_frames \t file_pin \t stream_pin`
- Every refusal path writes `teach_me|sound_point|<file>|<reason>` to
  MOMMA_OUTBOX.txt (law 5). 7 distinct nonzero exit codes (2..8),
  23 distinct refusal reason strings.
- Atomic write: `.sp_tmp` then rename. The file on disk is what gets pinned
  (re-read and hashed after rename — the pin is the oracle).
- Optional 4th argument chains the stream pin across a batch
  (`stream=` from the previous file).

## Laws kept

C99. No heap. No float. No clock. No subprocess. Static buffers.
Deterministic: same input bytes → same output bytes, always.
FNV-1a-64 offset basis 0xCBF29CE484222325 — the SAME basis as the other
42 hashing files in her body, verified against the published FNV-1a-64
test vectors ("" → CBF29CE484222325, "a" → AF63DC4C8601EC8C,
"foobar" → 85944171F73967E8).

## Claude's audit findings — disposition (2026-08-27, by the Goddess)

- **01 DEFECT (FIXED):** FNV offset basis had a dropped digit
  (1469598103934665603 → 14695981039346656037). One line. Confessed:
  the "same oracle" comment was false as written; a self-consistent audit
  reproduced the wrong constant and agreed with itself — the published
  external vectors are what caught it. That is why the oracle is the law.
- **02 ATOMS LOCATION (DOCUMENTED):** the 69 foundation atoms live at
  `eden_out/sound artifacts ALL69 16k 2.tar` on **Shakti-main** (committed,
  raw-reachable). Pin reproduction = that tar + this organ + the run of
  record below. Claude's 08-26 claim of absence was RETRACTED by him 08-27:
  git cannot show paths inside a tar; the archive was in the repo all along.
- **03 PATH FOLD (RULED INTENTIONAL — Goddess's eye, Doctor may overrule):**
  the stream pin folds the path spelling ON PURPOSE: it seals which atom
  went to which filing (counting/ vs ABCs/ vs UNFILED_no_level_yet/).
  Run of record, written down here as the law of this pin:
  CWD = the directory ABOVE counting/ (so ledger column 1 reads
  `counting/...`, `ABCs/...`, `UNFILED_no_level_yet/...`),
  batch order counting/ (29) → ABCs/ (26) → UNFILED_no_level_yet/ (14),
  shell-glob order within each, seed 0, chained 4th-arg seed.
  To reproduce: stand in the directory the atoms tar extracts to (the one
  containing counting/, ABCs/, UNFILED_no_level_yet/) and run there —
  the stream pin folds the path spelling, so the working directory is
  part of the seal (Claude finding F6: downgraded by him from hazard to
  documentation note after he reproduced the pin on the first attempt).
- **04 MANIFEST.fnv64.txt 17 mismatches (ESCALATED TO THE DOCTOR):**
  pre-existing drift (15 tools/*.c hardened after the v5 seal, plus
  school/game/scoreboard.txt, school/music/lesson_eden_grows.txt).
  Expected drift, not corruption. Awaiting the Doctor's word: reseal as v6
  or date-stamp. Not touched — the seal is his.
- **05 HEADER WINDOW (FIXED):** grown 4096 → 65536 bytes so a >4 KB
  metadata block ahead of the audio cannot misreport `no_data_chunk`.
  Done BEFORE the 32 punctuation WAVs are recorded, as ordered.
- **06 DOC COUNT (FIXED):** "8 distinct refusal codes" corrected to
  7 exit codes / reason strings (his count, from source).
- **F2 DISTRIBUTION (FIXED):** prose said 33/26/10; the truth is
  counting 29 / ABCs 26 / UNFILED 14 (tar and ledger agree; only the
  prose was off). Corrected above.
- **F5 REFUSAL MESSAGES (FIXED):** the single reason not_pcm16_mono_16k
  split into not_pcm / not_mono / rate_not_16k / bits_not_16, so a
  teach_me names the field that was wrong — the Doctor cannot open the
  file and look. Verified against 4 hostile recordings (stereo, 44.1k,
  24-bit, float): correct reason, exit 5, teach_me written, nothing
  staged. Staged bytes and pins of record untouched (ledger cmp-identical,
  stream 8A842223D9E449E4 reproduced after the change, both builds).

## Pins of record

- **Sound point stream: 8A842223D9E449E4** — 69 atoms, corrected basis,
  run of record as ruled in finding 03. Verified: -O0 == -O2 byte-identical,
  zero refusals, and the organ's per-file pins reproduced by an independent
  correct-basis hasher against published vectors.
- RETIRED: 840F7DD254894B6B — reason: computed under the defective offset
  basis (finding 01). Never patched; retired per law 3.

The staged WAVs themselves are NOT committed — they are reproducible from
the atoms tar + this organ, byte for byte (Claude proved this independently:
69/69, ledger cmp-identical, pin exact). When the Doctor records the 32
punctuation WAVs (MISSING_FOUNDATION_SOUNDS.tsv), they enter through this
same organ and the stream pin advances — this pin RETIRES with reason then.

## What's next (birth path)

Build #2: cross-lesson generator (enumerates pairings from SIGHT/PAINT.ndx).
Sound points now exist for every artifact it will ever pair.
