#include "screen.h"

#include "eyes.h"

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

static unsigned long pixel_base(unsigned int x, unsigned int y)
{
    return ((unsigned long)y * (unsigned long)SCREEN_WIDTH +
            (unsigned long)x) * 4UL;
}

static void put_rgb(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    unsigned char r,
    unsigned char g,
    unsigned char b
)
{
    unsigned long base;

    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
        return;
    }

    base = pixel_base(x, y);

    if (screen->rgba[base] != r ||
        screen->rgba[base + 1UL] != g ||
        screen->rgba[base + 2UL] != b) {
        screen->pixels_drawn += 1UL;
    }

    screen->rgba[base] = r;
    screen->rgba[base + 1UL] = g;
    screen->rgba[base + 2UL] = b;
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
        screen->rgba[index * 4UL] = SCREEN_PAPER_R;
        screen->rgba[index * 4UL + 1UL] = SCREEN_PAPER_G;
        screen->rgba[index * 4UL + 2UL] = SCREEN_PAPER_B;
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

int screen_set_pixel_rgb(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    unsigned char r,
    unsigned char g,
    unsigned char b
)
{
    if (screen == NULL || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
        return 0;
    }

    put_rgb(screen, x, y, r, g, b);
    return 1;
}

int screen_set_pixel(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    int ink
)
{
    if (ink) {
        return screen_set_pixel_rgb(
            screen, x, y, SCREEN_INK_R, SCREEN_INK_G, SCREEN_INK_B);
    }

    return screen_set_pixel_rgb(
        screen, x, y, SCREEN_PAPER_R, SCREEN_PAPER_G, SCREEN_PAPER_B);
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

    base = pixel_base(x, y);
    luma = (77U * (unsigned int)screen->rgba[base] +
            150U * (unsigned int)screen->rgba[base + 1UL] +
            29U * (unsigned int)screen->rgba[base + 2UL] + 128U) / 256U;

    return luma < 128U ? 1 : 0;
}

int screen_fill_rect_rgb(
    screen_t *screen,
    unsigned int x,
    unsigned int y,
    unsigned int w,
    unsigned int h,
    unsigned char r,
    unsigned char g,
    unsigned char b
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
            put_rgb(screen, col, row, r, g, b);
        }
    }

    return 1;
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
    if (ink) {
        return screen_fill_rect_rgb(
            screen, x, y, w, h,
            SCREEN_INK_R, SCREEN_INK_G, SCREEN_INK_B);
    }

    return screen_fill_rect_rgb(
        screen, x, y, w, h,
        SCREEN_PAPER_R, SCREEN_PAPER_G, SCREEN_PAPER_B);
}

int screen_fill_disk_rgb(
    screen_t *screen,
    unsigned int cx,
    unsigned int cy,
    unsigned int radius,
    unsigned char r,
    unsigned char g,
    unsigned char b
)
{
    unsigned int row;
    unsigned int col;
    unsigned int y0;
    unsigned int y1;
    unsigned int x0;
    unsigned int x1;
    long radius_sq;
    long dy;
    long dx;

    if (screen == NULL || radius == 0U) {
        return 0;
    }

    radius_sq = (long)radius * (long)radius;

    y0 = (cy > radius) ? (cy - radius) : 0U;
    x0 = (cx > radius) ? (cx - radius) : 0U;
    y1 = cy + radius;
    x1 = cx + radius;

    if (y1 >= SCREEN_HEIGHT) {
        y1 = SCREEN_HEIGHT - 1U;
    }

    if (x1 >= SCREEN_WIDTH) {
        x1 = SCREEN_WIDTH - 1U;
    }

    for (row = y0; row <= y1; ++row) {
        dy = (long)row - (long)cy;

        for (col = x0; col <= x1; ++col) {
            dx = (long)col - (long)cx;

            if (dx * dx + dy * dy <= radius_sq) {
                put_rgb(screen, col, row, r, g, b);
            }
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
            unsigned int px;
            unsigned int py;

            px = origin_x + col;
            py = origin_y + row;

            if (px >= SCREEN_WIDTH || py >= SCREEN_HEIGHT) {
                continue;
            }

            if (bitmap[row][col] == '#') {
                put_rgb(screen, px, py,
                        SCREEN_INK_R, SCREEN_INK_G, SCREEN_INK_B);
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
            unsigned int px;
            unsigned int py;
            char cell;

            if (line[col] == '\0') {
                break;
            }

            cell = line[col];
            px = origin_x + col;
            py = origin_y + row;

            if (px >= SCREEN_WIDTH || py >= SCREEN_HEIGHT) {
                continue;
            }

            if (cell == '#' || cell == '1') {
                put_rgb(screen, px, py,
                        SCREEN_INK_R, SCREEN_INK_G, SCREEN_INK_B);
            }
        }
    }

    return 1;
}
