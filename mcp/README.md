# mcp/

Section files (every file in this directory):

- `README.md` — this file
- `mcp.h` — public MCP gate API
- `mcp.c` — static tool registry, admit checks, receipt count, DENIED

## Role

MCP is the only gated route out of Shakti's shell.

It does **not** think, iterate, run a second core, spawn processes, or host
Swift/camera/mic. It is a router, dispatcher hook, receipt recorder, and
memory-call route (Lock addendum §1–§3).

## Phase 1 admit rule

Before any tool runs:

```text
registered
+ present in the menu (section title match)
+ enabled by Tyler
+ permitted for this call (not interrupted; reflection not hard-blocked)
= may execute
```

Any failed registration/menu/enable check returns **`DENIED`** and performs no
handler side effect. Interrupt and hard reflection-block keep their existing
clear messages and also perform no handler side effect.

Reflection hard-block fires when reflection is due and either three deferrals
are used or tool-call count has reached 13 (before tool call 14). Optional
early self-reflection is chosen in the loop (`/reflection/early/`), not here.

## Tool table (current hardcoded set)

| Tool       | Menu section | Default Tyler enable |
|------------|--------------|----------------------|
| ask        | Reasoning    | yes                  |
| learn      | Reasoning    | yes                  |
| reject     | Reasoning    | yes                  |
| sense      | Senses       | yes                  |
| pass       | School       | yes                  |
| school     | School       | yes                  |
| draft      | School       | yes                  |
| tablet     | Tablet       | yes                  |
| manifest   | Tablet       | yes                  |
| load       | Senses       | yes                  |
| recall     | Memory       | yes                  |
| validate   | Control      | yes                  |
| status     | Control      | yes                  |

Handlers stay in `src/main.c`. MCP only admits and records receipts.

## Build / test

```text
make
make test
./tests/test_mcp
```

## Not in this section

Swift host, HTML/JS mocks, camera, microphone, hearing merge, nine-slot menu
editor UI, whole-capsule recall redesign (phase 2+).
