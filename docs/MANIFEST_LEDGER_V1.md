# Eden Manifest and Ledger Contract V1

## Purpose

The XML manifest is the curriculum source of truth. C code implements generic loading and validation only. Adding, removing, ordering, or experimenting with lessons happens in XML.

## Manifest structure

```xml
<?xml version="1.0" encoding="UTF-8"?>
<manifest schema="SHAKTI_EDEN_MANIFEST_V1">
  <status>OPEN</status>
  <artifact_root>../../eden_out</artifact_root>

  <level order="1">
    <id>counting</id>
    <title>Counting zero through one hundred</title>
    <requires>foundation_ascii</requires>
  </level>

  <tablet order="1">
    <level>counting</level>
    <lesson>counting_zero_to_ten_solo</lesson>
    <mode>solo</mode>
    <requires>foundation_ascii</requires>
    <state>planned</state>
    <path>XML_text/01_counting_zero_to_ten_solo.xml</path>
  </tablet>
</manifest>
```

## Inheritance

A level's `requires` field lists every earlier level that must already be mastered. Every required level must exist and have a lower order.

The current cumulative rule is:

```text
counting requires foundation
alphabet requires foundation and counting
colors require foundation, counting, and alphabet
shapes require foundation, counting, alphabet, and colors
math requires every prior level
```

## Tablet structure

```xml
<?xml version="1.0" encoding="UTF-8"?>
<tablet schema="SHAKTI_TABLET_4S_V2">
  <level>counting</level>
  <lesson>counting_zero_to_ten_solo</lesson>
  <stone_count>1</stone_count>

  <stone order="1">
    <text>one</text>
    <written_text>one.8x8.txt</written_text>
    <sound_art>one.wav</sound_art>
    <visual_art>one.svg</visual_art>
  </stone>
</tablet>
```

The XML must place one element per line. The restricted parser intentionally rejects broad XML behavior such as external entities, mixed content, and hidden transformations.

## Filename contract

The source text remains exact.

The asset key is deterministic:

```text
one -> one
Welcome to Eden -> Welcome_to_Eden
/ -> ascii_047
2 + 2 = 4 -> 2_%2B_2_%3D_4
```

Atomic stones use one key across all artifacts:

```text
KEY.8x8.txt
KEY.wav
KEY.svg
```

Composite stones keep the full text key for written and visual artifacts while the sound channel reuses approved atomic WAV files:

```text
Aa.8x8.txt
A.wav|a.wav
Aa.svg
```

The vertical bar separates clips inside one sound channel. Every clip is validated independently.

## Validation

A ready stone passes only when:

```text
the tablet schema is exact
stone order is monotonic
declared stone count matches
text produces the listed filenames
written text contains the exact source text
every character contains exactly eight rows of eight pixels
every sound clip is RIFF/WAVE, PCM, mono, 16-bit, and 16 kHz
sound sequences contain one through 64 safe `.wav` filenames
visual art contains an SVG root
```

## Ledger states

```text
PLANNED
READY
TABLET_LOAD_FAILED
```

A failed ready tablet causes `build_ledger` to return failure after writing the diagnostic row.

## Safety

The loader does not repair files, rename files, promote planned tablets, create missing sound, or treat a placeholder as sensory evidence.
