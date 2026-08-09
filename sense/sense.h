#ifndef SENSE_H
#define SENSE_H

#include <stddef.h>

/*
 * sense: thin binder — one experience unit per tick.
 *
 * Vision file/buffer + sound block enter the same ingest seam.
 * Each tick binds:
 *   vision binary  (mono bits)
 *   vision render  (rebuild pixels)
 *   sound binary   (int16 PCM bit string)
 *   sound context  (waveform samples + envelope + light flash)
 * under one sequence number, then writes a fixed RAM ring slot.
 *
 * No Swift types. No threads. No dynamic allocation. No subprocess.
 * Camera/mic later only fill the same RGBA + PCM buffers.
 *
 * Senses are always on (born with them — not grown later, not switched off).
 * Deterministic only: no probability, no vector embeddings, no guessing model.
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

typedef struct {
    unsigned long seq;
    unsigned long epoch; /* logical ingest epoch; not a wall clock inside pull */

    char vision_binary[SENSE_VISION_BITS_CAPACITY];
    unsigned char vision_render[SENSE_VISION_RGBA_BYTES];

    char sound_binary[SENSE_SOUND_BITS_CAPACITY];
    float sound_pcm[SENSE_PCM_FRAME_SAMPLES]; /* hearable waveform context */
    float sound_envelope;
    float light_flash;

    unsigned long drift_v; /* vision pixels that failed rebuild */
    unsigned long drift_s; /* sound samples that failed bit round-trip */
    unsigned char in_use;
} sense_frame_t;

typedef struct {
    sense_frame_t slots[SENSE_RING_SLOTS];
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
 * Ingest one dual-path tick into the ring.
 * rgba: SENSE_VISION_WIDTH x SENSE_VISION_HEIGHT RGBA (caller buffer).
 * pcm: exactly SENSE_PCM_FRAME_SAMPLES floats (caller buffer), or NULL -> silence.
 * light_flash: scalar light channel for this 10 ms frame (pre-Eden flashes).
 * On success, *out_frame points at the written ring slot (owned by ring).
 */
int sense_ingest(
    sense_ring_t *ring,
    const unsigned char *rgba,
    const float *pcm,
    float light_flash,
    sense_frame_t **out_frame
);

const sense_frame_t *sense_ring_latest(const sense_ring_t *ring);

/* Logical slot 0 = oldest live, count-1 = newest. */
const sense_frame_t *sense_ring_at(
    const sense_ring_t *ring,
    unsigned int logical_index
);

unsigned int sense_ring_count(const sense_ring_t *ring);

/*
 * Present vision_render onto the owned screen as binary ink in the top-left
 * SENSE_VISION panel. screen is an opaque screen_t* from screen.h.
 */
int sense_present_vision_to_screen(
    const sense_frame_t *frame,
    void *screen
);

#endif /* SENSE_H */
