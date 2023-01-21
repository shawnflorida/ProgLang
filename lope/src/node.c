#include "include/parser.h"

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
    node->type = PROGRAM;
    node->value.program = (programNode *)malloc(sizeof(programNode));
    node->value.program->statement = stmtN(parser, node);
    return node;
}

// blockHolder *stmtBlock(parser_t *parser, node_t *parent)
// {
//     blockHolder *block = (blockHolder *)malloc(sizeof(blockHolder));
//     node_t **node = (node_t **)malloc(sizeof(nodeValue *));
//     int statement_index = 0;
//     while (!nullCursor(parser) && parser->tok->type != TOKEN_UNKNOWN)
//     {
//         if (match_tokens(parser, 6, TOKEN_BOOL, TOKEN_INT, TOKEN_DBL, TOKEN_FLOAT, TOKEN_CHAR, TOKEN_STR))
//         {
//             node[statement_index] = assgnN(parser);
//         }
//         else if (check_Tokens(parser, TOKEN_WHLE))
//         {
//             parser_advance(parser);
//             node[statement_index] = whileN(parser);
//         }
//         else if (parent->type != PROGRAM && check_Tokens(parser, TOKEN_RBRACE))
//         {
//             block->block = node;
//             block->count = statement_index;
//             return block;
//         }
//         else if (match_tokens(parser, 3, TOKEN_COMMENT,
//                               TOKEN_COMMENT_VALUE_MULTI,
//                               TOKEN_COMMENT_VALUE_SINGLE))
//         {
//             if (parser_previous_peek(parser)->type == TOKEN_COMMENT_VALUE_MULTI)
//                 printf("MultiComment:\n<\t-\t>\n%s\n<\t-\t>\n", parser_previous_peek(parser)->value);
//             else
//
//                 printf("Comment:\n<\t-\t> %s <\t-\t>\n", parser_previous_peek(parser)->value);
//
//             if (nullCursor(parser) || check_Tokens(parser, TOKEN_UNKNOWN))
//             {
//                 block->block = node;
//                 block->count = statement_index;
//                 return block;
//             }
//             continue;
//         }
//         else
//         {
//             node[statement_index] = atomN(parser);
//         }
//         statement_index++;
//         node = realloc(node, (statement_index + 1) * sizeof(nodeValue));
//     }
//     block->block = node;
//     block->count = statement_index;
//     return block;
// }

// node_t *stmtBlock(parser_t *parser, node_t *parent)
// {
//     node_t *node = createNode();
//     node->value.stmt = (stmtBlockode *)malloc(sizeof(stmtBlockode));
//     node->type = STATEMENT;
//     if (nullCursor(parser))
//     {
//         return node;
//     }
//     else if (match_tokens(parser, 6, TOKEN_BOOL, TOKEN_INT, TOKEN_DBL, TOKEN_FLOAT, TOKEN_CHAR, TOKEN_STR))
//     {
//         node->value.stmt->statement = assgnN(parser);
//     }
//     else if (check_Tokens(parser, TOKEN_WHLE))
//     {
//         parser_advance(parser);
//         node->value.stmt->statement = whileN(parser);
//     }
//     else if (parent->type == WHILE && check_Tokens(parser, TOKEN_RBRACE))
//     {
//         return node;
//     }
//     else
//     {
//         return atomN(parser);
//     }
//     if (parser->tok == (void *)0)
//     {
//         node->value.stmt->nextStatement = NULL;
//     }
//     else
//     {
//         node->value.stmt->nextStatement = stmtBlock(parser, parent);
//     }
//     if ((node->value.stmt->statement == (void *)0) && (node->value.stmt->nextStatement == (void *)0))
//     {
//         free(node);
//     }
//     return node;
// }

