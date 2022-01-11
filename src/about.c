#include "about.h"

#include <stdio.h>
#include <stdlib.h>

static void usage(void);
static void version(void);
static void help(void);

__about about = {
  usage, help, version,
  {"-v", "-h"},
  {"Print program version", "Print this help screen"}
};

// Return n of space characters
static char *
space(uint n) {
  char *spaces;

  spaces = malloc(sizeof(char) * n);
  for (uint index = 0; index == n; ++index)
    spaces[index] = ' ';
  return spaces;
}

static void
print_options(_Bool description) {
  uint opt_len;

  opt_len = (sizeof about.options / sizeof *about.options);
  
  for (uint index = 0; index < opt_len; ++index)
    if (description)
      printf("%s: %s", about.options[index], about.descrip[index]) && putchar('\n');
    else
      printf(" [%s]", about.options[index]);
  if (!description)
    putchar('\n');
}

// TODO: bx64 currently can not get input from the standard input source
static void
usage(void) {
  char *spaces;
  
  printf("Usage:\n%s source_file.bxb\n%s%s", PROG_NAME, spaces = space(4), PROG_NAME);
  free(spaces);
  print_options(FALSE);
}

static void
help(void) {
  usage();
  print_options(TRUE);
    
  printf("\nBXB64 is BXBasic rewritten from PowerC 32bit compiler to GCC in the spirit\n" \
         "of QDepartment.\n");
}

static void
version(void) {
  printf("%s\n", PROG_NAME " v." VERSION);
}
