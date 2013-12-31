/**
 * @file error.c
 * @author Joe Wingbermuehle
 */

#include "xdiamonds.h"
#include "error.h"

/** Display a warning to the console. */
void Warning(const char *str, ...) {
   va_list ap;
   va_start(ap, str);

   fprintf(stderr, "xdiamonds: warning: ");
   vfprintf(stderr, str, ap);
   fprintf(stderr, "\n");

   va_end(ap);
}

