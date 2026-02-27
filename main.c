#include <stdio.h>
#include <string.h>
#include "xil_printf.h"
#include "axi_regs.h"
#include "userlogic/uart.h"
#include "userlogic/parser.h"
#include "corelogic/executor.h"
#include "corelogic/api.h"
#include "corelogic/afe_drivers.h"

#define CMD_BUFFER_SIZE 128

/**
 * @brief Main entry point for the MicroBlaze AFE Control System.
 */
int main()
{
    // 1. Initialize Peripheral Drivers
    uart_init();
    
    xil_printf("--- TI AFE SPI CONTROL SYSTEM READY ---\r\n");
    xil_printf("Format: functionName(arg1,arg2,arg3)\r\n");
    xil_printf("Available Commands:\r\n");
    xil_printf(" - spiRawWrite(afeInst,addr,data)\r\n");
    xil_printf(" - spiRawRead(afeInst,addr)\r\n");
    xil_printf(" - spiBurstWrite(afeInst,addr,size,d1,d2...)\r\n");
    xil_printf(" - spiBurstRead(afeInst,addr,size)\r\n");
    xil_printf(" - spiRawWriteMulti(sel,addr,data)\r\n");
    xil_printf(" - spiRawReadMulti(sel,addr)\r\n");
    xil_printf(" - spiBurstWriteMulti(sel,addr,size,d1,d2...)\r\n");
    xil_printf("----------------------------------------\r\n");

    char cmd_buffer[CMD_BUFFER_SIZE];

    while (1) 
    {
        xil_printf("\r\nCMD> ");
        
        // 2. Wait for user input over UART
        uart_getline(cmd_buffer, CMD_BUFFER_SIZE);
        
        if (strlen(cmd_buffer) == 0) continue;

        // 3. Parse string and pack directly into AXI Hardware (Continuous Memory)
        // This sets REG_COMMAND to 1 (START) upon success
        parse_and_store(cmd_buffer);

        // 4. Run the Executor State Machine
        // This calls the API wrappers and eventually the Drivers
        executor_poll();

        // 5. Check Execution Status
        u16 status = READ_STATUS();
        u8 opcode  = READ_OPCODE();

        if (status == TI_AFE_RET_EXEC_PASS) {
            xil_printf("[MAIN] SUCCESS: Command 0x%02X completed.\r\n", opcode);
            
            // For Read commands, results are sitting in HW_RESULT_BASE
            if (opcode == OPCODE_RAW_READ) {
                xil_printf("   -> Result: 0x%02X\r\n", HW_RESULT_BASE[0]);
            }
        } else {
            xil_printf("[MAIN] ERROR: Execution Failed (Status: 0x%04X)\r\n", status);
        }
    }

    return 0;
}
