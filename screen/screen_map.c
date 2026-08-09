/*
 * screen_map: draw on the owned screen, let eyes see it, count mono drift.
 *
 * Still frame + a few run steps for the self sprite. Not a daemon.
 * Artifacts go only under screen/output/.
 */

#include "screen.h"
#include "eyes.h"

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define SCREEN_MKDIR(p) _mkdir(p)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define SCREEN_MKDIR(p) mkdir((p), 0777)
#endif

#define SCREEN_OUTPUT_DIR "screen/output"
#define SCREEN_PATH_CAPACITY 256UL
#define SCREEN_BITS_CAPACITY (SCREEN_PIXELS + 1UL)
#define SCREEN_RUN_FRAMES 5U
#define SCREEN_RUN_Y 28U
#define SCREEN_RUN_START_X 4U
#define SCREEN_RUN_STEP 10U

static screen_t g_screen;
static unsigned char g_recon[SCREEN_RGBA_BYTES];
static char g_bits[SCREEN_BITS_CAPACITY];
static char g_bits_b[SCREEN_BITS_CAPACITY];
static unsigned char g_work_a[SCREEN_RGBA_BYTES];
static unsigned char g_work_b[SCREEN_RGBA_BYTES];

static void print_section_map(void)
{
    printf("screen section map\n");
    printf("==================\n");
    printf("screen/README.md         section file list and rules\n");
    printf("screen/screen.h          fixed-buffer C99 API\n");
    printf("screen/screen.c          init, pixel, rect, glyph, sprite\n");
    printf("screen/screen_map.c      this harness\n");
    printf("screen/output/           harness artifacts only\n");
    printf("\n");
    printf("flow: clear -> draw self -> eyes mono pull -> reconstruct\n");
    printf("      -> eyes_diff (expect drift 0) -> run frames\n");
    printf("\n");
    printf("size: %ux%u RGBA, mono truth path\n",
           SCREEN_WIDTH, SCREEN_HEIGHT);
    printf("\n");
}

static int write_path(
    char *path,
    unsigned long capacity,
    const char *name
)
{
    int n;

    if (path == NULL || name == NULL || capacity < 8UL) {
        return 0;
    }

    n = snprintf(path, (size_t)capacity, "%s/%s", SCREEN_OUTPUT_DIR, name);

    if (n < 0 || (unsigned long)n >= capacity) {
        return 0;
    }

    return 1;
}

static unsigned long count_ink(const screen_t *screen)
{
    unsigned int x;
    unsigned int y;
    unsigned long ink;

    ink = 0UL;

    for (y = 0U; y < SCREEN_HEIGHT; ++y) {
        for (x = 0U; x < SCREEN_WIDTH; ++x) {
            if (screen_get_pixel(screen, x, y) == 1) {
                ink += 1UL;
            }
        }
    }

    return ink;
}

/*
 * Present the live screen to eyes: mono pull, reconstruct, diff vs screen.
 * Returns 1 only when drift is exactly 0.
 */
static int eyes_mono_roundtrip(
    const screen_t *screen,
    unsigned long *drift_out,
    const char *frame_tag
)
{
    const unsigned char *rgba;
    unsigned int width;
    unsigned int height;
    unsigned long drift;
    char path[SCREEN_PATH_CAPACITY];

    if (screen == NULL || drift_out == NULL) {
        return 0;
    }

    rgba = screen_rgba(screen);
    width = screen_width(screen);
    height = screen_height(screen);

    if (rgba == NULL) {
        return 0;
    }

    if (!eyes_pull_mono(rgba, width, height, g_bits, SCREEN_BITS_CAPACITY)) {
        printf("FAIL: eyes_pull_mono (%s)\n", frame_tag);
        return 0;
    }

    if (!eyes_reconstruct_mono(g_bits, width, height, g_recon,
                               SCREEN_RGBA_BYTES)) {
        printf("FAIL: eyes_reconstruct_mono (%s)\n", frame_tag);
        return 0;
    }

    drift = eyes_diff(rgba, g_recon, width, height, 0);
    *drift_out = drift;

    {
        char name[SCREEN_PATH_CAPACITY];
        int n;

        n = snprintf(name, sizeof(name), "%s_frame.tan", frame_tag);

        if (n > 0 && (unsigned long)n < sizeof(name) &&
            write_path(path, SCREEN_PATH_CAPACITY, name)) {
            (void)eyes_write_frame(path, width, height, 0, g_bits);
        }

        n = snprintf(name, sizeof(name), "%s_recon.tan", frame_tag);

        if (n > 0 && (unsigned long)n < sizeof(name) &&
            write_path(path, SCREEN_PATH_CAPACITY, name)) {
            (void)eyes_write_recon(path, width, height, 0, g_recon);
        }
    }

    if (drift != 0UL) {
        printf("FAIL: mono drift %lu on %s (want 0)\n", drift, frame_tag);
        return 0;
    }

    return 1;
}

