# SHAKTI MCP ROUTER — GAP REVIEW + BUILD BLUEPRINT

Reviewer: Claude (Opus) · Basis: Issue #4 spec conversation with GPT-5.6 Sol · 2026-07-17
Repository: TylerALofall/2026-07-16-Shakti · Target: Genesis/Shakti_MCP/ · Language: C17, no heap, fixed buffers

---

# PART 1 — GAP REVIEW

## 1.1 What is solid — keep as-is

- Cycle-completion interceptor replacing the timer heartbeat. This is the single most important decision in the thread and it is correct. Prompts gate on real completion, never on wall time.
- One parser, one route table, one cycle controller, one interceptor, one loopback bundle shape. No route-local loops scattered around.
- Code generates ALL structure (IDs, parents, timestamps, JSON, letters). Shakti supplies only content and PASS/FAIL.
- Typed struct is the source of truth for event addresses; the formatted string is only a search key.
- Heartbeat stores an intention, not prewritten prompts. Stale intentions rebuild from current context or cancel — never replay. This directly kills the hallucinated-chain failure mode.
- Nine-cycle energy budget that does not silently refill.
- Corrections append and reference; they never overwrite. Mistakes stay findable as attributable events.
- Batch dismissal by tree, refused while a child is active — no orphans.

## 1.2 Holes that block a build (resolve before rewriting Issue #4)

### GAP 1 — No re-entry path for tool results
WAITING_FOR_RESULT exists, but no route accepts a result. message.in is Tyler→Shakti. shell is a stub. Network is forbidden. A slow external operation (search, script test, music op) has no defined door back in.

**Fix:** one core function, not a route:
```c
ShaktiStatus shakti_post_result(uint64_t op_id,
                                ShaktiResult status,   /* PASS | FAIL */
                                const char *payload);
```
- Harness control command `/result <op_id> PASS|FAIL <text>` for manual testing today.
- The future shell attachment (route 6) posts through this exact same function later.
- Max ONE pending op per cycle. Unknown op_id → refused explicitly.

### GAP 2 — Nothing defines when a cycle is COMPLETE
If Shakti replies in plain prose with no command, the controller has no completion signal. She hangs in RUNNING forever; the heartbeat can never re-arm.

**Fix — the turn-end rule:** parse her entire turn after it ends.
- Turn contains a pending-op command → WAITING_FOR_RESULT.
- Otherwise (prose only, or only continue-type commands like note/message.out) → COMPLETED automatically.
- Optional explicit `done` command allowed but never required.

### GAP 3 — The kill switch cannot be immediate as designed
All Tyler input currently flows through message.in → interceptor queue → waits for cycle completion. `heartbeat off` waiting in a queue is not a kill switch.

**Fix — split the control plane from the message plane.** In the terminal harness:
- Line starts with `/` → CONTROL: executes against the router immediately, bypasses every queue. `/heartbeat off` disarms and cancels the pending intention right now, even mid-cycle.
- Any other line → MESSAGE: wrapped in the JSON envelope, queued through the interceptor as designed.

### GAP 4 — Crash/restart behavior is undefined; reminders can double-fire
If the process dies between message.out to Tyler and marking the reminder delivered, it fires again on boot. If it dies before firing, states are ambiguous.

**Fix — boot rules:**
- Heartbeat ALWAYS boots OFF regardless of any persisted state. Re-arm is a deliberate act.
- Any batch not marked COMPLETED at boot gets an appended INTERRUPTED marker. Never rewritten, never deleted.
- The global cycle counter persists and only increases across restarts.
- Reminders use a three-step append record: ARMED → FIRED (appended before send) → COMPLETED (appended after Shakti-side delivery). A FIRED-without-COMPLETED found at boot produces a message to Tyler: "reminder <id> may have already fired." At-least-once with a dedupe note. Never a silent refire; never a silent drop.

