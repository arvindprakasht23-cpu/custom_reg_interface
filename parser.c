#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "axi_regs.h"
#include "../corelogic/api.h"

void parse_and_store(char *input) {
    if (READ_CMD() != 0) return;
    char *cmd = strtok(input, "(");
    if (!cmd) return;
    char *arg_string = strtok(NULL, ")");
    if (!arg_string) return;

    char *token = strtok(arg_string, ",");
    uint32_t offset = 0;

    while (token != NULL) {
        size_t len = strlen(token);
        uint32_t val = (uint32_t)strtoul(token, NULL, 16);

        if (len <= 2) { // 8-bit
            HW_OPERAND_BASE[offset++] = (u8)val;
        } 
        else if (len <= 4) { // 16-bit (Little Endian: LSB First)
            HW_OPERAND_BASE[offset++] = (u8)(val & 0xFF);        // LSB
            HW_OPERAND_BASE[offset++] = (u8)((val >> 8) & 0xFF); // MSB
        } 
        else { // 32-bit (Little Endian: LSB First)
            HW_OPERAND_BASE[offset++] = (u8)(val & 0xFF);
            HW_OPERAND_BASE[offset++] = (u8)((val >> 8) & 0xFF);
            HW_OPERAND_BASE[offset++] = (u8)((val >> 16) & 0xFF);
            HW_OPERAND_BASE[offset++] = (u8)((val >> 24) & 0xFF);
        }
        token = strtok(NULL, ",");
    }

    uint8_t opcode = 0xFF;
    if (strcmp(cmd, "spiRawWrite") == 0)             opcode = OPCODE_RAW_WRITE;
    else if (strcmp(cmd, "spiRawRead") == 0)          opcode = OPCODE_RAW_READ;
    else if (strcmp(cmd, "spiBurstWrite") == 0)       opcode = OPCODE_BURST_WRITE;
    else if (strcmp(cmd, "spiBurstRead") == 0)        opcode = OPCODE_BURST_READ;
    else if (strcmp(cmd, "spiRawWriteMulti") == 0)    opcode = OPCODE_RAW_WRITE_MULTI;
    else if (strcmp(cmd, "spiRawReadMulti") == 0)     opcode = OPCODE_RAW_READ_MULTI;
    else if (strcmp(cmd, "spiBurstWriteMulti") == 0)  opcode = OPCODE_BURST_WRITE_MULTI;

    WRITE_OPCODE(opcode);
    if (opcode != 0xFF) WRITE_CMD(1);
}
