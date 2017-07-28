#include <stdlib.h>
#include "f_header.inc"

#define MAXLINES  20

/* DOVALUE */
int dovalue(int type, int upper, int *num, char newvalue[][MWORD],
            char value[][MWORD], char name[][MWORD])
{
  int  i, max;
  char response[MWORD];

  for (;;) {
    /* Print the command line and the value */
    printf("\nCommands: D  F  H  L  Q  <new value>  <return>\n");
    printf("Name: %-10s  Previous Value: %s\n", name[*num], newvalue[*num]);
    printf("Enter response: ");
    getres(response, MWORD, TRUE);

    /* Help */
    if (strcmp(response,"H") == 0 || strcmp(response,"HELP") == 0) {
      help();

    /* Define */
    } else if (strcmp(response,"D") == 0 || strcmp(response,"DEF") == 0) {
      define(name[*num]);

    /* List */
    } else if (strcmp(response,"L") == 0 || strcmp(response,"LIST") == 0) {
      max = *num + MAXLINES;
      if (max >= upper) {
        max = upper;
      }
      for (i = *num; i < max; i++) {
        printf("\nName: %-10s  Previous Value: %s", name[i], newvalue[i]);
      }
      printf("\n");

    /* Find */
    } else if (strcmp(response,"F") == 0 || strcmp(response,"FIND") == 0) {
      printf("\n   Enter name to look for:\n");
      getres(response, MWORD, TRUE);
      uppercase(response);
      if (strcmp(response, NULLSTR) != 0) {
        find(type, num, response, name);
      }

    /* Quit */
    } else if (strcmp(response,"Q") == 0 || strcmp(response, "QUIT") == 0) {
      return TRUE;

    /* Return (continue to next value) */
    } else if (strcmp(response, NULLSTR) == 0) {
      (*num)++;
      return FALSE;

    /* Get new value */
    } else {
      if (chkans(response) == TRUE && chkrange(name[*num],response) == TRUE) {
        strcpy(newvalue[*num], response);
        (*num)++;  
        return FALSE;
      } else {
        strcpy(newvalue[*num], value[*num]);
      }
    }
  }
}
