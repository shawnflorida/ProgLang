#ifndef NODES
#define NODES

#include "token.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"

typedef struct node_t node_t;
typedef struct parser_t parser_t;
// Nodetype enums
typedef enum
{
    ASSIGN,
    PROGRAM,
    STATEMENT,
    COMPARISON,
    INEQUALITY,
    ADDSUB,
    MULDIV,
    UNARY,
    WHILE,
    ERROR,
    ATOM
} nodeType;

typedef struct
{
    node_t **statements;
    int stmtCount;
} statementNode;
typedef struct
{
    node_t *statement;
} programNode;
typedef struct
{
    char *error;
    token_t *token;
} errorNode;
typedef struct
{
    token_t *nodeToken;
} tokenNode;
typedef struct
{
    node_t *expr;
    node_t *statements;
} whileNode; // *WHILE
typedef struct
{
    node_t *operation;
    node_t *token;
} unaryNode; // *UNARY
typedef struct
{
    node_t *left;      // expr
    node_t *operation; // tokenNode
    node_t *right;     // expr
} comparisonNode, inequalityNode, addsubNode, muldivNode;
typedef struct
{
    node_t *dataType;
    node_t *identifier;
    node_t *assignType;
    node_t *expr;
} assgnNode;

// typedef struct
// {
//     node_t **block;
//     int count;
// } blockHolder;
// typedef struct
// {
//     node_t *statements;
// } programNode;

typedef union
{
    programNode *program;
    statementNode *stmt;
    whileNode *whl;
    assgnNode *assgn;
    comparisonNode *comp;
    inequalityNode *ineq;
    muldivNode *md;
    addsubNode *ad;
    unaryNode *unary;
    tokenNode *atom;
    errorNode *error;
} nodeValue;

struct node_t
{
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
// assign = [TOKEN_TYPE] [TOKEN_ID] [TOKEN_ANY_ASSIGN] <comparison> [TOKEN_SEMI]
node_t *assgnN(parser_t *parser);
// comparison = inequality ([== | !=] inequality)*
node_t *comparisonN(parser_t *parser);
// inequality = addsub ([+ | -] addsub)*
node_t *inequalityN(parser_t *parser);
// addsub = muldiv ([* | /] muldiv)*
node_t *addsubN(parser_t *parser);
// muldiv = unary ([! | -]unary)*
node_t *muldivN(parser_t *parser);
// unary = literal (literal)*
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
 * @brief generates the token node, doesn't use parserAdvance, useful when parser_match() is used
 * @param parser
 * @return atomNode
 */
node_t *atomN_from_previous(parser_t *parser);

#endif