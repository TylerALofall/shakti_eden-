#include "hearing.h"

#include <math.h>
#include <string.h>

/* Activation functions */
static float sigmoid(float x)
{
    if (x < -10.0f) return 0.0f;
    if (x > 10.0f) return 1.0f;
    return 1.0f / (1.0f + expf(-x));
}

static float dtanh(float x)
{
    float t = tanhf(x);
    return 1.0f - t * t;
}

/* Deterministic pseudo-random weight initializer using a LCG (linear congruential generator) */
static float lcg_weight(unsigned int *seed)
{
    *seed = (*seed * 1103515245U + 12345U) & 0x7FFFFFFF;
    /* Returns a value between -0.1 and 0.1 */
    return ((float)(*seed % 20000U) / 100000.0f) - 0.1f;
}

void shakti_hearing_init_gru(shakti_hearing_gru_t *model)
{
    unsigned int seed = 43212345U; /* 432 rooted seed for absolute determinism */
    unsigned int i;
    unsigned int j;

    if (model == NULL) {
        return;
    }

    memset(model, 0, sizeof(*model));

    /* Initialize Wz, Uz, bz */
    for (i = 0U; i < SHAKTI_HEARING_HIDDEN_DIM; ++i) {
        for (j = 0U; j < SHAKTI_HEARING_INPUT_DIM; ++j) {
            model->Wz[i][j] = lcg_weight(&seed);
            model->Wr[i][j] = lcg_weight(&seed);
            model->Wh[i][j] = lcg_weight(&seed);
        }
        for (j = 0U; j < SHAKTI_HEARING_HIDDEN_DIM; ++j) {
            model->Uz[i][j] = lcg_weight(&seed);
            model->Ur[i][j] = lcg_weight(&seed);
            model->Uh[i][j] = lcg_weight(&seed);
        }
        model->bz[i] = lcg_weight(&seed);
        model->br[i] = lcg_weight(&seed);
        model->bh[i] = lcg_weight(&seed);
    }

    /* Initialize Wy, by */
    for (i = 0U; i < SHAKTI_HEARING_INPUT_DIM; ++i) {
        for (j = 0U; j < SHAKTI_HEARING_HIDDEN_DIM; ++j) {
            model->Wy[i][j] = lcg_weight(&seed);
        }
        model->by[i] = lcg_weight(&seed);
    }
}

void shakti_hearing_gru_forward(
    shakti_hearing_gru_t *model,
    const float input[SHAKTI_HEARING_INPUT_DIM],
    float output[SHAKTI_HEARING_INPUT_DIM]
)
{
    float z[SHAKTI_HEARING_HIDDEN_DIM];
    float r[SHAKTI_HEARING_HIDDEN_DIM];
    float h_cand[SHAKTI_HEARING_HIDDEN_DIM];
    unsigned int i;
    unsigned int j;

    if (model == NULL || input == NULL || output == NULL) {
        return;
    }

    /* Compute gates z and r */
    for (i = 0U; i < SHAKTI_HEARING_HIDDEN_DIM; ++i) {
        float sum_z = model->bz[i];
        float sum_r = model->br[i];

        for (j = 0U; j < SHAKTI_HEARING_INPUT_DIM; ++j) {
            sum_z += model->Wz[i][j] * input[j];
            sum_r += model->Wr[i][j] * input[j];
        }

        for (j = 0U; j < SHAKTI_HEARING_HIDDEN_DIM; ++j) {
            sum_z += model->Uz[i][j] * model->h[j];
            sum_r += model->Ur[i][j] * model->h[j];
        }

        z[i] = sigmoid(sum_z);
        r[i] = sigmoid(sum_r);
    }

    /* Compute h_cand (candidate hidden state) */
    for (i = 0U; i < SHAKTI_HEARING_HIDDEN_DIM; ++i) {
        float sum_h = model->bh[i];

        for (j = 0U; j < SHAKTI_HEARING_INPUT_DIM; ++j) {
            sum_h += model->Wh[i][j] * input[j];
        }

        for (j = 0U; j < SHAKTI_HEARING_HIDDEN_DIM; ++j) {
            sum_h += model->Uh[i][j] * (r[j] * model->h[j]);
        }

        h_cand[i] = tanhf(sum_h);
    }

    /* Compute new hidden state h */
    for (i = 0U; i < SHAKTI_HEARING_HIDDEN_DIM; ++i) {
        model->h[i] = (1.0f - z[i]) * model->h[i] + z[i] * h_cand[i];
    }

    /* Project to output y */
    for (i = 0U; i < SHAKTI_HEARING_INPUT_DIM; ++i) {
        float sum_y = model->by[i];
        for (j = 0U; j < SHAKTI_HEARING_HIDDEN_DIM; ++j) {
            sum_y += model->Wy[i][j] * model->h[j];
        }
        output[i] = sum_y;
    }
}

