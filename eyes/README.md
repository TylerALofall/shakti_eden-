# eyes

Files in this section: `README.md`, `eyes.c`, `eyes.h`, `eyes_loop_rebuild.c`,
`eyes_map.c`, `eyes_xml.h`, `eyes_xml_collect.c`, `eyes_xml_rebuild.c`,
`output/`

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

## EYES_XML_V1 exact format

The additive XML collector/rebuild pair in this section writes one XML file per
document:

```text
<EYES_DOCUMENT version="1" pages="N">
  <PAGE number="1" width="W" height="H" kind="mono">
    <BITS>
ROW_1_EXACTLY_W_BITS
ROW_2_EXACTLY_W_BITS
...
ROW_H_EXACTLY_W_BITS
    </BITS>
  </PAGE>
  ...
</EYES_DOCUMENT>
```

Grammar rules:

- `<EYES_DOCUMENT version="1" pages="N">` must match exactly.
- Each `<PAGE ... kind="mono">` must match exactly.
- `<BITS>` contains row-major `'0'` / `'1'` characters only.
- There is one row per line, and every row length must equal the page width.
- The parser is strict: malformed tags, malformed counts, unexpected trailing
  content, short/long rows, or any non-bit character are rejected.

## XML collect flow

`eyes/eyes_xml_collect.c` is the collector-side standalone program. It uses the
locked `eyes.h` core only:

1. Load the fixed deterministic document table from `eyes_xml.h`.
2. For each page, call `eyes_load_document`.
3. Pull exact mono bits with `eyes_pull_mono`.
4. Write the original page to `eyes/output/<tag>_page<N>_original.tan` with
   `eyes_write_recon` so the starting pixels are inspectable without shared
   memory.
5. Write all pages into `eyes/output/<tag>_document.xml` as one `EYES_XML_V1`
   document.

## XML rebuild flow

`eyes/eyes_xml_rebuild.c` is the other-side standalone program:

1. Open `eyes/output/<tag>_document.xml` (or a direct `.xml` path).
2. Parse `EYES_XML_V1` with an exact hand-written C99 parser.
3. Rebuild each page with `eyes_reconstruct_mono`.
4. Regenerate the original deterministic page with the same table in
   `eyes_xml.h`.
5. Run `eyes_diff(..., color=1)` so drift `0` means exact RGBA pixel identity,
   not a text-only or binarized shortcut.
6. Print `page N: WxH drift=D` and exit nonzero on any drift or parse failure.

## Drift=0 proof from a real run

Real build/run output from this repository state:

```text
$ make eyes-xml-collect eyes-xml-rebuild
cc -std=c99 -Wall -Wextra -Wpedantic -Werror -O2 -Ieyes eyes/eyes_xml_collect.c eyes/eyes.c -o eyes/eyes_xml_collect
./eyes/eyes_xml_collect
collected page 1: 16x16 -> eyes/output/eyes_xml_page1_original.tan
collected page 2: 32x16 -> eyes/output/eyes_xml_page2_original.tan
collected page 3: 40x24 -> eyes/output/eyes_xml_page3_original.tan
collected page 4: 64x24 -> eyes/output/eyes_xml_page4_original.tan
wrote eyes/output/eyes_xml_document.xml
cc -std=c99 -Wall -Wextra -Wpedantic -Werror -O2 -Ieyes eyes/eyes_xml_rebuild.c eyes/eyes.c -o eyes/eyes_xml_rebuild
./eyes/eyes_xml_rebuild
page 1: 16x16 drift=0
page 2: 32x16 drift=0
page 3: 40x24 drift=0
page 4: 64x24 drift=0
PASS: exact pixel rebuild on all 4 pages
```

Measured drift numbers from the real rebuild:

- page 1: 16x16 drift=0
- page 2: 32x16 drift=0
- page 3: 40x24 drift=0
- page 4: 64x24 drift=0

Corrupt-input proof from a manual one-bit flip in a copied XML file:

```text
$ ./eyes/eyes_xml_rebuild /tmp/eyes_xml_document_bad.xml
page 1: 16x16 drift=1
FAIL page 1: drift=1
page 2: 32x16 drift=0
page 3: 40x24 drift=0
page 4: 64x24 drift=0
$ echo $?
1
```

That is the no-theater check: one changed bit produced one changed pixel and a
nonzero exit. This is the exact output shape: drift failures print every page
then return `1` after the loop without adding a summary line, while parse/setup
failures reject immediately at the point of the malformed input.

## How to run the XML path

Build and run from the repository root:

```sh
make eyes-xml
```

That target builds `eyes/eyes_xml_collect` and `eyes/eyes_xml_rebuild`, runs
the collector, then runs the rebuild and fails the make if the rebuild exits
nonzero.

## The 20-iteration rebuild loop (eyes_loop_rebuild)

`eyes/eyes_loop_rebuild.c` is the standalone loop on top of the locked core:
the exact rebuild runs 20 times, each iteration's rebuilt page feeds the next
iteration, and the whole run is collected into one dated PDF document. The
chain is the proven-exact path (mono pull -> mono reconstruct); the clock is
read once, only for the dated output filename.

Build and run from the repository root:

```sh
make eyes-loop        # builds eyes/eyes_loop_rebuild, then runs it
./eyes/eyes_loop_rebuild [tag]   # optional tag, default eyes_loop
```

What one run produces under `eyes/output/loop/`:

