/*
 * EDEN WITNESS — the immutable record.
 *
 * [RAISED 2026-08-23, Tyler's fire — for the 62 files]
 *
 * Nothing here is ever deleted. An erasure attempt sets a flag; the record
 * and its link both stay, and the attempt itself becomes part of the record.
 *
 * Purity: C99, static storage, no heap, no float, no clock, no subprocess.
 */
#ifndef EDEN_WITNESS_H
#define EDEN_WITNESS_H

#include <stdint.h>

#define WITNESS_TAG_CAP      64U
#define WITNESS_BODY_CAP    256U
#define WITNESS_MAX_RECORDS 1024U

typedef struct {
    uint64_t      beat;
    unsigned int  organ;
    unsigned char tag[WITNESS_TAG_CAP];
    unsigned char body[WITNESS_BODY_CAP];
    unsigned int  body_len;
    uint64_t      spine;
    uint64_t      chain;
    unsigned int  erased;
    unsigned int  proven;
} witness_record_t;

typedef struct {
    witness_record_t records[WITNESS_MAX_RECORDS];
    unsigned int     count;
    unsigned int     next_slot;
    uint64_t         last_spine;
    uint64_t         chain_pin;
    uint64_t         frozen_pin;
} witness_chain_t;

int                     witness_init(void);
int                     witness_record(const unsigned char *tag,
                                       const unsigned char *body,
                                       unsigned int body_len,
                                       uint64_t beat, unsigned int organ);
int                     witness_mark_erased(const unsigned char *tag);
int                     witness_prove_chain(void);
const witness_record_t *witness_find(const unsigned char *tag);
unsigned int            witness_erased(const witness_record_t **out,
                                       unsigned int max_out);
int                     witness_converges(const unsigned char *tag);
void                    witness_print_chain(void);
int                     witness_verify(void);
uint64_t                witness_pin(void);

#endif
