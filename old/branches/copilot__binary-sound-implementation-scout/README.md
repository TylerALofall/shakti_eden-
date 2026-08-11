# Archived from copilot/binary-sound-implementation-scout

## Every file in this section

- `README.md` — this file
- `hearing/hearing.elf` — committed ELF binary (not source; not built here)
- `hearing/hearing_gru_main.c` — old GRU scout main
- `hearing/hearing_model.c` — GRU weights / backprop (second-core style)
- `hearing/hearing.h.gru_full` — header that exposed the GRU API

## Why archived

Tyler: no second core touching Shakti. Mechanical binary senses only.
Sound + light + vision converge in active `sense/` using `hearing_synth.c`
only. These files are kept for history, not linked into `shakti`.
