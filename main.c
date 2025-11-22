#include"utility.h"

int main(int argc, char *argv[])
{
    if(argc!=2)
    {
        printf("Usage: %s <source_file.c>\n", argv[0]);
        return failure;
    }
    char *file_content;
    file_content = readFileContent(argv[1]);
    
    if(file_content == NULL)
    {
        printf("Error:Could not process file %s\n",argv[1]);
        return 0;
    }

    //printf("%s",file_content);
    lexicalAnalyzer(file_content);

    free(file_content);

    return 0;

}