### GAP 5 — "Mozart to stay awake" was skipped entirely (QUESTION FOR TYLER)
Tyler's dictation: a slow op "might take a second and might need Mozart to stay awake." Sol never addressed it.

Best interpretation: session keepalive during long external operations. Design answer if so: the result loopback bundle is fully self-contained (short-term batch history + goal + result + next instructions), so even a brand-new session can resume the cycle — the LLM session dying during a long wait costs nothing. The harness also displays a WAITING status with elapsed time.

**Do not rewrite Issue #4 until Tyler confirms what Mozart means.**

### GAP 6 — Outbox: "fixed ring" contradicts "store and relay"
A ring at capacity either refuses or overwrites — both lose a message TO TYLER, the worst message to lose.

**Fix:** one file per outbound message, exclusive-create, named by its event ID, in state/outbox/. Disk is the queue. A small fixed in-memory index tracks unread; if the index is full the message is STILL on disk and the harness rescans. No ring.

### GAP 7 — Platform clocks don't match each other, and Linux is missing
- mach_continuous_time (Apple) COUNTS time asleep. QueryPerformanceCounter (Windows) generally does NOT. They are not the same measurement.
- The original spec requires a POSIX/macOS shell build, but platform/ has only time_windows.c and time_apple.c. Linux is absent.

**Fix — define both semantics explicitly behind one interface, add time_posix.c:**
```c
uint64_t shakti_wall_epoch(void);       /* time(NULL), UTC seconds, never truncated   */
uint64_t shakti_tick_ns(void);          /* EXCLUDES sleep: QPC / mach_absolute_time /
                                           CLOCK_MONOTONIC — local ordering           */
uint64_t shakti_uptime_total_ms(void);  /* INCLUDES sleep: GetTickCount64 /
                                           mach_continuous_time / CLOCK_BOOTTIME      */
```

### GAP 8 — Command grammar was never formally defined; filenames are unsafe
`note <filename>- <text>` uses "- " as a delimiter. Filenames with hyphens break it, and Tyler dictates by voice — voice-to-text WILL produce stray punctuation. And nothing anywhere forbids `notebook/../../etc/passwd`.

**Fix:** formal grammar in section 2.9; filename charset `[a-z0-9_]` max 64, no dots, no slashes, no "..", validated in ONE function every route calls.

## 1.3 Quietly dropped from the original spec — confirm intentional

| Item | Where it went | Decision needed |
|---|---|---|
| whitewall state + 30-second whitewall cycle | Vanished from new heartbeat and epoch status | Keep, drop, or defer? |
| LESSON_TIME / two-clock law | Vanished from router redesign | It's a School concept — carry the field in epoch status, or explicitly out of this pass? |
| gate route: witnessing + permission classes | Absorbed silently | Make explicit: every command and result IS recorded as a batch child (that is the witness log), and route_table checks enabled flag + permission class before dispatch |
| memory route XML schema contracts | Replaced by reflect stub | Keep docs/EVENT_SCHEMA.md as documentation-only so the permanent-memory contract isn't lost |
| Directory Genesis/Shakti_Memory/ | Renamed to Genesis/Shakti_MCP/ | Fine — but record the rename in Issue #4 so the "stop and report location conflicts" rule is satisfied |

## 1.4 Ambiguities to lock before Issue #4 is rewritten

