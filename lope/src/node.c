#include "include/parser.h"

node_t *createNode() {
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL) {
        printf("\nFailed to allocate memory!\n");
        exit(-1);
    }
    return node;
}
node_t *programN(parser_t *parser) {
    node_t *node = createNode();
    node->type = PROGRAM;
    node->value.program = (programNode *)malloc(sizeof(programNode));
    node->value.program->statement = stmtN(parser, node);
    return node;
}
node_t *stmtN(parser_t *parser, node_t *parent) {
    node_t *node = createNode();
    node->type = STATEMENT;
    node->value.stmt = (statementNode *)malloc(sizeof(statementNode));
    node_t **nodes = (node_t **)malloc(sizeof(nodeValue *));
    int statement_index = 0;
    while (!nullCursor(parser)) {
        if (match_tokens(parser, 6, TOKEN_BOOL, TOKEN_INT, TOKEN_DBL,
                         TOKEN_FLOAT, TOKEN_CHAR, TOKEN_STR)) {
            nodes[statement_index] = t_AssgnN(parser);
            if (!match(parser, TOKEN_SEMI))
                nodes[statement_index] = errorN(
                    parser, "Missing Semi-colon on Declaration Statement");
        } else if (match_tokens(parser, 2, TOKEN_ID, TOKEN_CAPITAL)) {
            nodes[statement_index] = assgnN(parser);
            if (!match(parser, TOKEN_SEMI))
                nodes[statement_index] = errorN(
                    parser, "Missing Semi-colon on Assignment Statement");
        } else if (match(parser, TOKEN_WHLE)) {
            nodes[statement_index] = whileN(parser);
        } else if (match(parser, TOKEN_IF)) {
            nodes[statement_index] = ifN(parser);
        } else if (match(parser, TOKEN_FOR)) {
            nodes[statement_index] = forN(parser);
        } else if (parent->type != PROGRAM && check(parser, TOKEN_RBRACE)) {
            node->value.stmt->statements = nodes;
            node->value.stmt->stmtCount = statement_index;
            return node;
        } else if (match_tokens(parser, 3, TOKEN_COMMENT,
                                TOKEN_COMMENT_VALUE_MULTI,
                                TOKEN_COMMENT_VALUE_SINGLE)) {
            if (nullCursor(parser) || check(parser, TOKEN_UNKNOWN)) {
                node->value.stmt->statements = nodes;
                node->value.stmt->stmtCount = statement_index;
                return node;
            }
            continue;
        } else {
            nodes[statement_index] =
                errorN(parser, "[Program] Unexpected Token");
            parser_advance(parser);
        }
        statement_index++;
        nodes = realloc(nodes, (statement_index + 1) * sizeof(nodeValue));
    }
    node->value.stmt->statements = nodes;
    node->value.stmt->stmtCount = statement_index;
    return node;
}
node_t *forN(parser_t *parser) {
    node_t *node = createNode();  // FOR
    node->type = FOR;
    node->value.fr = (forNode *)malloc(sizeof(forNode));
    //(int x = 2)
    //(;)
    if (!match(parser, TOKEN_LPAREN))
        return errorN(parser, "[Para] Missing Left Parenthesis");
    if (match_tokens(parser, 6, TOKEN_BOOL, TOKEN_INT, TOKEN_DBL, TOKEN_FLOAT,
                     TOKEN_CHAR, TOKEN_STR)) {
        node->value.fr->variable = t_AssgnN(parser);
    } else if (match(parser, TOKEN_ID)) {
        node->value.fr->variable = assgnN(parser);
    }
    if (!match(parser, TOKEN_SEMI))
        return errorN(parser, "[Para] Missing Semi-colon");
    node->value.fr->condition = orN(parser);
    if (!match(parser, TOKEN_SEMI))
        return errorN(parser, "[Para] Missing Semi-colon");
    if (check_Tokens(parser, 2, TOKEN_INCR, TOKEN_DECR) ||
        (check(parser, TOKEN_ID) &&
         (parser_advance_peek(parser)->type == TOKEN_INCR ||
          parser_advance_peek(parser)->type == TOKEN_DECR)))
        node->value.fr->iterator = unaryN(parser);
    else {
        parser_advance(parser);
        node->value.fr->iterator = assgnN(parser);
    }
    if (!match(parser, TOKEN_RPAREN))
        return errorN(parser, "[Para] Missing Right Paranthesis");

    if (!match(parser, TOKEN_LBRACE))
        return errorN(parser, "[Para] Missing Left Brace");
    node->value.fr->statements = stmtN(parser, node);
    if (!match(parser, TOKEN_RBRACE))
        return errorN(parser, "[Para] Missing Right Brace");
    return node;
}
node_t *whileN(parser_t *parser) {
    node_t *node = createNode();
    node->type = WHILE;
    node->value.whl = (whileNode *)malloc(sizeof(whileNode));

    if (!match(parser, TOKEN_LPAREN))
        return errorN(parser, "[Habang] Missing Left Paranthesis");
    node->value.whl->expr = orN(parser);
    if (!match(parser, TOKEN_RPAREN))
        return errorN(parser, "[Habang] Missing Right Parenthesis");
    if (!match(parser, TOKEN_LBRACE))
        return errorN(parser, "[Habang] Missing Left Bracket");
    node->value.whl->statements = stmtN(parser, node);
    if (!match(parser, TOKEN_RBRACE))
        return errorN(parser, "[Habang] Missing Right Bracket");
    return node;
}
node_t *ifN(parser_t *parser) {
    node_t *node = createNode();
    node->type = IF;
    node->value.f = (ifNode *)malloc(sizeof(ifNode));
    if (!match(parser, TOKEN_LPAREN))
        return errorN(parser, "[Kung] Missing Left Paranthesis");
    node->value.f->condition = orN(parser);
    if (!match(parser, TOKEN_RPAREN))
        return errorN(parser, "[Kung] Missing Right Parenthesis");
    if (!match(parser, TOKEN_LBRACE))
        return errorN(parser, "[Kung] Missing Left Bracket");
    node->value.f->statements = stmtN(parser, node);
    if (!match(parser, TOKEN_RBRACE))
        return errorN(parser, "[Kung] Missing Right Bracket");
    if (match(parser, TOKEN_ELSE))
        node->value.f->elseifns = elseifN(parser);
    else
        node->value.f->elseifns = NULL;
    if (match(parser, TOKEN_ELIF)) {
        if (!match(parser, TOKEN_LBRACE))
            return errorN(parser, "[Kundi] Missing Left Bracket");
        node->value.f->elsestmt = stmtN(parser, node);
        if (!match(parser, TOKEN_RBRACE))
            return errorN(parser, "[Kundi] Missing Right Bracket");
    } else
        node->value.f->elsestmt = NULL;
    return node;
}
node_t *elseifN(parser_t *parser) {
    node_t *node = createNode();
    node->type = ELSEIF;
    node->value.elif = (elseifNode *)malloc(sizeof(elseifNode));
    if (!match(parser, TOKEN_LPAREN))
        return errorN(parser, "[Sakali] Missing Left Paranthesis");
    node->value.elif->condition = orN(parser);
    if (!match(parser, TOKEN_RPAREN))
        return errorN(parser, "[Sakali] Missing Right Parenthesis");
    if (!match(parser, TOKEN_LBRACE))
        return errorN(parser, "[Sakali] Missing Left Bracket");
    node->value.elif->statements = stmtN(parser, node);
    if (!match(parser, TOKEN_RBRACE))
        return errorN(parser, "[Sakali] Missing Right Bracket");
    if (match(parser, TOKEN_ELSE))
        node->value.elif->nextelseif = elseifN(parser);
    else
        node->value.elif->nextelseif = NULL;
    return node;
}
node_t *t_AssgnN(parser_t *parser) {
    node_t *node = createNode();
    node->value.t_Assgn = (typedAssgnNode *)malloc(sizeof(typedAssgnNode));
    node->type = TYPED_ASSIGN;
    node->value.t_Assgn->dataType = atomN_from_previous(parser);
    node->value.t_Assgn->identifier =
        parser_match_tokens(parser, 2, TOKEN_CAPITAL, TOKEN_ID);
    node->value.t_Assgn->assignType = parser_match_tokens(
        parser, 7, TOKEN_EQUALS, TOKEN_ADD_ASGN, TOKEN_SUB_ASGN,
        TOKEN_MULT_ASGN, TOKEN_INTDIV_ASGN, TOKEN_DIV_ASGN, TOKEN_MOD_ASGN);
    node->value.t_Assgn->expr = orN(parser);
    return node;
}
node_t *assgnN(parser_t *parser) {
    node_t *node = createNode();
    node->value.assgn = (assgnNode *)malloc(sizeof(assgnNode));
    node->type = ASSIGN;                                          // cursor = x
    node->value.assgn->identifier = atomN_from_previous(parser);  // cursor = x
    node->value.assgn->assignType = parser_match_tokens(
        parser, 7, TOKEN_EQUALS, TOKEN_ADD_ASGN, TOKEN_SUB_ASGN,
        TOKEN_MULT_ASGN, TOKEN_INTDIV_ASGN, TOKEN_DIV_ASGN, TOKEN_MOD_ASGN);
    node->value.assgn->expr = orN(parser);
    return node;
}
node_t *orN(parser_t *parser) {
    node_t *node = andN(parser);
    while (match(parser, TOKEN_OR)) {
        node_t *rNode = createNode();
        rNode->type = OR;
        rNode->value.comp = (comparisonNode *)malloc(sizeof(comparisonNode));
        rNode->value.comp->left = node;
        rNode->value.comp->operation = atomN_from_previous(parser);
        rNode->value.comp->right = andN(parser);
        node = rNode;
    }
    return node;
}
node_t *andN(parser_t *parser) {
    node_t *node = equalityN(parser);
    while (match(parser, TOKEN_AND)) {
        node_t *ndNode = createNode();
        ndNode->type = AND;
        ndNode->value.comp = (comparisonNode *)malloc(sizeof(comparisonNode));
        ndNode->value.comp->left = node;
        ndNode->value.comp->operation = atomN_from_previous(parser);
        ndNode->value.comp->right = equalityN(parser);
        node = ndNode;
    }
    return node;
}
node_t *equalityN(parser_t *parser) {
    node_t *node = inequalityN(parser);
    while (match_tokens(parser, 2, TOKEN_NOT_EQ, TOKEN_EQ_TO)) {
        node_t *eqNode = createNode();
        eqNode->type = EQUALITY;
        eqNode->value.comp = (comparisonNode *)malloc(sizeof(comparisonNode));
        eqNode->value.comp->left = node;
        eqNode->value.comp->operation = atomN_from_previous(parser);
        eqNode->value.comp->right = inequalityN(parser);
        node = eqNode;
    }
    return node;
}
node_t *inequalityN(parser_t *parser) {
    node_t *node = addsubN(parser);
    while (match_tokens(parser, 4, TOKEN_GREATER, TOKEN_GR_THAN_EQ, TOKEN_LESS,
                        TOKEN_LS_THAN_EQ)) {
        node_t *ineqNode = createNode();
        ineqNode->type = INEQUALITY;
        ineqNode->value.comp = (comparisonNode *)malloc(sizeof(comparisonNode));
        ineqNode->value.comp->left = node;
        ineqNode->value.comp->operation = atomN_from_previous(parser);
        ineqNode->value.comp->right = addsubN(parser);
        node = ineqNode;
    }
    return node;
}
node_t *addsubN(parser_t *parser) {
    node_t *node = muldivN(parser);
    while (match_tokens(parser, 2, TOKEN_ADD, TOKEN_SUB)) {
        node_t *adNode = createNode();
        adNode->type = ADDSUB;
        adNode->value.comp = (comparisonNode *)malloc(sizeof(comparisonNode));
        adNode->value.comp->left = node;
        adNode->value.comp->operation = atomN_from_previous(parser);
        adNode->value.comp->right = muldivN(parser);
        node = adNode;
    }
    return node;
}

