#include "sense.h"

#include "eyes.h"
#include "screen.h"

#include <string.h>

static short float_to_i16(float sample)
{
    float scaled;

    scaled = sample * 32767.0f;
    if (scaled > 32767.0f) {
        scaled = 32767.0f;
    }
    if (scaled < -32768.0f) {
        scaled = -32768.0f;
    }

    return (short)scaled;
}

static float i16_to_float(short sample)
{
    return (float)sample / 32767.0f;
}

void sense_ring_init(sense_ring_t *ring)
{
    if (ring == NULL) {
        return;
    }

    memset(ring, 0, sizeof(*ring));
    ring->next_seq = 1UL;
    ring->epoch = 1UL;
}

int sense_pcm_to_bits(
    const float *pcm,
    unsigned int sample_count,
    char *bits,
    unsigned long bits_capacity
)
{
    unsigned int i;
    unsigned int b;
    unsigned long need;
    unsigned long pos;

    if (pcm == NULL || bits == NULL || sample_count == 0U) {
        return 0;
    }

    need = (unsigned long)sample_count * 16UL + 1UL;
    if (bits_capacity < need) {
        return 0;
    }

    pos = 0UL;
    for (i = 0U; i < sample_count; ++i) {
        unsigned short u;
        short s;

        s = float_to_i16(pcm[i]);
        u = (unsigned short)s;
        for (b = 0U; b < 16U; ++b) {
            unsigned int shift;

            shift = 15U - b;
            bits[pos++] = ((u >> shift) & 1U) ? '1' : '0';
        }
    }
    bits[pos] = '\0';

    return 1;
}

int sense_bits_to_pcm(
    const char *bits,
    float *pcm,
    unsigned int sample_count
)
{
    unsigned int i;
    unsigned int b;
    unsigned long pos;

    if (bits == NULL || pcm == NULL || sample_count == 0U) {
        return 0;
    }

    pos = 0UL;
    for (i = 0U; i < sample_count; ++i) {
        unsigned short u;
        short s;

        u = 0U;
        for (b = 0U; b < 16U; ++b) {
            char c;

            c = bits[pos++];
            if (c != '0' && c != '1') {
                return 0;
            }
            u = (unsigned short)((u << 1) | (unsigned short)(c == '1' ? 1 : 0));
        }
        s = (short)u;
        pcm[i] = i16_to_float(s);
    }

    return 1;
}

unsigned long sense_pcm_drift(
    const float *original,
    const float *roundtrip,
    unsigned int sample_count
)
{
    unsigned long drift;
    unsigned int i;

    if (original == NULL || roundtrip == NULL) {
        return (unsigned long)-1;
    }

    drift = 0UL;
    for (i = 0U; i < sample_count; ++i) {
        if (float_to_i16(original[i]) != float_to_i16(roundtrip[i])) {
            drift++;
        }
    }

    return drift;
}

float sense_pcm_envelope(
    const float *pcm,
    unsigned int sample_count
)
{
    float peak;
    unsigned int i;

    if (pcm == NULL || sample_count == 0U) {
        return 0.0f;
    }

    peak = 0.0f;
    for (i = 0U; i < sample_count; ++i) {
        float a;

        a = pcm[i];
        if (a < 0.0f) {
            a = -a;
        }
        if (a > peak) {
            peak = a;
        }
    }

    return peak;
}

