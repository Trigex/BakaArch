#include <stdio.h>
#include <stdlib.h>

char* read_file_into_string(char* path);
void create_file_and_write_binary(char* path, unsigned char* bin, long bin_size);
char *sgets(char *s, int n, const char **strp);
unsigned char str2hex(char* hex);
void remove_char(char *s, char c);