node_t *muldivN(parser_t *parser) {
    node_t *node = negateN(parser);
    while (match_tokens(parser, 4, TOKEN_INTDIV, TOKEN_MOD, TOKEN_MULT,
                        TOKEN_DIV)) {
        node_t *mdimNode = createNode();
        mdimNode->type = MULDIV;
        mdimNode->value.comp = (comparisonNode *)malloc(sizeof(comparisonNode));
        mdimNode->value.comp->left = node;
        mdimNode->value.comp->operation = atomN_from_previous(parser);
        mdimNode->value.comp->right = negateN(parser);
        node = mdimNode;
    }
    return node;
}

node_t *negateN(parser_t *parser) {
    while (match(parser, TOKEN_NEGATE)) {
        node_t *node = createNode();
        node->type = UNARY;
        node->value.unary = (unaryNode *)malloc(sizeof(unaryNode));
        node->value.unary->operation = atomN_from_previous(parser);
        node->value.unary->token = literalTerm(parser);
        return node;
    }
    return literalTerm(parser);
}
node_t *unaryN(parser_t *parser) {
    node_t *node = createNode();
    node->type = UNARY;
    node->value.unary = (unaryNode *)malloc(sizeof(unaryNode));
    if (check(parser, TOKEN_ID)) {
        node->value.unary->token = atomN(parser);
        node->value.unary->operation = atomN(parser);
    } else {
        node->value.unary->operation = atomN(parser);
        node->value.unary->token = atomN(parser);
    }

    return node;
}
node_t *literalTerm(parser_t *parser) {
    if (check(parser, TOKEN_LPAREN)) {
        if (!match(parser, TOKEN_LPAREN))
            return errorN(parser, "[Expr] Missing Left Paranthesis here");
        node_t *node = orN(parser);
        if (!match(parser, TOKEN_RPAREN))
            return errorN(parser, "[Expr] Missing Left Paranthesis here");
        return node;
    } else if (match_tokens(parser, 7, TOKEN_ID, TOKEN_NUM, TOKEN_CHAR_LIT,
                            TOKEN_STRING, TOKEN_FLOAT_LIT, TOKEN_BOOLT,
                            TOKEN_BOOLM)) {
        return atomN_from_previous(parser);
    }
    return errorN(parser, "[Expr] Missing variable or expression");
}

node_t *atomN(parser_t *parser) {
    if (nullCursor(parser)) {
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
node_t *atomN_from_previous(parser_t *parser) {
    node_t *node = createNode();
    node->value.atom = (tokenNode *)malloc(sizeof(tokenNode));
    node->type = ATOM;
    node->value.atom->nodeToken = parser_previous_peek(parser);
    // printf("ATOM PREVIOUS: %s\n", node->value.atom->nodeToken->value);
    return node;
}
node_t *errorN(parser_t *parser, char *errorMsg) {
    node_t *node = createNode();
    node->value.error = (errorNode *)malloc(sizeof(errorNode));
    node->type = ERROR;
    node->value.error->error = errorMsg;
    node->value.error->token = parser_previous_peek(parser);
    node->value.error->token->cpos =
        parser_previous_peek(parser)->cpos +
        strlen(parser_previous_peek(parser)->value);
    if (parser->tok == (void *)0) {
        token_t *token = (token_t *)malloc(sizeof(token_t));
        token->cpos = parser_previous_peek(parser)->cpos + 1;
        token->lpos = parser_previous_peek(parser)->lpos;
        token->value = NULL;
        token->type = TOKEN_UNKNOWN;
        node->value.error->token = token;
    }
    return node;
}
