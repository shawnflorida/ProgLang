#include "include/parser.h"

const char *token_type[] =
    {
        "Identifier/Value",
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
        "While",
        "If",
        "Else",
        "Else If",
        "For",
        "Function",
        "Import",
        "Try",
        "Except",
        "Finally",
        "Break",
        "Continue",
        "Return",
        "And",
        "Or",
        "In",
        "Global",
        "Boolean",
        "True",
        "False",
        "Integer",
        "Num Literal",
        "String",
        "String Literal",
        "None",
        "Float",
        "Double",
        "Character",
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
        "From"};

parser_t *parser_init(token_t **tokens)
{
    parser_t *parser = (parser_t *)malloc(sizeof(parser_t));
    parser->tokens = tokens;
    parser->cursor = 0;
    parser->token = parser->tokens[parser->cursor];
    return parser;
}
void exec_parser(parser_t *parser)
{
    node_t *program = programN(parser);
    free(parser);
    free(program);
    printf("Success");
}
// program = statement
node_t *createNode()
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL)
    {
        printf("\nFailed to allocate memory!\n");
        exit(-1);
    }
    return node;
}
node_t *programN(parser_t *parser)
{
    node_t *node = createNode();
    node->value.program = (programNode *)malloc(sizeof(programNode));
    node->type = PROGRAM;
    node->value.program->statements = statementN(parser);
    return node;
}
node_t *statementN(parser_t *parser)
{
    node_t *node = createNode();
    node->value.stmt = (statementNode *)malloc(sizeof(statementNode));
    node->type = STATEMENT;
    // numero
    if (match_tokens(parser, 6, TOKEN_BOOL, TOKEN_INT, TOKEN_DBL, TOKEN_FLOAT, TOKEN_CHAR, TOKEN_STR))
    {
        // x
        node->value.stmt->statement = assgnN(parser);
    }
    else
    {
        node->value.stmt->statement = atomN(parser);
    }
    if (parser->token == (void *)0)
    {
        node->value.stmt->nextStatement = NULL;
    }
    else
    {
        node->value.stmt->nextStatement = statementN(parser);
    }

    if ((node->value.stmt->statement == (void *)0) && (node->value.stmt->nextStatement == (void *)0))
    {
        free(node);
    }
    return node;
}
/*
numero y = 3 * 2 + 1;
program
statement
assignment

model = video from shawn
architecture = crafting intepreters

numero data
y id
= assgn
&&
1: 3                                // cursor = 3 // comp->atom returns 3
2: muldiv(3, traverse)              // cursor = * // when backtracking stop at muldiv because multoken is spotted
3: muldiv(3, 2)                     // cursor = 2 // righthand returns 2
4: addsub(muldiv(3, 2), traverse)   // cursor = + // when backtracking stop at addsub because addtoken is spotted
5: addsub(muldiv(3, 2), 1)          // cursor = 1 // now it will return this into the expr
*/
/*
numero x = y + 3 * 2;
numero
x
=
addsub(y, muldiv(3, 2))
*/

