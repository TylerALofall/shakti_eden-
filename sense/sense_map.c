/*
 * sense_map: dual-path sense harness.
 *
 * 1) Generated vision fixture + synthetic PCM -> one bound frame, drift 0.
 * 2) Ring flow across multiple ticks.
 * 3) Prenatal heartbeat+light stream: dark then flashes, same seq binds both.
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
    printf("sense/sense.h       frame capsule + ring API\n");
    printf("sense/sense.c       ingest binder + PCM bits + present\n");
    printf("sense/sense_map.c   this harness\n");
    printf("sense/output/       harness artifacts only\n");
    printf("\n");
    printf("vision capsule: %ux%u mono binary + rebuild\n",
           SENSE_VISION_WIDTH, SENSE_VISION_HEIGHT);
    printf("sound capsule:  %u samples @ %u Hz (10 ms) + envelope + light\n",
           SENSE_PCM_FRAME_SAMPLES, SENSE_PCM_RATE);
    printf("ring slots:     %u\n", SENSE_RING_SLOTS);
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

static void print_frame(const char *tag, const sense_frame_t *frame)
{
    if (frame == NULL) {
        printf("%s: (null frame)\n", tag);
        return;
    }

    printf(
        "%s: seq=%lu epoch=%lu drift_v=%lu drift_s=%lu env=%.4f light=%.4f "
        "vision_bits0=%.16s sound_bits0=%.16s\n",
        tag,
        (unsigned long)frame->seq,
        (unsigned long)frame->epoch,
        (unsigned long)frame->drift_v,
        (unsigned long)frame->drift_s,
        (double)frame->sound_envelope,
        (double)frame->light_flash,
        frame->vision_binary,
        frame->sound_binary
    );
}

static int run_fixture_dual_path(void)
{
    sense_frame_t *frame;

    fill_vision_text_page();
    fill_pcm_tone(0.75f);

    if (!sense_ingest(&g_ring, g_rgba, g_pcm, 0.0f, &frame)) {
        printf("FAIL: fixture sense_ingest\n");
        return 0;
    }

    print_frame("fixture", frame);

    if (frame->drift_v != 0UL) {
        printf("FAIL: vision mono drift %lu (want 0)\n",
               (unsigned long)frame->drift_v);
        return 0;
    }

    if (frame->drift_s != 0UL) {
        printf("FAIL: sound bit drift %lu (want 0)\n",
               (unsigned long)frame->drift_s);
        return 0;
    }

    if (!screen_init(&g_screen) ||
        !sense_present_vision_to_screen(frame, &g_screen)) {
        printf("FAIL: present vision to screen\n");
        return 0;
    }

    printf("PASS: fixture dual-path bound under seq %lu\n",
           (unsigned long)frame->seq);
    return 1;
}

static int run_ring_flow(void)
{
    unsigned int t;
    unsigned long first_seq;
    const sense_frame_t *latest;

    first_seq = 0UL;
    for (t = 0U; t < SENSE_RING_FLOW_TICKS; ++t) {
        sense_frame_t *frame;

        fill_pcm_tone(0.2f + (float)t * 0.02f);
        fill_vision_from_light((float)(t % 5U) * 0.2f);

        if (!sense_ingest(&g_ring, g_rgba, g_pcm, (float)t * 0.01f, &frame)) {
            printf("FAIL: ring flow ingest tick %u\n", t);
            return 0;
        }

        if (t == 0U) {
            first_seq = frame->seq;
        }

        if (frame->seq != first_seq + (unsigned long)t) {
            printf("FAIL: seq not advancing (got %lu)\n",
                   (unsigned long)frame->seq);
            return 0;
        }
    }

    if (sense_ring_count(&g_ring) != SENSE_RING_SLOTS) {
        printf("FAIL: ring count %u want %u\n",
               sense_ring_count(&g_ring), SENSE_RING_SLOTS);
        return 0;
    }

    latest = sense_ring_latest(&g_ring);
    if (latest == NULL ||
        latest->seq != first_seq + (unsigned long)(SENSE_RING_FLOW_TICKS - 1U)) {
        printf("FAIL: latest ring slot\n");
        return 0;
    }

    print_frame("ring_latest", latest);
    printf("PASS: ring advanced %u ticks, slots=%u\n",
           SENSE_RING_FLOW_TICKS, sense_ring_count(&g_ring));
    return 1;
}

static int run_prenatal(void)
{
    unsigned int frame_index;
    unsigned int total_frames;
    unsigned int checked_dark;
    unsigned int checked_light;
    sense_frame_t *bound;
    const sense_frame_t *latest;

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
        sense_frame_t *frame;
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

        if (!sense_ingest(&g_ring, g_rgba, g_pcm, light, &frame)) {
            printf("FAIL: prenatal ingest frame %u\n", frame_index);
            return 0;
        }

        if (frame->drift_s != 0UL) {
            printf("FAIL: prenatal sound drift at frame %u\n", frame_index);
            return 0;
        }

        if (frame_index < SENSE_DARK_END_FRAME) {
            if (frame->light_flash != 0.0f) {
                printf("FAIL: dark phase light non-zero at frame %u (%.4f)\n",
                       frame_index, (double)frame->light_flash);
                return 0;
            }
            checked_dark++;
        } else {
            if (frame->light_flash > 0.0f) {
                checked_light++;
                bound = frame;
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

    latest = sense_ring_latest(&g_ring);
    if (latest == NULL) {
        printf("FAIL: prenatal latest missing\n");
        return 0;
    }

    /* Light and audio share the capsule identity. */
    if (latest->light_flash != latest->light_flash) {
        printf("FAIL: light NaN\n");
        return 0;
    }

    print_frame("prenatal_latest", latest);
    printf(
        "PASS: prenatal dark_frames~%u light_frames=%u latest_seq=%lu light=%.4f\n",
        checked_dark,
        checked_light,
        (unsigned long)latest->seq,
        (double)latest->light_flash
    );
    return 1;
}

int main(void)
{
    int ok;

    SENSE_MKDIR(SENSE_OUTPUT_DIR);

    puts("==================================================");
    puts("Shakti sense: binary + rendered = one experience");
    puts("==================================================");
    print_section_map();

    sense_ring_init(&g_ring);

    ok = 1;
    if (!run_fixture_dual_path()) {
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

    puts("SUCCESS: sense dual-path continuous RAM flow verified.");
    return 0;
}
