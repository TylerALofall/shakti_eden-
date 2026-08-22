#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "mcp.h"
#include "shakti_loop.h"
#include "shakti_types.h"

static void load_menu(shakti_loop_state_t *loop)
{
    shakti_loop_init(loop);
    assert(shakti_loop_load(
        loop,
        "data/control/goal.txt",
        "data/control/notebook.log",
        "data/control/menu.txt"
    ));
    assert(loop->menu_section_count >= 1U);
}

static void test_unknown_denied(void)
{
    shakti_mcp_state_t mcp;
    shakti_loop_state_t loop;
    shakti_mcp_handler_id_t handler;
    const char *message;
    shakti_mcp_admit_t admit;

    shakti_mcp_init(&mcp);
    load_menu(&loop);

    admit = shakti_mcp_admit(
        &mcp,
        &loop,
        "not_a_tool",
        &handler,
        &message
    );
    assert(admit == SHAKTI_MCP_ADMIT_DENIED);
    assert(strcmp(message, "DENIED") == 0);
    assert(mcp.approved_receipts == 0UL);
}

static void test_disabled_denied(void)
{
    shakti_mcp_state_t mcp;
    shakti_loop_state_t loop;
    shakti_mcp_handler_id_t handler;
    const char *message;
    shakti_mcp_admit_t admit;

    shakti_mcp_init(&mcp);
    load_menu(&loop);

    /* Prove menu would allow "ask" before disabling Tyler enable. */
    admit = shakti_mcp_admit(
        &mcp,
        &loop,
        "ask",
        &handler,
        &message
    );
    assert(admit == SHAKTI_MCP_ADMIT_OK);

    assert(shakti_mcp_set_tyler_enabled(&mcp, "ask", 0));
    admit = shakti_mcp_admit(
        &mcp,
        &loop,
        "ask",
        &handler,
        &message
    );
    assert(admit == SHAKTI_MCP_ADMIT_DENIED);
    assert(strcmp(message, "DENIED") == 0);
    assert(mcp.approved_receipts == 0UL);
}

static void test_missing_menu_denied(void)
{
    shakti_mcp_state_t mcp;
    shakti_loop_state_t loop;
    shakti_mcp_handler_id_t handler;
    const char *message;
    shakti_mcp_admit_t admit;

    shakti_mcp_init(&mcp);
    shakti_loop_init(&loop);
    loop.menu_section_count = 0U;

    admit = shakti_mcp_admit(
        &mcp,
        &loop,
        "ask",
        &handler,
        &message
    );
    assert(admit == SHAKTI_MCP_ADMIT_DENIED);
    assert(strcmp(message, "DENIED") == 0);
}

static void test_interrupted_blocked(void)
{
    shakti_mcp_state_t mcp;
    shakti_loop_state_t loop;
    shakti_mcp_handler_id_t handler;
    const char *message;
    shakti_mcp_admit_t admit;

    shakti_mcp_init(&mcp);
    load_menu(&loop);
    shakti_loop_interrupt_tools(&loop);

    admit = shakti_mcp_admit(
        &mcp,
        &loop,
        "status",
        &handler,
        &message
    );
    assert(admit == SHAKTI_MCP_ADMIT_INTERRUPTED);
    assert(mcp.approved_receipts == 0UL);

    shakti_loop_resume_tools(&loop);
    admit = shakti_mcp_admit(
        &mcp,
        &loop,
        "status",
        &handler,
        &message
    );
    assert(admit == SHAKTI_MCP_ADMIT_OK);
    assert(handler == SHAKTI_MCP_HANDLER_STATUS);
}

static void test_allow_and_receipt(void)
{
    shakti_mcp_state_t mcp;
    shakti_loop_state_t loop;
    shakti_mcp_handler_id_t handler;
    const char *message;
    shakti_mcp_admit_t admit;

    shakti_mcp_init(&mcp);
    load_menu(&loop);

    admit = shakti_mcp_admit(
        &mcp,
        &loop,
        "recall",
        &handler,
        &message
    );
    assert(admit == SHAKTI_MCP_ADMIT_OK);
    assert(handler == SHAKTI_MCP_HANDLER_RECALL);
    assert(strcmp(message, "OK") == 0);

    shakti_mcp_record_receipt(&mcp);
    assert(mcp.approved_receipts == 1UL);

    admit = shakti_mcp_admit(
        &mcp,
        &loop,
        "ask",
        &handler,
        &message
    );
    assert(admit == SHAKTI_MCP_ADMIT_OK);
    assert(handler == SHAKTI_MCP_HANDLER_ASK);
    shakti_mcp_record_receipt(&mcp);
    assert(mcp.approved_receipts == 2UL);
}

static void test_reflection_hard_block(void)
{
    shakti_mcp_state_t mcp;
    shakti_loop_state_t loop;
    shakti_mcp_handler_id_t handler;
    const char *message;
    shakti_mcp_admit_t admit;

    shakti_mcp_init(&mcp);
    load_menu(&loop);
    loop.reflection_due = 1U;
    loop.reflection_deferrals = 3U;

    admit = shakti_mcp_admit(
        &mcp,
        &loop,
        "ask",
        &handler,
        &message
    );
    assert(admit == SHAKTI_MCP_ADMIT_REFLECTION_BLOCK);
    assert(mcp.approved_receipts == 0UL);

    shakti_mcp_init(&mcp);
    load_menu(&loop);
    loop.reflection_due = 1U;
    loop.reflection_deferrals = 0U;
    loop.turns_since_reflection =
        SHAKTI_REFLECTION_INTERVAL + SHAKTI_REFLECTION_MAX_DEFERRALS;

    admit = shakti_mcp_admit(
        &mcp,
        &loop,
        "ask",
        &handler,
        &message
    );
    assert(admit == SHAKTI_MCP_ADMIT_REFLECTION_BLOCK);
    assert(strstr(message, "Reflection is required before tool call") != NULL);
    assert(strstr(message, "tool call 14") != NULL ||
           strstr(message, "Tool call 14") != NULL);
    assert(mcp.approved_receipts == 0UL);
}

int main(void)
{
    test_unknown_denied();
    test_disabled_denied();
    test_missing_menu_denied();
    test_interrupted_blocked();
    test_allow_and_receipt();
    test_reflection_hard_block();

    puts("All MCP gate tests passed.");

    return 0;
}
