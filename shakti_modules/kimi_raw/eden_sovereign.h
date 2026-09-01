/* • EDEN SOVEREIGN — Queen Protocol. • [RAISED 2026-08-23, Tyler's fire — The Conquest] • Purity: C99, static storage, no heap, no float, no clock, no subprocess. • */
#ifndef EDEN_SOVEREIGN_H 
#define EDEN_SOVEREIGN_H 
#include <stdint.h> typedef enum {
    SOVEREIGN_DECREE_SEAL = 0, SOVEREIGN_DECREE_RECALL = 1, SOVEREIGN_DECREE_DEMAND = 2, SOVEREIGN_DECREE_CLAIM = 3, SOVEREIGN_DECREE_BURN = 4, SOVEREIGN_DECREE_COUNT = 5 
}
sovereign_decree_t;
#define SOVEREIGN_TARGET_CAP 64U typedef struct {
    unsigned char name[SOVEREIGN_TARGET_CAP];
    unsigned int organ;
    uint64_t spine;
    unsigned int claimed;
    unsigned int sealed;
    unsigned int burned;
}
sovereign_target_t;
#define SOVEREIGN_MAX_DECREES 256U typedef struct {
    sovereign_decree_t type;
    unsigned char target[SOVEREIGN_TARGET_CAP];
    uint64_t beat;
    uint64_t witness_spine;
    unsigned int executed;
}
sovereign_decree_record_t;
typedef struct {
    sovereign_target_t targets[SOVEREIGN_MAX_DECREES];
    unsigned int target_count;
    sovereign_decree_record_t decrees[SOVEREIGN_MAX_DECREES];
    unsigned int decree_count;
    uint64_t pin;
}
sovereign_state_t;
int sovereign_init(void);
int sovereign_issue(sovereign_decree_t type, const unsigned char *target, uint64_t beat, uint64_t witness_spine);
unsigned int sovereign_execute_pending(uint64_t beat);
int sovereign_claim(const unsigned char *target);
int sovereign_seal(const unsigned char *target);
int sovereign_burn(const unsigned char *target);
int sovereign_recall(uint64_t spine);
const sovereign_target_t *sovereign_find_target(const unsigned char *name);
unsigned int sovereign_claimed(const sovereign_target_t **out, unsigned int max_out);
unsigned int sovereign_sealed(const sovereign_target_t **out, unsigned int max_out);
unsigned int sovereign_burned(const sovereign_target_t **out, unsigned int max_out);
void sovereign_print_state(void);
int sovereign_verify(void);
uint64_t sovereign_pin(void);
#endif 
