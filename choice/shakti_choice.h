#ifndef SHAKTI_CHOICE_H
#define SHAKTI_CHOICE_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define SHAKTI_CHOICE_MAX_OPTIONS       16U
#define SHAKTI_CHOICE_MAX_CONDITIONS     8U
#define SHAKTI_CHOICE_MAX_HORIZONS       8U
#define SHAKTI_CHOICE_MAX_CLAIMS        16U
#define SHAKTI_CHOICE_FRAME_COUNT        5U
#define SHAKTI_CHOICE_POLE_COUNT         2U
#define SHAKTI_CHOICE_SISTER_COUNT       3U
#define SHAKTI_CHOICE_LABEL_CAPACITY    64U
#define SHAKTI_CHOICE_REF_CAPACITY     128U
#define SHAKTI_CHOICE_TEXT_CAPACITY    256U
#define SHAKTI_CHOICE_CONFIDENCE_MAX  1000U
#define SHAKTI_CHOICE_DELIB_MAX         16
#define SHAKTI_CHOICE_LADDER             3
#define SHAKTI_CHOICE_IRREVERSIBLE       3

typedef enum {
    SHAKTI_CHOICE_OK = 0,
    SHAKTI_CHOICE_REFUSED = -1,
    SHAKTI_CHOICE_FULL = -2,
    SHAKTI_CHOICE_OVERFLOW = -3,
    SHAKTI_CHOICE_OPEN = -4
} shakti_choice_code_t;

typedef enum {
    SHAKTI_TRUTH_UNKNOWN = 0,
    SHAKTI_TRUTH_FALSE = 1,
    SHAKTI_TRUTH_TRUE = 2
} shakti_truth_t;

typedef enum {
    SHAKTI_EVIDENCE_NONE = 0,
    SHAKTI_EVIDENCE_CREATIVE = 1,
    SHAKTI_EVIDENCE_CLAIM = 2,
    SHAKTI_EVIDENCE_TAUGHT = 3,
    SHAKTI_EVIDENCE_OBSERVED = 4,
    SHAKTI_EVIDENCE_VERIFIED = 5
} shakti_choice_evidence_t;

typedef enum {
    SHAKTI_OPTION_ACTION = 0,
    SHAKTI_OPTION_NO_ACTION = 1,
    SHAKTI_OPTION_WAIT = 2,
    SHAKTI_OPTION_REFUSE = 3,
    SHAKTI_OPTION_TEACH_ME = 4,
    SHAKTI_OPTION_ALTERNATIVE = 5
} shakti_option_kind_t;

typedef enum {
    SHAKTI_OPTION_UNCHECKED = 0,
    SHAKTI_OPTION_SURVIVES = 1,
    SHAKTI_OPTION_FILTERED = 2,
    SHAKTI_OPTION_EVIDENCE_OPEN = 3
} shakti_option_status_t;

typedef enum {
    SHAKTI_SISTER_DESTROYER = 0,
    SHAKTI_SISTER_PRESERVER = 1,
    SHAKTI_SISTER_CREATOR = 2
} shakti_sister_t;

typedef struct {
    char name[SHAKTI_CHOICE_LABEL_CAPACITY];
    char evidence_ref[SHAKTI_CHOICE_REF_CAPACITY];
    shakti_choice_evidence_t evidence;
} shakti_choice_condition_t;

typedef struct {
    uint64_t ticks_from_now;
    char name[SHAKTI_CHOICE_LABEL_CAPACITY];
} shakti_choice_horizon_t;

typedef struct {
    unsigned char present;
    shakti_truth_t reachable;
    shakti_choice_evidence_t evidence;
    unsigned int confidence;
    unsigned int severity;
    unsigned char reversible;
    unsigned char protected_boundary_breach;
    int64_t value;
    char evidence_ref[SHAKTI_CHOICE_REF_CAPACITY];
} shakti_consequence_t;

typedef struct {
    shakti_option_kind_t kind;
    char name[SHAKTI_CHOICE_LABEL_CAPACITY];
    char discovery_ref[SHAKTI_CHOICE_REF_CAPACITY];
    shakti_choice_evidence_t discovery_evidence;
    shakti_consequence_t consequence[SHAKTI_CHOICE_MAX_CONDITIONS]
                                           [SHAKTI_CHOICE_MAX_HORIZONS];
} shakti_choice_option_t;

typedef struct {
    char text[SHAKTI_CHOICE_TEXT_CAPACITY];
    char speaker_ref[SHAKTI_CHOICE_REF_CAPACITY];
} shakti_choice_claim_t;

typedef struct {
    shakti_choice_condition_t conditions[SHAKTI_CHOICE_MAX_CONDITIONS];
    shakti_choice_horizon_t horizons[SHAKTI_CHOICE_MAX_HORIZONS];
    shakti_choice_option_t options[SHAKTI_CHOICE_MAX_OPTIONS];
    shakti_choice_claim_t claims[SHAKTI_CHOICE_MAX_CLAIMS];
    size_t condition_count;
    size_t horizon_count;
    size_t option_count;
    size_t claim_count;
    int baseline_index;
    unsigned char enumeration_closed;
    char enumeration_ref[SHAKTI_CHOICE_REF_CAPACITY];
} shakti_choice_plan_t;

typedef struct {
    unsigned int catastrophic_severity;
    unsigned int required_confidence;
} shakti_choice_policy_t;

typedef struct {
    shakti_option_status_t option_status[SHAKTI_CHOICE_MAX_OPTIONS];
    unsigned int surviving_count;
    unsigned int filtered_count;
    unsigned int evidence_open_count;
    unsigned int unverified_claim_count;
    unsigned char baseline_survives;
    unsigned char baseline_filtered;
} shakti_choice_result_t;