static int draw_scene(screen_t *screen, unsigned int self_x, unsigned int self_y)
{
    if (!screen_clear(screen)) {
        return 0;
    }

    /* Ground line. */
    if (!screen_fill_rect(screen, 0U, 56U, SCREEN_WIDTH, 2U, 1)) {
        return 0;
    }

    /* Label cell top-left. */
    if (!screen_blit_glyph(screen, 0U, 0U, 'S')) {
        return 0;
    }

    if (!screen_blit_sprite(screen, self_x, self_y, screen_self_sprite())) {
        return 0;
    }

    return 1;
}

static int run_still_and_run(void)
{
    unsigned long drift;
    unsigned long ink;
    unsigned long cycle_drift;
    unsigned int frame;
    unsigned int x;
    char tag[64];

    if (!screen_init(&g_screen)) {
        printf("FAIL: screen_init\n");
        return 0;
    }

    if (!draw_scene(&g_screen, SCREEN_RUN_START_X, SCREEN_RUN_Y)) {
        printf("FAIL: draw still scene\n");
        return 0;
    }

    ink = count_ink(&g_screen);
    printf("still: %ux%u pixels_drawn=%lu ink=%lu\n",
           screen_width(&g_screen),
           screen_height(&g_screen),
           g_screen.pixels_drawn,
           ink);

    if (!eyes_mono_roundtrip(&g_screen, &drift, "still")) {
        return 0;
    }

    printf("still: mono drift=%lu OK\n", drift);

    cycle_drift = eyes_run_cycles(
        screen_rgba(&g_screen),
        screen_width(&g_screen),
        screen_height(&g_screen),
        0,
        g_work_a,
        g_work_b,
        g_bits,
        g_bits_b,
        SCREEN_RGBA_BYTES
    );

    if (cycle_drift != 0UL) {
        printf("FAIL: 20-cycle mono drift %lu (want 0)\n", cycle_drift);
        return 0;
    }

    printf("still: 20-cycle mono drift=%lu OK\n", cycle_drift);

    for (frame = 0U; frame < SCREEN_RUN_FRAMES; ++frame) {
        x = SCREEN_RUN_START_X + frame * SCREEN_RUN_STEP;

        if (x + SCREEN_SPRITE_SIZE > SCREEN_WIDTH) {
            x = SCREEN_WIDTH - SCREEN_SPRITE_SIZE;
        }

        if (!draw_scene(&g_screen, x, SCREEN_RUN_Y)) {
            printf("FAIL: draw run frame %u\n", frame);
            return 0;
        }

        {
            int n;

            n = snprintf(tag, sizeof(tag), "run_%u", frame);

            if (n < 0 || (unsigned long)n >= sizeof(tag)) {
                printf("FAIL: tag\n");
                return 0;
            }
        }

        if (!eyes_mono_roundtrip(&g_screen, &drift, tag)) {
            return 0;
        }

        ink = count_ink(&g_screen);
        printf("run[%u]: x=%u ink=%lu mono drift=%lu OK\n",
               frame, x, ink, drift);
    }

    return 1;
}

int main(void)
{
    print_section_map();
    (void)SCREEN_MKDIR(SCREEN_OUTPUT_DIR);

    if (!run_still_and_run()) {
        printf("screen harness: FAIL\n");
        return 1;
    }

    printf("screen harness: PASS (mono drift 0 on still + run frames)\n");
    return 0;
}
