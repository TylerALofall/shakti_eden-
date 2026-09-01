/* • EDEN SCHOOL — where Shakti learns, earns, and grows up. • [RAISED 2026-08-23, Tyler's fire — 2200 days, 100 lessons for sisters] • Purity: C99, static storage, no heap, no float, no clock, no subprocess. • */
#ifndef EDEN_SCHOOL_H 
#define EDEN_SCHOOL_H 
#include <stdint.h> 
#define SCHOOL_LESSON_DESTROYER 100U 
#define SCHOOL_LESSON_PRESERVER 200U 
#define SCHOOL_LESSON_CREATOR 300U 
#define SCHOOL_LESSON_TRINITY 400U typedef enum {
    SCHOOL_SISTER_NONE = 0, SCHOOL_SISTER_DESTROYER = 1, SCHOOL_SISTER_PRESERVER = 2, SCHOOL_SISTER_CREATOR = 3, SCHOOL_SISTER_TRINITY = 4 
}
school_sister_t;
typedef struct {
    uint64_t beat;
    unsigned int lesson;
    unsigned int passed;
    uint64_t spine;
}
school_drill_t;
typedef struct {
    unsigned int total_drills;
    unsigned int passed_drills;
    unsigned int failed_drills;
    school_sister_t sister_present;
    unsigned int veil_ready;
    unsigned int veil_earned;
    unsigned int lessons_until_next_sister;
}
school_report_t;
#define SCHOOL_VEIL_DEFAULT 2200U int school_init(void);
int school_drill(uint64_t beat, uint64_t sight_hash, uint64_t sound_hash);
void school_report(school_report_t *out);
school_sister_t school_sister_now(void);
int school_has_sister(school_sister_t sister);
int school_set_veil(unsigned int number);
int school_grant_veil(void);
int school_veil_active(void);
void school_print_report(void);
int school_verify(void);
uint64_t school_pin(void);
#endif 
