#ifndef NODES
#define NODES

#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "token.h"

typedef struct node_t node_t;
typedef struct parser_t parser_t;
// Nodetype enums
typedef enum {
    TYPED_ASSIGN,
    ASSIGN,
    PROGRAM,
    STATEMENT,
    EQUALITY,
    INEQUALITY,
    ADDSUB,
    MULDIV,
    UNARY,
    AND,
    OR,
    IF,
    ELSEIF,
    WHILE,
    FOR,
    ERROR,
    EMPTY,
    SCAN,
    PRINT_STR,
    PRINT_VAL,
    PRINT_EXP,
    DECLARATION,
    ATOM
} nodeType;
typedef struct {
    node_t **statements;
    int stmtCount;
} statementNode;
typedef struct {
    node_t *statement;
} programNode;
typedef struct {
    char *error;
    token_t *token;
} errorNode;
typedef struct {
    token_t *nodeToken;
} tokenNode;
typedef struct {
    node_t *variable;
    node_t *condition;
    node_t *iterator;
    node_t *statements;
} forNode;
typedef struct {
    node_t *expr;
    node_t *statements;
} whileNode;  // *WHILE
typedef struct {
    node_t *operation;
    node_t *token;
} unaryNode;  // *UNARY
typedef struct {
    node_t *left;       // expr
    node_t *operation;  // tokenNode
    node_t *right;      // expr
} comparisonNode;
typedef struct {
    node_t *dataType;
    node_t *identifier;
    node_t *assignType;
    node_t *expr;
} declarationNode;
typedef struct {
    node_t *identifier;
    node_t *assignType;
    node_t *expr;
} assgnNode;
typedef struct {
    node_t *condition;
    node_t *statements;
    node_t *elseifns;
    node_t *elsestmt;
} ifNode;
typedef struct {
    node_t *condition;
    node_t *statements;
    node_t *nextelseif;
} elseifNode;

typedef struct {
    node_t *dataType;
    node_t *identifier;
} initializationNode;
typedef struct {
    node_t *stringFormat;
    node_t *varAddress;
} scanNode;
typedef struct {
    node_t *stringValue;
} printStringNode;
typedef struct {
    node_t *stringFormat;
    node_t *identifier;
} printValueNode;
typedef struct {
    node_t *stringFormat;
    node_t *expression;
} printExp;

typedef union {
    programNode *program;
    statementNode *stmt;
    whileNode *whl;
    forNode *fr;
    assgnNode *assgn;
    comparisonNode *comp;
    unaryNode *unary;
    tokenNode *atom;
    errorNode *error;
    ifNode *f;
    elseifNode *elif;
    initializationNode *decStmnt;
    declarationNode *t_Assgn;
    scanNode *input;
    printStringNode *printString;
    printValueNode *printValue;
    printExp *printExpression;
} nodeValue;

struct node_t {
    nodeType type;
    nodeValue value;
};

// Generate the nodes

// allocates a memory for the node
node_t *createNode();

/**
 * @brief generates an errorNode, replaces the expected Node
 * @warning avoid usage for delimeters < , [] {} () ; >
 * @param parser
 * @param errorMsg message
 * @return errorNode
 */
node_t *errorN(parser_t *parser, char *errorMsg);

// program = statements
node_t *programN(parser_t *parser);
/**
 * @brief returns an array of statements, used in nodes:
 *        [program, while, for, if, else, elseif]
 *
 * @param parser
 * @param parent used for identifying if it is a block or the main program
 * @return node_t**
 */
node_t *stmtN(parser_t *parser, node_t *parent);
node_t *whileN(parser_t *parser);
node_t *forN(parser_t *parser);
node_t *ifN(parser_t *parser);
node_t *elseifN(parser_t *parser);
node_t *initializationN(parser_t *parser);
node_t *declarationN(parser_t *parser);
node_t *scanN(parser_t *parser);
node_t *printN(parser_t *parser);
// assign = [TOKEN_TYPE] [TOKEN_ID] [TOKEN_ANY_ASSIGN] <comparison> [TOKEN_SEMI]
node_t *assgnN(parser_t *parser);
// comparison = inequality ([== | !=] inequality)*
node_t *orN(parser_t *parser);
node_t *andN(parser_t *parser);
node_t *equalityN(parser_t *parser);
// inequality = addsub ([+ | -] addsub)*
node_t *inequalityN(parser_t *parser);
// addsub = muldiv ([* | /] muldiv)*
node_t *addsubN(parser_t *parser);
// muldiv = unary ([! | -]unary)*
node_t *muldivN(parser_t *parser);
// unary = literal (literal)*
node_t *negateN(parser_t *parser);
node_t *unaryN(parser_t *parser);
// literal = atom | comparison
node_t *literalTerm(parser_t *parser);

// Generates the token nodes

/**
 * @brief generates the token node and advances
 * @param parser
 * @return atomNode
 */
node_t *atomN(parser_t *parser);
/**
 * @brief generates the token node, doesn't use parserAdvance, useful when
 * parser_match() is used
 * @param parser
 * @return atomNode
 */
node_t *atomN_from_previous(parser_t *parser);

#endif