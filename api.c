#include "api.h"
#include "axi_regs.h"
#include "afe_drivers.h"

// -----------------------------------------------------------------------------
// Wrapper Functions (Extracting Little Endian widths directly from HW memory)
// -----------------------------------------------------------------------------

/**
 * @brief Raw Write Wrapper
 * Driver signature: uint32_t afeSpiRawWrite(uint8_t afeInst, uint16_t addr, uint8_t data)
 */
u16 api_afeSpiRawWrite_wrapper(volatile u8 *operands) {
    uint8_t  afeInst = operands[0];
    
    // Extract 16-bit address (Little Endian: LSB at index 1, MSB at index 2)
    uint16_t addr    = (uint16_t)(operands[1] & 0xFF) | 
                       (uint16_t)((operands[2] & 0xFF) << 8);
                       
    uint8_t  data    = operands[3];
    
    return (u16)afeSpiRawWrite(afeInst, addr, data);
}

/**
 * @brief Raw Read Wrapper
 * Driver signature: uint32_t afeSpiRawRead(uint8_t afeInst, uint16_t addr, uint8_t *readVal)
 */
u16 api_afeSpiRawRead_wrapper(volatile u8 *operands) {
    uint8_t  afeInst = operands[0];
    
    uint16_t addr    = (uint16_t)(operands[1] & 0xFF) | 
                       (uint16_t)((operands[2] & 0xFF) << 8);
    
    // Route the read directly into the hardware result register
    return (u16)afeSpiRawRead(afeInst, addr, (uint8_t *)HW_RESULT_BASE);
}

/**
 * @brief Burst Write Wrapper
 * Driver signature: uint32_t afeSpiBurstWrite(uint8_t afeInst, uint16_t addr, uint8_t *data, uint16_t dataArraySize)
 */
u16 api_afeSpiBurstWrite_wrapper(volatile u8 *operands) {
    uint8_t  afeInst = operands[0];
    
    uint16_t addr    = (uint16_t)(operands[1] & 0xFF) | 
                       (uint16_t)((operands[2] & 0xFF) << 8);
                       
    // Extract 16-bit size (Little Endian: LSB at index 3, MSB at index 4)
    uint16_t size    = (uint16_t)(operands[3] & 0xFF) | 
                       (uint16_t)((operands[4] & 0xFF) << 8);
    
    // Pass the remaining operand buffer (starting at index 5) for the burst payload
    return (u16)afeSpiBurstWrite(afeInst, addr, (uint8_t *)&operands[5], size);
}

/**
 * @brief Burst Read Wrapper
 * Driver signature: uint32_t afeSpiBurstRead(uint8_t afeInst, uint16_t addr, uint16_t dataArraySize, uint8_t *data)
 */
u16 api_afeSpiBurstRead_wrapper(volatile u8 *operands) {
    uint8_t  afeInst = operands[0];
    
    uint16_t addr    = (uint16_t)(operands[1] & 0xFF) | 
                       (uint16_t)((operands[2] & 0xFF) << 8);
                       
    uint16_t size    = (uint16_t)(operands[3] & 0xFF) | 
                       (uint16_t)((operands[4] & 0xFF) << 8);
    
    // Route the burst read directly into the contiguous result space
    return (u16)afeSpiBurstRead(afeInst, addr, size, (uint8_t *)HW_RESULT_BASE);
}

/**
 * @brief Raw Write Multi Wrapper
 * Driver signature: uint32_t afeSpiRawWriteMulti(uint8_t afeInstSel, uint16_t addr, uint8_t data)
 */
u16 api_afeSpiRawWriteMulti_wrapper(volatile u8 *operands) {
    uint8_t  sel  = operands[0];
    
    uint16_t addr = (uint16_t)(operands[1] & 0xFF) | 
                    (uint16_t)((operands[2] & 0xFF) << 8);
                    
    uint8_t  data = operands[3];
    
    return (u16)afeSpiRawWriteMulti(sel, addr, data);
}

/**
 * @brief Raw Read Multi Wrapper
 * Driver signature: uint32_t afeSpiRawReadMulti(uint8_t afeInstSel, uint16_t addr, uint8_t *readVal)
 */
u16 api_afeSpiRawReadMulti_wrapper(volatile u8 *operands) {
    uint8_t  sel  = operands[0];
    
    uint16_t addr = (uint16_t)(operands[1] & 0xFF) | 
                    (uint16_t)((operands[2] & 0xFF) << 8);
    
    // HW_RESULT_BASE will be populated with NUM_SPI bytes
    return (u16)afeSpiRawReadMulti(sel, addr, (uint8_t *)HW_RESULT_BASE);
}

/**
 * @brief Burst Write Multi Wrapper
 * Driver signature: uint32_t afeSpiBurstWriteMulti(uint8_t afeInstSel, uint16_t addr, uint8_t *data, uint16_t dataArraySize)
 */
u16 api_afeSpiBurstWriteMulti_wrapper(volatile u8 *operands) {
    uint8_t  sel  = operands[0];
    
    uint16_t addr = (uint16_t)(operands[1] & 0xFF) | 
                    (uint16_t)((operands[2] & 0xFF) << 8);
                    
    uint16_t size = (uint16_t)(operands[3] & 0xFF) | 
                    (uint16_t)((operands[4] & 0xFF) << 8);
    
    return (u16)afeSpiBurstWriteMulti(sel, addr, (uint8_t *)&operands[5], size);
}

// -----------------------------------------------------------------------------
// API Dispatch Table
// -----------------------------------------------------------------------------
/**
 * @brief Dispatch Table mapping Opcodes to Wrapper Functions.
 * The index MUST correspond exactly to the opcode_t enum in api.h.
 */
api_func_ptr api_table[API_TABLE_SIZE] = {
    api_afeSpiRawWrite_wrapper,       // 0x00: OPCODE_RAW_WRITE
    api_afeSpiRawRead_wrapper,        // 0x01: OPCODE_RAW_READ
    api_afeSpiBurstWrite_wrapper,     // 0x02: OPCODE_BURST_WRITE
    api_afeSpiBurstRead_wrapper,      // 0x03: OPCODE_BURST_READ
    api_afeSpiRawWriteMulti_wrapper,  // 0x04: OPCODE_RAW_WRITE_MULTI
    api_afeSpiRawReadMulti_wrapper,   // 0x05: OPCODE_RAW_READ_MULTI
    api_afeSpiBurstWriteMulti_wrapper // 0x06: OPCODE_BURST_WRITE_MULTI
};
