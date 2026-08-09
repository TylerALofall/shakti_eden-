#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "eyes.h"
#include "hearing.h"
#include "screen.h"
#include "sense.h"

static unsigned char g_rgba[SENSE_VISION_RGBA_BYTES];
static float g_pcm[SENSE_PCM_FRAME_SAMPLES];
static shakti_prenatal_sensory_stream_t g_stream;

static void fill_text_page(void)
{
    eyes_text_page_t page;

    memset(&page, 0, sizeof(page));
    memcpy(page.line1, "HI", 3);
    assert(eyes_load_document(
        0U,
        SENSE_VISION_WIDTH,
        SENSE_VISION_HEIGHT,
        &page,
        1UL,
        g_rgba,
        SENSE_VISION_RGBA_BYTES
    ));
}

static void fill_pcm_pattern(void)
{
    unsigned int i;

    for (i = 0U; i < SENSE_PCM_FRAME_SAMPLES; ++i) {
        g_pcm[i] = ((i % 20U) < 5U) ? 0.5f : -0.25f;
    }
}

static void test_pcm_roundtrip(void)
{
    char bits[SENSE_SOUND_BITS_CAPACITY];
    float back[SENSE_PCM_FRAME_SAMPLES];
    unsigned long drift;

    fill_pcm_pattern();
    assert(sense_pcm_to_bits(
        g_pcm,
        SENSE_PCM_FRAME_SAMPLES,
        bits,
        SENSE_SOUND_BITS_CAPACITY
    ));
    assert(sense_bits_to_pcm(bits, back, SENSE_PCM_FRAME_SAMPLES));
    drift = sense_pcm_drift(g_pcm, back, SENSE_PCM_FRAME_SAMPLES);
    assert(drift == 0UL);
}

static void test_dual_path_ingest(void)
{
    sense_ring_t ring;
    sense_frame_t *frame;
    screen_t screen;

    sense_ring_init(&ring);
    fill_text_page();
    fill_pcm_pattern();

    assert(sense_ingest(&ring, g_rgba, g_pcm, 0.0f, &frame));
    assert(frame != NULL);
    assert(frame->seq == 1UL);
    assert(frame->drift_v == 0UL);
    assert(frame->drift_s == 0UL);
    assert(frame->vision_binary[0] == '0' || frame->vision_binary[0] == '1');
    assert(frame->sound_binary[0] == '0' || frame->sound_binary[0] == '1');
    assert(sense_ring_count(&ring) == 1U);
    assert(sense_ring_latest(&ring) == frame);

    assert(screen_init(&screen));
    assert(sense_present_vision_to_screen(frame, &screen));
}

static void test_ring_advances(void)
{
    sense_ring_t ring;
    unsigned int i;
    const sense_frame_t *latest;
    unsigned long last_seq;

    sense_ring_init(&ring);
    fill_text_page();
    fill_pcm_pattern();

    for (i = 0U; i < SENSE_RING_SLOTS + 3U; ++i) {
        sense_frame_t *frame;

        g_pcm[0] = (float)i * 0.01f;
        assert(sense_ingest(&ring, g_rgba, g_pcm, (float)i, &frame));
        assert(frame->seq == (unsigned long)i + 1UL);
    }

    assert(sense_ring_count(&ring) == SENSE_RING_SLOTS);
    latest = sense_ring_latest(&ring);
    assert(latest != NULL);
    last_seq = (unsigned long)(SENSE_RING_SLOTS + 3U);
    assert(latest->seq == last_seq);

    /* Oldest live logical slot should be last_seq - 7 */
    {
        const sense_frame_t *oldest;

        oldest = sense_ring_at(&ring, 0U);
        assert(oldest != NULL);
        assert(oldest->seq == last_seq - (unsigned long)(SENSE_RING_SLOTS - 1U));
    }
}

static void test_prenatal_light_bound_to_seq(void)
{
    sense_ring_t ring;
    unsigned int frame_index;
    unsigned int saw_light;
    sense_frame_t *lit;

    sense_ring_init(&ring);
    shakti_hearing_init_stream(&g_stream);
    assert(shakti_hearing_synthesize_prenatal(&g_stream, 21.0));
    assert(g_stream.flash_count > 2000U);

    saw_light = 0U;
    lit = NULL;

    for (frame_index = 0U; frame_index < g_stream.flash_count; ++frame_index) {
        unsigned int s;
        unsigned int base;
        float light;
        sense_frame_t *frame;
        unsigned long p;

        light = g_stream.flash_intensities[frame_index];
        base = frame_index * SENSE_PCM_FRAME_SAMPLES;
        if (base + SENSE_PCM_FRAME_SAMPLES > g_stream.sample_count) {
            break;
        }

        for (s = 0U; s < SENSE_PCM_FRAME_SAMPLES; ++s) {
            g_pcm[s] = g_stream.sample_buffer[base + s];
        }

        /* Flat field from light — mono rebuild of uniform field is exact. */
        for (p = 0UL; p < SENSE_VISION_PIXELS; ++p) {
            unsigned char level;
            unsigned long o;

            level = (unsigned char)(light * 255.0f);
            o = p * 4UL;
            g_rgba[o + 0UL] = level;
            g_rgba[o + 1UL] = level;
            g_rgba[o + 2UL] = level;
            g_rgba[o + 3UL] = 255U;
        }

        assert(sense_ingest(&ring, g_rgba, g_pcm, light, &frame));
        assert(frame->drift_s == 0UL);

        if (frame_index < 2000U) {
            assert(frame->light_flash == 0.0f);
        } else if (frame->light_flash > 0.0f) {
            saw_light = 1U;
            lit = frame;
            /* Same capsule holds audio bits and light. */
            assert(lit->seq == frame->seq);
            assert(lit->sound_binary[0] == '0' || lit->sound_binary[0] == '1');
        }
    }

    assert(saw_light == 1U);
    assert(lit != NULL);
}

int main(void)
{
    test_pcm_roundtrip();
    test_dual_path_ingest();
    test_ring_advances();
    test_prenatal_light_bound_to_seq();
    puts("All sense tests passed.");
    return 0;
}
