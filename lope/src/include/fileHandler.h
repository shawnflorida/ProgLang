#ifndef ADDFILE_H
#define ADDFILE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"

extern FILE *input_ptr, *output_ptr;
extern lexer_t *lexer;
extern char *fileContent, fileName[50];
extern long fileLen;

int *find_file();
int scanFile(lexer_t *lexer);
int check_Extension(char *file);
void create_TokenOutput(char *file);

#endif