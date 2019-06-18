#include "emulator.h"

emulator* create_emulator()
{
    emulator* emu; 
    emu = malloc(sizeof(emulator));
    emu->pc = 0x000;
    emu->opcode = 0;
    emu->cmp = 0;
    emu->running = 0;
    emu->debug = 1;

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

    // clear operands
    for(int i=0;i<sizeof(emu->operands);i++)
    {
        emu->operands[i] = 0;
    }

    return emu;
}

void load_binary(emulator* emu, unsigned char* buf, int buf_length)
{
    for(int i=0;i<buf_length;i++)
    {
        emu->memory[i] = *(buf + i);
        if(emu->debug == 1){printf("0x%x: 0x%x\n", i, emu->memory[i]);}
    }
}

void start_emulator(emulator* emu)
{
    emu->running = 1;
    while(emu->running == 1)
    {
        emulate_cycle(emu);
        print_registers(emu);
        fgetc(stdin);
    }
}

void emulate_cycle(emulator* emu)
{
    int inc_cnt = 0; // how many bytes to increment by at the cycle's end
    // get opcode
    emu->opcode = emu->memory[emu->pc];
    unsigned char reg_flag = 0;
    unsigned char reg_label = 0;
    unsigned char value = 0;
    switch(emu->opcode)
    {
        case 0x0: // NOP
            inc_cnt = 1;
            if(emu->debug == 1){printf("NOP\n");}
            break;
        case 0x1: // ADD
            reg_flag = emu->memory[emu->pc+1]; // register flag (0xff)
            if(reg_flag != 0xff) {printf("Invalid register argument!\n"); break;}
            reg_label = emu->memory[emu->pc+2]; // register number (0x1)
            if(emu->memory[emu->pc+3] == 0xff) // register value supplied
            {
                value = emu->r[emu->memory[emu->pc+4]]; // set value to value of supplied register
                inc_cnt = 5; // 5 byte instruction
            } else // 8 bit number value supplied 
            {
                value = emu->memory[emu->pc+3]; // set value to next byte
                inc_cnt = 4; // 4 byte instruction
            }
            
            // add
            emu->r[reg_label] += value;
            if(emu->debug == 1){printf("ADD: r%x, 0x%x\n", reg_label, value);}
            break;
        case 0x2: // SUB
            reg_flag = emu->memory[emu->pc+1]; // register flag (0xff)
            if(reg_flag != 0xff) {printf("Invalid register argument!\n"); break;}
            reg_label = emu->memory[emu->pc+2]; // register number (0x1)
            if(emu->memory[emu->pc+3] == 0xff) // register value supplied
            {
                value = emu->r[emu->memory[emu->pc+4]]; // set value to value of supplied register
                inc_cnt = 5; // 5 byte instruction
            } else // 8 bit number value supplied 
            {
                value = emu->memory[emu->pc+3]; // set value to next byte
                inc_cnt = 4; // 4 byte instruction
            }
            
            // sub
            emu->r[reg_label] -= value;
            if(emu->debug == 1){printf("SUB: r%x, 0x%x\n", reg_label, value);}
            //inc_cnt = 4;
            break;
        case 0x3: // LD
            reg_flag = emu->memory[emu->pc+1]; // register flag (0xff)
            if(reg_flag != 0xff) {printf("Invalid register argument!\n"); break;}
            reg_label = emu->memory[emu->pc+2]; // register number (0x1)
            if(emu->memory[emu->pc+3] == 0xff) // register value supplied
            {
                value = emu->r[emu->memory[emu->pc+4]]; // set value to value of supplied register
                inc_cnt = 5; // 5 byte instruction
            } else // 8 bit number value supplied 
            {
                value = emu->memory[emu->pc+3]; // set value to next byte
                inc_cnt = 4; // 4 byte instruction
            }
            
            // load
            emu->r[reg_label] = value;
            if(emu->debug == 1){printf("LD: r%x, 0x%x\n", reg_label, value);}
            break;
        case 0x4: // JMP
            inc_cnt = 0;
            emu->pc = emu->memory[emu->pc+1];
            if(emu->debug == 1){printf("JMP: 0x%x\n", emu->memory[emu->pc+1]);}
            break;
        case 0x5: // CMP
            reg_flag = emu->memory[emu->pc+1]; // register flag (0xff)
            if(reg_flag != 0xff) {printf("Invalid register argument!\n"); break;}
            reg_label = emu->memory[emu->pc+2]; // register number (0x1)
            if(emu->memory[emu->pc+3] == 0xff) // register value supplied
            {
                value = emu->r[emu->memory[emu->pc+4]]; // set value to value of supplied register
                inc_cnt = 5; // 5 byte instruction
            } else // 8 bit number value supplied 
            {
                value = emu->memory[emu->pc+3]; // set value to next byte
                inc_cnt = 4; // 4 byte instruction
            }

            // equal
            if(emu->r[reg_label] == value) {emu->cmp = 0;}
            // less than
            if(emu->r[reg_label] < value) {emu->cmp = 2;}
            // not equal
            if(emu->r[reg_label] != value) {emu->cmp = 3;}
            // greater than
            if(emu->r[reg_label] > value) {emu->cmp = 1;}
            // not less than
            if(!(emu->r[reg_label] < value)) {emu->cmp = 5;}
            // not greater than
            if(!(emu->r[reg_label] > value)) {emu->cmp = 4;}
            //greater than or equal to
            if(emu->r[reg_label] >= value) {emu->cmp = 6;}
            // less than or equal to
            if(emu->r[reg_label] <= value) {emu->cmp = 7;}

            if(emu->debug == 1){printf("CMP: r%x, 0x%x\n", reg_label, value);}
            break;
        case 0x6: // JE
            value = emu->memory[emu->pc+1];
            if(emu->cmp == 0) {emu->pc = value; inc_cnt = 0;}
            else {inc_cnt = 2;}
            break;
        case 0x7: // JNE
            value = emu->memory[emu->pc+1];
            if(emu->debug == 1){printf("JNE: 0x%x\n", value);}
            if(emu->cmp == 3) {emu->pc = value; inc_cnt = 0; printf("Jumped to 0x%x\n", value);}
            else {inc_cnt = 2;}
            break;
        case 0x8: // JGT
            value = emu->memory[emu->pc+1];
            if(emu->cmp == 1) {emu->pc = value; inc_cnt = 0;}
            else {inc_cnt = 2;}
            break;
        case 0x9: // JLT
            value = emu->memory[emu->pc+1];
            if(emu->cmp == 2) {emu->pc = value; inc_cnt = 0;}
            else {inc_cnt = 2;}
            break;
        case 0xa: // INC
            reg_flag = emu->memory[emu->pc+1]; // register flag (0xff)
            if(reg_flag != 0xff) {printf("Invalid register argument!\n"); break;}
            reg_label = emu->memory[emu->pc+2]; // register number (0x1)

            emu->r[reg_label] += 1;
            inc_cnt = 3;
            if(emu->debug == 1){printf("INC: r%x\n", reg_label);}
            break;
        case 0xb: // DEINC
            reg_flag = emu->memory[emu->pc+1]; // register flag (0xff)
            if(reg_flag != 0xff) {printf("Invalid register argument!\n"); break;}
            reg_label = emu->memory[emu->pc+2]; // register number (0x1)

            emu->r[reg_label] -= 1;
            inc_cnt = 3;
            if(emu->debug == 1){printf("DEINC: r%x\n", reg_label);}
            break;
        case 0xc: // PRINT
            if(emu->memory[emu->pc+1] == 0xff) // register value supplied
            {
                value = emu->r[emu->memory[emu->pc+2]]; // set value to value of supplied register
                inc_cnt = 3; // 3 byte instruction
            } else // 8 bit number value supplied 
            {
                value = emu->memory[emu->pc+1]; // set value to next byte
                inc_cnt = 2; // 2 byte instruction
            }

            printf("PRINT: %d\n", value);

            break;
        case 0xd: // HALT
            inc_cnt = 0;
            emu->running = 0;
            if(emu->debug == 1){printf("HALT\n");}
            break;
        default:
            printf("Unknown Opcode: 0x%x\n", emu->opcode);
            inc_cnt = 1;
            break;
    }

    emu->pc += inc_cnt;
}

void destroy_emulator(emulator* emu)
{
    free(emu);
}

void print_registers(emulator *emu)
{
    for(int i=0;i<sizeof(emu->r);i++)
    {
        printf("r%x: %x\n", i, emu->r[i]);
    }

    printf("pc: 0x%x\n", emu->pc);
    printf("cmp: %x\n", emu->cmp);
}