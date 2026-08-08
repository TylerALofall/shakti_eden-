#ifndef SHAKTI_RECEPTOR_H
#define SHAKTI_RECEPTOR_H

#include "shakti_types.h"

#define SHAKTI_RECEPTOR_MAX_PIXELS 64U
#define SHAKTI_RECEPTOR_PIXEL_CAPACITY 4U
#define SHAKTI_RECEPTOR_TEXT_CAPACITY \
    (SHAKTI_RECEPTOR_MAX_PIXELS * SHAKTI_RECEPTOR_PIXEL_CAPACITY + 1U)

/* Neutral placeholder level for the simulated 0-9 scale, pending real
 * camera input. Digital collection is binary at the pixel; the graded
 * scale only rehearses the shape of a future sensor. */
#define SHAKTI_RECEPTOR_DEFAULT_LEVEL 5U
/* Cells at or above this level binarize to 1; below it to 0. */
#define SHAKTI_RECEPTOR_BINARIZE_THRESHOLD 5U

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int exposure_ticks;
    unsigned int gain;
    unsigned int binning;
    unsigned char shutter_open;
    shakti_tick_t exposure_start;
    unsigned int poll_count;
    char frame_text[SHAKTI_RECEPTOR_TEXT_CAPACITY];
} shakti_receptor_t;

void shakti_receptor_init(shakti_receptor_t *receptor);

int shakti_receptor_configure(
    shakti_receptor_t *receptor,
    unsigned int width,
    unsigned int height,
    unsigned int exposure_ticks,
    unsigned int gain,
    unsigned int binning
);

int shakti_receptor_open_shutter(
    shakti_receptor_t *receptor,
    shakti_tick_clock_t *clock_state
);

int shakti_receptor_poll(shakti_receptor_t *receptor);

int shakti_receptor_readout(shakti_receptor_t *receptor);

int shakti_receptor_write_frame_artifact(
    const shakti_receptor_t *receptor,
    const char *path
);

int shakti_receptor_binarize_frame(
    const char *frame_text,
    unsigned int cell_count,
    char *output,
    unsigned long output_capacity
);

#endif