node_t *stmtN(parser_t *parser, node_t *parent)
{
    node_t *node = createNode();
    statementNode *stmtNode = (statementNode *)malloc(sizeof(statementNode));
    node->type = STATEMENT;
    node->value.stmt = stmtNode;
    statementNode *block = node->value.stmt;
    node_t **nodes = (node_t **)malloc(sizeof(nodeValue *));
    int statement_index = 0;
    while (!nullCursor(parser) && parser->tok->type != TOKEN_UNKNOWN)
    {
        if (match_tokens(parser, 6, TOKEN_BOOL, TOKEN_INT, TOKEN_DBL, TOKEN_FLOAT, TOKEN_CHAR, TOKEN_STR))
        {
            nodes[statement_index] = assgnN(parser);
        }
        else if (check_Tokens(parser, TOKEN_WHLE))
        {
            parser_advance(parser);
            nodes[statement_index] = whileN(parser);
        }
        else if (parent->type != PROGRAM && check_Tokens(parser, TOKEN_RBRACE))
        {
            block->statements = nodes;
            block->stmtCount = statement_index;
            return node;
        }
        else if (match_tokens(parser, 3, TOKEN_COMMENT,
                              TOKEN_COMMENT_VALUE_MULTI,
                              TOKEN_COMMENT_VALUE_SINGLE))
        {
            // if (parser_previous_peek(parser)->type == TOKEN_COMMENT_VALUE_MULTI)
            //     printf("MultiComment:\n<\t-\t>\n%s\n<\t-\t>\n", parser_previous_peek(parser)->value);
            // else
            //     printf("Comment:\n<\t-\t> %s <\t-\t>\n", parser_previous_peek(parser)->value);

            // if (nullCursor(parser) || check_Tokens(parser, TOKEN_UNKNOWN))
            {
                block->statements = nodes;
                block->stmtCount = statement_index;
                return node;
            }
            continue;
        }
        else
        {
            nodes[statement_index] = errorN(parser, "Unexpected Token Here");
            parser_advance(parser);
        }
        statement_index++;
        nodes = realloc(nodes, (statement_index + 1) * sizeof(nodeValue));
    }
    block->statements = nodes;
    block->stmtCount = statement_index;
    return node;
}
node_t *whileN(parser_t *parser)
{
    node_t *node = createNode();
    node->type = WHILE;
    node->value.whl = (whileNode *)malloc(sizeof(whileNode));
    // node->value.whl->expr = literalTerm(parser);
    if (!match(parser, TOKEN_LPAREN))
        return errorN(parser, "Expecting %s at [Line %d, Pos %d], Got %s");
    node->value.whl->expr = comparisonN(parser);
    if (!match(parser, TOKEN_RPAREN))
        return errorN(parser, "Missing Token here");
    if (!match(parser, TOKEN_LBRACE))
        return errorN(parser, "Missing Token here");
    node->value.whl->statements = stmtN(parser, node);
    if (!match(parser, TOKEN_RBRACE))
        return errorN(parser, "Missing Token here");
    return node;
}
node_t *assgnN(parser_t *parser)
{
    node_t *node = createNode();
    node->value.assgn = (assgnNode *)malloc(sizeof(assgnNode));
    node->type = ASSIGN;
    node->value.assgn->dataType = atomN_from_previous(parser);                                                                                                        // cursor = x
    node->value.assgn->identifier = parser_match(parser, TOKEN_ID);                                                                                                   // cursor = x
    node->value.assgn->assignType = parser_match_tokens(parser, 6, TOKEN_EQUALS, TOKEN_ADD_ASGN, TOKEN_SUB_ASGN, TOKEN_MULT_ASGN, TOKEN_INTDIV_ASGN, TOKEN_DIV_ASGN); // cursor 3
    node->value.assgn->expr = comparisonN(parser);
    if (!match(parser, TOKEN_SEMI))
        return errorN(parser, "Missing Semi-colon on Assignment Statement");
    return node;
}
node_t *comparisonN(parser_t *parser)
{
    node_t *node = inequalityN(parser);
    while (match_tokens(parser, 2, TOKEN_NOT_EQ, TOKEN_EQ_TO))
    {
        node_t *compNode = createNode();
        compNode->type = COMPARISON;
        compNode->value.comp = (comparisonNode *)malloc(sizeof(comparisonNode));
        compNode->value.comp->left = node;
        compNode->value.comp->operation = atomN_from_previous(parser);
        compNode->value.comp->right = inequalityN(parser);
        node = compNode;
    }
    return node;
}
node_t *inequalityN(parser_t *parser)
{
    node_t *node = addsubN(parser);
    while (match_tokens(parser, 4, TOKEN_GREATER, TOKEN_GR_THAN_EQ, TOKEN_LESS, TOKEN_LS_THAN_EQ))
    {
        node_t *ineqNode = createNode();
        ineqNode->type = INEQUALITY;
        ineqNode->value.ineq = (inequalityNode *)malloc(sizeof(inequalityNode));
        ineqNode->value.ineq->left = node;
        ineqNode->value.ineq->operation = atomN_from_previous(parser);
        ineqNode->value.ineq->right = addsubN(parser);
        node = ineqNode;
    }
    return node;
}
node_t *addsubN(parser_t *parser)
{
    node_t *node = muldivN(parser);
    while (match_tokens(parser, 2, TOKEN_ADD, TOKEN_SUB))
    {
        node_t *adNode = createNode();
        adNode->type = ADDSUB;
        adNode->value.ad = (addsubNode *)malloc(sizeof(addsubNode));
        adNode->value.ad->left = node;
        adNode->value.ad->operation = atomN_from_previous(parser);
        adNode->value.ad->right = muldivN(parser);
        node = adNode;
    }
    return node;
}
node_t *muldivN(parser_t *parser)
{
    node_t *node = unaryN(parser);

    while (match_tokens(parser, 2, TOKEN_MULT, TOKEN_DIV))
    {
        node_t *mdNode = createNode();
        mdNode->type = MULDIV;
        mdNode->value.md = (muldivNode *)malloc(sizeof(muldivNode));
        mdNode->value.md->left = node;
        mdNode->value.md->operation = atomN_from_previous(parser);
        mdNode->value.md->right = unaryN(parser);
        node = mdNode;
    }
    return node;
}
node_t *unaryN(parser_t *parser)
{
    while (match_tokens(parser, 1, TOKEN_NEGATE))
    {
        node_t *node = createNode();
        node->type = UNARY;
        node->value.unary = (unaryNode *)malloc(sizeof(unaryNode));
        node->value.unary->operation = atomN_from_previous(parser);
        node->value.unary->token = literalTerm(parser);
        return node;
    }
    return literalTerm(parser);
}
node_t *literalTerm(parser_t *parser)
{
    if (check_Tokens(parser, TOKEN_LPAREN))
    {
        if (!match(parser, TOKEN_LPAREN))
            return errorN(parser, "Missing Token here");
        node_t *node = comparisonN(parser);
        if (!match(parser, TOKEN_RPAREN))
            return errorN(parser, "Missing Token here");
        return node;
    }
    else if (match_tokens(parser, 4, TOKEN_ID, TOKEN_NUM, TOKEN_CHAR_LIT, TOKEN_STRING))
    {
        return atomN_from_previous(parser);
    }
    return errorN(parser, "Unexpected Token ");
}

