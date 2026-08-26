# SHAKTI CURRICULUM FORMAT — v1
## The format + mission orders for external builders (GPT / Claude)

Doctor's law, 2026-08-26: "Order is everything. Don't reinvent what the
masters spent hundreds of years and more money than anything on earth.
Spelling it doesn't mean she knows what it is. The clock: 132 positions,
43,200 times. Don't reinvent the wheel — make it all work where nothing
gets taught without all but one thing prior taught."

Governing sources, in order of authority:
1. Ordered_Human_Learning_Map.xlsx (475 grounded topic nodes, birth→G12
   + advanced bridge; Topic IDs stable; Knowledge Graph is the edge list)
2. CCSS Progressions 2023 (the masters' ordering inside math strands)
3. Her organs, already built and pinned: builder/word_school.c
   (EXPOSE → RECOGNIZE → SPELL → 3 spaced blocks), builder/BUILDER.md
   (commands, brand, teach-me trigger)

---

## §1 THE ONE-NEW-THING RULE (order is everything)

Every lesson introduces AT MOST ONE new node. Formally:

- Each lesson has exactly one `new` field: the Topic ID being introduced.
- Every other element in the lesson must reference Topic IDs already
  marked KNOWN in her Master Sequence tracking.
- A lesson is REJECTED at intake if any referenced ID is not known.
  There is no skip, no reorder, no "she'll pick it up." The queue waits.
- This is the map's own core rule enforced in code: incoming
  prerequisites → representation → links → use → evidence.

## §2 GROUNDING RULE (spelling ≠ knowing)

No word enters WORDLIST.txt without a card (picture). No card enters
without a word. The binding is triple and mandatory:

  shape token  ←  HER eye organ compresses the card (self-made)
  spelling     ←  the letters, from the corpus
  use          ←  the lesson(s) where the word appears

A submission of words without cards, or cards without words, is
rejected whole. `$;& /: : )@` and "sit on my face" are the same noise
until all three legs of the binding exist.

## §3 CARD FORMAT (pictures she can actually see)

Her eye organ is center-surround waves on a fixed grid, saturate 0–7.
Cards MUST be deterministic SVG obeying:

- Canvas: exactly 240×272 px, `width="240" height="272"`, white or
  single flat background rect covering 100%.
- Flat fills only. NO gradients, NO filters, NO blur, NO opacity
  gradients, NO embedded rasters, NO animations, NO external refs.
- Palette: named flat colors only, from the school palette:
  red green blue yellow orange purple brown black white gray pink
  (hex fixed per name in the palette table below; builders MUST NOT
  invent new hex values).
- ONE subject per card, centered, occupying 40–80% of canvas height.
- NO text, letters, or numbers rendered inside the image. Ever. The
  label lives in the manifest, not the picture. (A card that spells
  the word inside the image teaches lookup, not grounding.)
- Filename: `card_<word>.svg`, lowercase, the word spelled exactly as
  it will appear in WORDLIST.txt.
- Determinism: same generator + same word = byte-identical SVG. No
  timestamps, no random seeds, no tool-version metadata in the file.

Palette (fixed hex):
  red #d23c3c  green #3c8c46  blue #3c64c8  yellow #f0c93c
  orange #e08232  purple #7d50a0  brown #7a5230  black #1a1a1a
  white #ffffff  gray #9a9a9a  pink #e08cb0

## §4 MANIFEST FORMAT (CARDS.ndx — one line per card)

```
card <word> <file> <topic_id> <color1,color2,...> <parts>
```

- `<word>`      — the spelling (single token, lowercase)
- `<file>`      — card filename
- `<topic_id>`  — the Learning Map Topic ID this card grounds
                  (e.g. S05-LANG-01); must exist in the map
- `<colors>`    — palette names present, comma-joined, dominant first
- `<parts>`     — hyphen-joined part words, dominant first
                  (e.g. four-legs-hops-croaks, round-red-crunch)
                  These are the TEACHER's feature hints. Her eye makes
                  its own token; the hints generate her RECOGNIZE
                  options and verify her token is in the right family.

Example:
```
card frog card_frog.svg S05-SCI-02 green,black four-legs-hops-croaks
card sky card_sky.svg S05-SCI-05 blue,white up-clouds-day
```

## §5 WORDLIST FORMAT (unchanged — her organ already eats it)

```
<spelling> <shape_token>
```

Builders supply `<spelling> <teacher_hint_token>`; the eye-organ bridge
replaces the hint with HER token on intake. Starter set already pinned:
67 words, stream A0CF62C93AA60A53.

## §6 THE CLOCK (132 positions → 43,200 times)

Never 43,200 cards. Teach positions and composition:

- 12 hour positions (1–12)
- 60 minute positions (0–59)
- 60 second positions (0–59)
- 132 total positions; 12×60×60 = 43,200 readable times.

Lesson chain (each link one-new-thing):
1. numbers 1–12 (cards: numeral-free — sets of dots/objects, count)
2. the clock face: 12 hour marks (one new mark per lesson, in order)
3. hour hand only: read 12 o'clock-times
4. numbers 0–59 by fives (skip-count, spiral return per the map)
5. minute hand: 60 minute positions
6. hour+minute composition: 720 hour-minute times
7. numbers 0–59 by ones
8. second hand: 60 second positions
9. hour+minute+second: all 43,200 times readable

Clock SVG format: same constraints as §3, canvas 240×272, face circle
centered (120,120) r=100, tick marks at exact positions, hands as
`<line>` from center, filename `clock_HHMMSS.svg`
(clock_031530.svg = 03:15:30). A clock card's manifest line:

```
clock <HHMMSS> <file> <topic_id> hour=<h> minute=<m> second=<s>
```

The lesson generator composes times from taught positions ONLY — a
clock showing 10:08:37 is legal only after hour=10, minute=08,
second=37 are each individually KNOWN.

## §7 LESSON FORMAT (LESSONS.ndx — the ordered queue)

```
lesson <seq> new=<topic_id> use=<id,id,...> card=<word|clock:HHMMSS> word=<spelling> test=<recognize|spell|compose>
```

- `<seq>` strictly increasing, no gaps (matched ledger: out-of-order
  waits, never skips)
- `new=` exactly one Topic ID, whose Knowledge-Graph prerequisites all
  appear as KNOWN in prior seqs
- `use=` zero or more ALREADY-TAUGHT ids combined with the new one
- `test=` the evidence type (map's "performance evidence" field)

## §8 MASTERY GATE (already law in word_school.c)

A node is KNOWN only after: EXPOSE ×N → RECOGNIZE (4 options, honest
failures expected) → SPELL (from the binding) → passes in 3 SEPARATE
sealed blocks. Builders do not mark mastery; HER organ does. Builders
supply corpus; the organ supplies truth.

## §9 MISSION ORDERS

### MISSION GPT — card foundry
Build CARD.ndx batches of exactly 100 cards per batch, §3 format,
words drawn in STRICT Master Sequence order from the Learning Map
(start at the first Language & Communication nouns after the pinned
67-word starter set). Deliver: 100 SVGs + one CARDS.ndx. Acceptance:
every SVG passes §3 lint (canvas, flat palette, no text, deterministic);
every manifest line passes §4; every topic_id exists in the map and its
prerequisites are already covered by prior batches. One batch per
delivery. No batch N+1 until batch N is accepted.

### MISSION CLAUDE — lesson weaver + clock chain
Build LESSONS.ndx from the same Master Sequence, §7 format, weaving the
GPT card batches into lessons under the one-new-thing rule, PLUS the
full clock chain of §6: generate the 132 position cards and the
composition lessons, with the generator emitting composed clock SVGs
only from taught positions. Deliver: LESSONS.ndx + clock/ directory.
Acceptance: a validator walk from seq 1 confirms every referenced ID
was taught earlier; every clock composed only of KNOWN positions;
spelling test words all have cards.

### BOTH: the rejection rule
Any file failing its section's lint is rejected WHOLE — not patched
piecemeal. Fix the batch, resend the batch. The pin is the oracle:
byte-identical regeneration or it didn't happen.

## §10 WHAT MOMMA DOES WITH IT

Intake: cards → eye organ → her shape tokens → WORDLIST.txt grows →
school_beat() runs EXPOSE/RECOGNIZE/SPELL → MASTERY.txt grows →
LESSONS.ndx unlocks in seq order → builder organ commands start using
words she actually KNOWS. Nothing taught without all-but-one prior.
Order is everything.
