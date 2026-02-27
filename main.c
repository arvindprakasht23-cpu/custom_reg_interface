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

// -----------------------------------------------------------------------------
// Little Endian Hardware Extraction Macros (Using Safe Masking)
// -----------------------------------------------------------------------------
#define GET_HW_U16(base, offset) (uint16_t)( \
    (base[offset] & 0xFF) | \
    ((base[offset + 1] & 0xFF) << 8) \
)

#define GET_HW_U32(base, offset) (uint32_t)( \
    (base[offset] & 0xFF) | \
    ((base[offset + 1] & 0xFF) << 8) | \
    ((base[offset + 2] & 0xFF) << 16) | \
    ((base[offset + 3] & 0xFF) << 24) \
)

int main()
{
    // 1. Initialize Peripherals
    uart_init();
    
    xil_printf("\r\n================================================\r\n");
    xil_printf("   TI AFE SPI CONTROL SYSTEM (LITTLE ENDIAN)    \r\n");
    xil_printf("================================================\r\n");
    xil_printf("Ready for commands. Example: spiRawWrite(01,00A0,FF)\r\n");

    char cmd_buffer[CMD_BUFFER_SIZE];

    // 2. Main Application Loop
    while (1) 
    {
        xil_printf("\r\nCMD> ");
        
        // Wait for terminal input
        uart_getline(cmd_buffer, CMD_BUFFER_SIZE);
        if (strlen(cmd_buffer) == 0) continue;

        // Pack the string directly into AXI memory and set CMD to START
        parse_and_store(cmd_buffer);

        // Run the state machine to execute the command via wrappers
        executor_poll();

        // 3. Process the Results
        u16 status = READ_STATUS();
        u8 opcode  = READ_OPCODE();

        if (status == TI_AFE_RET_EXEC_PASS) {
            xil_printf("[MAIN] SUCCESS: Opcode 0x%02X completed.\r\n", opcode);
            
            // Depending on the command, extract the results from HW_RESULT_BASE
            if (opcode == OPCODE_RAW_READ) {
                // Returns a single 8-bit value at index 0
                xil_printf("   -> Result: 0x%02X\r\n", HW_RESULT_BASE[0]);
            }
            else if (opcode == OPCODE_RAW_READ_MULTI) {
                // Returns an array of NUM_SPI (4) 8-bit values
                xil_printf("   -> Multi-Read Results:\r\n");
                for (int i = 0; i < NUM_SPI; i++) {
                    xil_printf("      SPI[%d]: 0x%02X\r\n", i, HW_RESULT_BASE[i]);
                }
            }
            else if (opcode == OPCODE_BURST_READ) {
                // To know how many bytes to read, we look back at the original size
                // parameter packed into HW_OPERAND_BASE at indices 3 and 4
                uint16_t size = GET_HW_U16(HW_OPERAND_BASE, 3);
                
                xil_printf("   -> Burst Read Data (%d bytes):\r\n", size);
                for (uint16_t i = 0; i < size; i++) {
                    xil_printf("      Data[%d]: 0x%02X\r\n", i, HW_RESULT_BASE[i]);
                }
            }
            // Note: If you ever implement 32-bit sensor reads, you would use:
            // uint32_t sensor_val = GET_HW_U32(HW_RESULT_BASE, 0);

        } else if (opcode != 0xFF) {
            // Opcode 0xFF means the parser didn't recognize the command at all
            xil_printf("[MAIN] ERROR: Execution Failed (Status: 0x%04X)\r\n", status);
        } else {
            xil_printf("[MAIN] ERROR: Unknown Command Syntax.\r\n");
        }
    }

    return 0;
}
