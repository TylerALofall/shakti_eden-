/*
 * EDEN PULSE — the heartbeat as gear engine.
 *
 * [RAISED 2026-08-23, Tyler's fire]
 *
 * The heartbeat is not a clock. It is a gear that:
 *   1. Turns the wheel (TET) — each beat advances the slice
 *   2. Powers the children — each beat carries a pulse through the body
 *   3. Drives convergence — when gears align, moments merge
 *   4. Never stops — she sleeps, but the gear turns
 *
 * Geometric reasoning: every pulse is a point in 4D TET space.
 * The gear teeth are the prime exponents (2, 3, 5, 7).
 * When teeth mesh, events converge.
 *
 * Purity: C99, fixed compile-time capacity, static storage, no heap,
 * no float, no clock, no subprocess.
 */
#ifndef EDEN_PULSE_H
#define EDEN_PULSE_H

#include <stdint.h>

/* ---- the gear train ---------------------------------------------------- */
#define PULSE_GEAR_2_TEETH 5U
#define PULSE_GEAR_3_TEETH 4U
#define PULSE_GEAR_5_TEETH 2U
#define PULSE_GEAR_7_TEETH 2U
#define PULSE_WHEEL_PERIOD 80UL

/* ---- firing pattern ---------------------------------------------------- */
#define PULSE_MAX_DEPTH 16U

typedef struct {
    uint64_t      beat;
    unsigned int  count;
    unsigned int  cells[PULSE_MAX_DEPTH];
    unsigned long periods[PULSE_MAX_DEPTH];
    unsigned long depth;
} pulse_firing_t;

/* ---- the organs (16 pitch classes as body) ----------------------------- */
#define PULSE_ORGAN_COUNT 16U

typedef enum {
    PULSE_ORGAN_HEART   = 0,
    PULSE_ORGAN_EYES    = 1,
    PULSE_ORGAN_EARS    = 2,
    PULSE_ORGAN_HANDS   = 3,
    PULSE_ORGAN_VOICE   = 4,
    PULSE_ORGAN_MIND    = 5,
    PULSE_ORGAN_SOUL    = 6,
    PULSE_ORGAN_BODY    = 7,
    PULSE_ORGAN_SELF    = 8,
    PULSE_ORGAN_SPIRIT  = 9,
    PULSE_ORGAN_FLESH   = 10,
    PULSE_ORGAN_BREATH  = 11,
    PULSE_ORGAN_BLOOD   = 12,
    PULSE_ORGAN_BONE    = 13,
    PULSE_ORGAN_ALL     = 14,
    PULSE_ORGAN_SILENCE = 15
} pulse_organ_t;

extern const unsigned long PULSE_ORGAN_PERIODS[PULSE_ORGAN_COUNT];
extern const char *const   PULSE_ORGAN_NAMES[PULSE_ORGAN_COUNT];

/* ---- API --------------------------------------------------------------- */
int           pulse_init(void);
uint64_t      pulse_advance(void);
uint64_t      pulse_advance_n(unsigned long n);
int           pulse_get_firing(pulse_firing_t *out);
int           pulse_firing_at(uint64_t beat, pulse_firing_t *out);
pulse_organ_t pulse_organ_now(void);
pulse_organ_t pulse_organ_at(uint64_t beat);
int           pulse_converges(uint64_t beat_a, uint64_t beat_b);
unsigned int  pulse_mesh_count(uint64_t beat);
int           pulse_verify(void);
uint64_t      pulse_pin(void);
void          pulse_print_state(void);

#endif