node_t *assgnN(parser_t *parser)
{
    node_t *node = createNode();
    node->value.assgn = (assgnNode *)malloc(sizeof(assgnNode));
    node->type = ASSIGN;
    node->value.assgn->dataType = atomN_from_previous(parser);                                                                                                        // cursor = x
    node->value.assgn->identifier = parser_match(parser, TOKEN_ID);                                                                                                   // cursor = x
    node->value.assgn->assignType = parser_match_tokens(parser, 6, TOKEN_EQUALS, TOKEN_ADD_ASGN, TOKEN_SUB_ASGN, TOKEN_MULT_ASGN, TOKEN_INTDIV_ASGN, TOKEN_DIV_ASGN); // cursor 3
    node->value.assgn->expr = comparisonN(parser);
    match(parser, TOKEN_SEMI);
    return node;
}
node_t *comparisonN(parser_t *parser)
{
    node_t *node = inequalityN(parser);
    while (match_tokens(parser, 2, TOKEN_NOT_EQ, TOKEN_EQ_TO))
    {
        node_t *compNode = createNode();
        compNode->value.comp = (comparisonNode *)malloc(sizeof(comparisonNode));
        compNode->value.comp->left = node;
        compNode->value.comp->operation = atomN_from_previous(parser);
        compNode->value.comp->right = inequalityN(parser);
        node = compNode;
        // in cases that matchtokens match a next token, this will still point to the previous compNode, despite compNode being given a new memory allocation
        // so the previous compNode is different from the current compNode
        // this means that compNode->value.ad->left will point to the previous compNode
    }
    return node;
}
node_t *inequalityN(parser_t *parser)
{
    node_t *node = addsubN(parser);
    while (match_tokens(parser, 4, TOKEN_GREATER, TOKEN_GR_THAN_EQ, TOKEN_LESS, TOKEN_LS_THAN_EQ))
    {
        node_t *compNode = createNode();
        compNode->value.ineq = (comparisonNode *)malloc(sizeof(comparisonNode));
        compNode->value.ineq->left = node;
        compNode->value.ineq->operation = atomN_from_previous(parser);
        compNode->value.ineq->right = addsubN(parser);
        node = compNode;
    }
    return node;
}
node_t *addsubN(parser_t *parser)
{
    node_t *node = muldivN(parser);
    while (match_tokens(parser, 2, TOKEN_ADD, TOKEN_SUB))
    {
        node_t *compNode = createNode();
        compNode->value.ad = (addsubNode *)malloc(sizeof(addsubNode));
        compNode->value.ad->left = node;
        compNode->value.ad->operation = atomN_from_previous(parser);
        compNode->value.ad->right = muldivN(parser);
        node = compNode;
    }
    return node;
}
node_t *muldivN(parser_t *parser)
{
    node_t *node = unaryN(parser);
    while (match_tokens(parser, 2, TOKEN_MULT, TOKEN_DIV))
    {
        node_t *compNode = createNode();
        compNode->value.md = (muldivNode *)malloc(sizeof(muldivNode));
        compNode->value.md->left = node;
        compNode->value.md->operation = atomN_from_previous(parser);
        compNode->value.md->right = unaryN(parser);
        node = compNode;
    }
    return node;
}
node_t *unaryN(parser_t *parser)
{
    while (match_tokens(parser, 1, TOKEN_NEGATE, TOKEN_SUB))
    {
        node_t *compNode = createNode();
        compNode->value.unary = (unaryNode *)malloc(sizeof(unaryNode));
        compNode->value.unary->operation = atomN_from_previous(parser);
        compNode->value.unary->token = literalTerm(parser);
        return compNode;
    }
    return literalTerm(parser);
}
node_t *literalTerm(parser_t *parser)
{
    if (check_Token(parser, TOKEN_LPAREN))
    {
        match(parser, TOKEN_LPAREN);
        node_t *node = comparisonN(parser);
        match(parser, TOKEN_RPAREN);
        return node;
    }
    else if (match_tokens(parser, 2, TOKEN_ID, TOKEN_NUM))
    {
        return atomN_from_previous(parser);
    }

    return errorN(parser, "Unexpected Token ");
}

node_t *atomN(parser_t *parser)
{
    node_t *node = createNode();
    node->value.atom = (tokenNode *)malloc(sizeof(tokenNode));
    node->type = ATOM;
    node->value.atom->nodeToken = parser->token;
    printf("ATOM: %s\n", node->value.atom->nodeToken->value);
    parser_advance(parser);
    return node;
}
node_t *atomN_from_previous(parser_t *parser)
{
    node_t *node = createNode();
    node->value.atom = (tokenNode *)malloc(sizeof(tokenNode));
    node->type = ATOM;
    node->value.atom->nodeToken = parser_previous_peek(parser);
    printf("ATOM: %s\n", node->value.atom->nodeToken->value);
    return node;
}
node_t *errorN(parser_t *parser, char *errorMsg)
{
    printf("Error: %s on %s\n", errorMsg, parser_peek(parser)->value);
    node_t *node = createNode();
    node->value.error = (errorNode *)malloc(sizeof(errorNode));
    node->type = ERROR;
    node->value.error->error = errorMsg;
    node->value.error->token = parser->token;
    return node;
}

