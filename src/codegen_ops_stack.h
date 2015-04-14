static uint32_t ropPUSH_16(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(-2);
        host_reg = LOAD_REG_W(opcode & 7);
        MEM_STORE_ADDR_EA_W(&_ss, host_reg);
        SP_MODIFY(-2);
        
        return op_pc;
}
static uint32_t ropPUSH_32(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(-4);
        host_reg = LOAD_REG_L(opcode & 7);
        MEM_STORE_ADDR_EA_L(&_ss, host_reg);
        SP_MODIFY(-4);

        return op_pc;
}

static uint32_t ropPUSH_imm_16(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        uint16_t imm = fetchdat & 0xffff;
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(-2);
        host_reg = LOAD_REG_IMM(imm);
        MEM_STORE_ADDR_EA_W(&_ss, host_reg);
        SP_MODIFY(-2);
        
        return op_pc+2;
}
static uint32_t ropPUSH_imm_32(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        uint32_t imm = fastreadl(cs + op_pc);
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(-4);
        host_reg = LOAD_REG_IMM(imm);
        MEM_STORE_ADDR_EA_L(&_ss, host_reg);
        SP_MODIFY(-4);
        
        return op_pc+4;
}

static uint32_t ropPUSH_imm_b16(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        uint16_t imm = fetchdat & 0xff;
        int host_reg;

        if (imm & 0x80)
                imm |= 0xff00;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(-2);
        host_reg = LOAD_REG_IMM(imm);
        MEM_STORE_ADDR_EA_W(&_ss, host_reg);
        SP_MODIFY(-2);
        
        return op_pc+1;
}
static uint32_t ropPUSH_imm_b32(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        uint32_t imm = fetchdat & 0xff;
        int host_reg;

        if (imm & 0x80)
                imm |= 0xffffff00;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(-4);
        host_reg = LOAD_REG_IMM(imm);
        MEM_STORE_ADDR_EA_L(&_ss, host_reg);
        SP_MODIFY(-4);
        
        return op_pc+1;
}

static uint32_t ropPOP_16(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(0);
        MEM_LOAD_ADDR_EA_W(&_ss);
        SP_MODIFY(2);
        STORE_REG_TARGET_W_RELEASE(0, opcode & 7);
        
        return op_pc;
}
static uint32_t ropPOP_32(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(0);
        MEM_LOAD_ADDR_EA_L(&_ss);
        SP_MODIFY(4);
        STORE_REG_TARGET_L_RELEASE(0, opcode & 7);
        
        return op_pc;
}

static uint32_t ropRET_16(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(0);
        MEM_LOAD_ADDR_EA_W(&_ss);
        STORE_HOST_REG_ADDR((uintptr_t)&pc, 0);
        SP_MODIFY(2);
        
        return -1;
}
static uint32_t ropRET_32(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(0);
        MEM_LOAD_ADDR_EA_L(&_ss);
        STORE_HOST_REG_ADDR((uintptr_t)&pc, 0);
        SP_MODIFY(4);
        
        return -1;
}

static uint32_t ropRET_imm_16(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        uint16_t offset = fetchdat & 0xffff;
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(0);
        MEM_LOAD_ADDR_EA_W(&_ss);
        STORE_HOST_REG_ADDR((uintptr_t)&pc, 0);
        SP_MODIFY(2+offset);
        
        return -1;
}
static uint32_t ropRET_imm_32(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        uint16_t offset = fetchdat & 0xffff;
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(0);
        MEM_LOAD_ADDR_EA_L(&_ss);
        STORE_HOST_REG_ADDR((uintptr_t)&pc, 0);
        SP_MODIFY(4+offset);
        
        return -1;
}

static uint32_t ropCALL_r16(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        uint16_t offset = fetchdat & 0xffff;
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(-2);
        host_reg = LOAD_REG_IMM(op_pc+2);
        MEM_STORE_ADDR_EA_W(&_ss, host_reg);
        SP_MODIFY(-2);
        STORE_IMM_ADDR_L((uintptr_t)&pc, (op_pc+2+offset) & 0xffff);
        
        return -1;
}
static uint32_t ropCALL_r32(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        uint32_t offset = fastreadl(cs + op_pc);
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_STACK_TO_EA(-4);
        host_reg = LOAD_REG_IMM(op_pc+4);
        MEM_STORE_ADDR_EA_L(&_ss, host_reg);
        SP_MODIFY(-4);
        STORE_IMM_ADDR_L((uintptr_t)&pc, op_pc+4+offset);
        
        return -1;
}

static uint32_t ropLEAVE_16(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_EBP_TO_EA(0);
        MEM_LOAD_ADDR_EA_W(&_ss);
        host_reg = LOAD_REG_W(REG_BP); /*SP = BP + 2*/
        STORE_REG_TARGET_W_RELEASE(0, REG_BP); /*BP = POP_W()*/
        ADD_HOST_REG_IMM_W(host_reg, 2);
        STORE_REG_TARGET_W_RELEASE(host_reg, REG_SP);

        return op_pc;
}
static uint32_t ropLEAVE_32(uint8_t opcode, uint32_t fetchdat, uint32_t op_32, uint32_t op_pc, codeblock_t *block)
{
        int host_reg;

        STORE_IMM_ADDR_L((uintptr_t)&oldpc, op_old_pc);
        LOAD_EBP_TO_EA(0);
        MEM_LOAD_ADDR_EA_L(&_ss);
        host_reg = LOAD_REG_L(REG_EBP); /*ESP = EBP + 4*/
        STORE_REG_TARGET_L_RELEASE(0, REG_EBP); /*EBP = POP_L()*/
        ADD_HOST_REG_IMM(host_reg, 4);
        STORE_REG_TARGET_L_RELEASE(host_reg, REG_ESP);

        return op_pc;
}