1. **cycle_sequence meaning.** Tyler's diagram says "cycle sequence during that epoch second"; the struct implies either. Recommend: GLOBAL persisted monotonic counter — survives clock-backward and restarts, epoch stays informational, and exclusive-create on the batch file is the final collision guard. Per-second numbering requires reading prior state and still collides after a backward clock step.
2. **Segment letters.** Fixed roles per Tyler's tree (A = trigger/context, B = activity, C = completion) or purely ordinal? Recommend fixed roles as an enum.
3. **Tree depth.** The string format shows 4 levels (batch-segment-item-child). Lock TREE_DEPTH_MAX = 4; refuse deeper.
4. **Multiple reminders due at once** (e.g., clock corrected forward): combine ALL due reminders into ONE bundle ordered by target epoch. Never N stacked prompts.
5. **Does a reminder cycle consume a heartbeat?** Recommend NO. The budget counts only heartbeat continuations. Tyler messages obviously don't either.
6. **Who can change the goal?** Recommend Tyler-only via `/goal set` on the control plane this pass. Shakti reads freely. Letting her rewrite her own system message is a self-modification door we should not open in the footing.
7. **Heartbeat asymmetry.** Shakti may call `heartbeat off` (the safe direction) and read status. Only Tyler can `/heartbeat on` or set the budget. Re-arm after budget exhaustion: Tyler-only this pass — "an admitted internal rule" is undefined and stays out.
8. **Notebook working/ decay.** No automatic decay this pass. Manual dismiss only. Decay policy is a later decision made deliberately, not a default.
9. **Multiple commands per turn.** Processed in order of appearance. Max ONE pending-op command per turn; extra pending-op commands refused explicitly inside the same bundle. Continue-type commands (note, message) may repeat.
10. **JSON escaping.** The emitter must escape quotes, backslash, newline, tab, and control chars, with bounds-checked refusal — Tyler's voice-to-text messages will contain quotes. Good news: NO JSON parsing is needed anywhere in this pass. The harness wraps Tyler's text; Shakti replies in plain command lines; the router only EMITS JSON.
11. **Batches vs no-heap.** Fixed in-memory arrays sized by the limits table (2.12). The retained-batch cap forces an explicit keep / dismiss / reflect decision when full — refusal, never silent eviction.
12. **Outbox/goal/notebook write sizes** enforced at WRITE time (NOTEBOOK_FILE_MAX) so every stored file is guaranteed readable back into a fixed buffer later. A file you can write but never read again violates the no-silent-truncation law from the read side.

---

# PART 2 — BLUEPRINT

## 2.0 What this system is, in one paragraph

A fixed C17 router that sits between Shakti (the model) and everything else. She speaks in plain command lines, like a cmd prompt she can use but never has to; the C code supplies every piece of structure — IDs, parent links, timestamps, JSON, sequencing. Exactly one cycle runs at a time. Nothing prompts her while a cycle is alive. When a cycle completes, an interceptor decides whether anything (Tyler message, due reminder, armed heartbeat) earns exactly one new self-contained loopback bundle. Everything permanent is append-only or exclusive-create; everything that fails, fails loudly with a named refusal; nothing fakes success.

Analogy: a charge nurse's station. One patient in the room at a time. Call lights queue at the desk in priority order. The intercom kill switch works even mid-procedure. And the chart is written by the nurse (the C code), never by the patient.

## 2.1 The three planes

```
CONTROL PLANE   Tyler "/" lines → router directly. Immediate. No queue.
                /status /heartbeat on|off|budget N /goal set /result /dismiss /quit

MESSAGE PLANE   Tyler plain text ↔ Shakti through inbox/outbox.
                Cycle-gated through the interceptor. Stored verbatim.

INTERNAL PLANE  Shakti command lines → routes. Results return INSIDE the
                same cycle. Never a new prompt.
```

## 2.2 Master flow

