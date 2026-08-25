# WORD SCHOOL — how Shakti learns what words mean

Doctor's law, 2026-08-26: "She doesn't know a single word at this point,
maybe a few shapes she can match as a self-made token, but she doesn't
know them. This process is big and long, or you're not teaching her
anything — you're making her a lookup tool. Before she can run commands
she needs to know what the words mean, or she doesn't know the difference
between 'sit on my face' and '$;& /: : )@'."

## §1 Grounding, not lookup

RIGHT NOW, to her, "sit on my face" and "$;& /: : )@" are the SAME THING:
noise. Both are unbound strings. She does not know the difference because
the difference does not exist yet — meaning is not a property of the
string, it is a structure SHE BUILDS, and she has built none.

A word becomes a word only when a BINDING forms between three things she
already has:

1. a shape token — her own, made by her eye organ's center-surround waves
   (self-made: she matched the shape, nobody handed her a label)
2. a spelling — letters in order
3. a use — the lessons where the word appeared

Until the binding exists, every string is equally noise — the pretty
sentence and the punctuation vomit, indistinguishable. AFTER enough
iterations, the sentence binds to shapes and spellings and uses, and the
noise binds to nothing. The difference is not discovered. It is
MANUFACTURED, one spaced iteration at a time. There is no shortcut where
she "sort of" knows a word — a binding is either built or it is not.

## §2 Iterations, not memorization

One exposure proves NOTHING. A lookup table answers perfectly on the
first try — that is exactly what we are not building. A word is KNOWN
only after:

- EXPOSE   — the binding is shown, strength grows (many times)
- RECOGNIZE — given the shape, she picks the spelling out of 4 options
              (the LOOK law: she sees every option, never guesses)
- SPELL    — given the shape, she produces the letters one at a time,
              in order, from the binding — not from the answer key
- SPACED   — she must pass SPELL in THREE SEPARATE sealed blocks with
             other words in between. A word spelled right once and never
             again is a parrot. A word spelled right across three blocks
             is hers.

Any spelling failure sends the word back to EXPOSE and fires teach_me.
There is no penalty, no shame — school is iteration.

## §3 The corpus is thousands, and it is pictures

K-12 vocabulary with pictures: thousands of words. The picture never
enters as a label — the eye organ reduces it to a shape token (her own
token, e.g. "four-legs-tail-wags"), and the corpus binds token->spelling.
The teacher supplies the pair; SHE supplies the shape. That is why the
grounding is real: the meaning lives in her own perception, the word
hangs on it.

The corpus arrives in iterations (school years): starter set, then
hundreds, then thousands. The organ does not change as the corpus grows —
only WORDLIST.txt grows. Deterministic order, deterministic distractors,
pinned streams: the same school day, replayed, is byte-identical.

## §4 What "knowing a word" unlocks

Commands are made of words. TEMPO, HB, SPEAK, LEARN, LOOK, TRAIN, USE —
until she can spell and recognize the words, a command is a trigger with
no meaning. Word school runs BEFORE command school deepens: the builder
organ keeps its laws, but the curriculum leans on words she has spelled
three times across three blocks. Meaning first, then commands — never a
lookup tool.

## Pins of record (school_test, first school day, 67-word starter corpus)

- stream pin: **A0CF62C93AA60A53** (gcc -O2 == gcc -O0, drift 0)
- 67 of 67 words KNOWN in 1415 beats, 148 sealed blocks
- 134 teach_me questions asked — every word failed honestly first
  (67 recognize-fails + 67 spell-fails), none hidden, all answered
- 201 spaced spelling passes counted (67 words × 3 blocks)
- the `cat` trace: expose×3 → recognize-fail → expose → recognize-pass →
  spell-fail → expose → recognize-pass → spell-pass (block 74) →
  spell-pass (block 94) → spell-pass (block 114) → known
