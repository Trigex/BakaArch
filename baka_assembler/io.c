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
            printf("The file did not completely read!");
            return NULL;
        }

        fclose(fp);
        return buf;
    } else {
         printf("Could not open file");
         return NULL;
    }
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
