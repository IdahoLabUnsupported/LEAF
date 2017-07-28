#include <stdlib.h>
#include "f_header.inc"

/* DOOPTIOPN    */
void dooption(int type, int action, int saveask, int head, int headlines[],
              char abbrev[], char descript[], char value[][MWORD],
              char name[][MWORD], char heading[][BWORD])
{
  int  answer, done, doq, headnum, i, lower, upper;
  char response[BWORD];
  char newabbrev[SWORD], newdescript[BWORD], newvalue[MAXVALS][MWORD];

  /* Get a new abbreviation and description */
  if (action == ADD) {
    for (;;) {
      printf("\n   Enter a new abbreviation:\n");
      strcpy(newabbrev, NULLSTR);
      scanf("%5s", newabbrev);
      uppercase(newabbrev);
      if (uniqueabbrev(newabbrev) == TRUE) {
        break;
      }
    }
    printf("\n   Enter a new description:\n");
    scanf("%s", newdescript);
  } else if (action == CHG || action == ONECHG) {
    for (;;) {
      printf("\n   Enter a new abbreviation or a <return>\n");
      printf("   to use the existing abbreviation:\n");
      getres(response, BWORD, TRUE);
      if (strcmp(response, NULLSTR) != 0) {
        sscanf(response, "%5s", newabbrev);
        uppercase(newabbrev);
        if (uniqueabbrev(newabbrev) == TRUE) {
          break;
        }
      } else {
        strcpy(newabbrev, abbrev);
        break;
      }
    }
    printf("\n   Enter a new description or a <return>\n");
    printf("   to use the existing description:\n");
    getres(response, BWORD, FALSE);
    if (strcmp(response, NULLSTR) != 0) {
      sscanf(response, "%s", newdescript);
    } else {
      strcpy(newdescript, descript);
    }
  }

  /* Set up the new values */
  for (i = 0; i < lines[type]; i++) {
    strcpy(newvalue[i], value[i]);
  }

  done = FALSE;

  while (done == FALSE) {
    if (head == TRUE) {
      for (;;) {
        headnum = 0;
        printf("\n   Which subheading do you want to work with?\n");
        printf("   %d. %s\n", headnum, heading[headnum]);
        headnum++;
        printf("   %d. %s\n", headnum, heading[headnum]);
        headnum++;
        while (headlines[headnum] > 0) {
          printf("   %d. %s\n", headnum, heading[headnum]);
          headnum++;
        }
        printf("\n   Enter selection:\n");
        answer = getint();
        if (answer == QUIT) {
          savevalues(type, saveask, abbrev, descript, value,
                     newabbrev, newdescript, newvalue);
          return;
        } else if (answer > QUIT && answer < headnum) {
          lower = headlines[answer];
          upper = headlines[answer+1];
          if (upper == 0) {
            upper = lines[type] - headnum + 1;
          }
          break;
        } else {
          printf("   Invalid response. Please re-enter.\n");
        }
      }
    } else {
      done = TRUE;
      lower = 0;
      upper = lines[type];
    }

    doq = FALSE;

    while (lower < upper && doq == FALSE) {
      doq = dovalue(type, upper, &lower, newvalue, value, name);
    }
  }

  savevalues(type, saveask, abbrev, descript, value,
             newabbrev, newdescript, newvalue);
}


/* SAVEVALUES */
void savevalues(int type, int saveask, char abbrev[], char descript[],
                char value[][MWORD], char newabbrev[], char newdescript[], 
                char newvalue[][MWORD])
{
  int i;
  char response[SWORD];

  /* Does the user want to save the values? */
  if (saveask == TRUE) {
    printf("\n   Do you want to save the changes made? (default = Y)\n");
    getres(response, SWORD, TRUE);
  } else {
    strcpy(response, "Y");
  }
  if (strcmp(response, "Y") == 0 || strcmp(response, NULLSTR) == 0) {
    strcpy(abbrev, newabbrev);
    strcpy(descript, newdescript);
    for (i = 0; i < lines[type]; i++) {
      strcpy(value[i], newvalue[i]);
    }
  }
}
