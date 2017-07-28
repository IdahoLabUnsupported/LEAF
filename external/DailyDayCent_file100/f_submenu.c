#include <stdlib.h>
#include "f_header.inc"

/* SUBMENU */
void submenu(int type)
{
  int   action, i, j, k, head, numfound, printime,
        headlines[HEADERS];
  char  response[SWORD], pstr[BWORD];
  char  abbrev[SWORD], descript[BWORD], fndabbrev[MAXDIFF][SWORD],
        value[MAXVALS][MWORD], name[MAXVALS][MWORD], 
        heading[HEADERS][BWORD], diffname[MAXVALS][MWORD];
  float fndval[MAXDIFF][MAXVALS];

  action = QUIT;
  /* Display this menu until choice #0 is chosen */
  while (action != ONECHG) {
    /* Print the menu if necessary */
    if (type == FIX || type == SITE) {
      action = ONECHG;
    } else {
      for (;;) {
        printf("\n   What action would you like to take?\n");
        printf("   0. Return to main menu\n");
        printf("   1. Review all options\n");
        printf("   2. Add a new option\n");
        printf("   3. Change an option\n");
        printf("   4. Delete an option\n");
        printf("   5. Compare options\n");
        printf("\n   Enter selection: ");
        action = getint();
        if (action == QUIT) {
          return;
        }
        if (action >= QUIT && action <= DIFF) {
          break;
        }
        printf("   Invalid response. Please re-enter.\n");
      }
    }

    /* Open the files, returning if an error is found */
    if (openfiles(type,"null") == FALSE) {
      return;
    }

    /* Printing strings */
    if (action == REV) {
      printf("\n   List of options in this file:\n");
    } else if (action == ADD) {
      strcpy(pstr, "Is this the option you wish to start with?");
    } else if (action == CHG) {
      strcpy(pstr, "Is this an option you wish to change?");
    } else if (action == DEL) {
      strcpy(pstr, "Is this an option you wish to delete?");
    }

    /* Get all of the abbreviations for checking against */
    if (action == ADD || action == CHG || 
        action == ONECHG || action == DIFF) {
      getabbrevs(type, &head, headlines, abbrev, descript, 
                 value, name, heading);
    }

    /* If DIFF, find options and compare */
    if (action == DIFF) {
      /* Display all options */
      printabbrevs();

      /* Get the options to be compared */
      for (numfound = 0; numfound < MAXDIFF; numfound++) {
        strcpy(fndabbrev[numfound], NULLSTR);
      }
      numfound = 0;
      strcpy(response, "xxx");
      printf("\n   Current limit of options to compare is %d.\n",
             MAXDIFF);
      while (strcmp(response, NULLSTR) != 0 && numfound < MAXDIFF) {
        printf("\n   Enter an option to compare, <return> to quit: ");
        getres(response, SWORD, TRUE);
        strcpy(fndabbrev[numfound++], response);
      }
      numfound--;

      /* Load the options to be compared */
      while (readoption(type, &head, headlines, abbrev, descript,
                        value, name, heading) == FALSE) {
        writeoption(type, head, headlines, abbrev, descript,
                    value, name, heading); 
        for (i = 0; i < numfound; i++) {
          if (strcmp(fndabbrev[i], abbrev) == 0) {
            for (j = 0; j < MAXVALS; j++) {
              fndval[i][j] = (float)atof(value[j]);
              strcpy(diffname[j], name[j]);
            }
          }
        }
      }

      /* Do the comparisons */
      printime = 0;
      for (i = 0; i < numfound; i++) {
        for (j = i + 1; j < numfound; j++) {
          for (k = 0; k < MAXVALS; k++) {
            if (fndval[i][k] != fndval[j][k]) {
              printf("\nDifference:  Abbrev   Name        Value\n");
              printf("             %-5.5s    %-10.10s  %f\n", 
                     fndabbrev[i], diffname[k], fndval[i][k]);
              printf("             %-5.5s    %-10.10s  %f\n", 
                     fndabbrev[j], diffname[k], fndval[j][k]);
              if (++printime == 4) {
                printf("\n   Hit <return> to continue, Q to quit.\n");
                getres(response, SWORD, TRUE); 
                if (strcmp(response, "Q") == 0) {  
                  k = MAXVALS;
                  j = i = numfound;
                }
                printime = 0;
              }
            }
          }
        }
      }
    } /* action == DIFF */

    /* Deal with each option found in the file (unless at EOF from DIFF) */
    while (readoption(type, &head, headlines, abbrev, descript,
                      value, name, heading) == FALSE) {
      switch (action) {
        case REV:
          printf(" %5s %-s\n", abbrev, descript);
          writeoption(type, head, headlines, abbrev, descript,
                      value, name, heading);
          break;
        case ONECHG:
          printf("\n   Current option is: %5s %-s\n", abbrev, descript);
          dooption(type, action, TRUE, head, headlines,
                   abbrev, descript, value, name, heading);
          writeoption(type, head, headlines, abbrev, descript, 
                      value, name, heading);
          break; 
        case ADD:
        case CHG:
        case DEL:
          printf("\n   Current option is: %5s %-s\n", abbrev, descript);
          printf("   %s\n", pstr);
          getres(response, SWORD, TRUE); 
          if (strcmp(response, "Q") == 0) {
            writeoption(type, head, headlines, abbrev, descript, 
                        value, name, heading);
            while (readoption(type, &head, headlines, abbrev, descript,
                              value, name, heading) == FALSE) {
              writeoption(type, head, headlines, abbrev, descript, 
                          value, name, heading);
            }
          } else if (strcmp(response, "Y") == 0) { 
            if (action == ADD) { 
              writeoption(type, head, headlines, abbrev, descript, 
                          value, name, heading);
              dooption(type, action, FALSE, head, headlines,
                       abbrev, descript, value, name, heading);
              writeoption(type, head, headlines, abbrev, descript, 
                          value, name, heading);
              while (readoption(type, &head, headlines, abbrev, descript,
                                value, name, heading) == FALSE) {
                writeoption(type, head, headlines, abbrev, descript, 
                            value, name, heading);
              }
            } else if (action == CHG) {
              dooption(type, action, TRUE, head, headlines,
                       abbrev, descript, value, name, heading);
              writeoption(type, head, headlines, abbrev, descript, 
                          value, name, heading);
            } else if (action == DEL) {
              printf("\n   Are you sure you want to delete %5s %-s?\n",
                     abbrev, descript);
              getres(response, SWORD, TRUE);
              if (strcmp(response, NULLSTR) == 0 || 
                  strcmp(response, "N") == 0) {
                writeoption(type, head, headlines, abbrev, descript, 
                            value, name, heading);
              }
            }
          } else {
            writeoption(type, head, headlines, abbrev, descript, 
                        value, name, heading);
          }
          break;
        case DIFF:
          writeoption(type, head, headlines, abbrev, descript,
                      value, name, heading); 
          break;
        default:
          break;
      } /* end of switch */
    } /* end of readoption while */

    /* Close all files */
    closefiles();

    /* Print Done message */ 
    printf("\n   Done.\n");
  }
}
