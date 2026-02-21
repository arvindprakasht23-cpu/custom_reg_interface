#ifndef AXI_REGS_H
#define AXI_REGS_H

#include "xparameters.h"
#include "xil_io.h"
#include "xil_types.h"

#define CMD_BASE_ADDR   XPAR_MY_REG_IP_0_S00_AXI_BASEADDR

/* ---------------- Operands (0x00 – 0x1F) ---------------- */
/* 8 × 32-bit */
#define REG_OPERAND(n)   (CMD_BASE_ADDR + (0x00 + ((n) * 4)))
#define MAX_OPERANDS     8

/* ---------------- Control Bytes ---------------- */
/* 0x20–0x23 (inside one 32-bit register) */
#define REG_OPCODE_ADDR   (CMD_BASE_ADDR + 0x20)   // 8-bit
#define REG_COMMAND_ADDR  (CMD_BASE_ADDR + 0x21)   // 8-bit
#define REG_STATUS_ADDR   (CMD_BASE_ADDR + 0x22)   // 16-bit (0x22–0x23)

/* ---------------- Results (0x24 – 0x43) ---------------- */
/* 8 × 32-bit */
#define REG_RESULT(n)     (CMD_BASE_ADDR + (0x24 + ((n) * 4)))
#define MAX_RESULTS       8

/* Command values */
#define CMD_IDLE   0
#define CMD_START  1
#define CMD_BUSY   2

/* Access macros */
#define WRITE32(addr, val)  Xil_Out32((addr), (u32)(val))
#define READ32(addr)        Xil_In32((addr))

#define WRITE8(addr, val)   Xil_Out8((addr), (u8)(val))
#define READ8(addr)         Xil_In8((addr))

#define WRITE16(addr, val)  Xil_Out16((addr), (u16)(val))
#define READ16(addr)        Xil_In16((addr))

#endif
