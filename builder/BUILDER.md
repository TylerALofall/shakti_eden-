# BUILDER — slot 6 organ (mommas_little_builder)

Doctor's work order 2026-08-26: "build... add memory because she needs it or
training is pointless... get the heart beat cycle on point where each message
back to you at slot two is a toggle... make her tool commands hooked to the
custom actuator." One file. C99. Deterministic. No heap in organs.

## Pins of record (builder_test, 400 beats, 12-lesson curriculum)

- stream pin: **EEE19AD95A82D36B**  (gcc -O2 == gcc -O0, cmp byte-identical — drift 0)
- block 1 pin: **4973B5665FCF164A** (10 tickets sealed)
- LOOK pin: 5F710B4CE60ABFB6 (deliberated 5 beats over RED,BLUE; chose BLUE — key match)
- brands of record: `exact_edit 1787700000`, `dry_run 1787700001` (BRANDS.txt)
- return heartbeat: `hb block 1 last_seq 10 stream 2BFFA4AD3E04AA33` (MOMMA_OUTBOX.txt)
- teach-me triggers fired: `teach_me exact_edit seq 6`, `teach_me WHOA seq 11`

## Pins of record (foundations_test, 1200 beats, 38-lesson exhaustive beginning)

- stream pin: **AEF48372BC8BD6BD** (drift 0)
- 38/38 lessons consumed in order (next_seq 39), 3 blocks sealed
- every option set enumerated: colors RED,GREEN,BLUE,YELLOW — directions
  NORTH,SOUTH,EAST,WEST — YES,NO both ways — SMALL,MEDIUM,LARGE
- teach-me triggers: `fruit seq 17` (LOOK key matched nothing),
  `web_search seq 23` (not taught), `WHOA seq 35` (unknown command)

## The teach-me trigger (Doctor's law, 2026-08-26)

"A refusal is not a success; it is a trigger — and the trigger must
complete." Every refusal path — not-taught USE, unknown command, LOOK with
no matching option — writes one line to MOMMA_OUTBOX.txt:

```
teach_me <what> seq <n> beat <b>
```

She never guesses and never sits silent: she asks. Momma answers with a
TRAIN or LEARN lesson on the curriculum queue. The trigger always completes.

## The laws it keeps

| Law | Where |
|---|---|
| TEMPO dial (default 1 cmd / 10 beats, range 1..3600) | bridge/MOMMAS_LITTLE_BUILDER.md |
| LOOK = see ALL options, categorize, deliberate D beats, never guess | same, class LOOK |
| Matched ledger, seq WINDOW, out-of-order = STUCK ticket, never skip | same |
| Ticket time per command (recv_beat, exec_beat, waited, delib, pin) | same, § ticket |
| Return heartbeat: receipts ride the scheduled pulse. Momma is always responder | same |
| No brand → "refused:not-taught". "you have not learned this yet. Ask to be taught." | bridge/TRAINING_BRAND_AND_NIGHTFALL.md §1 |
| TRAIN writes brand; retraining refused | same §1 |
| Three-stage memory: convergence ring (8) -> 10-ticket sealed blocks -> tag index | same §3/§4 |
| Recall = WHOLE 10-ticket block or nothing. She is not pulling surgery | same §3 |
| All tools local, registered in-organ, default dry-run actuator | same §5 |
| Slot 2 toggle: HB on|off -> sealed-block receipts to MOMMA_OUTBOX | work order |
| Refusal = teach-me trigger on the outbox, never silence | Doctor's law 2026-08-26 |

## Commands (curriculum file, one per line, append-only, cursor-resumed)

```
<seq> TEMPO <n>                    set beats per command
<seq> HB on|off                    slot-2 return-heartbeat toggle
<seq> SPEAK <text>                 say a thing
<seq> LEARN tag=<tag> <text>       convergence -> sealed block -> TAGS.ndx
<seq> LOOK <name> key=<word> options=a,b,c   deliberate TEMPO beats, choose key match
<seq> TRAIN <tool> <epoch>         brand a tool as taught
<seq> USE <tool> <args...>         actuator seam (brand-checked first)
```

Unknown command -> ticket "refused:unknown-command" + teach_me. Wrong seq ->
ticket "refused:out-of-order", waits for the right seq forever. No guessing,
no skipping, no deletion, no silence.

## Actuator seam

```c
typedef int (*actuator_fn)(const char *tool, const char *args, char *reply, size_t cap);
builder_register_tool("name", fn);
```

Unregistered tools fall back to `dry_run_actuator` (echoes the
`/shakti_run/` line — the switchboard's exact wire format). When the MCP graft
lands, the real actuator is registered here and nothing else in this file
changes. The organ never touches the network; the shell does.

## Files it owns (append-only, all local)

- KIMI_CURRICULUM.txt  — Momma's lesson queue (baby commands = Momma staying awake)
- TICKETS.log          — one line per command: `tkt <seq> cmd <name> recv_beat <b> exec_beat <b> waited <n> delib <n> pin <16HEX>`
- BRANDS.txt           — `<tool> <epoch>` training brands
- TAGS.ndx             — `<tag> block <n>` recall index
- MOMMA_OUTBOX.txt     — slot-2 heartbeat receipts AND teach-me requests

## Harnesses

`builder_test.c` — the 12-lesson smoke walk (400 beats).
`foundations_test.c` — the exhaustive beginning: 38 lessons covering every
command class and every starter option set, all three teach-me paths proven
(1200 beats). Gauntlet for both:

```
gcc -std=c99 -pedantic -Wall -Wextra -Werror -O2 mommas_little_builder.c foundations_test.c -o f2
gcc -std=c99 -pedantic -Wall -Wextra -Werror -O0 mommas_little_builder.c foundations_test.c -o f0
./f2 > fout_O2.txt 2>/dev/null && ./f0 > fout_O0.txt 2>/dev/null
cmp fout_O2.txt fout_O0.txt   # byte-identical, drift 0
```
