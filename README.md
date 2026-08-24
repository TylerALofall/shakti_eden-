# Shakti Runnable MVP — C99

this file is old do not trust it completely 





This package is a single-process, fixed-memory C99 implementation of Shakti's runnable beginning.

## What runs now

- Restores Eden, School, Goal, Notebook, Menu, Learned memory, and reflection routes.
- Produces an exact startup self-report from the XML manifest ledger.
- Runs the nine-stage loop:
  1. Epoch
  2. Heartbeat
  3. Goal/system message
  4. `/notebook/`
  5. `/menu/`
  6. `/shakti_run/`
  7. `/message_tyler/` and `/note_tyler/`
  8. `/message_shakti/` and `/note_shakti/`
  9. `/reflection/`
- Accepts an external interrupt that stops MCP tools while leaving Shakti awake.
- Loads the four channels: `text`, `written_text`, `sound_art`, and `visual_art`.
- Runs Pass 1 as observation without changing School mastery.
- Runs Pass 2–4 actuator drills.
- Requires a ten-correct consecutive streak for symbol mastery.
- Preserves every correct and incorrect attempt in readable append-only logs.
- Reports unknown answers as `I do not know`.
- Uses creative candidates only after independent logic authorization.

## Process boundary

The runtime is one C process and one `shakti_runtime_t` state.

It does not invoke:

- another language runtime,
- another language model,
- a subprocess,
- a shell command,
- `system`, `popen`, `fork`, `exec`, or `spawn`,
- dynamic allocation.

`/shakti_run/` is an internal MCP dispatcher to approved C functions.

## Current curriculum data

Implemented and validated:

- Foundation 0: printable ASCII 32–126 as exposure and 8x8 handwriting.
- Level 1: counting zero through ten, including timed-light metadata.
- Level 2: uppercase, lowercase, case pairs, counting bridges, and growing sequences.

Planned but not promoted:

- counting eleven through one hundred,
- colors,
- shapes,
- basic exact whole-number math.

Eden remains `OPEN`. Runnable means the implemented beginning validates; it does not mean Eden is final or locked.

## Build

```sh
make clean
make
```

## Verify

```sh
make test
./shakti --check
./shakti --demo
```

`--check` validates the startup boundary and exits.

`--demo` runs a read-only Pass 1 presentation of the first staged tablet. It writes the startup anchor but does not change School mastery.

## Run interactively

```sh
./shakti
```

Useful commands:

```text
/help/
/status/
/heartbeat/ 0
/menu/
/shakti_run/ status
/shakti_run/ pass 4
/shakti_run/ school +
/interrupt/
/resume/
/quit/
```

During a drill, these controls are recognized before scoring:

```text
/status/
/help/
/interrupt/
/resume/
/stop/
/quit/
```

`/interrupt/` stops the active drill and MCP tools while Shakti remains awake.
`/stop` remains accepted as a compatibility alias for `/stop/`.

The MVP tests create a fresh temporary house and reset mutable School,
Learned, message, and reflection files. Running `make test` never depends on
or modifies Shakti's live progress.

## Exact current validation

```text
Implemented data: 1033/1033 checks = 100%
Verified stones: 282/282
Complete tablets: 7
Required channel gaps: 0
Optional Foundation spoken punctuation sounds deferred: 33
Future planned tablets: 10
Eden lock ready: NO
Runnable beginning: YES
```

See:

- `CHECK_RUN_OUTPUT.txt`
- `DEMO_RUN_OUTPUT.txt`
- `DRILL_RUN_OUTPUT.txt`
- `BUILD_TEST_REPORT.txt`
