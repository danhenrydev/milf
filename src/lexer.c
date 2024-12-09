#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "error.h"

milf_lexer_t *milf_lexer_create(char *content, size_t content_length) {

  milf_lexer_t *lexer = malloc(sizeof(milf_lexer_t));

  lexer->content = content;
  lexer->content_length = content_length;
  lexer->cursor = 0;
  lexer->pos.row = 1;
  lexer->pos.col = 1;

  return lexer;
}

void milf_lexer_trim(milf_lexer_t *lexer) {
  while(milf_lexer_cursor_safe(lexer) && isspace(lexer->content[lexer->cursor])) {

    if(lexer->content[lexer->cursor + sizeof(char)] == '/') {
      milf_lexer_seek_cursor(lexer);
      milf_lexer_seek_cursor(lexer);
      while (milf_lexer_cursor_safe(lexer) && lexer->content[lexer->cursor] != '\n') {
        milf_lexer_seek_cursor(lexer);
      }
    }

    if(lexer->content[lexer->cursor] == '\n') {
      lexer->pos.row++;
      lexer->pos.col = 0;
    }

    milf_lexer_seek_cursor(lexer);
  }
}

milf_lexer_token_t milf_lexer_next_token(milf_lexer_t *lexer) {

  milf_lexer_trim(lexer);

  milf_lexer_token_t token = {
    .text = &lexer->content[lexer->cursor],
    .length = 0,
    .type = TOKEN_END
  };
  
  // Check for end of content
  if(!milf_lexer_cursor_safe(lexer)) {
    
    return token;
    
  }

  token.text = &lexer->content[lexer->cursor];

  // check for keyword/identifier
  if(isalpha(lexer->content[lexer->cursor])) {

    token.type = TOKEN_IDENTIFIER;
    token.length++;
    token.pos = lexer->pos;
    milf_lexer_seek_cursor(lexer);

    while(milf_lexer_cursor_safe(lexer) && isalnum(lexer->content[lexer->cursor])) {

      token.length++;
      milf_lexer_seek_cursor(lexer);

    }

    if(milf_lexer_token_is_keyword(token)) {
      token.type = TOKEN_KEYWORD;
    }

    return token;

  }

  if(isnumber(lexer->content[lexer->cursor])) {

    token.type = TOKEN_NUMBER;
    token.length++;
    token.pos = lexer->pos;
    milf_lexer_seek_cursor(lexer);

    while(isnumber(lexer->content[lexer->cursor])) {
      token.length++;
      milf_lexer_seek_cursor(lexer);
    }

    return token;
    
  }



  switch(lexer->content[lexer->cursor]) {

    case '(':
      token.type = TOKEN_PAREN_OPEN;
      token.length = 1;
      token.pos = lexer->pos;
      milf_lexer_seek_cursor(lexer);
      return token;
    case ')':
      token.type = TOKEN_PAREN_CLOSE;
      token.length = 1;
      token.pos = lexer->pos;
      milf_lexer_seek_cursor(lexer);
      return token;
    case '{':
      token.type = TOKEN_CURLY_OPEN;
      token.length = 1;
      token.pos = lexer->pos;
      milf_lexer_seek_cursor(lexer);
      return token;
    case '}':
      token.type = TOKEN_CURLY_CLOSE;
      token.length = 1;
      token.pos = lexer->pos;
      milf_lexer_seek_cursor(lexer);
      return token;
    case '=':
      token.type = TOKEN_EQUAL;
      token.length = 1;
      token.pos = lexer->pos;
      milf_lexer_seek_cursor(lexer);
      return token;
    case ';':
      token.type = TOKEN_SEMICOLON;
      token.length = 1;
      token.pos = lexer->pos;
      milf_lexer_seek_cursor(lexer);
      return token;
    case '<':
      if(lexer->content[lexer->cursor + sizeof(char)] == '=') {
        token.type = TOKEN_LTE;
        token.length = 2;
        token.pos = lexer->pos;
        milf_lexer_seek_cursor(lexer);
        milf_lexer_seek_cursor(lexer);
        return token;
      }
      token.type = TOKEN_LT;
      token.length = 1;
      token.pos = lexer->pos;
      milf_lexer_seek_cursor(lexer);
      return token;
    case '>':
      if(lexer->content[lexer->cursor + sizeof(char)] == '=') {
        token.type = TOKEN_GTE;
        token.length = 2;
        token.pos = lexer->pos;
        milf_lexer_seek_cursor(lexer);
        milf_lexer_seek_cursor(lexer);
        return token;
      }
      token.type = TOKEN_GT;
      token.length = 1;
      token.pos = lexer->pos;
      milf_lexer_seek_cursor(lexer);
      return token;
    case '+':
      if(lexer->content[lexer->cursor + sizeof(char)] == '+') {
        token.type = TOKEN_PLUS_PLUS;
        token.length = 2;
        token.pos = lexer->pos;
        milf_lexer_seek_cursor(lexer);
        milf_lexer_seek_cursor(lexer);
        return token;
      }
      token.type = TOKEN_PLUS;
      token.length = 1;
      token.pos = lexer->pos;
      milf_lexer_seek_cursor(lexer);
      return token;
    case '/':
      token.type = TOKEN_SLASH_FORWARD;
      token.length = 1;
      token.pos = lexer->pos;
      milf_lexer_seek_cursor(lexer);
    case '-':
      if(lexer->content[lexer->cursor + sizeof(char)] == '-') {
        token.type = TOKEN_MINUS_MINUS;
        token.length = 2;
        token.pos = lexer->pos;
        milf_lexer_seek_cursor(lexer);
        milf_lexer_seek_cursor(lexer);
        return token;
      }
      token.type = TOKEN_MINUS;
      token.length = 1;
      token.pos = lexer->pos;
      milf_lexer_seek_cursor(lexer);
      return token;
      
  }
  

  // Did not match any of the token types, return an invalid token
  token.type = TOKEN_INVALID;
  token.text = &lexer->content[lexer->cursor];
  token.pos = lexer->pos;
  milf_lexer_seek_cursor(lexer);

  milf_error_throw_at_source_position(ERROR_WARNING, ERROR_LEXER_INVALID_TOKEN, token.pos.row, token.pos.col, lexer->content, lexer->content_length, lexer->cursor);

  return token;

}

