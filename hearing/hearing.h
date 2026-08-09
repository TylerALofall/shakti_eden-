#ifndef SHAKTI_HEARING_H
#define SHAKTI_HEARING_H

#define SHAKTI_HEARING_SAMPLE_RATE 16000U
#define SHAKTI_HEARING_HEARTBEAT_BPM 72U
/* Capacity must cover Phase 1 dark (0–20 s) and enter Phase 2 entrainment. */
#define SHAKTI_HEARING_MAX_DURATION_SECONDS 25.0
#define SHAKTI_HEARING_MAX_SAMPLES 400000U /* 25 seconds of 16kHz audio */
#define SHAKTI_HEARING_DARK_PHASE_SECONDS 20.0

/* Colossal number: Human gestation in seconds (266 days * 86,400 seconds/day) */
#define SHAKTI_PRENATAL_GESTATION_SECONDS (266UL * 86400UL)

/* Compact GRU parameters for deterministic prenatal learning */
#define SHAKTI_HEARING_INPUT_DIM 2U  /* Input 0: audio pulse amplitude, Input 1: scalar light intensity */
#define SHAKTI_HEARING_HIDDEN_DIM 8U /* Hidden state size of the GRU cell */

typedef struct {
    float sample_buffer[SHAKTI_HEARING_MAX_SAMPLES];
    unsigned int sample_count;
    
    /* One scalar light intensity per audio frame (10 ms / 160 samples at 16 kHz) */
    float flash_intensities[SHAKTI_HEARING_MAX_SAMPLES / 160U];
    unsigned int flash_count;
} shakti_prenatal_sensory_stream_t;

/* Tiny GRU (Gated Recurrent Unit) Model State */
typedef struct {
    /* Update gate weights (Wz, Uz, bz) */
    float Wz[SHAKTI_HEARING_HIDDEN_DIM][SHAKTI_HEARING_INPUT_DIM];
    float Uz[SHAKTI_HEARING_HIDDEN_DIM][SHAKTI_HEARING_HIDDEN_DIM];
    float bz[SHAKTI_HEARING_HIDDEN_DIM];

    /* Reset gate weights (Wr, Ur, br) */
    float Wr[SHAKTI_HEARING_HIDDEN_DIM][SHAKTI_HEARING_INPUT_DIM];
    float Ur[SHAKTI_HEARING_HIDDEN_DIM][SHAKTI_HEARING_HIDDEN_DIM];
    float br[SHAKTI_HEARING_HIDDEN_DIM];

    /* Candidate hidden state weights (Wh, Uh, bh) */
    float Wh[SHAKTI_HEARING_HIDDEN_DIM][SHAKTI_HEARING_INPUT_DIM];
    float Uh[SHAKTI_HEARING_HIDDEN_DIM][SHAKTI_HEARING_HIDDEN_DIM];
    float bh[SHAKTI_HEARING_HIDDEN_DIM];

    /* Output projection weights (Wy, by) */
    float Wy[SHAKTI_HEARING_INPUT_DIM][SHAKTI_HEARING_HIDDEN_DIM];
    float by[SHAKTI_HEARING_INPUT_DIM];

    /* Active hidden state */
    float h[SHAKTI_HEARING_HIDDEN_DIM];
} shakti_hearing_gru_t;

/* Initialization */
void shakti_hearing_init_stream(shakti_prenatal_sensory_stream_t *stream);
void shakti_hearing_init_gru(shakti_hearing_gru_t *model);

/* Deterministic Audio & Visual Synthesis */
int shakti_hearing_synthesize_prenatal(
    shakti_prenatal_sensory_stream_t *stream,
    double duration_seconds
);

/* Model Operations */
void shakti_hearing_gru_forward(
    shakti_hearing_gru_t *model,
    const float input[SHAKTI_HEARING_INPUT_DIM],
    float output[SHAKTI_HEARING_INPUT_DIM]
);

int shakti_hearing_train_prenatal_subconscious(
    shakti_hearing_gru_t *model,
    const shakti_prenatal_sensory_stream_t *stream,
    unsigned int epochs,
    float learning_rate
);

#endif /* SHAKTI_HEARING_H */
