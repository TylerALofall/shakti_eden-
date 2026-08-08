# eyes

Shakti's document collector and the reconstruction side that did not exist
before. Everything is plain C99, deterministic, no subprocess, no dynamic
allocation. Build and run from the repository root:

```sh
make eyes        # builds eyes/eyes_map, then runs it
```

`make eyes` runs a handful of documents through the full pipeline, writes
every artifact into `eyes/output/`, prints the section map, and sweeps
page sizes until the collector reports an error (the breaking point).

## What is actually collected

A document is a grid of RGBA pixels. Two pulls run on every document:

- **Color pull** (`eyes_pull_color`): for each pixel, luma decides ink vs
  paper. Ink keeps its dominant RGB channel; paper is forced white. A pull
  bit is `1` only when that channel is a full-strength primary (one channel
  saturated, the other two zero). That is the old color-printer fact: a real
  black is three colors mixed, so no single channel reads saturated, and the
  color pull **does not see black text**. It only keeps saturated primaries.

- **Mono pull** (`eyes_pull_mono`): luma at or above the midpoint is paper
  `0`, below is ink `1`. This is the text side, and on clean documents it is
  lossless.

The classifier sits **before** these pulls. Its only job is to look at a
page and decide whether it is color or text. This module does not make that
decision; it pulls **both** ways every time so the two can be checked
against each other.

## The other side (reconstruction)

`eyes_reconstruct_color` / `eyes_reconstruct_mono` turn collected bits back
into a bitmap. The old `test_roundtrip` never rebuilt anything -- it fed the
bit string back through binarize and compared text, so its "0 pixels lost"
was trivially true and proved nothing about fidelity. Here the bits actually
become pixels again, and `eyes_diff` compares the reconstruction against the
original **per pixel**.

## Honest loss numbers (from `eyes/output/summary.tan`)

- **Mono pull: drift 0 at every size.** Ink/paper survives the round trip
  exactly, and 20 cycles of pull -> reconstruct -> re-pull drift 0 bits.
- **Color pull: lossy on purpose.** On a black-text page the text is lost
  (e.g. 32x32 `text_hi` loses 101 pixels = the text). On a saturated-color
  pattern the primaries survive (green/blue band boundaries show up as
  drift). This is the measured answer to "did we lose nothing" -- mono yes,
  color no.
- **Text recognition**: `eyes_recognize_text` matches each 8x8 cell against
  the clean 5x7 font and redraws it clean (`*_clean.tan`). It recovers
  exactly the characters that fit on the page (a 16x16 page fits 2 columns,
  so "HI EDEN" clips to "HI"). Nothing is guessed.
- **Breaking point**: the sweep escalates N x N and stops when a page
  exceeds the configured capacity (`EYES_MAX_PIXELS`). It currently reports
  the first error at **2112x2112** (4,194,304-pixel ceiling). Drift never
  appeared at any size before that -- the limit is capacity, not fidelity.
  Raise `EYES_MAX_PIXELS` to push past 5000; the buffers grow accordingly.

## Output structure

`eyes/output/` holds one set per document and size:

- `<tag>_<WxH>_color.tan` / `<tag>_<WxH>_mono.tan` -- the raw collected pull
  (`EYES_FRAME_V1`: `WIDTH= HEIGHT= KIND= TEXT=` + a grid of bits).
- `<tag>_<WxH>_color_recon.tan` / `<tag>_<WxH>_mono_recon.tan` -- the
  reconstructed bitmap (`EYES_RECON_V1`), `#` ink / `.` paper, one plane per
  RGB channel plus LUMA in color mode, a single INK plane in mono mode.
- `<tag>_<WxH>_<side>_clean.tan` -- the recognized text redrawn in the clean
  font.
- `summary.tan` -- per-document drift, recognition counts, the recovered
  text, and the sweep result.

## Loading / Swift attachment

Everything goes through one boundary: hand `eyes_load_document` (or, later,
the Swift host) raw RGBA bytes plus a width and height, and get back bits
(`eyes_pull_color` / `eyes_pull_mono`) and text (`eyes_recognize_text`).
The collector owns no file handles, no threads, no state between calls, so a
Swift host can attach at that seam without the module knowing Swift exists.

## Recommended ingestor path

The last receptor PR was useful as a first prompt, but the honest rebuild
comparison belongs here in `eyes/`, not in a simulated `0..9` brightness
collector. The convergence check that matters is:

1. ingest pixels,
2. pull mono bits,
3. rebuild the bitmap on the other side,
4. recognize text,
5. redraw the clean page,
6. run the lenses again,
7. compare that rebuilt page back to the original per pixel for 20 loops.

For a future video ingestor, keep this same seam and normalize frames before
the pull: accept raw host frames, optionally resize them to a small approved
set of standard page sizes, then sample at low FPS so the deterministic core
only sees stable document frames instead of camera noise. That keeps the
comparison point fixed while making a later video processor easy to swap in.
