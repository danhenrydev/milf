#ifndef ERROR_H
#define ERROR_H

#include <stddef.h>

typedef enum {

  ERROR_ERROR,
  ERROR_WARNING

} milf_error_severity_t;

typedef enum {

  ERROR_LEXER_INVALID_TOKEN,

} milf_error_type_t;

void milf_error_throw_at_source_position(milf_error_severity_t severity, milf_error_type_t type, size_t row, size_t column, char *content, size_t content_length, int cursor);

#endif // ERROR_H
