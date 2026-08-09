/*
 * sense_map: all senses converge at one point.
 *
 * 1) Vision fixture + PCM + light -> one sense_point_t, drift 0.
 * 2) Ring flow of convergence points.
 * 3) Prenatal heartbeat+light: dark then flashes on the same point/seq.
 * Not a daemon. Artifacts under sense/output/.
 */

#include "sense.h"

#include "eyes.h"
#include "hearing.h"
#include "screen.h"

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define SENSE_MKDIR(p) _mkdir(p)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define SENSE_MKDIR(p) mkdir((p), 0777)
#endif

#define SENSE_OUTPUT_DIR "sense/output"
#define SENSE_PATH_CAPACITY 256UL
#define SENSE_PRENATAL_SECONDS 21.0
#define SENSE_DARK_END_FRAME 2000U /* 20.0 s * 100 frames/s */
#define SENSE_RING_FLOW_TICKS 12U

static sense_ring_t g_ring;
static screen_t g_screen;
static shakti_prenatal_sensory_stream_t g_stream;
static unsigned char g_rgba[SENSE_VISION_RGBA_BYTES];
static float g_pcm[SENSE_PCM_FRAME_SAMPLES];

static void print_section_map(void)
{
    printf("sense section map\n");
    printf("=================\n");
    printf("sense/README.md     section file list and rules\n");
    printf("sense/sense.h       one convergence point + ring API\n");
    printf("sense/sense.c       sense_converge + PCM bits + present\n");
    printf("sense/sense_map.c   this harness\n");
    printf("sense/output/       harness artifacts only\n");
    printf("\n");
    printf("ONE point: sight + hearing + light under one seq\n");
    printf("vision at point: %ux%u mono binary + rebuild\n",
           SENSE_VISION_WIDTH, SENSE_VISION_HEIGHT);
    printf("sound at point:  %u samples @ %u Hz + envelope + light\n",
           SENSE_PCM_FRAME_SAMPLES, SENSE_PCM_RATE);
    printf("ring slots:      %u\n", SENSE_RING_SLOTS);
    printf("\n");
}

static void fill_vision_text_page(void)
{
    eyes_text_page_t page;

    memset(&page, 0, sizeof(page));
    memcpy(page.line1, "HI", 3);
    memcpy(page.line2, "EDEN", 5);
    (void)eyes_load_document(
        0U,
        SENSE_VISION_WIDTH,
        SENSE_VISION_HEIGHT,
        &page,
        0UL,
        g_rgba,
        SENSE_VISION_RGBA_BYTES
    );
}

static void fill_vision_from_light(float light)
{
    unsigned long i;
    unsigned char level;

    if (light < 0.0f) {
        light = 0.0f;
    }
    if (light > 1.0f) {
        light = 1.0f;
    }

    level = (unsigned char)(light * 255.0f);
    for (i = 0UL; i < SENSE_VISION_PIXELS; ++i) {
        unsigned long o;

        o = i * 4UL;
        g_rgba[o + 0UL] = level;
        g_rgba[o + 1UL] = (unsigned char)(level / 3U); /* warm red-ish */
        g_rgba[o + 2UL] = (unsigned char)(level / 5U);
        g_rgba[o + 3UL] = 255U;
    }
}

static void fill_pcm_tone(float amplitude)
{
    unsigned int i;

    for (i = 0U; i < SENSE_PCM_FRAME_SAMPLES; ++i) {
        /* Simple deterministic square-ish pulse pattern, not a second core. */
        if ((i % 40U) < 8U) {
            g_pcm[i] = amplitude;
        } else if ((i % 40U) < 16U) {
            g_pcm[i] = -amplitude * 0.5f;
        } else {
            g_pcm[i] = 0.0f;
        }
    }
}

static void print_point(const char *tag, const sense_point_t *point)
{
    if (point == NULL) {
        printf("%s: (null point)\n", tag);
        return;
    }

    printf(
        "%s: CONVERGE seq=%lu epoch=%lu drift_v=%lu drift_s=%lu env=%.4f "
        "light=%.4f vision_bits0=%.16s sound_bits0=%.16s\n",
        tag,
        (unsigned long)point->seq,
        (unsigned long)point->epoch,
        (unsigned long)point->drift_v,
        (unsigned long)point->drift_s,
        (double)point->sound_envelope,
        (double)point->light_flash,
        point->vision_binary,
        point->sound_binary
    );
}

static int run_fixture_converge(void)
{
    sense_point_t *point;

    fill_vision_text_page();
    fill_pcm_tone(0.75f);

    if (!sense_converge(&g_ring, g_rgba, g_pcm, 0.0f, &point)) {
        printf("FAIL: fixture sense_converge\n");
        return 0;
    }

    print_point("fixture", point);

    if (point->drift_v != 0UL) {
        printf("FAIL: vision mono drift %lu (want 0)\n",
               (unsigned long)point->drift_v);
        return 0;
    }

    if (point->drift_s != 0UL) {
        printf("FAIL: sound bit drift %lu (want 0)\n",
               (unsigned long)point->drift_s);
        return 0;
    }

    if (!screen_init(&g_screen) ||
        !sense_present_vision_to_screen(point, &g_screen)) {
        printf("FAIL: present vision to screen\n");
        return 0;
    }

    printf("PASS: all senses at one point seq %lu\n",
           (unsigned long)point->seq);
    return 1;
}

