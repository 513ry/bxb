char *options[] = {"-v", "-h"};
char *describe[] = {"Print program verson", "Print this help screen"};
uint opt_len = (sizeof options / sizeof *options);

// bxb64 just parses files for now
void usage(void) {
  printf("Usage: bxb64 source_file.bxb\n");
  for (uint index = 0; index < opt_len; ++index)
    printf("       bxb64 %s\n", options[index]);
}

void version(void) {
  printf("%s\n", PROG_NAME);
}

void help(void) {
  usage();
  for (uint index = 0; index < opt_len; ++index)
    printf("%s: %s\n", options[index], describe[index]);
  printf("\nBXB64 is BXBasic rewritten from PowerC 32bit compiler to GCC in the spirit\n" \
         "of QDepartment.\n");
}
