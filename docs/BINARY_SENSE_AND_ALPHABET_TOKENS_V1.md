# Binary Sense and Alphabet Tokens V1

Status: design note, awaiting Tyler's ratification
Date: 2026-08-08
Authority: records Tyler's direct decisions from this session; it does not
amend `SHAKTI_LOCK_V1_1.md`.

## Alphabet tokens, not glyph codes

Shakti is learning the shapes of letters and how to write. She does not write
in glyphs; she writes in alphabet tokens. The 8x8 `uint64_t` written-text
bitmap remains the letter's visual shape, but her written output is emitted
as exact Tier 1 alphabet characters, consistent with the lock's two-tier
token system.

## Deterministic core as validator

Her deterministic side controls all senses and acts subconsciously as a
validator. Sense-processing done at the binary level lives outside her
deterministic core; she carries the skills and memory to access those talents
but does not invoke them unless the situation demands it.

## Camera path

The camera remains a future Swift-hosted capability reached only through the
gated MCP route (`SHAKTI_LOCK_V1_1.md`). The current receptor is a simulated
collector. `SHAKTI_RECEPTOR_DEFAULT_LEVEL 5` is a neutral placeholder on the
simulated 0-9 scale, not a camera brightness decision; digital collection is
binary at the pixel (ink or no ink).

## Round-trip fidelity harness

`tests/test_roundtrip.c` rehearses the document path: collect a frame,
binarize each cell to 0/1 (`SHAKTI_RECEPTOR_BINARIZE_THRESHOLD`), reconstruct
the bitmap from the bits, feed it back as the next input, and repeat for 20
passes while counting drifted pixels. It runs under `make test` and reports
per-pass and cumulative loss so we can see what 1-bit collection keeps and
what it loses.