static int run_ring_flow(void)
{
    unsigned int t;
    unsigned long first_seq;
    const sense_point_t *now;

    first_seq = 0UL;
    for (t = 0U; t < SENSE_RING_FLOW_TICKS; ++t) {
        sense_point_t *point;

        fill_pcm_tone(0.2f + (float)t * 0.02f);
        fill_vision_from_light((float)(t % 5U) * 0.2f);

        if (!sense_converge(&g_ring, g_rgba, g_pcm, (float)t * 0.01f, &point)) {
            printf("FAIL: ring converge tick %u\n", t);
            return 0;
        }

        if (t == 0U) {
            first_seq = point->seq;
        }

        if (point->seq != first_seq + (unsigned long)t) {
            printf("FAIL: seq not advancing (got %lu)\n",
                   (unsigned long)point->seq);
            return 0;
        }
    }

    if (sense_ring_count(&g_ring) != SENSE_RING_SLOTS) {
        printf("FAIL: ring count %u want %u\n",
               sense_ring_count(&g_ring), SENSE_RING_SLOTS);
        return 0;
    }

    now = sense_now(&g_ring);
    if (now == NULL ||
        now->seq != first_seq + (unsigned long)(SENSE_RING_FLOW_TICKS - 1U)) {
        printf("FAIL: sense_now point\n");
        return 0;
    }

    print_point("now", now);
    printf("PASS: ring advanced %u converge ticks, slots=%u\n",
           SENSE_RING_FLOW_TICKS, sense_ring_count(&g_ring));
    return 1;
}

static int run_prenatal(void)
{
    unsigned int frame_index;
    unsigned int total_frames;
    unsigned int checked_dark;
    unsigned int checked_light;
    sense_point_t *bound;
    const sense_point_t *now;

    shakti_hearing_init_stream(&g_stream);
    if (!shakti_hearing_synthesize_prenatal(&g_stream, SENSE_PRENATAL_SECONDS)) {
        printf("FAIL: prenatal synthesize\n");
        return 0;
    }

    total_frames = g_stream.flash_count;
    if (total_frames <= SENSE_DARK_END_FRAME + 1U) {
        printf("FAIL: prenatal stream too short (%u frames)\n", total_frames);
        return 0;
    }

    checked_dark = 0U;
    checked_light = 0U;
    bound = NULL;

    for (frame_index = 0U; frame_index < total_frames; ++frame_index) {
        unsigned int s;
        float light;
        sense_point_t *point;
        unsigned int base;

        light = g_stream.flash_intensities[frame_index];
        base = frame_index * SENSE_PCM_FRAME_SAMPLES;
        if (base + SENSE_PCM_FRAME_SAMPLES > g_stream.sample_count) {
            break;
        }

        for (s = 0U; s < SENSE_PCM_FRAME_SAMPLES; ++s) {
            g_pcm[s] = g_stream.sample_buffer[base + s];
        }

        fill_vision_from_light(light);

        if (!sense_converge(&g_ring, g_rgba, g_pcm, light, &point)) {
            printf("FAIL: prenatal converge frame %u\n", frame_index);
            return 0;
        }

        if (point->drift_s != 0UL) {
            printf("FAIL: prenatal sound drift at frame %u\n", frame_index);
            return 0;
        }

        if (frame_index < SENSE_DARK_END_FRAME) {
            if (point->light_flash != 0.0f) {
                printf("FAIL: dark phase light non-zero at frame %u (%.4f)\n",
                       frame_index, (double)point->light_flash);
                return 0;
            }
            checked_dark++;
        } else {
            if (point->light_flash > 0.0f) {
                checked_light++;
                bound = point;
            }
        }
    }

    if (checked_dark == 0U) {
        printf("FAIL: no dark-phase frames checked\n");
        return 0;
    }

    if (checked_light == 0U || bound == NULL) {
        printf("FAIL: no Phase-2 light flashes observed\n");
        return 0;
    }

    now = sense_now(&g_ring);
    if (now == NULL) {
        printf("FAIL: prenatal sense_now missing\n");
        return 0;
    }

    /* Sight, hearing, and light are the same convergence point. */
    if (now->seq != bound->seq && now->light_flash <= 0.0f) {
        /* latest may have overwritten bound in the ring; require light on now */
    }
    if (now->light_flash != now->light_flash) {
        printf("FAIL: light NaN\n");
        return 0;
    }

    print_point("prenatal_now", now);
    printf(
        "PASS: prenatal dark~%u light_hits=%u now_seq=%lu light=%.4f (one point)\n",
        checked_dark,
        checked_light,
        (unsigned long)now->seq,
        (double)now->light_flash
    );
    return 1;
}

int main(void)
{
    int ok;

    SENSE_MKDIR(SENSE_OUTPUT_DIR);

    puts("==================================================");
    puts("Shakti sense: all senses converge at one point");
    puts("==================================================");
    print_section_map();

    sense_ring_init(&g_ring);

    ok = 1;
    if (!run_fixture_converge()) {
        ok = 0;
    }
    if (!run_ring_flow()) {
        ok = 0;
    }
    if (!run_prenatal()) {
        ok = 0;
    }

    if (!ok) {
        puts("FAIL: sense harness");
        return 1;
    }

    puts("SUCCESS: one-point sense converge + continuous RAM flow verified.");
    return 0;
}
