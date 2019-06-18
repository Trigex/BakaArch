#include "io.h"

char* read_file_into_string(char* path)
{
    FILE *fp;
    char* buf = 0;
    long file_size;
    int read_size;

    fp = fopen(path, "r");
    if(fp != NULL) {
        // get size
        fseek(fp, 0, SEEK_END);
        file_size = ftell(fp);
        rewind(fp);
        
        // allocate memory
        buf = malloc(file_size);
        if(buf == NULL)
        {
            printf("Couldn't allocate memory for buffer!\n");
            return NULL;
        }

        read_size = fread(buf, 1, file_size, fp);
        if(read_size != file_size)
        {
            printf("The file did not completely read!\n");
            return NULL;
        }

        fclose(fp);
        return buf;
    } else {
         printf("Could not open file\n");
         return NULL;
    }
}

void create_file_and_write_binary(char* path, unsigned char* bin, long bin_size)
{
    FILE *fp;
    fp = fopen(path, "wb");
    fwrite(bin, bin_size, 1, fp);
    fclose(fp);
}

char *sgets(char *s, int n, const char **strp){
    if(**strp == '\0')return NULL;
    int i;
    for(i=0;i<n-1;++i, ++(*strp)){
        s[i] = **strp;
        if(**strp == '\0')
            break;
        if(**strp == '\n'){
            s[i+1]='\0';
            ++(*strp);
            break;
        }
    }
    if(i==n-1)
        s[i] = '\0';
    return s;
}

unsigned char str2hex(char* hex)  
{
    unsigned char result=0;

    while ((*hex)!='\0')
    {
        if (('0'<=(*hex))&&((*hex)<='9'))
            result = result*16 + (*hex) -'0';
        else if (('a'<=(*hex))&&((*hex)<='f'))
            result = result*16 + (*hex) -'a'+10;
        else if (('A'<=(*hex))&&((*hex)<='F'))
            result = result*16 + (*hex) -'A'+10; 
        hex++;
    }
    return(result);
}

void remove_char(char *s, char c)
{
    int j, n = strlen(s); 
    for (int i=j=0; i<n; i++) 
       if (s[i] != c) 
          s[j++] = s[i]; 
      
    s[j] = '\0'; 
}