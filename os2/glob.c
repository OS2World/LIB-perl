/*
 * Globbing for OS/2.  Relies on the expansion done by the library
 * startup code.
 */

#include <stdlib.h>
#include <io.h>

int main(int argc, char **argv)
{
  int i;
  char *f;

  _response(&argc, &argv);
  _wildcard(&argc, &argv);

  for (i = 1; i < argc; i++)
    write(1, argv[i], strlen(argv[i]) + 1);

  return 0 /* argc - 1 */;
}
