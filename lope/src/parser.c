#include "include/parser.h"

void exec_parser(parser_t *parser) {
    node_t *program = programN(parser);
    traverse(program, 0);
    free(parser);
    free(program);
    printf("\nSuccess");
}
parser_t *parser_init(token_t **tokens) {
    parser_t *parser = (parser_t *)malloc(sizeof(parser_t));
    parser->toks = tokens;
    parser->i = 0;
    parser->tok = parser->toks[parser->i];
    return parser;
}

void parser_advance(parser_t *parser) {
    parser->i++;
    parser->tok = parser->toks[parser->i];
}
int nullCursor(parser_t *parser) {
    if (parser->tok == (void *)0) {
        return 1;
    }
    return 0;
}

token_t *parser_peek(parser_t *parser) { return parser->tok; }
token_t *parser_advance_peek(parser_t *parser) {
    token_t *token = parser->toks[parser->i + 1];
    return token;
}
token_t *parser_previous_peek(parser_t *parser) {
    token_t *token = parser->toks[parser->i - 1];
    return token;
}

node_t *parser_match(parser_t *parser, type token_to_match) {
    if (nullCursor(parser)) {
        return errorN(parser, "Token Mismatch");
    } else if (parser->tok->type == token_to_match) {
        return atomN(parser);
    } else {
        return errorN(parser, "Token Mismatch");
    }
}
node_t *parser_match_tokens(parser_t *parser, int count, ...) {
    va_list args;
    va_start(args, count);
    type tok;
    for (int i = 0; i < count; i++) {
        tok = va_arg(args, type);
        if (nullCursor(parser)) {
            return errorN(parser, "End of Line");
        } else if (parser->tok->type == tok) {
            parser_advance(parser);
            return atomN_from_previous(parser);
        }
    }
    return errorN(parser, "Token Mismatch or Missing");
}

node_t *match_node(node_t *node, parser_t *parser, nodeType node_type) {
    if (node->type == node_type) {
        return node;
    }
    return errorN(parser, "Inspection error");
}
node_t *match_nodes(node_t *node, parser_t *parser, int count, ...) {
    va_list args;
    va_start(args, count);
    int tok;
    for (int i = 0; i < count; i++) {
        tok = va_arg(args, int);
        if (node == NULL) {
            return errorN(parser, "Missing expression");
        } else if (node->type == tok) {
            return node;
        }
    }
    return 0;
}

int match(parser_t *parser, type token_to_match) {
    if (nullCursor(parser)) {
        return 0;
    } else if ((parser_peek(parser)->type == token_to_match)) {
        // printf("DELT: %s\n", parser_peek(parser)->value);
        parser_advance(parser);
        return 1;
    } else {
        return 0;
    }
}
int check(parser_t *parser, type token_to_match) {
    if (nullCursor(parser)) {
        return 0;
    } else if ((parser_peek(parser)->type == token_to_match)) {
        return 1;
    } else {
        return 0;
    }
}

int match_tokens(parser_t *parser, int count, ...) {
    va_list args;
    va_start(args, count);
    int tok;
    for (int i = 0; i < count; i++) {
        tok = va_arg(args, int);
        if (nullCursor(parser)) {
            return 0;
        } else if ((parser_peek(parser)->type == tok)) {
            // printf("DELT: %s\n", parser_peek(parser)->value);
            parser_advance(parser);
            return 1;
        }
    }
    return 0;
}
int check_Tokens(parser_t *parser, int count, ...) {
    va_list args;
    va_start(args, count);
    int tok;
    for (int i = 0; i < count; i++) {
        tok = va_arg(args, int);
        if (nullCursor(parser)) {
            return 0;
        } else if ((parser_peek(parser)->type == tok)) {
            return 1;
        }
    }
    return 0;
}

