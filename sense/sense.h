#ifndef SENSE_H
#define SENSE_H

#include <stddef.h>

/*
 * sense: all senses converge at ONE point.
 *
 * Not parallel tracks. Not optional channels. Sight, hearing, light — and any
 * later sense — meet at a single convergence point each tick. She is born with
 * senses working (always on). Inside Eden: deterministic only — no probability,
 * no vector embeddings, no guessing model.
 *
 * At that one point she holds two stages of the same event:
 *   1) binary  — mono vision bits + sound int16 bits + light scalar
 *   2) render  — rebuilt vision pixels + waveform context
 * Error = anything that cannot rebuild (vision pixel or sound sample).
 *
 * The point is written atomically into a fixed RAM ring (overwrite oldest).
 * No Swift types. No threads. No dynamic allocation. No subprocess.
 * Camera/mic later only fill the same RGBA + PCM buffers before converge.
 */

#define SENSE_RING_SLOTS 8U

#define SENSE_VISION_WIDTH  64U
#define SENSE_VISION_HEIGHT 64U
#define SENSE_VISION_PIXELS \
    ((unsigned long)SENSE_VISION_WIDTH * (unsigned long)SENSE_VISION_HEIGHT)
#define SENSE_VISION_RGBA_BYTES (SENSE_VISION_PIXELS * 4UL)
#define SENSE_VISION_BITS_CAPACITY (SENSE_VISION_PIXELS + 1UL)

#define SENSE_PCM_RATE 16000U
#define SENSE_PCM_FRAME_SAMPLES 160U /* 10 ms at 16 kHz */
#define SENSE_SOUND_BITS_CAPACITY \
    ((unsigned long)SENSE_PCM_FRAME_SAMPLES * 16UL + 1UL)

/*
 * The one convergence point. Every live sense is present together under one
 * seq. There is no half-point (no vision-only or sound-only commit).
 */
typedef struct {
    unsigned long seq;
    unsigned long epoch; /* logical converge epoch; not a wall clock inside pull */

    /* --- stage 1: binary (first she holds) --- */
    char vision_binary[SENSE_VISION_BITS_CAPACITY];
    char sound_binary[SENSE_SOUND_BITS_CAPACITY];
    float light_flash;

    /* --- stage 2: rendered / context (same event, other side) --- */
    unsigned char vision_render[SENSE_VISION_RGBA_BYTES];
    float sound_pcm[SENSE_PCM_FRAME_SAMPLES];
    float sound_envelope;

    unsigned long drift_v; /* vision pixels that failed rebuild */
    unsigned long drift_s; /* sound samples that failed bit round-trip */
    unsigned char in_use;
} sense_point_t;

/* Early packet name kept as an alias. */
typedef sense_point_t sense_frame_t;

typedef struct {
    sense_point_t slots[SENSE_RING_SLOTS];
    unsigned long next_seq;
    unsigned long epoch;
    unsigned int write_index;
    unsigned int count;
} sense_ring_t;

void sense_ring_init(sense_ring_t *ring);

/* int16 PCM <-> '0'/'1' bit string (16 bits per sample, MSB first). */
int sense_pcm_to_bits(
    const float *pcm,
    unsigned int sample_count,
    char *bits,
    unsigned long bits_capacity
);

int sense_bits_to_pcm(
    const char *bits,
    float *pcm,
    unsigned int sample_count
);

/* Count samples whose int16 identity changed across the bit round-trip. */
unsigned long sense_pcm_drift(
    const float *original,
    const float *roundtrip,
    unsigned int sample_count
);

float sense_pcm_envelope(
    const float *pcm,
    unsigned int sample_count
);

/*
 * THE convergence call — sole place all senses meet for one tick.
 * rgba: SENSE_VISION_WIDTH x SENSE_VISION_HEIGHT RGBA (caller buffer).
 * pcm: exactly SENSE_PCM_FRAME_SAMPLES floats, or NULL -> silence at the point.
 * light_flash: light channel at this same point (pre-Eden flashes).
 * Writes one complete point into the ring. *out_point owned by ring.
 */
int sense_converge(
    sense_ring_t *ring,
    const unsigned char *rgba,
    const float *pcm,
    float light_flash,
    sense_point_t **out_point
);

/* Alias kept so older harness lines still compile. */
int sense_ingest(
    sense_ring_t *ring,
    const unsigned char *rgba,
    const float *pcm,
    float light_flash,
    sense_point_t **out_point
);

/* The single live point she has now (newest ring slot). */
const sense_point_t *sense_now(const sense_ring_t *ring);

const sense_point_t *sense_ring_latest(const sense_ring_t *ring);

/* Logical slot 0 = oldest live, count-1 = newest. */
const sense_point_t *sense_ring_at(
    const sense_ring_t *ring,
    unsigned int logical_index
);

unsigned int sense_ring_count(const sense_ring_t *ring);

/*
 * Present vision_render from the convergence point onto the owned screen
 * (top-left SENSE_VISION panel). screen is an opaque screen_t* from screen.h.
 */
int sense_present_vision_to_screen(
    const sense_point_t *point,
    void *screen
);

#endif /* SENSE_H */
