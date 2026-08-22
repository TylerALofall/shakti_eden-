#ifndef SHAKTI_MCP_H
#define SHAKTI_MCP_H

#include <stddef.h>

#include "shakti_types.h"

#define SHAKTI_MCP_MAX_TOOLS 16U
#define SHAKTI_MCP_NAME_CAPACITY 32U
#define SHAKTI_MCP_SECTION_CAPACITY 32U

typedef enum {
    SHAKTI_MCP_HANDLER_ASK = 0,
    SHAKTI_MCP_HANDLER_LEARN = 1,
    SHAKTI_MCP_HANDLER_REJECT = 2,
    SHAKTI_MCP_HANDLER_SENSE = 3,
    SHAKTI_MCP_HANDLER_PASS = 4,
    SHAKTI_MCP_HANDLER_SCHOOL = 5,
    SHAKTI_MCP_HANDLER_DRAFT = 6,
    SHAKTI_MCP_HANDLER_TABLET = 7,
    SHAKTI_MCP_HANDLER_MANIFEST = 8,
    SHAKTI_MCP_HANDLER_LOAD = 9,
    SHAKTI_MCP_HANDLER_RECALL = 10,
    SHAKTI_MCP_HANDLER_VALIDATE = 11,
    SHAKTI_MCP_HANDLER_STATUS = 12
} shakti_mcp_handler_id_t;

typedef enum {
    SHAKTI_MCP_ADMIT_OK = 0,
    SHAKTI_MCP_ADMIT_DENIED = 1,
    SHAKTI_MCP_ADMIT_INTERRUPTED = 2,
    SHAKTI_MCP_ADMIT_REFLECTION_BLOCK = 3
} shakti_mcp_admit_t;

typedef struct {
    char name[SHAKTI_MCP_NAME_CAPACITY];
    char menu_section[SHAKTI_MCP_SECTION_CAPACITY];
    unsigned char tyler_enabled;
    shakti_mcp_handler_id_t handler_id;
} shakti_mcp_tool_t;

typedef struct {
    shakti_mcp_tool_t tools[SHAKTI_MCP_MAX_TOOLS];
    size_t tool_count;
    unsigned long approved_receipts;
    /* Last admit message owned by this state (not a shared static). */
    char admit_message[192];
} shakti_mcp_state_t;

void shakti_mcp_init(shakti_mcp_state_t *state);

/*
 * Admit one tool name against the lock four-check.
 * On SHAKTI_MCP_ADMIT_OK, handler_id is set for the caller to dispatch.
 * message is always a stable non-NULL string for the caller to print.
 */
shakti_mcp_admit_t shakti_mcp_admit(
    shakti_mcp_state_t *state,
    const shakti_loop_state_t *loop,
    const char *tool_name,
    shakti_mcp_handler_id_t *handler_id,
    const char **message
);

void shakti_mcp_record_receipt(shakti_mcp_state_t *state);

int shakti_mcp_set_tyler_enabled(
    shakti_mcp_state_t *state,
    const char *tool_name,
    int enabled
);

void shakti_mcp_print_tools(const shakti_mcp_state_t *state);

#endif
