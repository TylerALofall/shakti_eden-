# Alphabet Curriculum V1

## Invariant

Every alphabet concept is presented alone before it is combined with counting, case, or sequence.

The alphabet generator reuses the canonical 8x8 character atlas. It does not define a second copy of any letter.

## Sound sequence contract

A single sound channel may reference one or more approved WAV files:

```xml
<sound_art>A.wav</sound_art>
```

or:

```xml
<sound_art>A.wav|a.wav</sound_art>
```

Every component must:

- be a safe filename,
- end in `.wav`,
- exist inside `Sound_art/`,
- validate as mono 16-bit PCM at 16 kHz.

The loader rejects empty clips, trailing separators, unsafe filenames, invalid WAV files, and sequences longer than 64 clips.

## Tablet coverage

### Uppercase solo

All 26 uppercase letters appear independently.

### Lowercase solo

All 26 lowercase letters appear independently.

### Case pairs

All 26 case pairs appear as separate bound stones:

```text
Aa
Bb
Cc
...
Zz
```

### Counting bridge

The grounded number words one through ten are bound to the first ten case pairs:

```text
1 A a
2 B b
...
10 J j
```

The generator stops there rather than inventing ungrounded pronunciations for eleven through twenty-six.

### Growing sequences

The elastic tablet contains:

```text
A
AB
ABC
...
ABCDEFGHIJKLMNOPQRSTUVWXYZ
```

```text
Ab
Abc
...
Abcdefghijklmnopqrstuvwxyz
```

```text
Aa
AaBb
AaBbCc
...
AaBbCc...Zz
```

The sequence sound channel calls the approved letter WAV files in order.

## Confidence

Each stone has four exact validation checks. A complete tablet score is the sum of those checks. Validation confidence is not a probability.

```text
passed / required = percent
```

A stone becomes lock-eligible only at `4/4 = 100%`.

Eden remains open until Tyler reviews the curriculum and all required channels.