node_t *atomN(parser_t *parser)
{
    if (nullCursor(parser))
    {
        return errorN(parser, "End of line, expected");
    }
    node_t *node = createNode();
    node->value.atom = (tokenNode *)malloc(sizeof(tokenNode));
    node->type = ATOM;
    node->value.atom->nodeToken = parser->tok;
    // printf("ATOM: %s\n", node->value.atom->nodeToken->value);
    parser_advance(parser);
    return node;
}
node_t *atomN_from_previous(parser_t *parser)
{
    node_t *node = createNode();
    node->value.atom = (tokenNode *)malloc(sizeof(tokenNode));
    node->type = ATOM;
    node->value.atom->nodeToken = parser_previous_peek(parser);
    // printf("ATOM PREVIOUS: %s\n", node->value.atom->nodeToken->value);
    return node;
}
node_t *errorN(parser_t *parser, char *errorMsg)
{
    // if (nullCursor(parser))
    // {
    //     printf("Error: %s", errorMsg);
    // }
    // else
    // {
    //     printf("Error: %s on %s\n", errorMsg, parser_peek(parser)->value);
    // }

    node_t *node = createNode();
    node->value.error = (errorNode *)malloc(sizeof(errorNode));
    node->type = ERROR;
    node->value.error->error = errorMsg;
    node->value.error->token = parser->tok;
    return node;
}
