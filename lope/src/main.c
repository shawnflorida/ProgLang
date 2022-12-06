#include <stdio.h>
#include <stdlib.h>
#include "include/main.h"
#include "include/addFile.h"
#include "include/lexer.h"

int main(int argc, char **argv)
{

    for (int i = 1; i < argc; i++)
    {
        fptr = fopen(argv[i], "r");
        if (fptr != NULL)
        {
            find_file();
            lexer = init_lexer(fileContent);
            scanFile(lexer);
        }
        else
        {
            lexer = init_lexer(
                "linya error = \"may problema\";");

            // numero a = 1;
            scanFile(lexer);
        }
    }
    free(fptr);
    free(fileContent);
    return 0;
}
