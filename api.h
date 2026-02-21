#ifndef API_H
#define API_H

#include "xil_types.h"

typedef u16 (*api_func_ptr)(void);

typedef enum {
    OPCODE_READ = 0,
    OPCODE_WRITE,
    OPCODE_ARRAY_WRITE,
    OPCODE_ARRAY_READ,
    API_TABLE_SIZE
} opcode_t;

extern api_func_ptr api_table[API_TABLE_SIZE];

#endif
