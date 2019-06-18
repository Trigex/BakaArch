#include <stdlib.h>
#include <stdio.h>
#define MEMORY 4096

typedef struct emulator 
{
    unsigned short pc; // program counter (value from 0x000 to 0xFFF)
    unsigned char cmp; // comparison result
    unsigned char r[5]; // 6 general purpose 8 bit registers
    unsigned char memory[MEMORY]; // 4095 bytes of memory
    unsigned char opcode; // the current base opcode
    unsigned short operands[2]; // up to three opcode operands
    int running; // enable loop
    int debug; // debug print
} emulator;

emulator* create_emulator();
void load_binary(emulator* emu, unsigned char* buf, int buf_length);
void start_emulator(emulator* emu);
void emulate_cycle(emulator* emu);
void destroy_emulator(emulator* emu);
void print_registers(emulator *emu);