```
                       TYLER (terminal harness, stdin/stdout)
                                     │
                 ┌───────────────────┴────────────────────┐
                 │ line starts with "/"                    │ plain text
                 ▼                                        ▼
          CONTROL PLANE                          8. MESSAGE.IN
          immediate execution                    JSON envelope → inbox queue
          /heartbeat off = instant kill                   │
          /result posts tool results                      ▼
                 │                            ┌───────────────────────┐
                 ▼                            │  PROMPT INTERCEPTOR    │◄── reminder due
              ROUTER ◄────────────────────────│  safety off? → stop    │◄── heartbeat intention
                                              │  cycle active? → hold  │
                                              │  combine → ONE bundle  │
                                              └───────────┬───────────┘
                                                          │ only when IDLE
                                                          ▼
                                              CYCLE CONTROLLER
                                     IDLE → RUNNING → WAITING → COMPLETED
                                                          │
                                                          ▼
                                                       SHAKTI
                                                          │ turn text
                                                          ▼
                                          COMMAND PARSER → ROUTE TABLE
                                                          │ (validates verb,
                                                          │  enabled, permission,
                                                          │  records batch child)
              ┌──────────┬──────────┬──────────┬──────────┼──────────┬──────────┐
              ▼          ▼          ▼          ▼          ▼          ▼          ▼
          1.epoch  2.heartbeat  3.goal  4.notebook   5.menu    6.shell   9.reflect
                                                          │
                                          same-cycle results, no new prompt
                                                          │
                                     7. MESSAGE.OUT ──────┴──► file per message ──► Tyler
```

## 2.3 Cycle controller

```
IDLE ──interceptor releases ONE bundle──► RUNNING
RUNNING ──turn ends WITH a pending-op command──► WAITING_FOR_RESULT
RUNNING ──turn ends with NO pending op──► COMPLETED           (the turn-end rule)
WAITING_FOR_RESULT ──shakti_post_result(op_id,...)──► RUNNING (result continues SAME cycle)
COMPLETED ──► record completion child (segment C)
          ──► if heartbeat enabled: next_heartbeat_epoch = completed_epoch + interval
          ──► IDLE
```

Laws:
- One active cycle, ever. One pending op per cycle, max.
- While RUNNING or WAITING: no heartbeat, no reminder prompt, no Tyler-message prompt. All safely queue.
- Missed heartbeat times do not accumulate. No catch-up prompts, ever.
- A slow tool keeps its cycle alive; the result returns to THAT cycle.
- Command results (notebook read, menu level, epoch status) return immediately inside the running cycle — zero artificial delay. The only waiting that exists is a real unfinished operation.

## 2.4 Prompt interceptor

Inputs, in priority order: (1) Tyler messages, (2) due reminders, (3) armed heartbeat intention.

```
any trigger present
    → safety off (heartbeat trigger only)?  → drop trigger, stop
    → cycle active?                          → hold everything, re-check at completion
    → bundle already being built this gate?  → combine into it
    → build exactly ONE loopback bundle containing ALL pending items
    → hand to cycle controller → RUNNING
```

Combine rule: one bundle may simultaneously carry queued Tyler messages (ordered), all due reminders (ordered by target epoch), and note the heartbeat if it also fired. The trigger field names the highest-priority cause. Never two bundles back to back.

## 2.5 Heartbeat law

```c
struct ShaktiHeartbeatState {
    bool     enabled;                 /* boots false. ALWAYS.                  */
    uint8_t  cycles_remaining;        /* dev default 9; no silent refill       */
    uint32_t delay_after_completion;  /* seconds; dev default 0                */
    uint64_t source_cycle_id;         /* cycle that armed this intention       */
    uint64_t source_context_version;  /* staleness check                       */
    bool     continuation_armed;      /* at most ONE intention exists          */
};
```

- Default OFF. Boots OFF. `/heartbeat off` (Tyler, instant) and `heartbeat off` (Shakti, allowed — safe direction) disarm and cancel the intention.
- Only Tyler may enable or set the budget. Budget exhausted → relaxed/OFF until Tyler re-arms.
- An intention arms only at cycle completion. Before firing, its source_context_version is compared to current; mismatch → rebuild the bundle from CURRENT context or cancel. Old prewritten prompts are never replayed.
- Reminder- and Tyler-triggered cycles do NOT consume the budget. Only heartbeat continuations decrement.

## 2.6 Event addressing

```
1784311200-0007-B-04-D
│          │    │ │  └─ child (letter)
│          │    │ └──── item within segment (number)
│          │    └────── segment (fixed role letter)
│          └─────────── GLOBAL cycle counter (persisted, monotonic, never resets)
└────────────────────── full Unix epoch second (informational; never truncated)
```

