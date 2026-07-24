#ifndef SHAKTI_LOOP_H
#define SHAKTI_LOOP_H

#include <stdio.h>

#include "shakti_types.h"

void shakti_loop_init(shakti_loop_state_t *state);

int shakti_loop_load(
    shakti_loop_state_t *state,
    const char *goal_path,
    const char *notebook_path,
    const char *menu_path
);

int shakti_loop_begin_cycle(
    shakti_runtime_t *runtime,
    shakti_tick_t *cycle_tick
);

int shakti_loop_finish_cycle(shakti_runtime_t *runtime);

int shakti_loop_set_heartbeat(
    shakti_loop_state_t *state,
    unsigned int minutes
);

int shakti_loop_set_goal(
    shakti_runtime_t *runtime,
    const char *goal
);

int shakti_loop_notebook(
    shakti_runtime_t *runtime,
    const char *entry
);

void shakti_loop_menu_titles(const shakti_loop_state_t *state);

int shakti_loop_menu_section(
    const shakti_loop_state_t *state,
    const char *title
);

int shakti_loop_record_message(
    shakti_runtime_t *runtime,
    const char *route,
    const char *text
);

void shakti_loop_interrupt_tools(shakti_loop_state_t *state);
void shakti_loop_resume_tools(shakti_loop_state_t *state);
int shakti_loop_tools_available(const shakti_loop_state_t *state);

int shakti_loop_defer_reflection(shakti_loop_state_t *state);

int shakti_loop_run_reflection(
    shakti_runtime_t *runtime,
    FILE *input,
    FILE *output
);

void shakti_loop_print_reflection_questions(FILE *output);
void shakti_loop_print_status(const shakti_loop_state_t *state);

#endif
