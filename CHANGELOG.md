# Early optional self-reflection

- Lock addendum gate: optional early self-reflection below 10 MCP tool calls;
  go early when work ends short of 10; due at 10; defer on 11–13; required
  before tool call 14.
- Reflection counter advances only on approved MCP tool calls.
- `/reflection/early/` is Shakti's optional early self-reflection choice.
- MCP blocks the next tool after three deferrals or at the tool-call-14 wall.

# Drill Control and Test-Isolation Fix

- Recognizes `/status/`, `/help/`, `/interrupt/`, `/resume/`, `/stop/`, and
  `/quit/` before a drill answer is scored.
- Stops the active drill immediately when Tyler interrupts MCP tools.
- Keeps `/stop` as a compatibility alias and documents `/stop/` as canonical.
- Prevents control commands from creating handwriting frames or WRONG trials.
- Rebuilds MVP tests inside a fresh temporary house with clean mutable state.
- Adds regression tests for mid-drill status, help, resume, interrupt, stop,
  quit, and zero false errors.
- Ignores `build_seed_curriculum` and remaining temporary test directories.

# Runnable MVP changelog

- Connected the startup self-report to the executable startup path.
- Added `--check` for exact loader and ledger verification.
- Added `--demo` for a Pass 1 observation run.
- Fixed Foundation 0 loading so deferred punctuation audio remains optional.
- Added a full ten-correct-streak School integration test.
- Verified the nine-stage loop, heartbeat, reflection deferrals, interrupt, and resume.
- Preserved every School trial in readable logs.
- Confirmed the C and header source contains no dynamic allocation or subprocess calls.
- Kept Eden status OPEN.
