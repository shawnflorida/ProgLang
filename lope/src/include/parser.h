#ifndef PARSER
#define PARSER

#include "fileHandler.h"
#include "node.h"

// Parser struct
struct parser_t
{
    token_t **toks; // token stream
    token_t *tok;   // current token
    int i;          // index of the parser
};

/**
 * @brief initializes the parser struct to be used
 *
 * @param tokens token stream
 * @return parser_t*
 */
parser_t *parser_init(token_t **tokens);
// starts the parsing with a program node as the root
void exec_parser(parser_t *parser);
/**
 * @brief advances the cursor
 * */
void parser_advance(parser_t *parser);
/**
 * !! @brief for debugging, not yet implemented
 * */
void raise_error(const char *msg, ...);

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
 * @brief used for matching nodes, useful for statements that needs logical evaluation
 *  such as while who doesn't want an addition, though it works in other languages
 * @param node
 * @param node_type
 * @return int
 */
node_t *match_node(node_t *node, parser_t *parser, nodeType node_type);
/**
 * @brief used for matching nodes, useful for statements that needs logical evaluation
 *  such as while who doesn't want an addition, though it works in other languages
 * @param node
 * @param count amount of nodes to be matched
 * @param ... the node types to be matched (must be an int)
 * @return int
 */
node_t *match_nodes(node_t *node, parser_t *parser, int count, ...);
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
 * @brief checks only doesn't consume and doesn't advance
 * @param token_to_match type of token
 */
int check(parser_t *parser, type token_to_match);
/**
 * @brief checks only doesn't consume and doesn't advance
 * @param token_to_match type of token
 */
int check_Tokens(parser_t *parser, type token_to_match);
// check if eoF
int nullCursor(parser_t *parser);

// For traversing/semantic checking

/**
 * !! @brief Not used yet
 *
 * @param node
 */
void display_grammar(node_t *node);
/**
 * @brief traverses the statement node, useful for traversing the tree
 * statement[x] type must be considered as a statement based on grammar rule
 *
 * @param statements
 */
void traverse_statements(node_t **statements, int count, int depth);
/**
 * @brief traverses a node
 *
 * @param node
 */
void traverse(node_t *node, int depth);
/**
 * @brief used for displaying the parse tree, indents according to the depth provided
 *
 * @param indent_count
 */
void indent(int indent_count);
#endif