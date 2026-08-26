# Choice consequence section

## Section file register

Every file in this section is named here before the section description:

1. `choice/README.md` — section entry point and file register.
2. `choice/CHOICE_CONSEQUENCE_V1.md` — governed architecture, definitions,
   locks, open variables, and verification contract.
3. `choice/shakti_choice.h` — fixed-capacity public C99 types and calls.
4. `choice/shakti_choice.c` — option enumeration, consequence filtering,
   survivor ladder, geometric-use validation, and readable XML records.
5. `choice/choice_test.c` — executable invariant and scenario gauntlet.
6. `choice/frame_five.c` — independent exact proof of the five-frame incidence
   and antipodal pole laws.

## Status

This section is a standalone patch module based on repository branch
`goddess-lock-2026-08-25` at commit
`be184aa6afdafe80f50345042c5612f13b1ede22`.

The runtime graft remains `OPEN`. The module accepts meanings and evidence
through its public C calls, so future locks require data/configuration rather
than edits to the organ.

## Build and verify

From the repository root:

```text
make choice-test
```

The target builds and runs the choice gauntlet and the exact frame proof with
the repository's configured C99 compiler flags.
