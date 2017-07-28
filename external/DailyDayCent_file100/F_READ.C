#include <stdlib.h>
#include "f_header.inc"

/* Globals */
extern FILE *infp;

/* READOPTION */
int readoption(int type, int *head, int headlines[], 
               char abbrev[], char descript[],
               char value[][MWORD], char name[][MWORD], 
               char heading[][BWORD])
{
  int  headnum, i, realnum, valuenum;
  char buf[BUFSIZE];

  /* Initialization */
  *head = FALSE;
  for (i = 0; i < HEADERS; i++) {
    headlines[i] = 0;
    strcpy(heading[i], NULLSTR);
  }
  strcpy(abbrev, NULLSTR);
  strcpy(descript, NULLSTR);
  for (i = 0; i < MAXVALS; i++) {
    strcpy(value[i], NULLSTR);
    strcpy(name[i], NULLSTR);
  }

  /* Test for end-of-file */
  if (fgets(buf, sizeof buf, infp) == NULL) {
    return TRUE;
  }
  sscanf(buf, "%5s %s", abbrev, descript);

  /* Initialize pointers */
  realnum = 0;
  headnum = 0;
  valuenum = 0;

  /* Set up the first heading */
  strcpy(heading[headnum++], "Return to main menu\0");

  /* Read headings and values */
  while (realnum < lines[type]) {
    fgets(buf, sizeof buf, infp);
    if (buf[0] == '*') {
      i = 0;
      while (buf[i+4] != '\n' && i < BWORD-1) {
        heading[headnum][i] = buf[i+4];
        i++;
      }
      heading[headnum][i] = '\0';
      headlines[headnum] = valuenum;
      headnum++;
    } else {
      /* Read out values and names */
      sscanf(buf, "%s %s", value[valuenum], name[valuenum]);
      valuenum++;
    }
    realnum++;
  }

  /* Set head */
  if (headnum > 1) {
    *head = TRUE;
  }

  return FALSE;
}
