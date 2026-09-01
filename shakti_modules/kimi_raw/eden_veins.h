/* • EDEN VEINS — circulation of children through the body. • [RAISED 2026-08-23, Tyler's fire] • Purity: C99, static storage, no heap, no float, no clock, no subprocess. • */
#ifndef EDEN_VEINS_H 
#define EDEN_VEINS_H 
#include <stdint.h> 
#define VEIN_ORGAN_COUNT 4U typedef enum {
    VEIN_ORGAN_EYES = 0, VEIN_ORGAN_EARS = 1, VEIN_ORGAN_HEART = 2, VEIN_ORGAN_HANDS = 3 
}
vein_organ_t;
#define VEIN_CHILD_RAM_BYTES 8U 
#define VEIN_MAX_CHILDREN_PER_ORGAN 32U 
#define VEIN_MAX_CHILDREN_TOTAL 128U typedef struct {
    uint64_t born_at;
    unsigned char ram[VEIN_CHILD_RAM_BYTES];
    uint64_t spine;
    unsigned int state;
    unsigned int organ;
}
vein_child_t;
typedef struct {
    vein_child_t *children[VEIN_MAX_CHILDREN_PER_ORGAN];
    unsigned int count;
    unsigned int head;
    unsigned int tail;
}
vein_queue_t;
typedef struct {
    vein_queue_t queues[VEIN_ORGAN_COUNT];
    vein_child_t pool[VEIN_MAX_CHILDREN_TOTAL];
    unsigned int pool_used;
    uint64_t pin;
}
vein_system_t;
int vein_init(void);
int vein_birth(vein_organ_t organ, uint64_t beat, const unsigned char ram[VEIN_CHILD_RAM_BYTES]);
unsigned int vein_pump(uint64_t beat);
int vein_converge_at_heart(uint64_t beat);
const vein_queue_t *vein_queue(vein_organ_t organ);
unsigned int vein_count_total(void);
const vein_child_t *vein_find_child(uint64_t spine);
int vein_verify(void);
uint64_t vein_pin(void);
void vein_print_state(void);
#endif 