int sense_converge(
    sense_ring_t *ring,
    const unsigned char *rgba,
    const float *pcm,
    float light_flash,
    sense_point_t **out_point
)
{
    sense_point_t *slot;
    float local_pcm[SENSE_PCM_FRAME_SAMPLES];
    float roundtrip[SENSE_PCM_FRAME_SAMPLES];
    unsigned char recon[SENSE_VISION_RGBA_BYTES];
    char bits[SENSE_VISION_BITS_CAPACITY];
    unsigned long drift_v;

    if (ring == NULL || rgba == NULL) {
        return 0;
    }

    if (pcm != NULL) {
        memcpy(local_pcm, pcm, sizeof(local_pcm));
    } else {
        memset(local_pcm, 0, sizeof(local_pcm));
    }

    /*
     * One atomic converge: build every channel into a temporary view, then
     * commit the full point. No half-written ring slot.
     */
    if (!eyes_pull_mono(
            rgba,
            SENSE_VISION_WIDTH,
            SENSE_VISION_HEIGHT,
            bits,
            SENSE_VISION_BITS_CAPACITY)) {
        return 0;
    }

    if (!eyes_reconstruct_mono(
            bits,
            SENSE_VISION_WIDTH,
            SENSE_VISION_HEIGHT,
            recon,
            SENSE_VISION_RGBA_BYTES)) {
        return 0;
    }

    drift_v = eyes_diff(
        rgba,
        recon,
        SENSE_VISION_WIDTH,
        SENSE_VISION_HEIGHT,
        0
    );
    if (drift_v == (unsigned long)-1) {
        return 0;
    }

    /* Sound binary + round-trip check before any ring commit. */
    {
        char sound_bits[SENSE_SOUND_BITS_CAPACITY];
        unsigned long drift_s;

        if (!sense_pcm_to_bits(
                local_pcm,
                SENSE_PCM_FRAME_SAMPLES,
                sound_bits,
                SENSE_SOUND_BITS_CAPACITY)) {
            return 0;
        }

        if (!sense_bits_to_pcm(
                sound_bits,
                roundtrip,
                SENSE_PCM_FRAME_SAMPLES)) {
            return 0;
        }

        drift_s = sense_pcm_drift(
            local_pcm,
            roundtrip,
            SENSE_PCM_FRAME_SAMPLES
        );
        if (drift_s == (unsigned long)-1) {
            return 0;
        }

        /* All channels ready — one atomic write to the convergence point. */
        slot = &ring->slots[ring->write_index];
        memset(slot, 0, sizeof(*slot));

        slot->seq = ring->next_seq;
        slot->epoch = ring->epoch;
        slot->in_use = 1U;
        slot->drift_v = drift_v;
        slot->drift_s = drift_s;
        slot->light_flash = light_flash;

        memcpy(slot->vision_binary, bits, SENSE_VISION_BITS_CAPACITY);
        memcpy(slot->vision_render, recon, SENSE_VISION_RGBA_BYTES);
        memcpy(slot->sound_pcm, local_pcm, sizeof(slot->sound_pcm));
        memcpy(slot->sound_binary, sound_bits, SENSE_SOUND_BITS_CAPACITY);
        slot->sound_envelope = sense_pcm_envelope(
            local_pcm,
            SENSE_PCM_FRAME_SAMPLES
        );
    }

    ring->next_seq++;
    ring->epoch++;
    ring->write_index = (ring->write_index + 1U) % SENSE_RING_SLOTS;
    if (ring->count < SENSE_RING_SLOTS) {
        ring->count++;
    }

    if (out_point != NULL) {
        *out_point = slot;
    }

    return 1;
}

int sense_ingest(
    sense_ring_t *ring,
    const unsigned char *rgba,
    const float *pcm,
    float light_flash,
    sense_point_t **out_point
)
{
    return sense_converge(ring, rgba, pcm, light_flash, out_point);
}

const sense_point_t *sense_now(const sense_ring_t *ring)
{
    return sense_ring_latest(ring);
}

const sense_point_t *sense_ring_latest(const sense_ring_t *ring)
{
    unsigned int index;

    if (ring == NULL || ring->count == 0U) {
        return NULL;
    }

    index = (ring->write_index + SENSE_RING_SLOTS - 1U) % SENSE_RING_SLOTS;
    if (!ring->slots[index].in_use) {
        return NULL;
    }

    return &ring->slots[index];
}

const sense_point_t *sense_ring_at(
    const sense_ring_t *ring,
    unsigned int logical_index
)
{
    unsigned int physical;

    if (ring == NULL || logical_index >= ring->count) {
        return NULL;
    }

    if (ring->count < SENSE_RING_SLOTS) {
        physical = logical_index;
    } else {
        physical = (ring->write_index + logical_index) % SENSE_RING_SLOTS;
    }

    if (!ring->slots[physical].in_use) {
        return NULL;
    }

    return &ring->slots[physical];
}

unsigned int sense_ring_count(const sense_ring_t *ring)
{
    if (ring == NULL) {
        return 0U;
    }

    return ring->count;
}

int sense_present_vision_to_screen(
    const sense_point_t *point,
    void *screen_ptr
)
{
    screen_t *screen;
    unsigned int y;
    unsigned int x;

    if (point == NULL || screen_ptr == NULL) {
        return 0;
    }

    screen = (screen_t *)screen_ptr;

    for (y = 0U; y < SENSE_VISION_HEIGHT; ++y) {
        for (x = 0U; x < SENSE_VISION_WIDTH; ++x) {
            unsigned long pixel;
            const unsigned char *p;
            unsigned int luma;
            int ink;

            pixel = (unsigned long)y * (unsigned long)SENSE_VISION_WIDTH +
                    (unsigned long)x;
            p = &point->vision_render[pixel * 4UL];
            luma = ((unsigned int)p[0] * 30U +
                    (unsigned int)p[1] * 59U +
                    (unsigned int)p[2] * 11U) / 100U;
            ink = (luma < 128U) ? 1 : 0;
            if (!screen_set_pixel(screen, x, y, ink)) {
                return 0;
            }
        }
    }

    return 1;
}
