#include "io.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Please supply a file\n");
        return 1;
    }

    if(argc < 3)
    {
        printf("Please supply an output path\n");
        return 1;
    }

    const char* file = (const char*)read_file_into_string(argv[1]);
    if(file == NULL)
    {
        return 1;
    }

    unsigned char* program;

    long program_size = parse_basm_into_binary(file, &program);

    create_file_and_write_binary(argv[2], program, program_size);

    free(program);
    free(file);

    return 0;
}