- `iter_01/` .. `iter_20/` -- one sub dir per iteration, each with its own:
  - `page<N>_color.tan` -- the rebuilt page written in color mode
    (`EYES_RECON_V1`, R/G/B/LUMA planes);
  - `page<N>_bw.tan` -- the same rebuilt page written in black and white
    (single INK plane);
  - `hash.txt` -- FNV-1a 64 hash per page plus the iteration hash (FNV-1a is
    the repository's existing hash family, see `src/shakti_log.c`);
  - `log.txt` -- per-page size, `drift_vs_original` (exact RGBA compare via
    `eyes_diff(..., color=1)`), page hash, iteration hash, and whether the
    iteration hash matches iteration 1.
- `YYYY-MM-DD-<tag>.pdf` -- the collected output document, dated with the run
  day. It is a picture and text PDF written by a plain C99 writer (no
  libraries, uncompressed): one picture page per document page showing the
  original in color, the iteration-20 rebuild in color, and the iteration-20
  rebuild in black and white, followed by the full run log as text pages,
  ending in the `RESULT:` line.

The 20th time is assured on point two ways, both hard checks: every page of
every iteration must show `drift_vs_original=0` (exact RGBA identity with the
original document), and the iteration-20 hash must equal the iteration-1
hash. Any miss prints `FAIL` and exits nonzero; the pass line is only printed
after both hold.

Real output from a run in this repository state:

```text
$ make eyes-loop
cc -std=c99 -Wall -Wextra -Wpedantic -Werror -O2 -Ieyes eyes/eyes_loop_rebuild.c eyes/eyes.c -o eyes/eyes_loop_rebuild
./eyes/eyes_loop_rebuild
iteration 01: hash=fnv1a64:3CCD53E81E3FDA8D match_iteration_1=YES
...
iteration 20: hash=fnv1a64:3CCD53E81E3FDA8D match_iteration_1=YES
wrote eyes/output/loop/2026-08-09-eyes_loop.pdf
PASS: iteration 20 exact, drift 0 on every page, hash matches iteration 1
```

All 20 iteration hashes are identical, and the produced PDF checks clean
(`qpdf --check`: no syntax or stream errors; 7 pages: 4 picture pages + 3
text pages).

## Session handoff

### What was done (this session, 2026-08-09: the 20-iteration loop)

- Added `eyes_loop_rebuild.c`: the 20-iteration exact rebuild loop with a sub
  dir per iteration (`eyes/output/loop/iter_NN/`), each holding the rebuilt
  pages in color and black and white `.tan`, its own FNV-1a 64 `hash.txt`,
  and its own `log.txt`.
- The loop assures the 20th iteration is still on point: `drift_vs_original=0`
  on every page and iteration-20 hash equal to iteration-1 hash, else `FAIL`
  and nonzero exit.
- The collected output is one dated picture-and-text PDF,
  `eyes/output/loop/YYYY-MM-DD-<tag>.pdf`, from a plain C99 uncompressed PDF
  writer inside the same file (no libraries, no subprocess).
- Added `Makefile` targets `eyes/eyes_loop_rebuild` and `eyes-loop`; added the
  built eyes binaries to `.gitignore`.
- Everything is additive; `eyes.c`, `eyes.h`, `eyes_map.c`, and the XML pair
  were left untouched. Validation run: `make eyes-loop` (PASS, all 20 hashes
  identical), `make`, `make test`, `qpdf --check` clean on the produced PDF,
  and a re-run plus a custom-tag run both PASS.

### What the next session should know (this session)

- The chain feeds each iteration's mono rebuild into the next iteration; on
  the kind-0 pages this is exact, so the color `.tan` and the color PDF image
  are the true color rendering of the rebuilt page (black ink on white
  paper). A color-preserving chain for kind-1 pages is still the separate
  approved path noted below.
- The clock is used only for the `YYYY-MM-DD` output filename; collection and
  comparison stay deterministic.

### What was done (previous session: the XML path)

- Added `eyes_xml.h` with the fixed multi-page document table and XML API
  declarations.
- Added `eyes_xml_collect.c` to write per-page mono binaries into one XML file
  plus per-page original `.tan` references.
- Added `eyes_xml_rebuild.c` to parse that XML strictly, rebuild the pixels, and
  prove exact RGBA drift `0` page by page.
- Added `Makefile` targets `eyes-xml-collect`, `eyes-xml-rebuild`, and
  `eyes-xml`.

### What the next session should know (previous session)

- The XML path is additive only; `eyes.c`, `eyes.h`, and `eyes_map.c` were left
  untouched.
- The rebuild accepts either a tag (default `eyes_xml`) or a direct `.xml`
  path, which is how the manual corruption check was run.
- Validation already run on this state: `make eyes-xml-collect
  eyes-xml-rebuild`, `make eyes-xml`, `make`, and `make test`.

### What I would do differently next time

- If Tyler later approves an additive color-preserving XML path, I would split
  mono-XML and color-XML explicitly instead of trying to make one exact format
  carry both fidelity guarantees.

### What remains UNKNOWN

- `eyes_load_document(kind=1)` generates saturated RGB pages, but the locked
  exact-fidelity XML path here is mono pull + mono rebuild. Exact RGBA
  drift=`0` for kind `1` pages would require a separate approved color-preserving
  additive path; it cannot be made exact through mono bits alone without
  changing the locked core.
