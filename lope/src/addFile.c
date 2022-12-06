#include <stdlib.h>
#include <stdio.h>
#include "include/addFile.h"
#include "include/main.h"

int *find_file()
{
    fseek(fptr, 0L, SEEK_END);
    fileLen = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);

    fileContent = calloc(fileLen, sizeof(char));
    fread(fileContent, sizeof(char), fileLen, fptr);
}
