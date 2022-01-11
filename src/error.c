#include "error.h"

#include <stdio.h>

/* --- Error Codes Ranges (0..n, n..n') --- */
#define ERR_PARSER 10
#define ERR_SYNTAX 20

#define NEWWAR(message)				\
  fprintf(stderr, "WARNING: %s\n", message);	\
  fatal = TRUE;					\
  break;
#define NEWERR(message)				\
  fprintf(stderr, "ERROR: %s\n", message);	\
  break;

void
a_bort(int code, uint line_index) {
  _Bool fatal;

  fatal = TRUE;
  
  // Detected error type in return code ranges
  if (code <= ERR_PARSER && code > 0) {
    // about.usage();
    fprintf(stderr, "(ParserError) ");
  } else if (code <= ERR_SYNTAX && code > ERR_PARSER) {
    printf("Failed to parse script at line %u.\n", line_index);
    fprintf(stderr, "(SyntaxError) ");
  } else
    fprintf(stderr, "(UnknownError)");
  
  // Specify error description
  switch(code) {
  case 1:
    NEWERR("Too many arguments passed.");
  case 2:
    NEWERR("File not found.");
  case 3:
    NEWWAR("Keywords must be passed as UPPERCASE.");
  case 4:
    NEWWAR("Unknown command passed.");
  default:
    NEWERR("Program aboard, unknown error.");
  }
  
  // Print error code and location (directly in the parser code)
  fprintf(stderr, "(%s:%i) %s has retured status %i.\n", __FILE__, __LINE__, __func__, code);
  
  if (fatal)
    exit(code);
}
