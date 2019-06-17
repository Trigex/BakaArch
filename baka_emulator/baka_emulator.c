#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"

unsigned char* load_binary_into_buffer(char* path)
{
    FILE *fp;
    long file_size;
    int buf_read_cnt;
    unsigned char* buf;

    // get file to load
    fp = fopen(path, "rb"); // open file in binary read mode

    if(fp != NULL)
    {
        // get size
        fseek(fp, 0, SEEK_END);
        file_size = ftell(fp);
        rewind(fp);
        printf("File size: %lu\n", file_size);

        // allocate memory for buffer
        buf = malloc(file_size * sizeof(unsigned char));

        if(buf == NULL)
        {
            printf("Buffer memory allocation failed!\n");
            return NULL;
        }

        // read
        buf_read_cnt = fread(buf, sizeof(unsigned char), file_size, fp);

        if(buf_read_cnt != file_size)
        {
            printf("Reading error occured...!\n");
            return NULL;
        }
        printf("Buffer read count: %i\n", buf_read_cnt);
        fclose(fp);

        return buf;
    } else 
    {
        printf("File not found...\n");
        return NULL;
    }
}

int main(int argc, char *argv[])
{
    unsigned char* buf;
    emulator* emu = NULL;

    if(argc < 2)
    {
        printf("Please enter a program to run...\n");
        return 1;
    }
    
    buf = load_binary_into_buffer(argv[1]);

    if(buf == NULL)
    {
        return 1;
    }

    // create emulator
    emu = create_emulator();
    // load binary into memory
    load_binary(emu, buf);

    // free buffer
    free(buf);
    // debug print program memory (sorry 0x0!)
    print_memory(emu);
    start_emulator(emu);
    
    // free emulator
    free(emu);

    return 0;
}