#include "uart_driver.h"
#include "parser.h"
#include "executor.h"
#include "axi_regs.h"
#include "xil_printf.h"

int main(void)
{
    char cmd_buf[128];

    uart_init();

    WRITE8(REG_COMMAND_ADDR, CMD_IDLE);

    while (1)
    {
        xil_printf("\r\n>> ");
        uart_getline(cmd_buf, sizeof(cmd_buf));

        parse_and_issue(cmd_buf);

        while (READ8(REG_COMMAND_ADDR) != CMD_IDLE)
        {
            executor_poll();
        }

        u16 status = READ16(REG_STATUS_ADDR);
        xil_printf("Status = 0x%04X\r\n", status);

        u32 result = READ32(REG_RESULT(0));
        xil_printf("Result[0] = 0x%08X\r\n", result);
    }

    return 0;
}
