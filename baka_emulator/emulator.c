#include "emulator.h"

emulator* create_emulator()
{
    emulator* emu; 
    emu = malloc(sizeof(emulator));
    emu->pc = 0x000;
    emu->opcode = 0;
    emu->cmp = 0;
    emu->bin_size = 0;
    emu->running = 1;

    // clear memory
    for(int i=0;i<sizeof(emu->memory);i++)
    {
        emu->memory[i] = 0;
    }

    // clear registers
    for(int i=0;i<sizeof(emu->r);i++)
    {
        emu->r[i] = 0;
    }

    // clear args
    for(int i=0;i<sizeof(emu->args);i++)
    {
        emu->args[i] = 0;
    }

    return emu;
}

void load_binary(emulator* emu, unsigned char* buf)
{
    // fill memory with binary starting at 0x000
    for(int i=0;i<18;i++)
    {
        emu->memory[i] = buf[i];
        printf("load: %x\n", emu->memory[i]);
    }
}

void start_emulator(emulator* emu)
{
    printf("location: %x", emu->memory[0xa]);
    emu->running = 0;
    while(emu->running == 0)
    {
        emulate_cycle(emu);
        fgetc(stdin);
    }
}

void emulate_cycle(emulator* emu)
{
    int inc_cnt = 0; // amount to increment pc by at end of cycle
    // get opcode
    emu->opcode = emu->memory[emu->pc];
    // opcode table, set args depending on opcode
    printf("0x%x:\n", emu->pc);
    switch(emu->opcode)
    {
        case 0x3: // LD, arg0 = register, arg1 = value (3 bytes in all)
            inc_cnt = 3;
            emu->args[0] = emu->memory[emu->pc+1]; // arg0
            emu->args[1] = emu->memory[emu->pc+2]; // arg1
            printf("LD: 0x%x, 0x%x\n", emu->args[0], emu->args[1]);
            emu->r[emu->args[0]] = emu->args[1]; // set register to value argument
            break;
        case 0x1: // ADD, arg0 = register, arg1 = value
            inc_cnt = 3;
            emu->args[0] = emu->memory[emu->pc+1]; // arg0
            emu->args[1] = emu->memory[emu->pc+2]; // arg1
            printf("ADD: 0x%x, 0x%x\n", emu->args[0], emu->args[1]);
            emu->r[emu->args[0]] += emu->args[1]; // add
            break;
        case 0x2: // SUB, arg0 = register, arg1 = value
            inc_cnt = 3;
            emu->args[0] = emu->memory[emu->pc+1]; // arg0
            emu->args[1] = emu->memory[emu->pc+2]; // arg1
            printf("SUB: 0x%x, 0x%x\n", emu->args[0], emu->args[1]);
            emu->r[emu->args[0]] -= emu->args[1]; // sub
            break;
        case 0xc: // OUT, arg0 = value
            inc_cnt = 2;
            emu->args[0] = emu->memory[emu->pc+1]; // arg0
            printf("OUT: 0x%x\n", emu->args[0]);
            printf("%x\n", emu->r[emu->args[0]]);
            break;
        case 0x4: // JMP, arg0 = address
            inc_cnt = 0;
            emu->args[0] = emu->memory[emu->pc+1]; // arg0
            printf("JMP: 0x%x\n", emu->args[0]);
            emu->pc = emu->args[0];
            break;
        case 0x0: // NOP
            inc_cnt = 1;
            printf("NOP\n");
            break;
        case 0x5: // CMP, arg0 = register, arg1 = register
            inc_cnt = 3;
            emu->args[0] = emu->memory[emu->pc+1]; // arg0
            emu->args[1] = emu->memory[emu->pc+2]; // arg1
            printf("CMP: 0x%x, 0x%x\n", emu->args[0], emu->args[1]);
            if(emu->r[emu->args[0]] == emu->r[emu->args[1]]) { // equal to
                emu->cmp = 0;
            } else if(emu->r[emu->args[0]] > emu->r[emu->args[1]]) { // greater than
                emu->cmp = 1;
            } else if(emu->r[emu->args[0]] < emu->r[emu->args[1]]) // less than
            {
                emu->cmp = 2;
            }
            break;
        case 0xa: // INC, arg0 = register
            inc_cnt = 2;
            emu->args[0] = emu->memory[emu->pc+1]; // arg0
            emu->r[emu->args[0]] += 1;
            printf("INC: 0x%x\n", emu->args[0]);
            break;
        case 0xb: // DEINC, arg0 = register
            inc_cnt = 2;
            emu->args[0] = emu->memory[emu->pc+1]; // arg0
            emu->r[emu->args[0]] -= 1;
            printf("DEINC: 0x%x\n", emu->args[0]);
            break;
        case 0x6: // JE (jump equal), arg0 = address
            emu->args[0] = emu->memory[emu->pc+1]; // arg0
            printf("JE: 0x%x\n", emu->args[0]);
            if(emu->cmp == 0)
            {
                emu->pc = emu->args[0];
                inc_cnt = 0;
            } else
            {
                inc_cnt = 2;
            }
            break;
        case 0x8: // JGT (jump greater than), arg0 = address
            emu->args[0] = emu->memory[emu->pc+1]; // arg0
            printf("JGT: 0x%x\n", emu->args[0]);
            if(emu->cmp == 1)
            {
                emu->pc = emu->args[0];
                inc_cnt = 0;
            } else
            {
                inc_cnt = 2;
            }
            break;
        case 0x9: // JLT (jump less than), arg0 = address
            emu->args[0] = emu->memory[emu->pc+1]; // arg0
            printf("JLT: 0x%x\n", emu->args[0]);
            if(emu->cmp == 2)
            {
                emu->pc = emu->args[0];
                inc_cnt = 0;
            } else
            {
                inc_cnt = 2;
            }
            break;
        case 0xd: // HALT
            printf("HALT\n");
            emu->running = 1;
            break;
        default:
            printf("Unknown Opcode: 0x%x\n", emu->opcode);
            break;
    }

    emu->pc += inc_cnt;
}

void stop_emulator(emulator* emu)
{

}

void print_memory(emulator *emu)
{
    for(int i=0;i<18;i++)
    {
        printf("0x%x\n", emu->memory[i]);
    }
}