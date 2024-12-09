#include <stdio.h>
#include <string.h>
#include "error.h"


void milf_error_throw_at_source_position(milf_error_severity_t severity, milf_error_type_t type, size_t row, size_t column, char *content, size_t content_length, int cursor) {

  switch(severity) {
    case ERROR_WARNING:
      printf("WARNING: ");
    break;
    case ERROR_ERROR:
      printf("ERROR: ");
    break;
  }

  switch(type) {
    case ERROR_LEXER_INVALID_TOKEN:
      printf("Invalid token");
    break;
  }

  char *line_start = &content[cursor - column];
  int line_length = 0;

  while(line_start[line_length] != '\n' && content_length > ((cursor - column) + line_length)) {
    line_length++;
  }

  char line[line_length+1];
  line[line_length + 1] = '\0';
  strncpy(line, line_start, line_length);

  printf(" at %zu:%zu\n", row, column);
  printf("%s\n", line);
  for(size_t i = 0; i < column-1; i++) {
    printf(" ");
  }
  printf("^ -- offending character\n");

}
