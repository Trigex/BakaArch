#include "parser.h"

// all these parse functions return 1 for true, 0 for false

static int is_register(char* str)
{
    if(strchr(str, 'r')!=NULL) { // contains r and second character is digit
        if(isdigit(str[1]!=0)) // check after the r check, so it's not ran on pure numeric arguments that aren't registers
        { 
            return 1;
        }
    } else {
        return 0;
    }
}

static int is_alpha(char* str) 
{
    int flag = 1;
    for(int i=0;i<strlen(str);i++)
    {
        if(isalpha(str[i])==0) { // if any character in the string is non alpha, return false
            flag = 0;
            break;
        }
    }
    return flag;
}

static int is_label(char* str)
{
    // copy so we don't modify passed string with remove_char
    char* copy = (char*)malloc(strlen(str) * sizeof(char));
    copy = strcpy(copy, str);

    if(strchr(copy, ':')!=NULL) { // string contains ":"
        // remove :
        remove_char(copy, ':');
        if(is_alpha(copy) == 1) // the label is alpha (probably change to allow for alpha numeric later)
        {
            free(copy);
            return 1;
        } else {
            free(copy);
            return 0;
        }
    } else {
        free(copy);
        return 0;
    }
}

long parse_basm_into_binary(const char* basm, unsigned char** buf)
{
    unsigned char program[4024]; // the buffer for the binary this will produce
    unsigned short counter = 0; // address of machine code instruction
    char *op1, *op2, *label; // label, operand 1, operand 2
    char* line[100]; // maxium 100 character line count (this will never be reached!)
    char* token = NULL; // opcode token
    struct label label_table[50]; // 50 avaliable labels
    int label_cnt = 0; // labels found during assembly
    struct jump jump_table[50]; // 50 avaliable jumps
    int jump_cnt = 0; // jumps found during assemly
    char* source = malloc(strlen(basm)); // copy of basm, so the original doesn't get modified
    source = strcpy(source, basm); // copy it

    while(NULL != sgets(line, sizeof(line), &basm)) // Inital assemble
    {
        int counter_last = counter;
        printf("Counter: %d\n", counter);

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

            if(is_register(op2)) // second operator is register
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
        } else if(strcmp(token, "halt") == 0) // HALT
        {
            program[counter] = 0xd;
            counter++;
        } else if(strcmp(token, "sub") == 0) // SUB
        {
             // get operators
            op1 = strtok(NULL,"\n\t\r ");
            op2 = strtok(NULL,"\n\t\r ");
            // remove , and r from op1
            remove_char(op1, ',');
            remove_char(op1, 'r');
            printf("Operand 1: %s\nOperand 2: %s\n", op1, op2);
            program[counter] = 0x2; // sub opcode
            counter++;
            program[counter] = 0xff; // register flag
            counter++;
            program[counter] = str2hex(op1); // register code

            if(is_register(op2)) // second operator is register
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
        } else if(strcmp(token, "jmp") == 0) // JMP
        {
            // get operator
            op1 = strtok(NULL,"\n\t\r ");
            printf("Operand: %s\n", op1);
            program[counter] = 0x4; // add opcode
            counter++;
            if(is_alpha(op1)==1) // jump is to label
            {
                printf("Jump to label\n");
                jump_table[jump_cnt].label = op1; // set jump to the target label
                jump_table[jump_cnt].address = counter; // address to later set
                jump_cnt++;
                program[counter] = 0x0; // set op1 to 0 for now
            } else { // jump is to direct address
                program[counter] = str2hex(op1);
            }
            counter++;
        } else {
            if(is_label(token)!=NULL) // LABEL
            {
                printf("Is label\n");
                // label should point to next instruction
                label_table[label_cnt].address = counter + 1;
                remove_char(token, ":"); // remove :
                label_table[label_cnt].label=token; // set label name
                label_cnt++;
            }
        }

        printf("Translated into: ");
        for(int i=counter_last;i<=counter;i++)
            printf("0x%x ", program[i]);

        printf("\n");
    }

    if(jump_cnt > 0 && label_cnt > 0)
    {
        printf("Converting jump label's into addresses\n");
        printf("Label: %s\n", label_table[0].label);
        printf("Jump: %s\n", jump_table[0].label);
        // convert jumps labels to addresses
        for(int i=0;i<jump_cnt;i++)
        {
            // get label
            for(int b=0;b<label_cnt;b++)
            {
                if(strcmp(label_table[b].label, jump_table[i].label) == 0) // found matching label
                {
                    program[jump_table[i].address] = label_table[b].address; // set to label's address
                    printf("Jump label address set\n");
                }
            }
        }
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