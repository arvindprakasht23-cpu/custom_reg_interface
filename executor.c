#include "executor.h"
#include "axi_regs.h"
#include "api.h"

void executor_poll(void)
{
    u8 command = READ8(REG_COMMAND_ADDR);

    if (command == CMD_START)
    {
        /* Mark busy */
        WRITE8(REG_COMMAND_ADDR, CMD_BUSY);

        u8 opcode = READ8(REG_OPCODE_ADDR);
        u16 status = 0xFFFF;

        if (opcode < API_TABLE_SIZE && api_table[opcode])
        {
            status = api_table[opcode]();
        }

        /* Write status */
        WRITE16(REG_STATUS_ADDR, status);

        /* Back to idle */
        WRITE8(REG_COMMAND_ADDR, CMD_IDLE);
    }
}
