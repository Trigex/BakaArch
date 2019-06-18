#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "io.h"

typedef struct label 
{
    unsigned short address;
    char *label;
} label;

typedef struct jump
{
    unsigned short address;
    char* label;
} jump;

long parse_basm_into_binary(const char* basm, unsigned char** buf);