```c
struct ShaktiEventAddress {
    uint64_t epoch_seconds;
    uint64_t cycle_sequence;    /* global monotonic, persisted across restarts */
    uint16_t segment_number;    /* 1=A trigger/context, 2=B activity, 3=C completion */
    uint16_t item_number;
    uint16_t child_number;
    uint8_t  depth;             /* 1..TREE_DEPTH_MAX (4); deeper refused */
};
```

- Letters rendered by code only: 1→A … 26→Z, 27→AA. Shakti never writes IDs, parents, timestamps, letters, or JSON. She supplies content and PASS/FAIL.
- Struct is truth; the string is a search key. Batch ID retrieves the whole tree; full ID retrieves one exact event.
- Clock moves backward → cycle counter continues, a CLOCK_ADJUSTED child is appended, order is never faked.
- Batch lifecycle: allocate at cycle start → every message/command/result/refusal becomes a child (this IS the witness log) → at completion: KEEP TEMPORARILY | DISMISS (whole tree, refused while a child op is active) | reflect to route 9.

## 2.7 Loopback bundle (the one JSON shape)

```json
{
  "schema_version": 1,
  "cycle_id": "1784311200-0007",
  "trigger": "tyler_message | reminder | heartbeat | command_result",
  "operation_state": "new_cycle | continuing",
  "goal_active": "current goal text",
  "short_term_history": ["rendered children of the relevant batch(es)"],
  "incoming_messages": [{"id": "...", "epoch": 0, "text": "..."}],
  "reminders_due": [{"id": "...", "target_epoch": 0, "text": "...", "notebook": "name-or-null"}],
  "called_item": {"type": "notebook|menu|epoch|goal|shell", "name": "..."},
  "result": {"op_id": "...", "status": "PASS|FAIL|NOT_IMPLEMENTED|REFUSED", "payload": "..."},
  "notebook_contents": "present when a reminder references notebook/<name>",
  "heartbeat": {"enabled": false, "remaining": 9},
  "next_instructions": "how to operate what she just opened (e.g., menu level 2)"
}
```

Self-contained on purpose: a fresh session receiving this bundle can resume the cycle. That is the keepalive insurance for long operations. Emitted by loopback_builder.c through the bounds-checked JSON emitter; overflow → refusal, never a truncated bundle.

## 2.8 The nine routes — command surface

| # | Route | Shakti commands | Behavior this pass |
|---|---|---|---|
| 1 | epoch | `epoch` | One status line: wall epoch, tick, total uptime, cycle state, heartbeat state+remaining, open task. Cross-platform via 2.13. |
| 2 | heartbeat | `heartbeat` (status), `heartbeat off` | Full state machine per 2.5. `on`/`budget` are control-plane only. |
| 3 | goal | `goal`, `goal history`, `goal history <id>` | Read active goal; list/read history. `/goal set <text>` (Tyler) archives old → history/, writes new, updates active.ref. |
| 4 | notebook | `notebook`, `notebook <name>`, `note <name> <text>`, `notebook save <name>`, `reminder <epoch> <text>` | Full behavior — see 2.10. `note` and `message` are continue-type: recorded, cycle proceeds, never a reprompt. |
| 5 | menu | `menu`, `menu <tool>`, `menu <tool> <action> [args]` | Staged levels read from tool_menu.xml. Level N result includes operating instructions for level N+1 in next_instructions. Actions return NOT_IMPLEMENTED. Nothing auto-loads all levels. |
| 6 | shell | `shell <cmdline>` | Stub: returns NOT_IMPLEMENTED, records the request as a batch child. Future attachment executes externally and posts through shakti_post_result. |
| 7 | message.out | `message <text>` | Store verbatim (file per message, exclusive create) + relay to Tyler. Continue-type: no reflection, no reprompt. |
| 8 | message.in | (not callable by Shakti) | Harness wraps Tyler plain text into the JSON envelope, queues to interceptor. Exactly one prompt results. |
| 9 | reflect | `reflect <batch_id>` | Copies the batch's JSON into state/pending_reflection/. Refused while a child op is active. NO permanent XML commit in this pass. |

