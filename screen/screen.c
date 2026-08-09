#include "screen.h"

#include "eyes.h"

#include <string.h>

/* Abstract self: small upright figure, 8x8 ink bits. */
static const screen_sprite_t SCREEN_SELF = {
    {
        "..####..",
        ".#....#.",
        ".#.##.#.",
        ".#....#.",
        "..####..",
        "...##...",
        "..#..#..",
        ".#....#."
    }
};

const screen_sprite_t *screen_self_sprite(void)
{
    return &SCREEN_SELF;
}

static void put_ink(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    int ink
)
{
    unsigned long base;
    unsigned char value;

    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
        return;
    }

    base = ((unsigned long)y * (unsigned long)SCREEN_WIDTH +
            (unsigned long)x) * 4UL;
    value = ink ? 0U : 255U;

    if (screen->rgba[base] != value ||
        screen->rgba[base + 1UL] != value ||
        screen->rgba[base + 2UL] != value) {
        screen->pixels_drawn += 1UL;
    }

    screen->rgba[base] = value;
    screen->rgba[base + 1UL] = value;
    screen->rgba[base + 2UL] = value;
    screen->rgba[base + 3UL] = 255U;
}

int screen_init(screen_t *screen)
{
    return screen_clear(screen);
}

int screen_clear(screen_t *screen)
{
    unsigned long index;

    if (screen == NULL) {
        return 0;
    }

    for (index = 0UL; index < SCREEN_PIXELS; ++index) {
        screen->rgba[index * 4UL] = 255U;
        screen->rgba[index * 4UL + 1UL] = 255U;
        screen->rgba[index * 4UL + 2UL] = 255U;
        screen->rgba[index * 4UL + 3UL] = 255U;
    }

    screen->pixels_drawn = 0UL;
    return 1;
}

unsigned int screen_width(const screen_t *screen)
{
    (void)screen;
    return SCREEN_WIDTH;
}

unsigned int screen_height(const screen_t *screen)
{
    (void)screen;
    return SCREEN_HEIGHT;
}

const unsigned char *screen_rgba(const screen_t *screen)
{
    if (screen == NULL) {
        return NULL;
    }

    return screen->rgba;
}

unsigned char *screen_rgba_mut(screen_t *screen)
{
    if (screen == NULL) {
        return NULL;
    }

    return screen->rgba;
}

int screen_set_pixel(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    int ink
)
{
    if (screen == NULL || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
        return 0;
    }

    put_ink(screen, x, y, ink);
    return 1;
}

int screen_get_pixel(
    const screen_t *screen,
    unsigned int x,
    unsigned int y
)
{
    unsigned long base;
    unsigned int luma;

    if (screen == NULL || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
        return -1;
    }

    base = ((unsigned long)y * (unsigned long)SCREEN_WIDTH +
            (unsigned long)x) * 4UL;
    luma = (77U * (unsigned int)screen->rgba[base] +
            150U * (unsigned int)screen->rgba[base + 1UL] +
            29U * (unsigned int)screen->rgba[base + 2UL] + 128U) / 256U;

    /* Match eyes mono: below midpoint = ink. */
    return luma < 128U ? 1 : 0;
}

int screen_fill_rect(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    unsigned int w,
    unsigned int h,
    int ink
)
{
    unsigned int row;
    unsigned int col;
    unsigned int x_end;
    unsigned int y_end;

    if (screen == NULL || w == 0U || h == 0U) {
        return 0;
    }

    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
        return 0;
    }

    x_end = x + w;
    y_end = y + h;

    if (x_end > SCREEN_WIDTH) {
        x_end = SCREEN_WIDTH;
    }

    if (y_end > SCREEN_HEIGHT) {
        y_end = SCREEN_HEIGHT;
    }

    for (row = y; row < y_end; ++row) {
        for (col = x; col < x_end; ++col) {
            put_ink(screen, col, row, ink);
        }
    }

    return 1;
}

int screen_blit_glyph(
    screen_t *screen,
    unsigned int origin_x,
    unsigned int origin_y,
    unsigned char character
)
{
    char bitmap[8][9];
    unsigned int row;
    unsigned int col;

    if (screen == NULL) {
        return 0;
    }

    if (!eyes_render_glyph(character, bitmap)) {
        return 0;
    }

    for (row = 0U; row < 8U; ++row) {
        for (col = 0U; col < 8U; ++col) {
            unsigned int x;
            unsigned int y;

            x = origin_x + col;
            y = origin_y + row;

            if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
                continue;
            }

            if (bitmap[row][col] == '#') {
                put_ink(screen, x, y, 1);
            }
        }
    }

    return 1;
}

int screen_blit_sprite(
    screen_t *screen,
    unsigned int origin_x,
    unsigned int origin_y,
    const screen_sprite_t *sprite
)
{
    unsigned int row;
    unsigned int col;

    if (screen == NULL || sprite == NULL) {
        return 0;
    }

    for (row = 0U; row < SCREEN_SPRITE_SIZE; ++row) {
        const char *line;

        line = sprite->rows[row];

        if (line == NULL) {
            continue;
        }

        for (col = 0U; col < SCREEN_SPRITE_SIZE; ++col) {
            unsigned int x;
            unsigned int y;
            char cell;

            if (line[col] == '\0') {
                break;
            }

            cell = line[col];
            x = origin_x + col;
            y = origin_y + row;

            if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
                continue;
            }

            if (cell == '#' || cell == '1') {
                put_ink(screen, x, y, 1);
            }
        }
    }

    return 1;
}