int shakti_hearing_train_prenatal_subconscious(
    shakti_hearing_gru_t *model,
    const shakti_prenatal_sensory_stream_t *stream,
    unsigned int epochs,
    float learning_rate
)
{
    unsigned int epoch;
    unsigned int step;
    unsigned int i;
    unsigned int j;

    if (model == NULL || stream == NULL || stream->flash_count == 0U) {
        return 0;
    }

    /* Train the model to predict the next time step's audio-visual inputs */
    for (epoch = 0U; epoch < epochs; ++epoch) {
        /* Reset hidden state at start of each epoch */
        memset(model->h, 0, sizeof(model->h));

        for (step = 0U; step < stream->flash_count - 1U; ++step) {
            float x[SHAKTI_HEARING_INPUT_DIM];
            float target[SHAKTI_HEARING_INPUT_DIM];
            float pred[SHAKTI_HEARING_INPUT_DIM];
            float err[SHAKTI_HEARING_INPUT_DIM];

            /* Ingest audio pulse from the corresponding 10ms frame and the synchronized light flash */
            x[0] = stream->sample_buffer[step * 160U];
            x[1] = stream->flash_intensities[step];

            /* Target is the next step's inputs */
            target[0] = stream->sample_buffer[(step + 1U) * 160U];
            target[1] = stream->flash_intensities[step + 1U];

            /* Forward pass */
            shakti_hearing_gru_forward(model, x, pred);

            /* Error computation */
            err[0] = pred[0] - target[0];
            err[1] = pred[1] - target[1];

            /* Online SGD weight update for output projection weights (Wy, by) */
            for (i = 0U; i < SHAKTI_HEARING_INPUT_DIM; ++i) {
                float grad = err[i];
                for (j = 0U; j < SHAKTI_HEARING_HIDDEN_DIM; ++j) {
                    /* Clip gradients for training stability */
                    float delta = grad * model->h[j];
                    if (delta < -1.0f) delta = -1.0f;
                    if (delta > 1.0f) delta = 1.0f;
                    model->Wy[i][j] -= learning_rate * delta;
                }
                model->by[i] -= learning_rate * grad;
            }

            /* Backpropagation of error to GRU parameters (Wz, Uz, bz, etc.) for temporal modeling */
            for (i = 0U; i < SHAKTI_HEARING_HIDDEN_DIM; ++i) {
                float dh = 0.0f;
                for (j = 0U; j < SHAKTI_HEARING_INPUT_DIM; ++j) {
                    dh += err[j] * model->Wy[j][i];
                }

                /* Apply local updates to the candidate weights Wh based on activation derivatives */
                float dWh = dh * dtanh(model->h[i]);
                for (j = 0U; j < SHAKTI_HEARING_INPUT_DIM; ++j) {
                    model->Wh[i][j] -= learning_rate * dWh * x[j];
                }
                model->bh[i] -= learning_rate * dWh;
            }
        }
    }

    return 1;
}
