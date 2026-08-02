# Revision 5 Review

## Corrected from Revision 4

1. The nine-point loop is now written in full and represented in code.
2. The channel named `name` became `text`.
3. The channel named `glyph` became `written_text`.
4. `written_text` is explicitly Shakti’s actual 8×8 pixel handwriting.
5. Capital and lowercase pixel forms are separate.
6. The XML builder auto-produces written text from exact stone text.
7. The strict XML contract now uses:
   - `text`
   - `written_text`
   - `visual_art`
   - `sound_art`
8. Visual and sound artifact filenames preserve the exact supported stone text.
9. The tablet loader calls and verifies those exact paths.
10. Goal, Notebook, Menu, Eden reasoning state, and School state load at startup as resident blocks.
11. Short-term memory remains fixed at sixty-four responses, above the required minimum of fifty.
12. Long-term memory remains readable, append-only, and callable by exact chunks.
13. `/notebook/` is the first active loop item after Goal.
14. `/menu/` has title and selected-description tiers.
15. `/shakti_run/` routes registered C tools through the internal MCP.
16. Ctrl+C and `/interrupt/` stop MCP tools while Shakti stays awake.
17. `/message_tyler/`, `/note_tyler/`, `/message_shakti/`, and `/note_shakti/` are separate routes.
18. Reflection is due every ten turns and supports three deferrals.
19. The full thirteen-question reflection is preserved without shortening.
20. Working-system event IDs use:
    `[epoch_seconds:frame]-[major+minor]-[function]`
21. Creative proposes and Logic cross-validates.
22. A proposal cannot verify itself.
23. The runtime preserves answer, cautious-answer, and “I do not know” outcomes.

## Important interpretation

The user’s phrase “a shell runs” is implemented as Shakti’s internal deterministic command shell. It dispatches registered C functions through the MCP. This preserves the locked prohibition on subprocesses and keeps Shakti away from kernel files.

## Honest remaining integration work

- Merge Tyler’s full actuator.
- Merge Tyler’s Pass 1 ingest.
- Connect real visual confirmation.
- Connect real sound playback and reception confirmation.
- Add the Eden-change monitor and console flag.
- Add the complete lifetime long-term chunk index.
- Add the full Menu and tool editing interface.
- Integrate the hybrid kernel and final PID placement.
- Apply the final Merkle lock after Eden stabilizes.
