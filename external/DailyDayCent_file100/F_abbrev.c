#include <stdlib.h>
#include "f_header.inc"

#define MAXABBREVS  50

/* Globals */
extern FILE *infp;
 
/* Local Globals */
char saveabbrev[MAXABBREVS][SWORD], savedescript[MAXABBREVS][BWORD];

/* GETABBREVS    */
void getabbrevs(int type, int *head, int headlines[],
                char abbrev[], char descript[],
                char value[][MWORD], char name[][MWORD],
                char heading[][BWORD])
{
  int i;

  for (i=0; i<MAXABBREVS; i++) {
    strcpy(saveabbrev[i], NULLSTR);
  }
  i = 0;
  rewind (infp);
  while (readoption(type, head, headlines, abbrev, descript,
         value, name, heading) == FALSE) {
    strcpy(saveabbrev[i], abbrev);
    strcpy(savedescript[i], descript);
    i++;
  }
  rewind (infp);
}


/* UNIQUEABBREV    */
int uniqueabbrev(char *oldabbrev)
{
  int i;

  i = 0;
  while (strcmp(saveabbrev[i], NULLSTR) != 0) {
    if (strcmp(saveabbrev[i], oldabbrev) == 0) {
      printf("\n   You must enter a unique abbreviation.\n");
      printf("   Existing abbreviation: %s %s\n", 
             saveabbrev[i], savedescript[i]);
      printf("   Please re-enter.\n");
      return FALSE;
    }
    i++;
  }
  return TRUE;
}


/* PRINTABBREVS    */
void printabbrevs()
{
  int i;

  printf("\n   Options are:\n\n");
  i = 1;
  while (strcmp(saveabbrev[i-1], NULLSTR) != 0) {
    printf("   %5.5s", saveabbrev[i-1]);
    if (i % 3 == 0) {
      printf("\n");
    } 
    i++;
  }
  if ((i-1) % 3 != 0) {
    printf("\n");
  }
}
