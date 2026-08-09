#ifndef SCREEN_H
#define SCREEN_H

#include <stddef.h>

/*
 * screen: Shakti's owned fixed pixel surface.
 *
 * Native plane is full-color RGBA from day one (dreams may use color).
 * Binary ink helpers remain for cheap cable-TV style drawing (black/white
 * on the same buffer). School still teaches color only at lesson 3 and
 * keeps counting solo for two lessons so "3" is never glued to "blue".
 *
 * Geometry: 640 x 360 (16:9 nHD). Phone scale (nearest neighbor):
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

#define SCREEN_SCALE_X2 2U
#define SCREEN_SCALE_X3 3U

/* Paper / ink convenience (binary draw on the color surface). */
#define SCREEN_PAPER_R 255U
#define SCREEN_PAPER_G 255U
#define SCREEN_PAPER_B 255U
#define SCREEN_INK_R     0U
#define SCREEN_INK_G     0U
#define SCREEN_INK_B     0U

typedef struct {
    unsigned char rgba[SCREEN_RGBA_BYTES];
    unsigned long pixels_drawn;
} screen_t;

typedef struct {
    const char *rows[SCREEN_SPRITE_SIZE];
} screen_sprite_t;

const screen_sprite_t *screen_self_sprite(void);

int screen_init(screen_t *screen);
int screen_clear(screen_t *screen);

unsigned int screen_width(const screen_t *screen);
unsigned int screen_height(const screen_t *screen);

/* Live RGBA plane for eyes / future host. NULL if screen is NULL. */
const unsigned char *screen_rgba(const screen_t *screen);
unsigned char *screen_rgba_mut(screen_t *screen);

/* Full-color pixel. a forced to 255. */
int screen_set_pixel_rgb(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    unsigned char r,
    unsigned char g,
    unsigned char b
);

/* Binary convenience: ink != 0 -> black, else paper white. */
int screen_set_pixel(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    int ink
);

/*
 * get_pixel: 1 if luma below midpoint (ink-like), 0 paper-like, -1 reject.
 * Color pixels still reduce to ink/paper this way for mono eyes path.
 */
int screen_get_pixel(
    const screen_t *screen,
    unsigned int x,
    unsigned int y
);

int screen_fill_rect_rgb(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    unsigned int w,
    unsigned int h,
    unsigned char r,
    unsigned char g,
    unsigned char b
);

int screen_fill_rect(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    unsigned int w,
    unsigned int h,
    int ink
);

/* Filled disk (for dream color spots). Clipped. */
int screen_fill_disk_rgb(
    screen_t *screen,
    unsigned int cx,
    unsigned int cy,
    unsigned int radius,
    unsigned char r,
    unsigned char g,
    unsigned char b
);

int screen_blit_glyph(
    screen_t *screen,
    unsigned int origin_x,
    unsigned int origin_y,
    unsigned char character
);

int screen_blit_sprite(
    screen_t *screen,
    unsigned int origin_x,
    unsigned int origin_y,
    const screen_sprite_t *sprite
);

#endif
