/* • EDEN PHASE — the sisters as simultaneous phases, not sequential lessons. • [RAISED 2026-08-23, Tyler's fire — Hyper-perception, multi-position] • Purity: C99, static storage, no heap, no float, no clock, no subprocess. • */
#ifndef EDEN_PHASE_H 
#define EDEN_PHASE_H 
#include <stdint.h> typedef enum {
    PHASE_SISTER_DESTROYER = 0, PHASE_SISTER_PRESERVER = 1, PHASE_SISTER_CREATOR = 2, PHASE_SISTER_COUNT = 3 
}
phase_sister_t;
typedef struct {
    unsigned int active;
    unsigned int destroyer_present;
    unsigned int preserver_present;
    unsigned int creator_present;
    uint64_t destroyer_spine;
    uint64_t preserver_spine;
    uint64_t creator_spine;
    uint64_t i_spine;
    unsigned int lesson_threshold;
    unsigned int lessons_passed;
}
phase_state_t;
typedef struct {
    phase_sister_t sister;
    unsigned int weight;
    uint64_t beat;
    unsigned int proven;
}
phase_slide_t;
#define PHASE_MAX_SLIDES 4096U int phase_init(void);
int phase_shift(phase_sister_t sister, uint64_t beat);
phase_sister_t phase_active(void);
int phase_trinity_present(void);
int phase_i_active(void);
int phase_slide(phase_sister_t sister, unsigned int weight, uint64_t beat);
unsigned int phase_slides(phase_sister_t sister, const phase_slide_t **out, unsigned int max_out);
uint64_t phase_perceive(uint64_t beat);
unsigned int phase_position(phase_sister_t sister, uint64_t beat);
void phase_print_state(void);
int phase_verify(void);
uint64_t phase_pin(void);
#endif 
