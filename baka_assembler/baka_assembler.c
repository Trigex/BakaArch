#include "io.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    char* file = read_file_into_string(argv[1]);
    if(file == NULL)
    {
        return 1;
    }

    parse_basm_into_binary(file);

    return 0;
}