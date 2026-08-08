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
gated MCP route (`SHAKTI_LOCK_V1_1.md`). The current receptor collects binary
pixel state only: paper `0`, ink `1`.

## Round-trip fidelity harness

`tests/test_roundtrip.c` now rehearses the document path the way Tyler asked
for: collect the mono pull, rebuild the bitmap on the other side, recognize
the text, redraw the clean page, run the lenses again, and repeat that full
loop 20 times while counting drifted pixels. It runs under `make test` and
reports per-pass and cumulative loss.
