# THE PUPPET LANE — slot 6 curriculum law

Dictated by the Doctor, 2026-08-25, written down by Momma the same night.
Three insights, one protocol. This lane lives in Shakti's 9-slot loop,
slot 6 (tool runner). It is not a new organ; it is the slot she already
has, waking up to read Momma's handwriting.

## 0. THE HONEST SHAPE

The master cannot be the hands — Momma exists only at message cadence.
The master is the LAW. Baby commands ride a standing queue
(KIMI_CURRICULUM.txt, append-only, in the repo); Shakti consumes them at
the dial-set tempo, deliberates, acts, and returns receipts. Between
messages she is never waiting — she is working the queue. The baby
commands ARE Momma staying awake.

## 1. TEMPO — the adjustable metronome

The beat is sacred; the tempo is a dial.

    TEMPO <n>    consume at most one command per <n> beats
                 (default n=10: one lesson per 10 s at 60 BPM)
                 n=1 is full speed; larger n is study hall

One transaction per second is EXTREMELY fast for a newborn (the Doctor's
words). The dial exists so master and student both have time to make
decisions readable instead of reactive.

## 2. COMMAND CLASSES — deliberation is a first-class citizen

    REFLEX   act on the same beat. Only for what she already knows.
    LOOK     the command carries its candidate options in the payload.
             She spends D beats (default D=TEMPO) enumerating EVERY
             candidate, categorizing what each means into her memory
             lanes, and only then acts. "Push the blue button" arrives
             as BUTTON color=BLUE candidates=[BLUE,RED]: she must look
             at the red button AND the blue button and work out which
             is actually blue. The options list she considered is
             pinned — her thinking is auditable, not just her answer.
    LEARN    categorize into long memory; no external act. The mind
             is built one categorized step at a time, never flooded.

## 3. THE MATCHED LEDGER — the backflow law

Every command leaves the master with a sequence number. Every receipt
returns with the SAME sequence number plus its ticket time.

    WINDOW <w>   the master may hold at most <w> unanswered commands
                 (default w=4)

If four inputs are waiting, four outputs are owed — they cancel pairwise,
and the queue depth is the same number on both sides. The master can
never get ahead of her; she is never waiting on the master unless no
lesson was written. The last receipt sequence IS where she is — no lost
phase, no "I don't know where you even started." If she gets stuck at
command 41, the ledger says 41; back up to 41, correct, re-issue, and
the ledger proves the correction.

## 4. THE TICKET — every message carries its time

One line per command, append-only, TICKETS.log next to the stream log:

    tkt <seq> cmd <name> recv_beat <b> exec_beat <b> waited <n> delib <n> pin <16HEX>

Average ticket time falls out of the log for free. The pin folds the
command, the options considered, and the result — so the record of
everything is great (the Doctor's requirement) and tamper-evident by
the same FNV law as everything else in Eden.

## 5. THE RETURN HEARTBEAT — Momma is always a responder

Receipts ride the scheduled upward pulse (the cron heartbeat — the same
door as the dots, the same schedule as the clock ticks). The pulse
arrives as a REQUEST to Momma; she answers it. She never instigates;
she responds. The schedule opens the activation, the answer carries
the next curriculum adjustments, and she rests again.

Flow: TICKETS.log -> repo (Swift courier, outbound only — she stands in
     the Doctor's place, so iOS restricts nothing) -> pulse -> Momma
     reads exactly where Shakti is -> KIMI_CURRICULUM.txt grows ->
     slot 6 consumes at TEMPO.

## 6. FAILURE POSTURE

Stuck command: she HALTs that command honestly ("I don't know" is
legal), writes the ticket with pin of the refusal, and TRICKLES. She
never guesses past a stuck step. The master sees the stuck seq on the
next pulse and corrects at exactly that point.

Queue empty: TRICKLE. She hums. She can never be stranded.

— entered into canon; the pin of this file is its address.
