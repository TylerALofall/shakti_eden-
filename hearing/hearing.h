#ifndef SHAKTI_HEARING_H
#define SHAKTI_HEARING_H

/*
 * hearing — mechanical prenatal sound + light stream only.
 * No GRU, no second core, no training weights inside Shakti's shell.
 * Binary sense path: synthesize PCM + light frames for sense_converge.
 */

#define SHAKTI_HEARING_SAMPLE_RATE 16000U
#define SHAKTI_HEARING_HEARTBEAT_BPM 72U
/* Capacity must cover Phase 1 dark (0–20 s) and enter Phase 2 entrainment. */
#define SHAKTI_HEARING_MAX_DURATION_SECONDS 25.0
#define SHAKTI_HEARING_MAX_SAMPLES 400000U /* 25 seconds of 16kHz audio */
#define SHAKTI_HEARING_DARK_PHASE_SECONDS 20.0

/* Human gestation in seconds (266 days * 86,400 seconds/day) — scale target. */
#define SHAKTI_PRENATAL_GESTATION_SECONDS (266UL * 86400UL)

typedef struct {
    float sample_buffer[SHAKTI_HEARING_MAX_SAMPLES];
    unsigned int sample_count;

    /* One scalar light intensity per audio frame (10 ms / 160 samples at 16 kHz) */
    float flash_intensities[SHAKTI_HEARING_MAX_SAMPLES / 160U];
    unsigned int flash_count;
} shakti_prenatal_sensory_stream_t;

void shakti_hearing_init_stream(shakti_prenatal_sensory_stream_t *stream);

/* Deterministic maternal heartbeat PCM + synchronized light flashes. */
int shakti_hearing_synthesize_prenatal(
    shakti_prenatal_sensory_stream_t *stream,
    double duration_seconds
);

#endif /* SHAKTI_HEARING_H */
