#include <stdlib.h>
#include "f_header.inc"

/* This program edits all .100 data files and generates weather */
/* statistics for placement in the site.100 file */

/* Globals */
FILE *infp, *outfp, *deffp;

/* MAIN */
void main()
{
  int answer, done, i;

  /* Print title and date */
  printf("\n   DayCent 4.5 File Updating Utility\n");
  printf("            03/01/04\n");

  done = FALSE;

  while (done == FALSE) {
    printf("\n   Enter the number of the file you wish to update:\n");
    printf("    0. quit\n");
    for (i = 1; i < NUMTYPES-2; i++) {
      printf("   %2i. %s.100\n", i, fstr[i]);
    }
    printf("   %2i. %s\n", i, fstr[i]);
    printf("   %2i. %s\n", i+1, fstr[i+1]);
    printf("\n   Enter selection:\n");
    answer = getint();
    if (answer == QUIT) {
      done = TRUE;
    } else if (answer == WTHR) {
      weather("null","null");
    } else if (answer == SOIL) {
      create_soil(SOIL);
    } else if (answer < QUIT || answer >= NUMTYPES) {
      printf("   Invalid response. Please re-enter.\n");
      continue;
    } else {
      submenu(answer);
    }
  }
}
