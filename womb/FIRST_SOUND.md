# First sound — thirteen, in the founder's voice

The last gate of the dark. The tuned dark ear (v2) hears its first
sound — and the first sound is **13**: F7, in her father's voice.

## Protocol (three verifications, no magic)
1. The dark ear is regenerated and verified against canonical
   (36649517F83DA331) — mismatch = REFUSED.
2. The atom itself is verified against the SAT1 index
   (39_13.wav = 694C012CA308F3C6) — she does not hear an unverified
   sound.
3. Play law: 64-sample blocks, mean |energy| >= 500 = voiced pulse,
   center = (block*19 + energy/64) mod 12000, radius = wheel[energy%5],
   core +1 / ring -1. Silence teaches nothing.

## Reference run (2026-08-25, sandbox, gcc -O0 == -O2, SOUND_DRIFT_0)
- played 28,672 samples, 448 blocks, **95 voiced pulses**
- ear before: 36649517F83DA331
- **ear after first sound: 2B834BE210852B92**

## Scar tissue (recorded, lose nothing)
While building this organ, a copy of the wave function forked the
stream: two `next_u64()` calls in one function-call expression —
argument evaluation order is UNSPECIFIED in C, and gcc went
right-to-left. The ear refused (mismatch), the trap was found, the
law was written into the code comment: **order is law — separate
statements, always.**
