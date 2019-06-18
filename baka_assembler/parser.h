#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

typedef struct label 
{
    unsigned short address;
    char *label;
} label;

long parse_basm_into_binary(const char* basm, unsigned char** buf);