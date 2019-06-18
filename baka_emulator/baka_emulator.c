#include <stdio.h>
#include <stdlib.h>
#include "emulator.h"

void print_buffer(unsigned char* buf, long buf_size)
{
    for(int i=0;i<buf_size;i++)
    {
        printf("%d: %x\n", i, *(buf+i));
    }
}

long load_binary_into_buffer(char* path, unsigned char** buf)
{
    FILE *fp;
    long file_size;
    int buf_read_cnt;

    // get file to load
    fp = fopen(path, "rb"); // open file in binary read mode

    if(fp != NULL)
    {
        // get size
        fseek(fp, 0, SEEK_END);
        file_size = ftell(fp);
        rewind(fp);

        // allocate memory for buffer
        *buf = calloc(file_size, sizeof(unsigned char));

        if(buf == NULL)
        {
            printf("Buffer memory allocation failed!\n");
            return 0;
        }

        // read
        buf_read_cnt = fread(*buf, sizeof(unsigned char), file_size, fp);

        if(buf_read_cnt != file_size)
        {
            printf("Reading error occured...!\n");
            return 0;
        }
        fclose(fp);
        return file_size;
    } else 
    {
        printf("File not found...\n");
        return 0;
    }
}

int main(int argc, char *argv[])
{
    unsigned char* buf = NULL;
    long buf_size = 0;
    emulator* emu = NULL;

    if(argc < 2)
    {
        printf("Please enter a program to run...\n");
        return 1;
    }
    
    buf_size = load_binary_into_buffer(argv[1], &buf);

    if(buf_size == 0)
        return 1;

    if(buf == NULL)
    {
        printf("The file couldn't be loaded!\n");
        return 1;
    }

    // create emulator
    emu = create_emulator();
    // load binary into memory
    load_binary(emu, buf, buf_size);

    // free buffer
    free(buf);
    // start emulator
    start_emulator(emu);
    // destroy once loop has stopped
    destroy_emulator(emu);

    return 0;
}