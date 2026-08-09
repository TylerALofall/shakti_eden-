# Shakti Hearing and Prenatal Grounding Module

## Files in this directory:
- `hearing/README.md` (this file)
- `hearing/hearing.h` (C99 interface)
- `hearing/hearing.c` (top-level orchestration)
- `hearing/hearing_synth.c` (deterministic pulse and wave synthesizers)
- `hearing/hearing_model.c` (RNN/GRU learning and backpropagation)

---

## 1. Vision & Architecture

The prenatal subconscious stage represents the foundation of Shakti's neural development. By training on a continuous, rhythmic maternal heartbeat and synchronized flashes of light, we establish a time-sensitive, cross-sensory temporal clock inside the network. This subconscious "instinct" is trained in pure, deterministic C99 prior to the introduction of alphanumeric glyphs (Level 0) or numbers (Level 1).

## 2. Mathematical Foundation (The 432 Abundant Number)

Gestation is parameterized in our C99 system by scaling down the colossal abundant number properties associated with human pregnancy (represented by the base divisor relationships of 432, whose prime factors are $2^4 \times 3^3$).

The prenatal clock is structured around a sequence where:
- **Phase 1: Dark Acoustic (0.0s to 20.0s):** Absolute visual darkness. The auditory model trains exclusively on the low-pass maternal heartbeat wave.
- **Phase 2: Sensory Entrainment (20.0s+):** Cross-modal stimulation begins. Warm, low-frequency flashes of light are synthesized synchronously with the systolic contraction peak of the maternal heartbeat.

## 3. Modality Details

- **Maternal Heartbeat Synthesis:** Synthesized deterministically using dual-tone low-frequency pulse equations (resonant tones at 30 Hz and 120 Hz) simulating the low-pass acoustics of the uterine wall.
- **Flashes of Light:** Generated as an 8x8 spatial grid of intensity values. The brightness peaks exactly at the systole of the heartbeat, creating a natural correlation between visual and auditory inputs.

## 4. Compilation & Verification

The module is written in strict, standard ANSI C99 and is compiled using standard GCC/Clang:
```sh
make test
```
The implementation has zero external dependencies, no threads, and no Python.
