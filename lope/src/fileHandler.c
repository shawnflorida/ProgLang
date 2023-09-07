#include "include/fileHandler.h"

#include "include/parser.h"
#include "include/token.h"

char *token_type[100] = {"Identifier",
                         "Equal Sign",
                         "Semi-Colon",
                         "Left Parenthesis",
                         "Right Parenthesis",
                         "Left Bracket",
                         "Right Bracket",
                         "Left Brace",
                         "Right Brace",
                         "Comment",
                         "Addition",
                         "Subtraction",
                         "Division",
                         "Integer Division",
                         "Multiplication",
                         "Modulus",
                         "Exponent",
                         "Lesser Than",
                         "Greater Than",
                         "Not",
                         "Colon",
                         "Underscore",
                         "Increment",
                         "Decrement",
                         "Equal To",
                         "Less than or Equal to",
                         "Greater than or Equal to",
                         "Not Equal to",
                         "Addition Assignment",
                         "Subtraction Assignment",
                         "Multiplication Assignment",
                         "Division Assignment",
                         "Modulo Assignment",
                         "Integer Division Assignment",
                         "Habang",
                         "Kung",
                         "Sakali",
                         "Kundi",
                         "Para",
                         "Function",
                         "Import",
                         "Try",
                         "Except",
                         "Finally",
                         "Break",
                         "Continue",
                         "Return",
                         "At",
                         "O",
                         "In",
                         "Global",
                         "Boolean",
                         "Totoo",
                         "Mali",
                         "Numero",
                         "Num Literal",
                         "Linya",
                         "String Literal",
                         "None",
                         "Punto",
                         "Doble",
                         "Karakter",
                         "Invalid",
                         "Single Comment",
                         "Multiple Comment",
                         "Constant",
                         "Comma Separator",
                         "Char literal",
                         "Float literal",
                         "Scan",
                         "Print",
                         "Delete",
                         "From",
                         "Address"};

int *find_file() {
    // file -> fseek -> from start to finish -> amount of character length to
    // memory ->
    fseek(input_ptr, 0L, SEEK_END);
    fileLen = ftell(input_ptr);
    fseek(input_ptr, 0L, SEEK_SET);

    fileContent = calloc(fileLen, sizeof(char));
    fread(fileContent, sizeof(char), fileLen, input_ptr);
}
int check_Extension(char *file) {
    char *extension;
    extension = strrchr(file, '.');
    if (compare_to_keyword(extension, ".lp") == 1) {
        return 1;
    }
    return 0;
}
void create_TokenOutput(char *file) {
    memset(fileName, 0, sizeof(fileName));
    strncpy(fileName, file, strrchr(file, '.') - file);
    strcat(fileName, "_SymbolTable");
    strcat(fileName, ".txt");
    output_ptr = fopen(fileName, "w");
}

