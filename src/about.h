#ifndef _About_h
#define _About_h

#include "bool.h"
//#import "get_version.h"

/* --- Some TopLevel Variables --- */
#define VERSION "0.1"
#define PROG_NAME "bx64"

typedef struct {
  void
  (*usage)(void);

  void
  (*help)(void);

  void
  (*version)(void);

  const char *options[2];
  const char *descrip[2];
} __about;

extern __about about;

#endif // _About_h
