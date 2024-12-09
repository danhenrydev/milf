#include <stdio.h>
#include <string.h>
#include "common.h"
#include "lexer.h"

int main(void) {

  char file[] = "./test.milf";
  char *content = milf_read_file(file); 

  if(content == NULL) {
    printf("Error: could not open file %s", file);
    return 1;
  }

  milf_lexer_t *lexer = milf_lexer_create(content, strlen(content));

  static milf_lexer_token_t empty_token = {
    .type = TOKEN_INVALID,
    .text = 0,
    .length = 0,
    .pos.row = 0,
    .pos.col = 0
  };

  milf_lexer_token_t token = milf_lexer_next_token(lexer);
  while(token.type != TOKEN_END) {

    printf("Found token '%.*s' of type: %s on line %d at col %d\n", token.length, token.text, milf_lexer_get_token_name(token), token.pos.row, token.pos.col);
    token = empty_token;
    token = milf_lexer_next_token(lexer);


  }


  return 0;
}
