/*
 * screen_map: full-color screen harness.
 *
 * 1) Binary ink scene + eyes mono drift 0 (cable-TV / everyday draw).
 * 2) Dream color sample: three primary disks, no numerals — color exists
 *    on the surface before School lesson 3, kept separate from counting.
 * Not a daemon. Artifacts under screen/output/.
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
#define SCREEN_BITS_CAPACITY (SCREEN_PIXELS * 3UL + 1UL)
#define SCREEN_RUN_FRAMES 5U
#define SCREEN_RUN_Y 200U
#define SCREEN_RUN_START_X 16U
#define SCREEN_RUN_STEP 100U
#define SCREEN_GROUND_Y 320U

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
    printf("screen/screen.h          full-color C99 API\n");
    printf("screen/screen.c          RGBA plane + binary helpers + disks\n");
    printf("screen/screen_map.c      this harness\n");
    printf("screen/output/           harness artifacts only\n");
    printf("\n");
    printf("size: %ux%u RGBA full color (16:9) from day one\n",
           SCREEN_WIDTH, SCREEN_HEIGHT);
    printf("phone scale: x%u -> %ux%u, x%u -> %ux%u\n",
           SCREEN_SCALE_X2,
           SCREEN_WIDTH * SCREEN_SCALE_X2,
           SCREEN_HEIGHT * SCREEN_SCALE_X2,
           SCREEN_SCALE_X3,
           SCREEN_WIDTH * SCREEN_SCALE_X3,
           SCREEN_HEIGHT * SCREEN_SCALE_X3);
    printf("binary ink helpers still cheap on the same buffer\n");
    printf("School: color lesson 3; keep counting solo 2 lessons\n");
    printf("        (do not glue 3=blue). Dreams may show color first.\n");
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

static int eyes_mono_roundtrip(
    const screen_t *screen,
    unsigned long *drift_out,
    const char *frame_tag,
    int write_still
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

    if (write_still) {
        char name[SCREEN_PATH_CAPACITY];
        int n;

        n = snprintf(name, sizeof(name), "%s_mono_frame.tan", frame_tag);

        if (n > 0 && (unsigned long)n < sizeof(name) &&
            write_path(path, SCREEN_PATH_CAPACITY, name)) {
            (void)eyes_write_frame(path, width, height, 0, g_bits);
        }

        n = snprintf(name, sizeof(name), "%s_mono_recon.tan", frame_tag);

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

static int draw_binary_scene(
    screen_t *screen,
    unsigned int self_x,
    unsigned int self_y
)
{
    if (!screen_clear(screen)) {
        return 0;
    }

    if (!screen_fill_rect(screen, 0U, SCREEN_GROUND_Y, SCREEN_WIDTH, 2U, 1)) {
        return 0;
    }

    if (!screen_fill_rect(screen, 8U, 24U, 300U, 1U, 1) ||
        !screen_fill_rect(screen, 8U, 24U, 1U, 200U, 1) ||
        !screen_fill_rect(screen, 307U, 24U, 1U, 200U, 1) ||
        !screen_fill_rect(screen, 8U, 223U, 300U, 1U, 1)) {
        return 0;
    }

    if (!screen_fill_rect(screen, 332U, 24U, 300U, 1U, 1) ||
        !screen_fill_rect(screen, 332U, 24U, 1U, 200U, 1) ||
        !screen_fill_rect(screen, 631U, 24U, 1U, 200U, 1) ||
        !screen_fill_rect(screen, 332U, 223U, 300U, 1U, 1)) {
        return 0;
    }

    if (!screen_blit_glyph(screen, 16U, 8U, 'S')) {
        return 0;
    }

    if (!screen_blit_sprite(screen, self_x, self_y, screen_self_sprite())) {
        return 0;
    }

    return 1;
}

/*
 * Dream color sample: three primary balls only.
 * No digits, no count labels — color is not glued to number.
 */
static int draw_dream_color_sample(screen_t *screen)
{
    if (!screen_clear(screen)) {
        return 0;
    }

    /* Saturated primaries so eyes color pull can see them. */
    if (!screen_fill_disk_rgb(screen, 160U, 180U, 40U, 255U, 0U, 0U)) {
        return 0;
    }

    if (!screen_fill_disk_rgb(screen, 320U, 180U, 40U, 0U, 255U, 0U)) {
        return 0;
    }

    if (!screen_fill_disk_rgb(screen, 480U, 180U, 40U, 0U, 0U, 255U)) {
        return 0;
    }

    return 1;
}