int scanFile(lexer_t *lexer) {
    token_t **tokens = malloc(sizeof(token_t *));
    const char *token_code[] = {"TOKEN_ID",
                                "TOKEN_EQUALS",
                                "TOKEN_SEMI",
                                "TOKEN_LPAREN",
                                "TOKEN_RPAREN",
                                "TOKEN_LBRACKET",
                                "TOKEN_RBRACKET",
                                "TOKEN_LBRACE",
                                "TOKEN_RBRACE",
                                "TOKEN_COMMENT",
                                "TOKEN_ADD",
                                "TOKEN_SUB",
                                "TOKEN_DIV",
                                "TOKEN_INTDIV",
                                "TOKEN_MULT",
                                "TOKEN_MOD",
                                "TOKEN_EXP",
                                "TOKEN_LESS",
                                "TOKEN_GREATER",
                                "TOKEN_NEGATE",
                                "TOKEN_COLON",
                                "TOKEN_UNDERSCORE",
                                "TOKEN_INCR",
                                "TOKEN_DECR",
                                "TOKEN_EQ_TO",
                                "TOKEN_LS_THAN_EQ",
                                "TOKEN_GR_THAN_EQ",
                                "TOKEN_NOT_EQ",
                                "TOKEN_ADD_ASGN",
                                "TOKEN_SUB_ASGN",
                                "TOKEN_MULT_ASGN",
                                "TOKEN_DIV_ASGN",
                                "TOKEN_MOD_ASGN",
                                "TOKEN_INTDIV_ASGN",
                                "TOKEN_HABANG",
                                "TOKEN_KUNG",
                                "TOKEN_SAKALI",
                                "TOKEN_KUNDI",
                                "TOKEN_PARA",
                                "TOKEN_FUNCTION",
                                "TOKEN_ANGKAT",
                                "TOKEN_SUBUKAN",
                                "TOKEN_MALIBAN",
                                "TOKEN_WAKAS",
                                "TOKEN_TIGIL",
                                "TOKEN_TULOY",
                                "TOKEN_BALIK",
                                "TOKEN_AT",
                                "TOKEN_O",
                                "TOKEN_NASA",
                                "TOKEN_LAHAT",
                                "TOKEN_BOLYAN",
                                "TOKEN_TAMA",
                                "TOKEN_MALI",
                                "TOKEN_NUMERO",
                                "TOKEN_NUM",
                                "TOKEN_LINYA",
                                "TOKEN_STRING",
                                "TOKEN_WALA",
                                "TOKEN_PUNTO",
                                "TOKEN_DOBLE",
                                "TOKEN_KAR",
                                "TOKEN_UNKNOWN",
                                "TOKEN_COMMENT_VALUE_SINGLE",
                                "TOKEN_COMMENT_VALUE_MULTI",
                                "TOKEN_CAPITAL",
                                "TOKEN_COMMA",
                                "TOKEN_CHAR_LIT",
                                "TOKEN_FLOAT_LIT",
                                "TOKEN_LAHAD",
                                "TOKEN_KUHA",
                                "TOKEN_IBURA",
                                "TOKEN_FROM",
                                "TOKEN_ADDRESS"};

    // printf("TOKEN-NUMBER\tTOKEN-CODE\t\t\tTOKEN-INFO\t\tLEXEME\t\t\tPOS.(LINE,
    // CH)\n"); fprintf(output_ptr,
    // "TOKEN-NUMBER\tTOKEN-CODE\t\t\t\t\tTOKEN-INFO\t\t\tLEXEME\t\t\tPOS.(LINE,
    // CH)\n");

    // printf("---------------------------------------------------------------------------------------------------------------------\n");
    // fprintf(output_ptr,
    // "---------------------------------------------------------------------------------------------------------------------\n");
    int i = 0;
    while ((tokens[i] = lexer_get_next_token(lexer)) != (void *)0) {
        i++;
        tokens = realloc(tokens, (i + 1) * sizeof(token_t *));
        // fprintf(output_ptr, "%s\t%s\t[%d, %d]\n", i,
        // token_type[tokens[i]->type], tokens[i]->value, tokens[i]->lpos,
        // tokens[i]->cpos); printf("%s\t%s\t[%d, %d]\n", i,
        // token_type[tokens[i]->type], tokens[i]->value, tokens[i]->lpos,
        // tokens[i]->cpos); if(token->type == 0 || token->type == 3 ||
        // token->type == 4)
        // {
        //     printf("%d\t\t%s\t\t\t%s\t%s\t\t[%d, %d]\n", i,
        //     token_code[token->type], token_type[token->type], token->value,
        //     token->lpos, token->cpos); if(token->type == 3 || token->type ==
        //     4)
        //     {
        //         fprintf(output_ptr, "%d\t\t\t\t%s\t\t\t\t%s\t%s\t\t\t[%d,
        //         %d]\n", i, token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else
        //     {
        //         fprintf(output_ptr, "%d\t\t\t\t%s\t\t\t\t\t%s\t%s\t\t\t[%d,
        //         %d]\n", i, token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        // }
        // else if(token->type == 1 || token->type == 2 || token->type == 57 ||
        // token->type == 55 || token->type == 65 || token->type == 67 ||
        // token->type == 45 || token->type == 7 || token->type == 8 ||
        // token->type == 39 || token->type == 22)
        // {
        //     printf("%d\t\t%s\t\t\t%s\t\t%s\t\t\t[%d, %d]\n", i,
        //     token_code[token->type], token_type[token->type], token->value,
        //     token->lpos, token->cpos); if(token->type == 1 || token->type ==
        //     7 || token->type == 8)
        //     {
        //         fprintf(output_ptr, "%d\t\t\t\t%s\t\t\t\t%s\t\t\t%s\t\t\t[%d,
        //         %d]\n", i, token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else if(token->type == 22)
        //     {
        //         fprintf(output_ptr,
        //         "%d\t\t\t\t%s\t\t\t\t\t%s\t\t\t%s\t\t\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else if(token->type == 2 || token->type == 55)
        //     {
        //         fprintf(output_ptr,
        //         "%d\t\t\t\t%s\t\t\t\t\t%s\t\t\t%s\t\t\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else if(token->type == 65 || token->type == 39 || token->type ==
        //     45)
        //     {
        //         fprintf(output_ptr, "%d\t\t\t\t%s\t\t\t\t%s\t\t\t%s\t\t\t[%d,
        //         %d]\n", i, token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else
        //     {
        //         fprintf(output_ptr, "%d\t\t\t\t%s\t\t\t\t%s\t\t%s\t\t\t[%d,
        //         %d]\n", i, token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        // }
        // else if(token->type == 63 || token->type == 64)
        // {
        //     if(token->type == 63)
        //     {
        //         printf("%d\t\t%s\t%s\t\t%s\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos); fprintf(output_ptr,
        //         "%d\t\t\t\t%s\t%s\t\t%s\t\t\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else
        //     {
        //         for(int i=0; i<strlen(token->value); i++)
        //             if(token->value[i] == '\n')
        //                 token->value[i] = ' ';
        //         printf("%d\t\t%s\t%s\t%s\t[%d, %d] \n",  i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos); fprintf(output_ptr,
        //         "%d\t\t\t\t%s\t%s\t%s\t\t\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        // }
        // else
        // {
        //     if(token->type == 25)
        //     {
        //         printf("%d\t\t%s\t\t%s\t%s\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos); fprintf(output_ptr,
        //         "%d\t\t\t\t%s\t\t%s\t%s\t\t\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else
        //     {
        //         printf("%d\t\t%s\t\t\t%s\t\t\t%s\t\t\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos); if(token->type == 62
        //         || token->type == 40 || token->type == 46 || token->type ==
        //         42 || token->type == 50)
        //         {
        //             fprintf(output_ptr,
        //             "%d\t\t\t\t%s\t\t\t\t%s\t\t\t\t%s\t\t\t[%d, %d]\n", i,
        //             token_code[token->type], token_type[token->type],
        //             token->value, token->lpos, token->cpos);
        //         }
        //         else if(token->type == 37)
        //         {
        //             fprintf(output_ptr,
        //             "%d\t\t\t\t%s\t\t\t\t\t%s\t\t\t\t%s\t\t\t[%d, %d]\n", i,
        //             token_code[token->type], token_type[token->type],
        //             token->value, token->lpos, token->cpos);
        //         }
        //         else if(token->type == 41 || token->type == 38)
        //         {
        //             fprintf(output_ptr,
        //             "%d\t\t\t\t%s\t\t\t\t\t%s\t\t\t\t\t%s\t\t\t[%d, %d]\n",
        //             i, token_code[token->type], token_type[token->type],
        //             token->value, token->lpos, token->cpos);
        //         }
        //         else if(token->type == 47 || token->type == 48 || token->type
        //         == 49)
        //         {
        //             fprintf(output_ptr,
        //             "%d\t\t\t\t%s\t\t\t\t\t%s\t\t\t\t\t%s\t\t\t[%d, %d]\n",
        //             i, token_code[token->type], token_type[token->type],
        //             token->value, token->lpos, token->cpos);
        //         }
        //         else
        //         {
        //             fprintf(output_ptr,
        //             "%d\t\t\t\t%s\t\t\t\t\t%s\t\t\t\t%s\t\t\t[%d, %d] \n", i,
        //             token_code[token->type], token_type[token->type],
        //             token->value, token->lpos, token->cpos);
        //         }
        //     }
        // }
        // i++;
        // printf("---------------------------------------------------------------------------------------------------------------------\n");
        // fprintf(output_ptr,
        // "---------------------------------------------------------------------------------------------------------------------\n");
    }

    for (int x = 0; x < i; x++) {
        if (tokens[x]->type == TOKEN_COMMENT_VALUE_MULTI ||
            tokens[x]->type == TOKEN_COMMENT_VALUE_SINGLE) {
            fprintf(output_ptr, "%s\t\t\t\t\t\t\t\t\t\t\t[%d, %d]\n",
                    token_code[tokens[x]->type], tokens[x]->lpos,
                    tokens[x]->cpos);
        } else {
            fprintf(output_ptr, "%s\t\t\t\t\t\t%s\t\t\t\t\t[%d, %d]\n",
                    token_code[tokens[x]->type], tokens[x]->value,
                    tokens[x]->lpos, tokens[x]->cpos);
        }
        // printf("%d\t\t%s\t\t\t%s\t%s\t\t[%d, %d]\n", x,
        // token_code[tokens[x]->type], token_type[tokens[x]->type],
        // tokens[x]->value, tokens[x]->lpos, tokens[x]->cpos);
    }
    exec_parser(parser_init(tokens));
    free(tokens);
}