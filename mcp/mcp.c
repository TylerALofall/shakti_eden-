#include "mcp.h"

#include <stdio.h>
#include <string.h>

#include "shakti_config.h"
#include "shakti_loop.h"

typedef struct {
    const char *name;
    const char *menu_section;
    unsigned char tyler_enabled;
    shakti_mcp_handler_id_t handler_id;
} shakti_mcp_default_t;

static const shakti_mcp_default_t MCP_DEFAULTS[] = {
    { "ask", "Reasoning", 1U, SHAKTI_MCP_HANDLER_ASK },
    { "learn", "Reasoning", 1U, SHAKTI_MCP_HANDLER_LEARN },
    { "reject", "Reasoning", 1U, SHAKTI_MCP_HANDLER_REJECT },
    { "sense", "Senses", 1U, SHAKTI_MCP_HANDLER_SENSE },
    { "pass", "School", 1U, SHAKTI_MCP_HANDLER_PASS },
    { "school", "School", 1U, SHAKTI_MCP_HANDLER_SCHOOL },
    { "draft", "School", 1U, SHAKTI_MCP_HANDLER_DRAFT },
    { "tablet", "Tablet", 1U, SHAKTI_MCP_HANDLER_TABLET },
    { "manifest", "Tablet", 1U, SHAKTI_MCP_HANDLER_MANIFEST },
    { "load", "Senses", 1U, SHAKTI_MCP_HANDLER_LOAD },
    { "recall", "Memory", 1U, SHAKTI_MCP_HANDLER_RECALL },
    { "validate", "Control", 1U, SHAKTI_MCP_HANDLER_VALIDATE },
    { "status", "Control", 1U, SHAKTI_MCP_HANDLER_STATUS }
};

static void copy_bounded(
    char *destination,
    size_t destination_size,
    const char *source
)
{
    size_t index;

    if (destination == NULL || destination_size == 0U) {
        return;
    }

    if (source == NULL) {
        destination[0] = '\0';
        return;
    }

    for (index = 0U;
         index + 1U < destination_size && source[index] != '\0';
         ++index) {
        destination[index] = source[index];
    }

    destination[index] = '\0';
}

static shakti_mcp_tool_t *find_tool(
    shakti_mcp_state_t *state,
    const char *tool_name
)
{
    size_t index;

    if (state == NULL || tool_name == NULL || tool_name[0] == '\0') {
        return NULL;
    }

    for (index = 0U; index < state->tool_count; ++index) {
        if (strcmp(state->tools[index].name, tool_name) == 0) {
            return &state->tools[index];
        }
    }

    return NULL;
}

static int menu_has_section(
    const shakti_loop_state_t *loop,
    const char *section
)
{
    size_t index;

    if (loop == NULL || section == NULL || section[0] == '\0') {
        return 0;
    }

    for (index = 0U; index < loop->menu_section_count; ++index) {
        if (strcmp(loop->menu_sections[index].title, section) == 0) {
            return 1;
        }
    }

    return 0;
}

void shakti_mcp_init(shakti_mcp_state_t *state)
{
    size_t index;
    size_t count;

    if (state == NULL) {
        return;
    }

    memset(state, 0, sizeof(*state));
    count = sizeof(MCP_DEFAULTS) / sizeof(MCP_DEFAULTS[0]);

    if (count > SHAKTI_MCP_MAX_TOOLS) {
        count = SHAKTI_MCP_MAX_TOOLS;
    }

    for (index = 0U; index < count; ++index) {
        copy_bounded(
            state->tools[index].name,
            sizeof(state->tools[index].name),
            MCP_DEFAULTS[index].name
        );
        copy_bounded(
            state->tools[index].menu_section,
            sizeof(state->tools[index].menu_section),
            MCP_DEFAULTS[index].menu_section
        );
        state->tools[index].tyler_enabled =
            MCP_DEFAULTS[index].tyler_enabled;
        state->tools[index].handler_id = MCP_DEFAULTS[index].handler_id;
    }

    state->tool_count = count;
    state->approved_receipts = 0UL;
}

