#ifndef EYES_H
#define EYES_H

#include <stddef.h>

/*
 * eyes: Shakti's document collector and reconstruction side.
 *
 * Pipeline:
 *   load a document (raw RGBA bytes or a generated clean-text page)
 *   -> pull color   (per-channel 0/1 bits, RGB-mix -> ink like 3-color print)
 *   -> pull mono    (luma threshold -> one bit per pixel)
 *   -> reconstruct  (bits back into a bitmap, the "other side")
 *   -> diff         (reconstruction vs original, per-pixel loss)
 *   -> recognize    (mono cells -> clean-font text, redraw clean)
 *
 * All collection is deterministic from the pixel values alone. No clock,
 * no subprocess, no dynamic allocation; caller supplies every buffer.
 * This is the load point a Swift host can attach to later: hand it RGBA
 * bytes and a size, get bits and text back.
 */

#define EYES_MAX_PIXELS   4194304UL
#define EYES_TEXT_MAX     256UL
#define EYES_RECOG_MAX    1024UL
#define EYES_WORD_MAX     32U
#define EYES_MONO_THRESHOLD 5U

/* One recognized character: the clean glyph and where it sits (cells). */
typedef struct {
    char character;
    unsigned int cell_col;
    unsigned int cell_row;
} eyes_recog_t;

/* Static clean-text page: up to 3 lines of text rendered in the clean
 * 5x7 font (8x8 cells), centered on the page. */
typedef struct {
    char line1[EYES_TEXT_MAX];
    char line2[EYES_TEXT_MAX];
    char line3[EYES_TEXT_MAX];
} eyes_text_page_t;

/*
 * Return codes: every function returns 1 on success, 0 on rejection
 * (NULL pointers, zero/oversized dimensions, malformed input).
 */

/* Count how many 8x8 text cells fit horizontally / vertically. */
unsigned int eyes_cells_x(unsigned int width);
unsigned int eyes_cells_y(unsigned int height);

/* Render the clean 5x7 font glyph of a supported character into an 8x8
 * bitmap of '#'/'.'. Returns 0 for unsupported characters. */
int eyes_render_glyph(unsigned char character, char bitmap[8][9]);

/* 1 when every character of text has a clean glyph (text may be empty). */
int eyes_text_supported(const char *text);

/*
 * Load a document into rgba (width*height*4 bytes, caller buffer).
 * kind 0 = white page with the text page rendered in black ink;
 * kind 1 = procedurally generated color pattern (red/green/blue bands).
 * Returns 1 on success.
 */
int eyes_load_document(
    unsigned int kind,
    unsigned int width,
    unsigned int height,
    const eyes_text_page_t *text,
    unsigned long seed,
    unsigned char *rgba,
    unsigned long rgba_capacity
);

/*
 * Color pull. For each pixel, luma decides ink vs paper; ink pixels keep
 * their dominant RGB channel, paper is forced to white. The pull bit for
 * channel c is 1 when that channel is saturated (a full-strength primary
 * needs the others at zero, so any single-channel pull reads the 3-color
 * mix as ink -- the old color-printer trick). Output: 3 chars per pixel
 * ('0'/'1', order R,G,B) plus NUL. bits_capacity must be >= pixels*3+1.
 */
int eyes_pull_color(
    const unsigned char *rgba,
    unsigned int width,
    unsigned int height,
    char *bits,
    unsigned long bits_capacity
);

/*
 * Mono pull. Luma at or above the midpoint is paper '0', below is ink '1'.
 * Output: 1 char per pixel ('0'/'1') plus NUL. bits_capacity >= pixels+1.
 */
int eyes_pull_mono(
    const unsigned char *rgba,
    unsigned int width,
    unsigned int height,
    char *bits,
    unsigned long bits_capacity
);

/*
 * Reconstruct a bitmap from collected bits (the other side).
 * color != 0: expects 3 bits per pixel (R,G,B) and rebuilds RGBA.
 * color == 0: expects 1 bit per pixel; '1' becomes black, '0' white.
 */
int eyes_reconstruct_color(
    const char *bits,
    unsigned int width,
    unsigned int height,
    unsigned char *rgba,
    unsigned long rgba_capacity
);

int eyes_reconstruct_mono(
    const char *bits,
    unsigned int width,
    unsigned int height,
    unsigned char *rgba,
    unsigned long rgba_capacity
);

/*
 * Diff a reconstruction against the original.
 * Mode 0 (mono): both sides are reduced to ink/paper and compared.
 * Mode 1 (color): exact RGBA byte compare per pixel. A drifted pixel is
 * one that did not come back; drift 0 means every pixel came back.
 * Returns the number of drifted pixels, or (unsigned long)-1 on rejection.
 */
unsigned long eyes_diff(
    const unsigned char *original,
    const unsigned char *reconstructed,
    unsigned int width,
    unsigned int height,
    int color
);

/*
 * Recognize text on the mono reconstruction: each 8x8 cell is matched
 * against the clean font; exact full matches are recorded. Returns the
 * number of characters recognized (0 when none or on rejection).
 */
unsigned long eyes_recognize_text(
    const unsigned char *mono_rgba,
    unsigned int width,
    unsigned int height,
    eyes_recog_t *out,
    unsigned long out_capacity
);

/* Render recognized characters back into a clean white RGBA page. */
int eyes_render_clean_page(
    unsigned int width,
    unsigned int height,
    const eyes_recog_t *characters,
    unsigned long character_count,
    unsigned char *rgba,
    unsigned long rgba_capacity
);

/*
 * File output (.tan = text artifact notation).
 * write_frame: raw collected pull, "WIDTH= HEIGHT= KIND= TEXT=" + grid.
 * write_recon: reconstructed bitmap, '#' ink / '.' paper, per RGB channel
 *   and luma in color mode, single plane in mono mode.
 */
int eyes_write_frame(
    const char *path,
    unsigned int width,
    unsigned int height,
    int color,
    const char *bits
);

int eyes_write_recon(
    const char *path,
    unsigned int width,
    unsigned int height,
    int color,
    const unsigned char *rgba
);

/* Cycle stability: pull -> reconstruct -> re-pull -> compare pulls.
 * Deterministic, so exactly 20 iterations. Returns total drifted bits. */
unsigned long eyes_run_cycles(
    const unsigned char *original,
    unsigned int width,
    unsigned int height,
    int color,
    unsigned char *work_a,
    unsigned char *work_b,
    char *bits_a,
    char *bits_b,
    unsigned long buffer_capacity
);

#endif
