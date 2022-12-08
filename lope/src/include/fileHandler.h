#ifndef ADDFILE_H
#define ADDFILE_H

#include <stdio.h>
#include "lexer.h"

FILE *input_ptr, *output_ptr;
lexer_t *lexer;
char *fileContent, fileName[50];
long fileLen;

int *find_file();
int scanFile(lexer_t *lexer);
int check_Extension(char *file);
void create_TokenOutput(char *file);

#endif