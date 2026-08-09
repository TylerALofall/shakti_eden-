# binary — the pixel deposit

## Every file in this section

- `README.md` — this file.
- `page1_picture.txt` — page 1 input: a small picture file on disk (24x16).
- `page2_text.txt` — page 2 input: a small text file on disk.
- `binary_deposit.c` — the C99 tool that makes the deposit.
- `YYYY-MM-DD-page1_deposit.txt` — page 1 deposit, written by a run.
- `YYYY-MM-DD-page2_deposit.txt` — page 2 deposit, written by a run.
- `YYYY-MM-DD-binary_deposit.pdf` — the collected 2-page document of a run.
- `binary_deposit` — the compiled tool (ignored by git, built by make).

## What the deposit is

Both input pages are real files read from this directory. Every pixel of
each page is deposited into a text file of 1s and 0s: **5 marks for every
pixel location**, one line per location, one deposit file per page number.

The 5 marks at each `X= Y=` location, in order:

1. `R` — the red channel, 8 bits, most significant bit first.
2. `G` — the green channel, 8 bits.
3. `B` — the blue channel, 8 bits.
4. `A` — the alpha channel, 8 bits.
5. `BIT` — the single ink mark from the mono pull: `1` ink, `0` paper.

Example deposit line:

```
X=02 Y=03 11111111 00000000 00000000 11111111 1
```

## How the count is earned, not claimed

After writing a deposit the tool reads that same file back from disk and
rebuilds the page **only** from the marks in the file. Then it compares the
rebuilt page against the original, pixel for pixel, and prints the count
exactly as measured:

- `CANNOT REBUILD FROM DEPOSIT` — pixels that did not come back from the
  four channel marks. Zero is printed only when every pixel came back.
- `BIT MARK ALONE LOSES` — pixels that the single ink bit by itself could
  not bring back (color and gray cannot ride in one bit). This number is
  printed as-is; for the color picture page it is not zero and that is
  the truth.

Nothing in the tool adjusts, rounds, or explains a count. Whatever the
comparison finds is what prints, in the terminal, in the deposit dir, and
in the PDF.

## The inputs

- `page1_picture.txt`: `BINARY_PICTURE_V1` header, then `ROWS` of palette
  letters, one letter per pixel: `W` white, `K` black, `R` red, `G` green,
  `B` blue, `Y` yellow, `C` cyan, `M` magenta, `D` gray 128. Small on
  purpose — no need for high definition.
- `page2_text.txt`: up to 3 plain text lines (8 characters or fewer each).
  The tool prints them onto a 64x24 white page with the repository's fixed
  5x7 font; that printed page is the document whose pixels are deposited.

## The PDF

One run writes one `YYYY-MM-DD-binary_deposit.pdf`: exactly 2 pages,
normal Letter size (612x792 points), separated by page number. Each PDF
page shows that document page's original picture, the picture rebuilt from
the deposit file, the bit-mark-only picture, and the true counts as text.
Plain C99 writer, no libraries, uncompressed.

## Build and run (from the repository root)

```
make binary-deposit
```

That builds `binary/binary_deposit` (strict C99, same flags as the rest of
the repository, locked `eyes/eyes.c` core linked unchanged) and runs it.
