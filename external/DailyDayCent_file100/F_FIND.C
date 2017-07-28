#include <stdlib.h>
#include "f_header.inc"

/* FIND */
void find(int type, int *num, char *term, char name[][MWORD])
{
  int  i, j;
  char newterm[MWORD+2];

  sprintf (newterm,"'%s'",term);

  for (i = 0; i < lines[type]; i++) {
    for (j = 0; j < MWORD; j++) {
      if (name[i][j] == '(') {
        break;
      }
    }

    if (strncmp(newterm, name[i], j) == 0) {
      *num = i;
      return;
    }
  }

  printf("\n   Name not found.\n");
}