void parser_advance(parser_t *parser)
{
    parser->cursor++;
    parser->token = parser->tokens[parser->cursor];
}
token_t *parser_peek(parser_t *parser)
{
    return parser->token;
}
token_t *parser_advance_peek(parser_t *parser)
{
    token_t *token = parser->tokens[parser->cursor + 1];
    return token;
}
token_t *parser_previous_peek(parser_t *parser)
{
    token_t *token = parser->tokens[parser->cursor - 1];
    return token;
}

node_t *parser_match(parser_t *parser, type token_to_match)
{
    if (nullCursor(parser))
    {
        printf("Parser Match mismatch %s, expected at: Line: %d Character: %d\n", token_type[token_to_match],
               parser_previous_peek(parser)->lpos,
               parser_previous_peek(parser)->cpos + 1);
        exit(-3);
    }
    else if (parser->token->type == token_to_match)
    {
        return atomN(parser);
    }
    else
    {
        return errorN(parser, "Token Mismatch");
    }
}
node_t *parser_match_tokens(parser_t *parser, int count, ...)
{
    va_list args;
    va_start(args, count);
    type tok;
    for (int i = 0; i < count; i++)
    {
        tok = va_arg(args, type);
        if (nullCursor(parser))
        {
            printf("Missing %s, expected at: Line: %d Character: %d\n", token_type[tok],
                   parser_previous_peek(parser)->lpos,
                   parser_previous_peek(parser)->cpos + 1);
            exit(-3);
        }
        else if (parser->token->type == tok)
        {
            parser_advance(parser);
            return atomN_from_previous(parser);
        }
    }
    return errorN(parser, "Error Node created");
}
int nullCursor(parser_t *parser)
{
    if (parser->token == (void *)0)
    {
        return 1;
    }
    return 0;
}
int match(parser_t *parser, type token_to_match)
{

    if (nullCursor(parser))
    {
        printf("Mismatch Expecting %s, expected at: Line: %d Character: %d\n", token_type[token_to_match],
               parser_previous_peek(parser)->lpos,
               parser_previous_peek(parser)->cpos + 1);
        exit(-3);
    }
    else if ((parser_peek(parser)->type == token_to_match))
    {
        printf("DELT: %s\n", parser_peek(parser)->value);
        parser_advance(parser);
        return 1;
    }
    else
    {
        printf("Expected %s, got %s\n", token_type[token_to_match], parser->token->value);
        exit(-5);
    }
}
int match_tokens(parser_t *parser, int count, ...)
{
    va_list args;
    va_start(args, count);
    type tok;
    for (int i = 0; i < count; i++)
    {
        tok = va_arg(args, int);
        if (nullCursor(parser))
        {
            // printf("Missing %s, expected at: Line: %d Character: %d\n", token_type[type],
            //        parser_previous_peek(parser)->lpos,
            //        parser_previous_peek(parser)->cpos + 1);
            // exit(-3);
            return 0;
        }
        else if ((parser_peek(parser)->type == tok))
        {
            parser_advance(parser);
            return 1;
        }
    }
    return 0;
}
int check_Token(parser_t *parser, type token_to_match)
{
    if (nullCursor(parser))
    {
        printf("Check Failed %s, expected at: Line: %d Character: %d\n", token_type[token_to_match],
               parser_previous_peek(parser)->lpos,
               parser_previous_peek(parser)->cpos + 1);
        exit(-3);
    }
    else if ((parser_peek(parser)->type == token_to_match))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
