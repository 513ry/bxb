/** bx64.c v.0.1
 * Copyright (C) 2021 Daniel Sierpiński
 *
 ******************************************************************************
 * BX64 is BXBasic rewritten from PowerC 32bit compiler to GCC for            *
 * accessibility, in the spirit of QDepartment.                               *
 *                                                                            *
 * What's a parser?                                                           *
 * BXB64 is using a simple handwritten LR Predictive parser. Please read      *
 * tutor/README.md if you want to learn more about parsers.                   *
 *                                                                            *
 * VMs and Interpreters?                                                      *
 * This project is written as I'm working at vm for building configuration of *
 * video games. To see how it works visit: [vinl#about][#1].                  *
 ******************************************************************************
 * License:
 * BXBasic is a [free software][#2], distributed under The Beer-Ware RG license.
 * Refer to LICENSE file for legal non/sense.
 *
 * Contributors:
 * Daniel <lordrubish> Sierpiński: siery@comic.com
 *
 * [#1](https://gitgud.io/rialto-games/vinl#about)
 * [#2](https://debian.org/intro/free)
 */

/* Program Meta */
#define GCC

/* --- Declare Headers --- */
#include <stdio.h>
// #include <ncurses.h>  // Refactor the conio.h code when needed
#include <sys/io.h>
#include <stdlib.h>  // malloc(), exit(), EXIT_* (add malloc.h for older systems)
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include "bool.h"
#include "error.h"

/* --- Some Top Level Constants --- */
#define BUFSIZE    96
#define SYMSIZE    6
#define TOKEN_LEN  21
#define MAX_ARGS   1

// TODO: Do NOT implement a real stack, at this point an array is enough.
/* --- Global Variables ------------------------------ */
FILE *f_in, *f_out;            // I/O file handlers
char *script_name;             // Processed source-file name
char line[BUFSIZE];            // Currently parsed line
char **p_script;               // Pointer to code array
char **p_lines;                // Pointer to symbol/index array
// char tok_holder[20];           // Token data holder
// char buf_holder[BUFSIZE];      // xstring (print) data holder
char token[TOKEN_LEN];         // The token string
char *xstring;                 // The print string
uint nrows;                    // Number of lines in source file
uint ncolumns = BUFSIZE;       // Dimension of p_prog[rows][columns]
uint line_index;               // Current execution line
uint start_ref, end_ref;       // Pointers to start & end of token

/* --- Program Procedures Protorypes --- */
void debug(void);
void a_bort(int code, uint line_index);
void line_cnt(char *argv[]);
void load_script(void);
void pgm_parser(void);         // A simple Predictive Parser
void get_token(void);          // Line tokenization (Lexical Analyzer)
void parser(void);             // Main token parser

/* --- Language Procedures Prototypes --- */
void beep(void);
void cls(void);
void xstring_array(void);
void get_prnstring(void);
void go_to(void);

// Run menu options and return the condition
static uint8_t
menu_condition(uint argc, char *arg) {
  uint8_t run_prog;

  run_prog = TRUE;
  
  if (argc > MAX_ARGS + 1)
    run_prog = 2;
  else if (argc == 1 || strcmp(arg, about.options[1]) == 0) {
    about.help();
    run_prog = FALSE;
  } else if (strcmp(arg, about.options[0]) == 0) {
    about.version();
    run_prog = FALSE;
  }
  
  return run_prog;
}

// Will return the program 'done?' condition or abort argument count exception
static _Bool
rmenu(uint argc, char *arg) {
  int ab_code;
  uint8_t ret;
  
  ab_code = 1;

  // abort on exception
  if ((ret = menu_condition(argc, arg)) == 2)
    a_bort(ab_code, 0);

  return ret;
}

/* --- Main Procedure --- */
int
main(int argc, char **argv) {
  if (!rmenu(argc, argv[1])) {
    ;;
  } else {
    printf("Running %s..\n", PROG_NAME);
    
    // strcpy(token, argv[1]);
    line_cnt(argv);
    load_script();
    
    /*** Run the parser's main loop ***/
    pgm_parser();
  }
  return EXIT_SUCCESS;
}

// Simple LR parser
void
pgm_parser(void) {
  line_index = 0;
  
  while (line_index < nrows) {
    // start_ref = end_ref = 0;
    get_token();
    parser();
    ++line_index;
  }
}

