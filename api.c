#include "api.h"
#include "axi_regs.h"

/* READ */
u16 api_read(void)
{
    u32 addr = READ32(REG_OPERAND(0));

    /* Dummy: echo address */
    WRITE32(REG_RESULT(0), addr);

    return 0;
}

/* WRITE */
u16 api_write(void)
{
    u32 addr = READ32(REG_OPERAND(0));
    u32 data = READ32(REG_OPERAND(1));

    WRITE32(REG_RESULT(0), data);

    return 0;
}

/* ARRAY WRITE */
u16 api_array_write(void)
{
    u32 count = READ32(REG_OPERAND(1));

    for (int i = 0; i < count && i < MAX_RESULTS; i++)
    {
        u32 val = READ32(REG_OPERAND(i + 2));
        WRITE32(REG_RESULT(i), val);
    }

    return 0;
}

/* ARRAY READ */
u16 api_array_read(void)
{
    u32 count = READ32(REG_OPERAND(1));

    for (int i = 0; i < count && i < MAX_RESULTS; i++)
    {
        WRITE32(REG_RESULT(i), i);
    }

    return 0;
}

/* Function table */
api_func_ptr api_table[API_TABLE_SIZE] =
{
    api_read,
    api_write,
    api_array_write,
    api_array_read
};
