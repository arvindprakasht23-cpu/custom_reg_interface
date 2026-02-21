#include "parser.h"
#include "axi_regs.h"
#include "api.h"
#include <string.h>

static u32 parse_hex(const char *s)
{
    u32 val = 0;

    if (!s) return 0;

    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s += 2;

    while (*s)
    {
        val <<= 4;

        if (*s >= '0' && *s <= '9')
            val |= (*s - '0');
        else if (*s >= 'A' && *s <= 'F')
            val |= (*s - 'A' + 10);
        else if (*s >= 'a' && *s <= 'f')
            val |= (*s - 'a' + 10);
        else
            break;

        s++;
    }

    return val;
}

void parse_and_issue(char *cmd)
{
    char *tok = strtok(cmd, " ");
    u32 operands[MAX_OPERANDS];
    int count = 0;

    if (!tok) return;

    u8 opcode = 0xFF;

    if (!strcmp(tok, "READ"))
        opcode = OPCODE_READ;
    else if (!strcmp(tok, "WRITE"))
        opcode = OPCODE_WRITE;
    else if (!strcmp(tok, "ARRAY_WRITE"))
        opcode = OPCODE_ARRAY_WRITE;
    else if (!strcmp(tok, "ARRAY_READ"))
        opcode = OPCODE_ARRAY_READ;

    if (opcode == 0xFF)
        return;

    while ((tok = strtok(NULL, " ")) && count < MAX_OPERANDS)
        operands[count++] = parse_hex(tok);

    for (int i = 0; i < count; i++)
        WRITE32(REG_OPERAND(i), operands[i]);

    WRITE8(REG_OPCODE_ADDR, opcode);

    /* Trigger execution */
    WRITE8(REG_COMMAND_ADDR, CMD_START);
}