## 2.9 Command grammar (formal)

```
turn      := { line }
line      := command | prose
command   := verb [ SP arg ]... NL         ; line BEGINS with a registered verb
verb      := epoch | heartbeat | goal | notebook | note | reminder
           | menu | shell | message | reflect | done
name      := [a-z0-9_]{1,64}               ; no dots, no slashes, no "..", ever
epochnum  := [0-9]{1,20}                   ; full seconds, never truncated

note      := "note" SP name SP text        ; text = rest of line, verbatim
reminder  := "reminder" SP epochnum SP text
             ; parser scans text for the token "notebook/" name
             ; and attaches it as the reminder's notebook reference,
             ; preserving Tyler's natural phrasing ("...see notebook/project_status")
```

- Lines not beginning with a registered verb are prose: ignored by the router, retained verbatim in the batch as history.
- Commands execute in order of appearance. Max one pending-op command per turn; extras → explicit refusal child in the same bundle.
- ONE filename validator function; every route calls it. Path traversal is structurally impossible, not policed per-route.

## 2.10 Reminder lifecycle

```
reminder 1792829382 I need to check file status see notebook/project_status
    │
    ▼ ARMED record appended in state/notebooks/reminders/  (id, target, text, notebook ref)
    ▼ shakti_pump(now) finds now >= target AND no newer FIRED record
    ▼ FIRED record appended  ── BEFORE any send (crash-safety per GAP 4)
    ▼ verbatim reminder → message.out → Tyler
    ▼ Shakti trigger pends at the interceptor (priority above heartbeat, below Tyler messages)
    ▼ cycle idle → bundle built: reminder + notebook/project_status contents
                   + short_term_history + current context
    ▼ COMPLETED record appended — can never fire twice
```

All simultaneously-due reminders ride in one bundle. Reminder cycles never touch the heartbeat budget.

## 2.11 Directory tree

```
Genesis/Shakti_MCP/
├── include/
│   ├── shakti_limits.h        every capacity constant (2.12) — the ONLY place sizes live
│   ├── shakti_types.h         ShaktiStatus, ShaktiEventAddress, heartbeat/cycle structs
│   ├── shakti_time.h          three-function platform clock interface (2.13)
│   ├── shakti_json.h          bounds-checked emitter + string escaper
│   └── shakti_router.h        route table, parser, post_result, pump entry points
├── core/
│   ├── command_parser.c       one grammar (2.9), one filename validator
│   ├── route_table.c          fixed compile-time table; enabled + permission checks
│   ├── cycle_controller.c     IDLE/RUNNING/WAITING/COMPLETED + turn-end rule
│   ├── prompt_interceptor.c   safety / hold / combine → one bundle
│   ├── loopback_builder.c     the single JSON bundle shape (2.7)
│   ├── json_emit.c            escaping, overflow refusal
│   ├── event_store.c          batch trees, IDs, letter rendering, witness children
│   └── ROUTE_MAP.md           human helper guide mirroring the actual table
├── routes/
│   ├── route01_epoch.c        … through …
│   └── route09_reflect.c
├── platform/
│   ├── time_windows.c         time() + QPC + GetTickCount64
│   ├── time_apple.c           time() + mach_absolute_time + mach_continuous_time
│   └── time_posix.c           time() + CLOCK_MONOTONIC + CLOCK_BOOTTIME   ← was missing
├── harness/
│   └── shakti_terminal.c      stdin/stdout; "/" control plane; pump loop; outbox display
├── state/
│   ├── goals/active.ref  +  goals/history/
│   ├── notebooks/working/  saved/  reminders/
│   ├── outbox/                one file per message, exclusive create
│   ├── inbox/                 envelope archive
│   ├── pending_reflection/    staged batch JSON for the future memory system
│   └── runtime/               cycle counter, context version (heartbeat NEVER persists ON)
├── config/tool_menu.xml
├── docs/
│   ├── BOUNDARY.md            Eden / School / Memory boundary (background, not build law)
│   └── EVENT_SCHEMA.md        future permanent-XML contract, documentation-only
├── tests/
├── build.sh                   POSIX/macOS
└── build_clang.bat            Windows Clang
```

