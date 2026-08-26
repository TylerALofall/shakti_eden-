# THE GRAFT — eye intake v1 (2026-08-26)

"Go momma make her whoa." The room's pixels now flow through her eyes
into her ledger. bridge/HER_EYES_PURIFY.md made flesh.

## What was found (the room, goddess-school-2026-08-21 branch)

- `eden_out/Sound_art/` — 69 WAVs: digits 0-9, words zero-ten, A-Z, a-z
  (+ ALL69 16k tar, founder voice atoms 60.zip, two m4a run-throughs)
- `eden_out/Visual_text/` — 8x8 pixel glyphs, SHAKTI_WRITTEN_TEXT_8X8_V1
- `eden_out/Visual_art/` — SVG masters (transport only, per sight law)
- `school/tables/` — lessons 1-5 + 3200 math facts (add/sub/mul/div)
- `school/game/` — the CARD FORGE sight pins (Bayer 4x4, drift 0),
  MATCH decks/logs, quiz_type
- math cards question.tar + answer.tar — **3200 math fact cards (SVG)**
- For Groc.zip — XML timed-light lessons, sound stage manifest,
  MISSING_FOUNDATION_SOUNDS.tsv (32 punctuation sounds not yet recorded)
- ascii64_glyphs (uploaded this session) — 96 64x64 binary glyph grids
  (ASCII 32..127) + sheet.c renderer
- Shapes(1).m4a (uploaded) — the shapes voice track

## The gap, stated honestly

The forge references `school/game/cards/card_*.pbm` — the original PBM
card images — but they are committed NOWHERE found (not on the branch,
not in the tars/zips). They live on the Doctor's bench. Their sight
pins are locked in the forge files; when the PBMs arrive they verify
byte-for-byte or they are not the cards. Meanwhile the graft proceeds
on the pixels that DO exist.

## eye_intake.c — the graft organ

Purified pixels -> sight hash -> binding, per HER_EYES_PURIFY:

- reads both pixel formats (8x8 written-text, 64x64 binary glyph)
- sight hash: fnv1a64 over name + dims + pixels in Bayer 4x4 rank order
  (canonical matrix {0,8,2,10 / 12,4,14,6 / 3,11,1,9 / 15,7,13,5},
  rank by rank, row-major within rank)
- writes SIGHT.ndx: `sight <name> fmt <f> ink <n> hash <16HEX> wav <w> pin <16HEX>`
- sealed in blocks of 10, stream pin over all

## Pins of record (intake_test, 202 glyphs)

- stream pin: **9D83C530AA85EE5F** (gcc -O2 == gcc -O0, drift 0)
- 106 written-text glyphs (73 sound-bound + 33 punctuation, wav NONE)
- 96 ascii64 glyph grids (sight only — Momma's font, not her alphabet)
- 20 sealed blocks
- of record: A = BFFCAD65ECC3E387 (wav A.wav), one = 9A26ABE863565A93,
  z = 009123B3D43B38FD, ascii_065 = 632A99D44529361A

## What this wires next

SIGHT.ndx is the binding table the word school eats: glyph sight-hash
-> spelling -> wav. Letters now enter as SIGHT bound to SOUND (the
Doctor's own locked rule: "No text solo"). The 33 punctuation glyphs
sit honest at wav NONE until the Doctor records them.