void display_grammar(node_t *node) {
    switch (node->type) {
        case ASSIGN:

            break;

        default:
            break;
    }
}
void traverse_statements(node_t **statements, int count, int depth) {
    indent(depth - 1);
    printf("Statements:\n");
    for (int x = 0; x < count || x > 1000; x++) {
        indent(depth);
        switch (statements[x]->type) {
            case WHILE:
                printf("Habang:\n");
                traverse(statements[x]->value.whl->expr, depth + 1);
                traverse(statements[x]->value.whl->statements, depth + 1);
                break;
            case ASSIGN:
                printf("Assignment:\n");
                traverse(statements[x], depth);
                break;
            case DECLARATION:
                printf("Declaration:\n");
                traverse(statements[x], depth);
                break;
            case TYPED_ASSIGN:
                printf("Declaration:\n");
                traverse(statements[x], depth);
                break;
            case IF:
                printf("Kung:\n");
                traverse(statements[x]->value.f->condition, depth + 1);
                traverse(statements[x]->value.f->statements, depth + 1);
                traverse(statements[x]->value.f->elseifns, depth + 1);
                if (statements[x]->value.f->elsestmt != NULL) {
                    indent(depth + 1);
                    printf("Kundi:\n");
                    traverse(statements[x]->value.f->elsestmt, depth + 2);
                }
                break;

            case FOR:
                printf("Para:\n");
                indent(depth + 1);
                printf("Expression:\n");
                traverse(statements[x]->value.fr->variable, depth + 1);
                traverse(statements[x]->value.fr->condition, depth + 1);
                traverse(statements[x]->value.fr->iterator, depth + 1);
                traverse(statements[x]->value.fr->statements, depth + 1);
                break;
            case ERROR:
                printf("Error:\n");
                traverse(statements[x], depth + 1);
                break;
            case ATOM:
                printf("Constant: %s\n",
                       statements[x]->value.atom->nodeToken->value);
                break;
            case SCAN:
                printf("Scan:\n");
                traverse(statements[x], depth);
                break;
            case PRINT_STR:
                printf("Print:\n");
                traverse(statements[x], depth);
                break;
            case PRINT_VAL:
                printf("Print:\n");
                traverse(statements[x], depth);
                break;
            case PRINT_EXP:
                printf("Print:\n");
                traverse(statements[x], depth);
                break;
            default:
                break;
        }
    }
}
void traverse(node_t *node, int depth) {
    if (node == NULL) {
        return;
    }
    switch (node->type) {
        case PROGRAM:
            indent(depth);
            printf("Program:\n");
            traverse(node->value.program->statement, depth + 1);
            break;
        case STATEMENT:
            indent(depth);
            printf("Statement:\n");
            traverse_statements(node->value.stmt->statements,
                                node->value.stmt->stmtCount, depth + 2);
            break;
        case ERROR:
            indent(depth);
            printf("Error: %s in [Line: %d, Pos: %d] Token:(\'%s\')\n",
                   node->value.error->error, node->value.error->token->lpos,
                   node->value.error->token->cpos,
                   node->value.error->token->value);
            break;
        case ATOM:
            indent(depth);
            printf("Constant: %s\n", node->value.atom->nodeToken->value);
            break;
        case ELSEIF:
            indent(depth);
            printf("Sakali:\n");
            if (node->value.elif == NULL)
                return;
            traverse(node->value.elif->condition, depth + 1);
            traverse(node->value.elif->statements, depth + 1);
            traverse(node->value.elif->nextelseif, depth + 1);
            break;
        case TYPED_ASSIGN:
            traverse(node->value.t_Assgn->dataType, depth + 1);
            traverse(node->value.t_Assgn->identifier, depth + 1);
            traverse(node->value.t_Assgn->assignType, depth + 1);
            traverse(node->value.t_Assgn->expr, depth + 1);
            break;
        case ASSIGN:
            traverse(node->value.assgn->identifier, depth + 1);
            traverse(node->value.assgn->assignType, depth + 1);
            traverse(node->value.assgn->expr, depth + 1);
            break;
        case DECLARATION:
            traverse(node->value.decStmnt->dataType, depth + 1);
            traverse(node->value.decStmnt->identifier, depth + 1);
            break;
        case AND:
        case OR:
        case EQUALITY:
        case INEQUALITY:
        case ADDSUB:
        case MULDIV:
            indent(depth);
            printf("Expression:\n");
            traverse(node->value.comp->left, depth + 1);
            traverse(node->value.comp->operation, depth + 1);
            traverse(node->value.comp->right, depth + 1);
            break;
        case UNARY:
            indent(depth);
            printf("Expression:\n");
            traverse(node->value.unary->operation, depth + 1);
            traverse(node->value.unary->token, depth + 1);
            break;
        case SCAN:
            traverse(node->value.input->stringFormat, depth + 1);
            traverse(node->value.input->varAddress, depth + 1);
            break;
        case PRINT_STR:
            traverse(node->value.printString->stringValue, depth + 1);
            break;
        case PRINT_VAL:
            traverse(node->value.printValue->stringFormat, depth + 1);
            traverse(node->value.printValue->identifier, depth + 1);
            break;
        case PRINT_EXP:
            traverse(node->value.printExpression->stringFormat, depth + 1);
            traverse(node->value.printExpression->expression, depth + 1);
            break;
        default:
            break;
    }
}
void indent(int indent_count) {
    for (int x = 0; x < indent_count - 1; x++) {
        printf("\t");
        if (x == indent_count - 2)
            printf("|");
    }
    if (indent_count > 0)
        printf("-->");
}
void raise_error(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    vprintf(msg, args);
    va_end(args);
}