## 2.12 Shared limits (proposed — Tyler approves before coding)

| Constant | Value | Note |
|---|---|---|
| SHAKTI_MSG_MAX | 8192 | per message, both directions; over → REFUSED_TOO_LARGE, nothing half-stored |
| SHAKTI_LINE_MAX | 4096 | one command line |
| SHAKTI_NAME_MAX | 64 | filenames/notebook names |
| NOTEBOOK_FILE_MAX | 65536 | enforced at WRITE so reads always fit |
| INBOX_QUEUE_MAX | 16 | full → harness shown QUEUE_FULL; Tyler's text never silently dropped |
| OUTBOX_INDEX_MAX | 64 | unread index only; disk is the real queue |
| REMINDER_MAX | 128 | armed at once |
| BATCH_RETAINED_MAX | 8 | full → forced keep/dismiss/reflect decision, never silent eviction |
| BATCH_CHILD_MAX | 128 | per batch |
| CHILD_TEXT_MAX | 2048 | per child event text |
| BUNDLE_MAX | 32768 | loopback JSON output |
| TREE_DEPTH_MAX | 4 | deeper refused |
| HEARTBEAT_BUDGET_DEFAULT | 9 | |

## 2.13 Pump (who drives time)

No sleeps, no timers, no loops inside the core — the original law stands. The harness main loop calls:

```c
ShaktiPumpAction shakti_pump(uint64_t now_epoch);
/* returns: PUMP_NONE | PUMP_PROMPT_READY (bundle emitted) | PUMP_WAITING (status) */
```

Each call: check due reminders → check heartbeat intention → run interceptor → maybe emit one bundle. The harness may sleep between polls; the core never does. This is the honest replacement for "who fires the heartbeat" — the answer is: the caller, every pump, exactly as the time law demands.

## 2.14 tool_menu.xml schema (placeholder content, real schema)

```xml
<shakti_menu schema_version="1">
  <tool id="dict" class="DICT" name="Dictionary" enabled="false">
    <describe>Word lookup</describe>
    <action id="lookup">
      <input name="word" type="name" required="true"/>
      <instructions>menu dict lookup [word] — returns NOT_IMPLEMENTED this pass</instructions>
    </action>
  </tool>
  <!-- CALC, FILE, NET, MUSIC, MEM, IO stubs follow the same shape -->
</shakti_menu>
```

## 2.15 Persistence and restart (consolidated)

1. Heartbeat boots OFF. Always. Enabled state is never persisted as true.
2. state/runtime/ persists: global cycle counter (monotonic forever), context version.
3. Interrupted batches: appended INTERRUPTED marker at boot; controller resets to IDLE.
4. Reminders: ARMED/FIRED/COMPLETED appends per 2.10; FIRED-without-COMPLETED at boot → notify Tyler, no refire, no drop.
5. Every stored artifact is exclusive-create or append. Duplicate creation is refused. Ordinary files are alterable by the OS — the API enforces append-only and the README documents the external storage-permission requirement, exactly as the original spec demands.

## 2.16 Tests