shakti_mcp_admit_t shakti_mcp_admit(
    shakti_mcp_state_t *state,
    const shakti_loop_state_t *loop,
    const char *tool_name,
    shakti_mcp_handler_id_t *handler_id,
    const char **message
)
{
    shakti_mcp_tool_t *tool;

    if (message != NULL) {
        *message = "DENIED";
    }

    if (handler_id != NULL) {
        *handler_id = SHAKTI_MCP_HANDLER_ASK;
    }

    if (state == NULL || loop == NULL || tool_name == NULL) {
        if (message != NULL) {
            *message = "DENIED";
        }

        return SHAKTI_MCP_ADMIT_DENIED;
    }

    if (!shakti_loop_tools_available(loop)) {
        if (message != NULL) {
            *message =
                "MCP tools are interrupted. Shakti remains awake. "
                "Use /resume/ to restore tool calls.";
        }

        return SHAKTI_MCP_ADMIT_INTERRUPTED;
    }

    if (loop->reflection_due &&
        (loop->reflection_deferrals >= SHAKTI_REFLECTION_MAX_DEFERRALS ||
         loop->turns_since_reflection >=
             (SHAKTI_REFLECTION_INTERVAL +
              SHAKTI_REFLECTION_MAX_DEFERRALS))) {
        if (loop->reflection_deferrals >=
            SHAKTI_REFLECTION_MAX_DEFERRALS) {
            (void)snprintf(
                state->admit_message,
                sizeof(state->admit_message),
                "Reflection is required before tool call %u. "
                "All %u deferrals used after tool call %u. "
                "Complete /reflection/ now. Tool call %u is blocked.",
                (unsigned int)SHAKTI_REFLECTION_HARD_TOOL_CALL,
                (unsigned int)SHAKTI_REFLECTION_MAX_DEFERRALS,
                (unsigned int)SHAKTI_REFLECTION_INTERVAL,
                (unsigned int)SHAKTI_REFLECTION_HARD_TOOL_CALL
            );
        } else {
            (void)snprintf(
                state->admit_message,
                sizeof(state->admit_message),
                "Reflection is required before tool call %u. "
                "Approved tool-call count is %u (gate: due at %u, "
                "defer %u-%u, required before %u). "
                "Complete /reflection/ now. Tool call %u is blocked.",
                (unsigned int)SHAKTI_REFLECTION_HARD_TOOL_CALL,
                loop->turns_since_reflection,
                (unsigned int)SHAKTI_REFLECTION_INTERVAL,
                (unsigned int)(SHAKTI_REFLECTION_INTERVAL + 1U),
                (unsigned int)(SHAKTI_REFLECTION_INTERVAL +
                               SHAKTI_REFLECTION_MAX_DEFERRALS),
                (unsigned int)SHAKTI_REFLECTION_HARD_TOOL_CALL,
                (unsigned int)SHAKTI_REFLECTION_HARD_TOOL_CALL
            );
        }

        if (message != NULL) {
            *message = state->admit_message;
        }

        return SHAKTI_MCP_ADMIT_REFLECTION_BLOCK;
    }

    tool = find_tool(state, tool_name);

    if (tool == NULL) {
        if (message != NULL) {
            *message = "DENIED";
        }

        return SHAKTI_MCP_ADMIT_DENIED;
    }

    if (!menu_has_section(loop, tool->menu_section)) {
        if (message != NULL) {
            *message = "DENIED";
        }

        return SHAKTI_MCP_ADMIT_DENIED;
    }

    if (!tool->tyler_enabled) {
        if (message != NULL) {
            *message = "DENIED";
        }

        return SHAKTI_MCP_ADMIT_DENIED;
    }

    if (handler_id != NULL) {
        *handler_id = tool->handler_id;
    }

    if (message != NULL) {
        *message = "OK";
    }

    return SHAKTI_MCP_ADMIT_OK;
}

void shakti_mcp_record_receipt(shakti_mcp_state_t *state)
{
    if (state == NULL) {
        return;
    }

    state->approved_receipts++;
}

int shakti_mcp_set_tyler_enabled(
    shakti_mcp_state_t *state,
    const char *tool_name,
    int enabled
)
{
    shakti_mcp_tool_t *tool;

    tool = find_tool(state, tool_name);

    if (tool == NULL) {
        return 0;
    }

    tool->tyler_enabled = enabled ? 1U : 0U;

    return 1;
}

void shakti_mcp_print_tools(const shakti_mcp_state_t *state)
{
    size_t index;

    if (state == NULL) {
        return;
    }

    puts("MCP tools:");

    for (index = 0U; index < state->tool_count; ++index) {
        printf(
            "%s [%s]%s\n",
            state->tools[index].name,
            state->tools[index].menu_section,
            state->tools[index].tyler_enabled ? "" : " (disabled)"
        );
    }
}