// Parse the last processed token
void
parser(void) {
  int ab_code;
  
  ab_code = 4;
  
  // TDOO: That conditioning is not efficient at all
  if (strcmp(token, "REM") == 0) {
    ;;
  } else if (strcmp(token, "PRINT") == 0) {
    xstring_array();
    get_prnstring();
  } else if (strcmp(token, "GOTO") == 0) {
    go_to();
  } else if (strcmp(token, "BEEP") == 0) {
    beep();
  } else if (strcmp(token, "CLS") == 0) {
    cls();
  } else if (strcmp(token, "END") == 0) {
    printf("\nProgram execution has ended\n");
    line_index = nrows;
  } else {
    a_bort(ab_code, line_index);
  }
}

// Featch a single key from line and store it in the token array
void
get_token(void) {
  uint pi, ti, slen;
  int ab_code;
  char c;

  ab_code = 3;
  // Position and termination indexes
  pi = ti = 0;
  // Update line index
  strcpy(line, p_script[line_index]);
  slen = strlen(line);
  c = line[pi];
  //printf("BUFFER: %s", p_script[line_index]);
  
  // Point the first uppercase character
  while (!isupper(c) && (pi < slen))
    c = line[++pi];

  // Aboard when no uppercase character found
  if (pi == slen)
    a_bort(ab_code, line_index);
  // Store token to a global table
  while (isupper(c)) {
    token[ti] = c;
    c = line[++pi];
    ++ti;
  }
  // Null terminated token
  token[ti] = '\0';
}

// Fetch lines data from the file to p_script pointer
void
load_script(void) {
  uint index, len, pi;
  char sym_holder[SYMSIZE], c;

  fprintf(stderr, "Loading up script to the program array..\n");
  
  // Allocate data for script and symbol array
  /// This get NOT freed anywhere yet
  /// TODO: Write a GC and remove this loop from here!!!
  p_script = malloc(nrows * sizeof(char *));
  p_lines = malloc(nrows * sizeof(char *));
  for (index = 0; index < nrows; ++index) {
    p_script[index] = malloc(ncolumns * sizeof(char));
    p_lines[index] = malloc(SYMSIZE * sizeof(char));
  }
  
  // --- Start reading conent from the file ---
  f_in = fopen(script_name, "r");
  index = 0;
  while (fgets(line, BUFSIZE, f_in)) {
    // Pass the whole buffer to p_script (program array)
    len = sizeof(line);
   
    strncpy(p_script[index], line, len);
    p_script[index][len] = '\0';         // Terminate the line

    // Store symbols array
    pi = 0;
    c = line[pi];
    while (isdigit(c)) {
      sym_holder[pi] = c;
      c = line[++pi];
    }
    sym_holder[pi] = '\0';               // Terminate the symbol
    strncpy(p_lines[index], sym_holder, SYMSIZE);
    
    ++index;
  }

  printf("Print formated script array:\n");
  printf("--------------------------------\n");
  for (uint iy = 0; iy < nrows; ++iy) {
    printf("%s", p_script[iy]);
  }
  printf("--------------------------------\n");
  
  fclose(f_in);
}

// Count line and setup the environment for the script parser
void
line_cnt(char *argv[]) {
  uint line_counter, fnam_len;
  int ab_code;

  fprintf(stderr, "Setting up the environment..\n");
  
  ab_code = 2;
  line_counter = 0;
  fnam_len = strlen(argv[1]);
  ++fnam_len;
  script_name = malloc(fnam_len * sizeof(char));
  strcpy(script_name, argv[1]);
  f_in = fopen(script_name, "r");
  
  if (f_in == NULL)
    a_bort(ab_code, 0);
  else {
    while (fgets(line, BUFSIZE, f_in))
      ++line_counter;
    fclose(f_in);
  }
  
  nrows = line_counter;
}

void beep(void) {
  printf("[beep]\a\n");
}

void cls(void) {
  printf("[cls]\n");
}

void xstring_array(void) {
  printf("[xstring_array]\n");
}

void get_prnstring(void) {
  printf("[get_prnstring]\n");
}

void go_to(void) {
  printf("[go_to]\n");
}
