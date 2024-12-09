#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>


/**
 * @brief Token types used by the lexer
 *
 * Enumeration of all possible token types that can be identified by the lexer.
 * Each token type represents a distinct lexical element in the source code.
 *
 * @note TOKEN_END represents the end of input, TOKEN_INVALID represents an invalid token
 */
typedef enum {

  TOKEN_END = 0,
  TOKEN_INVALID,
  TOKEN_KEYWORD,
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_PAREN_OPEN,
  TOKEN_PAREN_CLOSE,
  TOKEN_CURLY_OPEN,
  TOKEN_CURLY_CLOSE,
  TOKEN_EQUAL,
  TOKEN_SEMICOLON,
  TOKEN_LT,
  TOKEN_GT,
  TOKEN_LTE,
  TOKEN_GTE,
  TOKEN_PLUS,
  TOKEN_PLUS_PLUS,
  TOKEN_MINUS,
  TOKEN_MINUS_MINUS,
  TOKEN_SLASH_FORWARD,
  TOKEN_SLASH_FORWARD_DOUBLE,


} milf_lexer_token_type_t;

/**
 * @brief Position tracking structure for the lexer
 * 
 * Keeps track of the current row and column position in the source code
 * being lexed. Used for error reporting and debugging.
 *
 * @note Row and column numbers are 1-indexed
 */
typedef struct {

  int row;
  int col;
  
} milf_lexer_position_t;

/**
 * @brief Token structure containing lexical information
 * 
 * Represents a single token identified by the lexer, containing the token's
 * text content, length, type, and position in the source code.
 *
 * @field text The string content of the token
 * @field length The length of the token text in characters
 * @field type The type classification of the token
 * @field pos The position of the token in the source code
 */
typedef struct {

  char *text;
  int length;
  milf_lexer_token_type_t type;
  milf_lexer_position_t pos;

} milf_lexer_token_t;

/**
 * @brief Main lexer structure for tracking lexical analysis state
 * 
 * Contains the input content being lexed, cursor position tracking,
 * and current position information for error reporting.
 *
 * @field content The input text content being lexed
 * @field content_length The total length of the input content
 * @field cursor Current position in the content being processed
 * @field pos Current row/column position tracking
 *
 */
typedef struct {

  char *content;
  size_t content_length;
  size_t cursor;
  milf_lexer_position_t pos;

} milf_lexer_t;


/**
 * @brief Creates a new lexer instance
 * 
 * Allocates and initializes a new lexer with the provided content and cursor at position 0.
 * Caller is responsible for freeing the memory.
 *
 * @param content The input text content to be lexed
 * @param content_length The length of the input content
 * @return milf_lexer_t* Pointer to the newly created lexer instance
 *
 * @example
 * milf_lexer_t *lexer = milf_lexer_create("hello world", 11);
 * // lexer->content = "hello world"
 * // lexer->cursor = 0
 */
milf_lexer_t *milf_lexer_create(char *content, size_t content_length);


/**
 * @brief Trims whitespace from the current lexer position
 * 
 * Advances the lexer cursor past any whitespace characters (spaces, tabs, newlines)
 * at the current position.
 *
 * @param lexer Pointer to the lexer instance to trim whitespace from
 *
 * @example
 * milf_lexer_t lexer = {
 *   .content = "   hello",
 *   .cursor = 0
 * };
 * milf_lexer_trim(&lexer); // cursor will be at position 3, pointing to 'h'
 */
void milf_lexer_trim(milf_lexer_t *lexer);

/**
 * @brief Gets the next token from the lexer
 * 
 * Reads and returns the next token from the current lexer position,
 * advancing the cursor past the token.
 *
 * @param lexer Pointer to the lexer instance to get the next token from
 * @return milf_token_t The next token in the input
 *
 * @example
 * milf_lexer_t lexer = {
 *   .content = "hello world",
 *   .cursor = 0
 * };
 * milf_token_t token = milf_lexer_next_token(&lexer); // token.text will be "hello"
 */
milf_lexer_token_t milf_lexer_next_token(milf_lexer_t *lexer);

/**
 * @brief Checks if the lexer cursor is at a valid position
 * 
 * Verifies that the lexer cursor has not reached the end of the content
 * and is still within valid bounds for reading.
 *
 * @param lexer Pointer to the lexer instance to check
 * @return bool True if cursor is at a valid position, false otherwise
 *
 * @example
 * milf_lexer_t lexer = {
 *   .content = "hello",
 *   .cursor = 0
 * };
 * bool safe = milf_lexer_cursor_safe(&lexer); // returns true
 * lexer.cursor = 5;
 * safe = milf_lexer_cursor_safe(&lexer); // returns false
 */
bool milf_lexer_cursor_safe(milf_lexer_t *lexer);

/**
 * @brief Gets the string name of a token type
 * 
 * Returns a human-readable string representation of the token type.
 * The returned string is statically allocated and should not be freed.
 *
 * @param token The token to get the name for
 * @return char* String representation of the token type
 *
 * @example
 * milf_lexer_token_t token = {
 *   .type = TOKEN_KEYWORD,
 *   .text = "if",
 *   .length = 2
 * };
 * char *name = milf_lexer_get_token_name(token); // returns "KEYWORD"
 */
char *milf_lexer_get_token_name(milf_lexer_token_t token);

/**
 * @brief Checks if a token is a keyword
 * 
 * Verifies if the given token represents a language keyword by comparing
 * its text against the list of defined keywords.
 *
 * @param token The token to check
 * @return bool True if the token is a keyword, false otherwise
 *
 * @example
 * milf_lexer_token_t token = {
 *   .type = TOKEN_KEYWORD,
 *   .text = "if",
 *   .length = 2
 * };
 * bool is_keyword = milf_lexer_token_is_keyword(token); // returns true
 */
bool milf_lexer_token_is_keyword(milf_lexer_token_t token);

/**
 * @brief Advances the lexer cursor by one character
 * 
 * Moves the lexer cursor forward by one character and updates the column
 * position
 *
 * @param lexer Pointer to the lexer instance to advance the cursor for
 *
 * @example
 * milf_lexer_t lexer = {
 *   .content = "a\nb",
 *   .cursor = 0,
 *   .pos = {.row = 1, .col = 1}
 * };
 * milf_lexer_seek_cursor(&lexer); // cursor will be at position 1
 *                                 // pos will be {.row = 1, .col = 2}
 */
void milf_lexer_seek_cursor(milf_lexer_t *lexer);


#endif //LEXER_H
