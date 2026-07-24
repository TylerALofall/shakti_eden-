# Single-Process Runtime Boundary

## Identity

The executable named `shakti` contains the runtime directly. There is no secondary process presented as Shakti.

## Host responsibilities

The host supplies:

- process startup,
- standard input and output,
- current clock values,
- readable files,
- external interrupt delivery.

## Shakti responsibilities

`shakti_runtime_t` owns:

- the nine-stage loop,
- heartbeat state,
- Goal, Notebook, Menu, and messages,
- fixed convergence and short-term storage,
- School state and mastery history,
- manifest and tablet loading,
- exact evidence and abstention decisions,
- internal MCP dispatch,
- readable append-only logs.

## MCP

`/shakti_run/` dispatches approved C functions inside the same process.

The external interrupt changes MCP availability. It does not terminate the process or unload resident memory.

## Storage

Eden and School source files remain readable text and XML. WAV sensory artifacts remain ordinary validated WAV files. Runtime events are appended to readable logs.

## Allocation

All runtime capacities are compile-time constants. The source contains no calls to `malloc`, `calloc`, `realloc`, or `free`.

## Current limit

This MVP is a deterministic School and memory runtime. It is not an operating system, autonomous shell, hidden language model, or proof of consciousness.
