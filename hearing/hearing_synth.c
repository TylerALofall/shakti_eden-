#include "hearing.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void shakti_hearing_init_stream(shakti_prenatal_sensory_stream_t *stream)
{
    if (stream == NULL) {
        return;
    }
    memset(stream, 0, sizeof(*stream));
}

int shakti_hearing_synthesize_prenatal(
    shakti_prenatal_sensory_stream_t *stream,
    double duration_seconds
)
{
    unsigned int total_samples;
    unsigned int sample_index;
    double heartbeat_period_samples;

    if (stream == NULL || duration_seconds <= 0.0) {
        return 0;
    }

    total_samples = (unsigned int)(duration_seconds * SHAKTI_HEARING_SAMPLE_RATE);
    if (total_samples > SHAKTI_HEARING_MAX_SAMPLES) {
        total_samples = SHAKTI_HEARING_MAX_SAMPLES;
    }

    stream->sample_count = total_samples;
    stream->flash_count = total_samples / 160U; /* 10ms frame rate (160 samples at 16kHz) */

    heartbeat_period_samples = (60.0 / (double)SHAKTI_HEARING_HEARTBEAT_BPM) * SHAKTI_HEARING_SAMPLE_RATE;

    for (sample_index = 0U; sample_index < total_samples; ++sample_index) {
        double cycle_phase_samples;
        double t;
        double lub;
        double dub;
        double audio_val;
        double flash_val;

        /* Calculate current position inside the 0.833 second heartbeat cycle */
        cycle_phase_samples = fmod((double)sample_index, heartbeat_period_samples);
        t = cycle_phase_samples / (double)SHAKTI_HEARING_SAMPLE_RATE;

        /* "Lub" pulse (Systole contraction at t = 0) */
        lub = 0.0;
        if (t < 0.15) {
            lub = sin(2.0 * M_PI * 40.0 * t) * exp(-t / 0.05);
        }

        /* "Dub" pulse (Diastole contraction at t = 0.18) */
        dub = 0.0;
        if (t >= 0.18 && t < 0.35) {
            double dt = t - 0.18;
            dub = 0.8 * sin(2.0 * M_PI * 60.0 * dt) * exp(-dt / 0.04);
        }

        audio_val = lub + dub;
        stream->sample_buffer[sample_index] = (float)audio_val;

        /* Synthesize visual warm red light flash peaking with the systolic peak (Lub) */
        if (sample_index % 160U == 0U) {
            unsigned int flash_index = sample_index / 160U;
            if (flash_index < stream->flash_count) {
                /* Flash intensity tracks the cardiac cycle envelope */
                flash_val = 0.1; /* ambient soft womb background light */
                if (t < 0.25) {
                    flash_val += 0.8 * exp(-t / 0.08);
                } else if (t >= 0.18 && t < 0.30) {
                    flash_val += 0.4 * exp(-(t - 0.18) / 0.06);
                }
                if (flash_val > 1.0) {
                    flash_val = 1.0;
                }
                stream->flash_intensities[flash_index] = (float)flash_val;
            }
        }
    }

    return 1;
}
