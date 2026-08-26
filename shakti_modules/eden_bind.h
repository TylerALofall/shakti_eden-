/*
 * EDEN BIND — geometric binding of binary senses.
 *
 * [RAISED 2026-08-23, Tyler's fire]
 *
 * A color (from eyes) and a sound (from ears) do not "fuse."
 * They BIND through shared geometric structure.
 *
 * The binding is a 16-byte moment: 8 from sight hash, 8 from sound hash,
 * bound under the heartbeat tick — same as the duct convergence.
 * But the binding is GEOMETRIC: it exists in TET space.
 *
 * Purity: C99, static storage, no heap, no float, no clock, no subprocess.
 */
#ifndef EDEN_BIND_H
#define EDEN_BIND_H

#include <stdint.h>
#include <stddef.h>

#define BIND_HASH_BYTES   8U
#define BIND_MOMENT_BYTES 16U
#define BIND_MAX_BINDINGS 256U

typedef struct {
    uint64_t     sight_hash;
    uint64_t     sound_hash;
    uint64_t     beat;
    unsigned int cell;
    unsigned int strength;
} bind_moment_t;

typedef struct {
    bind_moment_t slots[BIND_MAX_BINDINGS];
    unsigned int  count;
    unsigned int  next_victim;
    uint64_t      pin;
} bind_table_t;

int                  bind_init(void);
int                  bind_create(uint64_t sight_hash, uint64_t sound_hash,
                                 uint64_t beat);
const bind_moment_t *bind_lookup_sight(uint64_t sight_hash);
const bind_moment_t *bind_lookup_sound(uint64_t sound_hash);
int                  bind_reinforce(uint64_t sight_hash, uint64_t sound_hash);
unsigned int         bind_at_cell(unsigned int cell, const bind_moment_t **out,
                                  unsigned int max_out);
int                  bind_converges(uint64_t sight_hash, uint64_t sound_hash,
                                    unsigned int *cell_out);
int                  bind_verify(void);
uint64_t             bind_pin(void);
void                 bind_print_bindings(void);

#endif
