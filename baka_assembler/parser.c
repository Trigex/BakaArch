#include "parser.h"

unsigned char* parse_basm_into_binary(char* basm)
{
    unsigned char* bin_buf;
    char string_buf[strlen(basm)];
    const char **p = &basm;

    // loop for every line
    while(NULL!=sgets(string_buf, sizeof(string_buf), p))
    {
        const char* cur_line = malloc(strlen(string_buf));
        const char* operation;
        strcpy(cur_line, string_buf);
        // parse line
        printf("Current line: %s\n", cur_line);
        operation = strtok(cur_line, " "); // get characters until first space
        printf("Operator: %s\n", operation);

        if(strcmp(operation, "ld")) { // ld operation, expects two paramters

        } else if(strcmp(operation, "add")) { // add operation, expects two paramaters

        } else if(strcmp(operation, "out")) { // out operation, expects 1 paramater

        } else {
            printf("Unknown operation: %s\n", operation);
        }
    }

    return bin_buf;
}