bool milf_lexer_cursor_safe(milf_lexer_t *lexer) {
  return lexer->cursor < lexer->content_length;
}

char *milf_lexer_get_token_name(milf_lexer_token_t token) {
  switch(token.type) {
  case TOKEN_END:
    return "TOKEN_END";
  case TOKEN_INVALID:
    return "TOKEN_INVALID";
  case TOKEN_KEYWORD:
    return "TOKEN_KEYWORD";
  case TOKEN_IDENTIFIER:
    return "TOKEN_IDENTIFIER";
  case TOKEN_NUMBER:
    return "TOKEN_NUMBER";
  case TOKEN_PAREN_OPEN:
    return "TOKEN_PAREN_OPEN";
  case TOKEN_PAREN_CLOSE:
    return "TOKEN_PAREN_CLOSE";
  case TOKEN_CURLY_OPEN:
    return "TOKEN_CURLY_OPEN";
  case TOKEN_CURLY_CLOSE:
    return "TOKEN_CURLY_CLOSE";
  case TOKEN_EQUAL:
    return "TOKEN_EQUAL";
  case TOKEN_SEMICOLON:
    return "TOKEN_SEMICOLON";
  case TOKEN_LT:
    return "TOKEN_LT";
  case TOKEN_GT:
    return "TOKEN_GT";
  case TOKEN_LTE:
    return "TOKEN_LTE";
  case TOKEN_GTE:
    return "TOKEN_GTE";
  case TOKEN_PLUS:
    return "TOKEN_PLUS";
  case TOKEN_PLUS_PLUS:
    return "TOKEN_PLUS_PLUS";
  case TOKEN_MINUS:
    return "TOKEN_MINUS";
  case TOKEN_MINUS_MINUS:
    return "TOKEN_MINUS_MINUS";
  case TOKEN_SLASH_FORWARD:
    return "TOKEN_SLASH_FORWARD";
  case TOKEN_SLASH_FORWARD_DOUBLE:
    return "TOKEN_SLASH_FORWARD_DOUBLE";
  default:
    return "UNIMPLEMENTED";

  }
}

bool milf_lexer_token_is_keyword(milf_lexer_token_t token) {


  // if it's this long, it must be an identifier
  if(token.length >= 30) {
    return false;
  }

  char *keywords[] = {
    "if",
    "else",
    "while",
    "return",
    "int",
    "void"
  };

  char token_text[30];
  strncpy(token_text, token.text, token.length);
  token_text[token.length] = '\0';
  for(size_t i = 0; i < (sizeof(keywords) / sizeof(keywords[0])); i++) {
    if(strcmp(token_text, keywords[i]) == 0) {
      return true;
    }
  }

  return false;
}

void milf_lexer_seek_cursor(milf_lexer_t *lexer) {
  lexer->cursor++;
  lexer->pos.col++;
}
