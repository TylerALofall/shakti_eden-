#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "shakti_receptor.h"
#include "shakti_time.h"

#define ROUNDTRIP_PASSES 20U

static unsigned int collect_frame(
    shakti_receptor_t *receptor,
    shakti_tick_clock_t *clock_state
)
{
    unsigned int poll;

    if (!shakti_receptor_open_shutter(receptor, clock_state)) {
        return 0U;
    }

    for (poll = 0U; poll < receptor->exposure_ticks; ++poll) {
        (void)shakti_receptor_poll(receptor);
    }

    return shakti_receptor_readout(receptor) ? 1U : 0U;
}

int main(void)
{
    shakti_receptor_t receptor;
    shakti_tick_clock_t clock_state;
    char reference[SHAKTI_RECEPTOR_TEXT_CAPACITY];
    char current[SHAKTI_RECEPTOR_TEXT_CAPACITY];
    char bits[SHAKTI_RECEPTOR_MAX_PIXELS + 1U];
    unsigned int pass;
    unsigned int cell_count;
    unsigned long total_drift;

    shakti_receptor_init(&receptor);
    shakti_tick_clock_init(&clock_state);

    assert(shakti_receptor_configure(&receptor, 8U, 8U, 2U, 1U, 1U));
    cell_count = receptor.width * receptor.height;

    assert(collect_frame(&receptor, &clock_state));
    assert(strlen(receptor.frame_text) == (size_t)cell_count);

    assert(shakti_receptor_binarize_frame(
        receptor.frame_text,
        cell_count,
        reference,
        sizeof(reference)
    ));
    assert(strlen(reference) == (size_t)cell_count);

    /* Reject malformed frames. */
    assert(!shakti_receptor_binarize_frame(NULL, cell_count, bits, sizeof(bits)));
    assert(!shakti_receptor_binarize_frame(
        receptor.frame_text,
        0U,
        bits,
        sizeof(bits)
    ));
    assert(!shakti_receptor_binarize_frame(
        receptor.frame_text,
        cell_count,
        NULL,
        sizeof(bits)
    ));
    assert(!shakti_receptor_binarize_frame(
        receptor.frame_text,
        cell_count,
        bits,
        (unsigned long)cell_count
    ));
    {
        char bad[2U] = { 'x', '\0' };

        assert(!shakti_receptor_binarize_frame(bad, 1U, bits, sizeof(bits)));
    }

    strcpy(current, reference);
    total_drift = 0UL;

    printf("pass drift cumulative\n");
    fflush(stdout);

    for (pass = 1U; pass <= ROUNDTRIP_PASSES; ++pass) {
        unsigned int index;
        unsigned int drift;

        /* Reconstruct: present the collected bits back as the next frame. */
        assert(shakti_receptor_binarize_frame(
            current,
            cell_count,
            bits,
            sizeof(bits)
        ));

        drift = 0U;

        for (index = 0U; index < cell_count; ++index) {
            if (bits[index] != reference[index]) {
                ++drift;
            }
        }

        total_drift += (unsigned long)drift;
        printf("%4u %5u %10lu\n", pass, drift, total_drift);
        fflush(stdout);

        strcpy(current, bits);
    }

    assert(total_drift == 0UL);
    printf("round-trip stable: 0 pixels lost over %u passes\n",
        ROUNDTRIP_PASSES);

    return 0;
}
