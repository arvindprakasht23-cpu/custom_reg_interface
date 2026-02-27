#ifndef AXI_REGS_H
#define AXI_REGS_H

#include "xparameters.h"
#include "xil_types.h"
#include "xil_io.h"

#define REG_BASE_ADDR  XPAR_REG_32IP_0_SREG_AXI_BASEADDR

// --- Continuous Byte-Stream Pointers (Zero Gaps) ---
#define HW_OPERAND_BASE ((volatile u8 *)(REG_BASE_ADDR + 0x00)) // 0x00 to 0x1F
#define HW_RESULT_BASE  ((volatile u8 *)(REG_BASE_ADDR + 0x24)) // 0x24 to 0x43

// --- Exact Control Offsets ---
#define REG_OPCODE      0x20 // 1 byte
#define REG_COMMAND     0x21 // 1 byte
#define REG_STATUS      0x22 // 2 bytes (0x22-0x23)

// --- Control Register Macros ---
#define WRITE_OPCODE(val)  Xil_Out8(REG_BASE_ADDR + REG_OPCODE, (u8)(val))
#define READ_OPCODE()      Xil_In8(REG_BASE_ADDR + REG_OPCODE)
#define WRITE_CMD(val)     Xil_Out8(REG_BASE_ADDR + REG_COMMAND, (u8)(val))
#define READ_CMD()         Xil_In8(REG_BASE_ADDR + REG_COMMAND)
#define WRITE_STATUS(val)  Xil_Out16(REG_BASE_ADDR + REG_STATUS, (u16)(val))
#define READ_STATUS()      Xil_In16(REG_BASE_ADDR + REG_STATUS)

#endif
