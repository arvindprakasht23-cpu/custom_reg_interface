#include "api.h"
#include "axi_regs.h"
#include "afe_drivers.h"

u16 api_afeSpiRawWrite_wrapper(volatile u8 *operands) {
    return (u16)afeSpiRawWrite(operands[0], (operands[1] << 8) | operands[2], operands[3]);
}

u16 api_afeSpiRawRead_wrapper(volatile u8 *operands) {
    return (u16)afeSpiRawRead(operands[0], (operands[1] << 8) | operands[2], (u8 *)HW_RESULT_BASE);
}

u16 api_afeSpiBurstWrite_wrapper(volatile u8 *operands) {
    u16 size = (operands[3] << 8) | operands[4];
    return (u16)afeSpiBurstWrite(operands[0], (operands[1] << 8) | operands[2], (u8 *)&operands[5], size);
}

u16 api_afeSpiBurstRead_wrapper(volatile u8 *operands) {
    u16 size = (operands[3] << 8) | operands[4];
    return (u16)afeSpiBurstRead(operands[0], (operands[1] << 8) | operands[2], size, (u8 *)HW_RESULT_BASE);
}

u16 api_afeSpiRawWriteMulti_wrapper(volatile u8 *operands) {
    return (u16)afeSpiRawWriteMulti(operands[0], (operands[1] << 8) | operands[2], operands[3]);
}

u16 api_afeSpiRawReadMulti_wrapper(volatile u8 *operands) {
    return (u16)afeSpiRawReadMulti(operands[0], (operands[1] << 8) | operands[2], (u8 *)HW_RESULT_BASE);
}

u16 api_afeSpiBurstWriteMulti_wrapper(volatile u8 *operands) {
    u16 size = (operands[3] << 8) | operands[4];
    return (u16)afeSpiBurstWriteMulti(operands[0], (operands[1] << 8) | operands[2], (u8 *)&operands[5], size);
}

api_func_ptr api_table[API_TABLE_SIZE] = {
    api_afeSpiRawWrite_wrapper, api_afeSpiRawRead_wrapper,
    api_afeSpiBurstWrite_wrapper, api_afeSpiBurstRead_wrapper,
    api_afeSpiRawWriteMulti_wrapper, api_afeSpiRawReadMulti_wrapper,
    api_afeSpiBurstWriteMulti_wrapper
};
