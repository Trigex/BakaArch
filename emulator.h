#include <stdlib.h>
#include <stdio.h>
#define MEMORY 4096

typedef struct emulator 
{
    unsigned short pc; // program counter (value from 0x000 to 0xFFF)
    unsigned char cmp; // comparison result
    unsigned char r[6]; // 7 general purpose 8 bit registers
    unsigned char memory[MEMORY]; // 4095 bytes of memory
    unsigned char opcode; // the base opcode
    unsigned char args[1]; // up to two opcode arguments
    unsigned short bin_size; // size of the binary loaded in
    int running;
} emulator;

emulator* create_emulator();
void load_binary(emulator* emu, unsigned char* buf);
void start_emulator(emulator* emu);
void emulate_cycle(emulator* emu);
void stop_emulator(emulator* emu);
void print_memory(emulator *emu);