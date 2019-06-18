#include "parser.h"

static int is_register(char* str)
{
    if(strchr(str, 'r')!=NULL) {
        return 1;
    } else {
        return 0;
    }
}

long parse_basm_into_binary(const char* basm, unsigned char** buf)
{
    unsigned char program[2024]; // the buffer for the binary this will produce
    unsigned short counter = 0; // address of machine code instruction
    char *opcode, *op1, *op2, *label; // opcode, label, operand 1, operand 2
    char* line[100]; // maxium 100 character line count (this will never be reached!)
    char* token = NULL; // opcode token
    struct label label_table[50]; // 50 avaliable labels
    int label_cnt = 0; // labels found during assembly
    char* source = malloc(strlen(basm)); // copy of basm, so the original doesn't get modified
    source = strcpy(source, basm); // copy it


    while(NULL != sgets(line, sizeof(line), &basm))
    {
        int counter_last = counter;

        printf("Current line: %s\n", line);
        token = strtok(line, "\n\t\r ");
        printf("Current token: %s\n", token);
        // determine token
        if(strcmp(token, "ld") == 0) // LD
        {
            // get operators
            op1 = strtok(NULL,"\n\t\r ");
            op2 = strtok(NULL,"\n\t\r ");
            // remove , and r from op1
            remove_char(op1, ',');
            remove_char(op1, 'r');
            printf("Operand 1: %s\nOperand 2: %s\n", op1, op2);

            program[counter] = 0x3; // set ld opcode
            counter++;
            program[counter] = 0xff; // register flag
            counter++;
            program[counter] = str2hex(op1); // register code
            if(is_register(op1)) // second operator is register
            {
                // remove r from op2
                remove_char(op2, 'r');
                counter++;
                program[counter] = 0xff; // register flag
                counter++;
                program[counter] = str2hex(op2);
            } else { // second operator is value
                counter++;
                program[counter] = str2hex(op2);
            }

            counter++; // increment for next instruction
        } else if(strcmp(token, "add") == 0) // ADD
        {
            // get operators
            op1 = strtok(NULL,"\n\t\r ");
            op2 = strtok(NULL,"\n\t\r ");
            // remove , and r from op1
            remove_char(op1, ',');
            remove_char(op1, 'r');
            printf("Operand 1: %s\nOperand 2: %s\n", op1, op2);
            program[counter] = 0x1; // add opcode
            counter++;
            program[counter] = 0xff; // register flag
            counter++;
            program[counter] = str2hex(op1); // register code

            if(is_register(op1)) // second operator is register
            {
                // remove r from op2
                remove_char(op2, 'r');
                counter++;
                program[counter] = 0xff; // register flag
                counter++;
                program[counter] = str2hex(op2);
            } else { // second operator is value
                counter++;
                program[counter] = str2hex(op2);
            }
            counter++;
        } else if(strcmp(token, "print") == 0) // PRINT
        {
            program[counter] = 0xc; // print opcode
            counter++;
            op1 = strtok(NULL,"\n\t\r ");
            printf("Operand: %s\n", op1);
            if(is_register(op1) != NULL)
            {
                program[counter] = 0xff; // register flag
                counter++;
                remove_char(op1, 'r');
                program[counter] = str2hex(op1); // register code
            } else {
                program[counter] = str2hex(op1);
            }
            counter++;
        } else if(strcmp(token, "halt") == 0)
        {
            program[counter] = 0xd;
            counter++;
        }

        printf("Translated into: ");
        for(int i=counter_last;i<=counter;i++)
            printf("0x%x ", program[i]);

        printf("\n");
    }

    // create new array that only contains program data
    *buf = malloc(counter * sizeof(unsigned char));
    for(int i=0;i<counter;i++)
    {
        *(*buf + i) = program[i];
    }

    printf("Completed program: ");
    for(int i=0;i<counter;i++)
    {
        printf("0x%x ", *(*buf + i));
    }
    printf("\n");

    return counter;
}