#ifndef PARSER
#define PARSER

#include "token.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"

typedef struct node_t node_t;
// Parser struct
typedef struct
{
    token_t **tokens;
    token_t *token;
    int cursor;
} parser_t;

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
    char *error;
    token_t *token;
} errorNode;

typedef struct
{
    token_t *nodeToken;
} tokenNode;

typedef struct
{
    node_t *keyword; // keyword while, should be consumed?
    node_t *expr;
    node_t *statements;
} whileNode;

typedef struct
{
    node_t *operation;
    node_t *token;
} unaryNode;

typedef struct
{
    node_t *left;      // expr
    node_t *operation; // tokenNode
    node_t *right;     // expr
} comparisonNode, inequalityNode, addsubNode, muldivNode;
// operation must be == or != ... if '(' or ')' exists, they are consumed

typedef struct
{
    node_t *dataType;
    node_t *identifier;
    node_t *assignType;
    node_t *expr;
} assgnNode;

typedef struct
{
    node_t *statement;
    node_t *nextStatement;
} statementNode;

typedef struct
{
    node_t *statements;
} programNode, blockNode;

typedef union
{
    programNode *program;
    statementNode *stmt;
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
// statments = statement | statements

// initializes the parser struct to be used
parser_t *parser_init(token_t **tokens);
// starts the parser
void exec_parser(parser_t *parser);
/**
 * @brief advances the cursor
 * */
void parser_advance(parser_t *parser);
/**
 * @brief for debugging, not yet implemented
 * */
void raise_error(char *msg);

/**
 * @brief generates an atom node that advances when match one of the tokens
 *
 * @param count count of tokens to match (used in vargs)
 * @param ...  tokens to be matched, seperated by commas
 */
node_t *parser_match_tokens(parser_t *parser, int count, ...);
/**
 * @brief
 *
 * @param token_to_match type of token
 * */
node_t *parser_match(parser_t *parser, type token_to_match);

// Helper methods for checking tokens

//
/**
 * @brief  to get the current token
 *
 * @param parser
 * @return token_t*
 */
token_t *parser_peek(parser_t *parser);
// to get the token ahead
token_t *parser_advance_peek(parser_t *parser);
// to get the previous token
token_t *parser_previous_peek(parser_t *parser);

// helper methods to advance and check the cursor's value

/**
 * @brief advances and consumes the token. returns true if match, exits the program if not and raises an error
 *
 * @param token_to_match type of token
 * */
int match(parser_t *parser, type token_to_match);

/**
 * @brief advances and consumes the token, retursn true if match, else false
 *
 * @param parser parser
 * @param count count of tokens to match (used in vargs)
 * @param ... tokens to be matched, seperated by commas
 * @return 0 - false; 1 - true
 *
 */
int match_tokens(parser_t *parser, int count, ...);
/**
 * @brief checks only doesn't consume and advance
 * @param token_to_match type of token
 */
int check_Token(parser_t *parser, type token_to_match);
// check if eoF
int nullCursor(parser_t *parser);

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
// statements = assignStatement
node_t *statementN(parser_t *parser);
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

// Sample: bolyan y = 3 + (2-1);
//                              program
//                             statement
//              assgnNode                       nextStatement
//      bolyan  y   =   addsub                        null
//               atom     +     addsub
//                3          atom  -  atom
//                             2        1

//  because we are adding, the expr from assignment => type id assgn expr semi
//  will eventually reach a node addsub
//  and since we use pointers what happens to 3 is that from comparison node will eventually point to an addsubNode,
//  the left of addsubnode is equal to muldiv until literal, where it will point to an atomnode
//
//  for 2, the right will eventually reach also the literal node, only it has a left paren which will recursively put it in to another
//  branch, that will also reach an addsubNode, the left once again will become an atom = 2 and the right being an atom will also become atom = 3
#endif