#include <stdio.h>
#include <stdlib.h>
#include "include/fileHandler.h"

FILE *input_ptr, *output_ptr;
lexer_t *lexer;
char *fileContent, fileName[50];
long fileLen;

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        input_ptr = fopen(argv[i], "r");
        if (input_ptr != NULL && check_Extension(argv[i]) == 1)
        {
            find_file();
            lexer = init_lexer(fileContent);
            create_TokenOutput(argv[i]);
            scanFile(lexer);
        }
        else
        {
            printf("Error on reading argument %s\n", argv[i]);
            // lexer = init_lexer(
            //     "linya error = \"may problema\";");

            // // numero a = 1;
            // scanFile(lexer);
        }
    }
    fclose(input_ptr);
    fclose(output_ptr);
    free(fileContent);
    return 0;
}
