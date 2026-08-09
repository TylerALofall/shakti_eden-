#ifndef SCREEN_H
#define SCREEN_H

#include <stddef.h>

/*
 * screen: Shakti's owned fixed pixel surface.
 *
 * Native plane is binary (cable-TV style): paper 0, ink 1.
 * Draw writes mono only — low energy. RGBA is built only when
 * presenting to eyes or a future phone host scaler.
 *
 * Geometry: 640 x 360 (16:9 nHD). Scales clean on a phone:
 *   x2 -> 1280 x 720
 *   x3 -> 1920 x 1080
 * Width 640 fits two side-by-side page panels (~320 each).
 *
 * No window, no GPU, no threads, no dynamic allocation.
 */

#define SCREEN_WIDTH  640U
#define SCREEN_HEIGHT 360U
#define SCREEN_PIXELS \
    ((unsigned long)SCREEN_WIDTH * (unsigned long)SCREEN_HEIGHT)
#define SCREEN_RGBA_BYTES (SCREEN_PIXELS * 4UL)
#define SCREEN_SPRITE_SIZE 8U

/* Integer scale factors a Swift host can use (nearest-neighbor). */
#define SCREEN_SCALE_X2 2U
#define SCREEN_SCALE_X3 3U

typedef struct {
    unsigned char mono[SCREEN_PIXELS]; /* 0 paper, 1 ink */
    unsigned long pixels_drawn;
} screen_t;

/* 8x8 sprite: 8 row strings of 8 chars, '#' or '1' = ink, else paper. */
typedef struct {
    const char *rows[SCREEN_SPRITE_SIZE];
} screen_sprite_t;

/* Built-in abstract self figure (ink silhouette). */
const screen_sprite_t *screen_self_sprite(void);

/* Init / clear to paper. pixels_drawn reset to 0. Returns 1. */
int screen_init(screen_t *screen);
int screen_clear(screen_t *screen);

unsigned int screen_width(const screen_t *screen);
unsigned int screen_height(const screen_t *screen);

/* Binary plane pointer (SCREEN_PIXELS bytes). NULL if screen is NULL. */
const unsigned char *screen_mono(const screen_t *screen);

/*
 * Expand mono -> RGBA for eyes / host (black ink, white paper, A=255).
 * capacity must be >= SCREEN_RGBA_BYTES. Returns 1 on success.
 */
int screen_present_rgba(
    const screen_t *screen,
    unsigned char *rgba,
    unsigned long capacity
);

/*
 * set_pixel: ink != 0 draws ink; ink == 0 draws paper.
 * get_pixel: returns 1 ink, 0 paper, -1 on rejection.
 */
int screen_set_pixel(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    int ink
);

int screen_get_pixel(
    const screen_t *screen,
    unsigned int x,
    unsigned int y
);

/* Fill clipped to screen. w/h are extents from (x,y). */
int screen_fill_rect(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    unsigned int w,
    unsigned int h,
    int ink
);

/* Stamp a clean eyes-font glyph at pixel origin (8x8 cell). */
int screen_blit_glyph(
    screen_t *screen,
    unsigned int origin_x,
    unsigned int origin_y,
    unsigned char character
);

/* Stamp an 8x8 sprite. */
int screen_blit_sprite(
    screen_t *screen,
    unsigned int origin_x,
    unsigned int origin_y,
    const screen_sprite_t *sprite
);

#endif
