# Drill Control and Test Isolation

## Drill control boundary

The active School drill reads one line at a time.

Before an input is treated as a symbol answer, the drill checks these exact
control commands:

- `/status/`
- `/help/`
- `/interrupt/`
- `/resume/`
- `/stop/`
- `/quit/`

`/stop` remains accepted as a compatibility alias.

Control commands never:

- create handwriting draft frames,
- count as correct attempts,
- count as errors,
- reset the mastery streak,
- append RIGHT or WRONG School records.

`/interrupt/` stops the active drill and MCP tools, then returns control to the
main awake loop. `/resume/` restores MCP tools. `/quit/` exits through the
normal readable-memory shutdown path.

## Test-house boundary

`tests/test_mvp.sh` builds a new temporary house for each scenario.

It copies immutable curriculum and artifact files, then creates clean mutable
files for:

- School state,
- Learned evidence,
- Learned stream,
- Tyler/Shakti messages,
- reflections.

The test never assumes that the repository's live School state is empty.
Therefore `make test` passes both before and after a real learning session and
does not modify Shakti's real progress.

## Regression scenarios

The MVP test verifies:

1. clean startup and Pass 1 demonstration,
2. `/status/`, `/help/`, and `/resume/` during a drill,
3. `/interrupt/` during a drill,
4. resume and complete ten-streak mastery,
5. zero false WRONG trials from control commands,
6. `/stop/` without changing progress,
7. `/quit/` during a drill,
8. safe readable-memory shutdown.
