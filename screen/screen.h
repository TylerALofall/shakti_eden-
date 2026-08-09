#ifndef SCREEN_H
#define SCREEN_H

#include <stddef.h>

/*
 * screen: Shakti's owned fixed pixel surface.
 *
 * One RGBA buffer. Draw writes pixels. Eyes read the same buffer.
 * No window, no GPU, no threads, no dynamic allocation.
 * Caller-owned screen_t; all buffers live inside the struct.
 */

#define SCREEN_WIDTH  64U
#define SCREEN_HEIGHT 64U
#define SCREEN_PIXELS ((unsigned long)SCREEN_WIDTH * (unsigned long)SCREEN_HEIGHT)
#define SCREEN_RGBA_BYTES (SCREEN_PIXELS * 4UL)
#define SCREEN_SPRITE_SIZE 8U

typedef struct {
    unsigned char rgba[SCREEN_RGBA_BYTES];
    unsigned long pixels_drawn;
} screen_t;

/* 8x8 sprite: 8 row strings of 8 chars, '#' or '1' = ink, else paper. */
typedef struct {
    const char *rows[SCREEN_SPRITE_SIZE];
} screen_sprite_t;

/* Built-in abstract self figure (ink silhouette). */
const screen_sprite_t *screen_self_sprite(void);

/* Init / clear to white paper. pixels_drawn reset to 0. Returns 1. */
int screen_init(screen_t *screen);
int screen_clear(screen_t *screen);

unsigned int screen_width(const screen_t *screen);
unsigned int screen_height(const screen_t *screen);

/* Raw RGBA for eyes (width*height*4). NULL if screen is NULL. */
const unsigned char *screen_rgba(const screen_t *screen);
unsigned char *screen_rgba_mut(screen_t *screen);

/*
 * set_pixel: ink != 0 draws black; ink == 0 draws white paper.
 * get_pixel: returns 1 ink, 0 paper, -1 on rejection.
 * Returns 1 on success, 0 if out of range or NULL.
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

/* Fill inclusive-start, exclusive-end style clipped to screen. */
int screen_fill_rect(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    unsigned int w,
    unsigned int h,
    int ink
);

/*
 * Stamp a clean eyes-font glyph at pixel origin (top-left of 8x8 cell).
 * Uses eyes_render_glyph. Unsupported character returns 0.
 */
int screen_blit_glyph(
    screen_t *screen,
    unsigned int origin_x,
    unsigned int origin_y,
    unsigned char character
);

/* Stamp an 8x8 sprite; rows may be shorter (treated as paper beyond). */
int screen_blit_sprite(
    screen_t *screen,
    unsigned int origin_x,
    unsigned int origin_y,
    const screen_sprite_t *sprite
);

#endif
