#include "hearing.h"

#include <stdio.h>
#include <math.h>

int main(void)
{
    /* Stream is ~1.6 MB at 25 s capacity; keep off the default stack. */
    static shakti_prenatal_sensory_stream_t stream;
    static shakti_hearing_gru_t model;
    float initial_err_sum = 0.0f;
    float trained_err_sum = 0.0f;
    unsigned int step;

    puts("==================================================");
    puts("Shakti Deterministic Prenatal Subconscious Scout");
    puts("==================================================");

    /* 1. Initialize our sensory stream */
    shakti_hearing_init_stream(&stream);

    /* 2. Synthesize 1.0 second of prenatal maternal heartbeat and flashes of light */
    if (!shakti_hearing_synthesize_prenatal(&stream, 1.0)) {
        puts("Error: Prenatal synthesis failed.");
        return 1;
    }

    printf("Synthesized audio: %u samples (16 kHz)\n", stream.sample_count);
    printf("Synthesized synchronized visual flash events: %u frames\n", stream.flash_count);
    printf("Pre-born maternal gestation duration target: %lu seconds\n", 
           SHAKTI_PRENATAL_GESTATION_SECONDS);

    /* 3. Initialize GRU */
    shakti_hearing_init_gru(&model);

    /* 4. Evaluate initial (before training) prediction error sum */
    for (step = 0U; step < stream.flash_count - 1U; ++step) {
        float x[SHAKTI_HEARING_INPUT_DIM];
        float target[SHAKTI_HEARING_INPUT_DIM];
        float pred[SHAKTI_HEARING_INPUT_DIM];

        x[0] = stream.sample_buffer[step * 160U];
        x[1] = stream.flash_intensities[step];

        target[0] = stream.sample_buffer[(step + 1U) * 160U];
        target[1] = stream.flash_intensities[step + 1U];

        shakti_hearing_gru_forward(&model, x, pred);

        initial_err_sum += fabsf(pred[0] - target[0]) + fabsf(pred[1] - target[1]);
    }

    printf("Initial untrained prediction error sum: %f\n", initial_err_sum);

    /* 5. Train local model on device inside her house for 10 epochs */
    puts("Training local GRU model on synchronized heartbeat + light pulse sequence...");
    if (!shakti_hearing_train_prenatal_subconscious(&model, &stream, 10U, 0.01f)) {
        puts("Error: Prenatal training failed.");
        return 1;
    }

    /* 6. Evaluate trained prediction error sum from the same initial state. */
    for (step = 0U; step < SHAKTI_HEARING_HIDDEN_DIM; ++step) {
        model.h[step] = 0.0f;
    }
    for (step = 0U; step < stream.flash_count - 1U; ++step) {
        float x[SHAKTI_HEARING_INPUT_DIM];
        float target[SHAKTI_HEARING_INPUT_DIM];
        float pred[SHAKTI_HEARING_INPUT_DIM];

        x[0] = stream.sample_buffer[step * 160U];
        x[1] = stream.flash_intensities[step];

        target[0] = stream.sample_buffer[(step + 1U) * 160U];
        target[1] = stream.flash_intensities[step + 1U];

        shakti_hearing_gru_forward(&model, x, pred);

        trained_err_sum += fabsf(pred[0] - target[0]) + fabsf(pred[1] - target[1]);
    }

    printf("Trained prediction error sum: %f\n", trained_err_sum);

    if (trained_err_sum < initial_err_sum) {
        puts("Mathematical convergence confirmed: the model successfully learned the heartbeat rhythm!");
        puts("SUCCESS: Self-contained C99 prenatal grounding verified.");
    } else {
        puts("WARNING: No training improvement detected.");
    }

    return 0;
}