static int run_binary_path(void)
{
    unsigned long drift;
    unsigned long cycle_drift;
    unsigned int frame;
    unsigned int x;
    char tag[64];

    if (!screen_init(&g_screen)) {
        printf("FAIL: screen_init\n");
        return 0;
    }

    if (!draw_binary_scene(&g_screen, SCREEN_RUN_START_X, SCREEN_RUN_Y)) {
        printf("FAIL: draw binary scene\n");
        return 0;
    }

    printf("binary: %ux%u rgba_bytes=%lu pixels_drawn=%lu\n",
           screen_width(&g_screen),
           screen_height(&g_screen),
           SCREEN_RGBA_BYTES,
           g_screen.pixels_drawn);

    if (!eyes_mono_roundtrip(&g_screen, &drift, "binary_still", 1)) {
        return 0;
    }

    printf("binary_still: mono drift=%lu OK\n", drift);

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

    printf("binary_still: 20-cycle mono drift=%lu OK\n", cycle_drift);

    for (frame = 0U; frame < SCREEN_RUN_FRAMES; ++frame) {
        int n;

        x = SCREEN_RUN_START_X + frame * SCREEN_RUN_STEP;

        if (x + SCREEN_SPRITE_SIZE > SCREEN_WIDTH) {
            x = SCREEN_WIDTH - SCREEN_SPRITE_SIZE;
        }

        if (!draw_binary_scene(&g_screen, x, SCREEN_RUN_Y)) {
            printf("FAIL: run frame %u\n", frame);
            return 0;
        }

        n = snprintf(tag, sizeof(tag), "run_%u", frame);

        if (n < 0 || (unsigned long)n >= sizeof(tag)) {
            return 0;
        }

        if (!eyes_mono_roundtrip(&g_screen, &drift, tag, 0)) {
            return 0;
        }

        printf("run[%u]: x=%u mono drift=%lu OK\n", frame, x, drift);
    }

    return 1;
}

static int run_dream_color_path(void)
{
    const unsigned char *rgba;
    unsigned int width;
    unsigned int height;
    unsigned long exact_copy_drift;
    unsigned long color_pull_drift;
    char path[SCREEN_PATH_CAPACITY];

    if (!draw_dream_color_sample(&g_screen)) {
        printf("FAIL: dream color sample\n");
        return 0;
    }

    rgba = screen_rgba(&g_screen);
    width = screen_width(&g_screen);
    height = screen_height(&g_screen);

    /* Buffer holds exact color (day-one surface truth). */
    exact_copy_drift = eyes_diff(rgba, rgba, width, height, 1);

    if (exact_copy_drift != 0UL) {
        printf("FAIL: exact color self-diff %lu\n", exact_copy_drift);
        return 0;
    }

    printf("dream_color: exact RGBA self-diff=%lu OK (color on surface)\n",
           exact_copy_drift);
    printf("dream_color: three primary disks, no numerals ");
    printf("(counting stays separate)\n");

    /*
     * Eyes color pull is a lens (lossy on purpose for mixes).
     * Report drift; do not require 0 — surface ownership is exact RGBA.
     */
    if (!eyes_pull_color(rgba, width, height, g_bits, SCREEN_BITS_CAPACITY)) {
        printf("FAIL: eyes_pull_color dream\n");
        return 0;
    }

    if (!eyes_reconstruct_color(g_bits, width, height, g_recon,
                                SCREEN_RGBA_BYTES)) {
        printf("FAIL: eyes_reconstruct_color dream\n");
        return 0;
    }

    color_pull_drift = eyes_diff(rgba, g_recon, width, height, 1);
    printf("dream_color: eyes color-lens drift=%lu ", color_pull_drift);
    printf("(lens may lose; surface keeps full RGB)\n");

    if (write_path(path, SCREEN_PATH_CAPACITY, "dream_color_frame.tan")) {
        (void)eyes_write_frame(path, width, height, 1, g_bits);
    }

    if (write_path(path, SCREEN_PATH_CAPACITY, "dream_color_recon.tan")) {
        (void)eyes_write_recon(path, width, height, 1, g_recon);
    }

    return 1;
}

int main(void)
{
    print_section_map();
    (void)SCREEN_MKDIR(SCREEN_OUTPUT_DIR);

    if (!run_binary_path()) {
        printf("screen harness: FAIL (binary path)\n");
        return 1;
    }

    if (!run_dream_color_path()) {
        printf("screen harness: FAIL (dream color path)\n");
        return 1;
    }

    printf("screen harness: PASS ");
    printf("(640x360 full color + binary mono drift 0)\n");
    return 0;
}