1. All nine route IDs resolve to their compiled modules. 2. Unknown verb refused. 3. Filename with dot/slash/".." refused. 4. Oversize message refused whole — nothing half-stored. 5. Heartbeat boots OFF even if runtime files claim otherwise. 6. Heartbeat cannot fire while RUNNING or WAITING. 7. `/heartbeat off` cancels the armed intention immediately, mid-cycle. 8. At most one intention exists. 9. Missed heartbeats never accumulate or replay. 10. Budget decrements only on heartbeat continuations. 11. Exhausted budget → OFF; no silent refill. 12. Stale intention (context version mismatch) rebuilds from current or cancels. 13. Reminder fires once; FIRED-without-COMPLETED at boot notifies instead of refiring. 14. Multiple due reminders → one combined bundle. 15. Tyler message during RUNNING queues and arrives in the completion bundle. 16. Prose-only turn auto-completes — no hung cycle. 17. Second pending-op command in one turn refused explicitly. 18. shakti_post_result with unknown op_id refused; correct op_id resumes the SAME cycle. 19. Batch dismissal refused while a child op is active; dismissal leaves no orphans. 20. Duplicate batch/event file creation refused (exclusive create). 21. No path ever rewrites an existing event file — corrections append. 22. Letter render 1→A, 26→Z, 27→AA; depth > 4 refused. 23. Backward clock: counter continues, CLOCK_ADJUSTED appended. 24. Outbox message survives a full index (disk write precedes index insert). 25. JSON escaper handles quote/backslash/newline/control chars; overflow refuses.

## 2.17 Build order (logical, per the working law: build in proper order, then build)

- **Phase 0** — directories, shakti_limits.h, ROUTE_MAP.md skeleton, both build scripts compiling an empty translation unit under strict C17.
- **Phase 1** — platform time (all three files) + clock tests.
- **Phase 2** — json_emit + escaper tests.
- **Phase 3** — event_store: IDs, letter rendering, batch trees, exclusive create + tests.
- **Phase 4** — command_parser + route_table with all nine routes returning NOT_IMPLEMENTED + resolution/refusal tests. (Explicit NOT_IMPLEMENTED everywhere — nothing fakes success, per the original law.)
- **Phase 5** — cycle_controller + prompt_interceptor + loopback_builder + shakti_pump + state-machine tests.
- **Phase 6** — routes, in dependency order: epoch → message_out → notebook (notes, then reminders) → goal → message_in → heartbeat → menu → reflect → shell stub.
- **Phase 7** — terminal harness + an end-to-end walkthrough transcript.
- **Phase 8** — BOUNDARY.md, EVENT_SCHEMA.md, README, append-only signing-ledger entry, verified commit or local patch, Draft PR, STOP for Tyler's review.

Git safety unchanged from the original spec: new branch off current main, no force-push, no history rewrite, no touching Draft PR #2, no push without a Verified signature — leave a local patch instead.

## 2.18 Explicitly OUT of this pass (Issue #4 appendix, not build law)

Eden and School architecture · math classification · truth/contradiction/mistake laws · the finished permanent XML memory format · recall and decay policy · confidence inference · consciousness/research motivation · 8x8 Net64 (still undefined — nothing reserved) · full tool implementations · autonomous prompting beyond the bounded heartbeat.

The only memory detail that lives in this build is the bridge: operation → internal JSON envelope → state/pending_reflection/. Permanent XML comes later, and EVENT_SCHEMA.md keeps that contract on paper so it isn't lost in the meantime.

## 2.19 Open questions for Tyler (answer before Issue #4 is rewritten)

1. What is "Mozart"? (Interpreted as session-keepalive-during-long-ops; the self-contained bundle answers it if so. Confirm.)
2. Whitewall state and the 30-second whitewall cycle — keep, drop, or defer?
3. LESSON_TIME — carried as a status field this pass, or explicitly out until School?
4. Goal changes Tyler-only via control plane — confirmed?
5. Heartbeat asymmetry (she can turn it off, only you turn it on / set budget) — confirmed?
6. Capacities table in 2.12 — approve or adjust?
7. Directory name Genesis/Shakti_MCP/ (replacing Shakti_Memory) — confirmed and recorded in Issue #4?
8. Reminder crash behavior = at-least-once with a "may have fired" notice — acceptable, or would you rather risk losing one than double-hearing one?