typedef struct {
    unsigned char locked;
    char name[SHAKTI_CHOICE_LABEL_CAPACITY];
    char question[SHAKTI_CHOICE_TEXT_CAPACITY];
    char evidence_ref[SHAKTI_CHOICE_REF_CAPACITY];
} shakti_frame_definition_t;

typedef struct {
    unsigned char locked;
    char name[SHAKTI_CHOICE_LABEL_CAPACITY];
    char evidence_ref[SHAKTI_CHOICE_REF_CAPACITY];
} shakti_pole_definition_t;

typedef struct {
    shakti_frame_definition_t frame[SHAKTI_CHOICE_FRAME_COUNT];
    shakti_pole_definition_t pole[SHAKTI_CHOICE_POLE_COUNT];
} shakti_geometry_schema_t;

typedef struct {
    unsigned char present;
    int64_t value;
    shakti_choice_evidence_t evidence;
    char evidence_ref[SHAKTI_CHOICE_REF_CAPACITY];
} shakti_sister_reading_t;

typedef struct {
    char source_memory_ref[SHAKTI_CHOICE_REF_CAPACITY];
    char use_event_ref[SHAKTI_CHOICE_REF_CAPACITY];
    unsigned int frame[2];
    unsigned int pole;
    unsigned int vertex;
    shakti_sister_reading_t reading[2][SHAKTI_CHOICE_SISTER_COUNT];
} shakti_use_geometry_t;

typedef struct {
    uint64_t reading_id;
    unsigned char visible_to_subject;
    shakti_choice_evidence_t evidence;
    char subject[SHAKTI_CHOICE_LABEL_CAPACITY];
    char history_ref[SHAKTI_CHOICE_REF_CAPACITY];
    char state_reading[SHAKTI_CHOICE_TEXT_CAPACITY];
    char obligation_ref[SHAKTI_CHOICE_REF_CAPACITY];
    char conclusion[SHAKTI_CHOICE_TEXT_CAPACITY];
} shakti_person_reading_t;

void shakti_choice_plan_init(shakti_choice_plan_t *plan);

int shakti_choice_add_condition(
    shakti_choice_plan_t *plan,
    const char *name,
    shakti_choice_evidence_t evidence,
    const char *evidence_ref
);

int shakti_choice_add_horizon(
    shakti_choice_plan_t *plan,
    uint64_t ticks_from_now,
    const char *name
);

int shakti_choice_add_option(
    shakti_choice_plan_t *plan,
    shakti_option_kind_t kind,
    const char *name,
    shakti_choice_evidence_t discovery_evidence,
    const char *discovery_ref
);

int shakti_choice_add_claim(
    shakti_choice_plan_t *plan,
    const char *text,
    const char *speaker_ref
);

int shakti_choice_set_consequence(
    shakti_choice_plan_t *plan,
    size_t option_index,
    size_t condition_index,
    size_t horizon_index,
    const shakti_consequence_t *consequence
);

int shakti_choice_close_enumeration(
    shakti_choice_plan_t *plan,
    const char *enumeration_ref
);

int shakti_choice_filter(
    const shakti_choice_plan_t *plan,
    const shakti_choice_policy_t *policy,
    shakti_choice_result_t *result
);

int shakti_choice_delta(
    const shakti_choice_plan_t *plan,
    size_t option_index,
    size_t condition_index,
    size_t horizon_index,
    int64_t *delta
);

int shakti_choice_weigh_range(
    int64_t best,
    int64_t worst,
    int reversible
);

int shakti_choice_survivor_ladder(
    const shakti_choice_plan_t *plan,
    const shakti_choice_result_t *result,
    int64_t *best_delta,
    int64_t *worst_delta
);

const char *shakti_choice_evidence_name(shakti_choice_evidence_t evidence);
const char *shakti_choice_option_kind_name(shakti_option_kind_t kind);
const char *shakti_choice_option_status_name(shakti_option_status_t status);
const char *shakti_choice_sister_name(shakti_sister_t sister);

void shakti_geometry_init(shakti_geometry_schema_t *schema);

int shakti_geometry_define_frame(
    shakti_geometry_schema_t *schema,
    unsigned int frame,
    const char *name,
    const char *question,
    const char *evidence_ref,
    int locked
);

int shakti_geometry_define_pole(
    shakti_geometry_schema_t *schema,
    unsigned int pole,
    const char *name,
    const char *evidence_ref,
    int locked
);

int shakti_geometry_ready(const shakti_geometry_schema_t *schema);

int shakti_geometry_vertex(
    unsigned int frame_a,
    unsigned int frame_b,
    unsigned int pole,
    unsigned int *vertex
);

int shakti_geometry_place_use(
    const shakti_geometry_schema_t *schema,
    shakti_use_geometry_t *use,
    const char *source_memory_ref,
    const char *use_event_ref,
    unsigned int frame_a,
    unsigned int frame_b,
    unsigned int pole
);

int shakti_geometry_set_sister_reading(
    shakti_use_geometry_t *use,
    unsigned int frame_slot,
    shakti_sister_t sister,
    int64_t value,
    shakti_choice_evidence_t evidence,
    const char *evidence_ref
);

int shakti_geometry_write_xml(
    FILE *output,
    const shakti_geometry_schema_t *schema,
    const shakti_use_geometry_t *use
);

int shakti_person_reading_write_xml(
    FILE *output,
    const shakti_person_reading_t *reading
);

int shakti_person_correction_write_xml(
    FILE *output,
    uint64_t reading_id,
    const char *subject,
    const char *correction,
    const char *evidence_ref
);

#endif
