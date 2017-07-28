#include <stdlib.h>
#include "f_header.inc"

#define MAXDEFLINES  20
enum {NO, OR, TO};

/* Globals */
extern FILE  *deffp;

/* DEFINE    */
void define(char *word)
{
  int  defpos, i;
  char defbuf[MAXDEFLINES][BUFSIZE];

  /* Get the definition */
  defpos = getdef(word, defbuf);

  /* If the definition was found */
  if (defpos > 0) {
    for (i = 0; i <= defpos; i++) {
      fputs(defbuf[i], stdout);
    }
  }
}


/* CHKRANGE */
int chkrange(char *word, char *response)
{
  int   defpos, rtype;
  float aval, bval, cval, newvalue;
  char  defbuf[MAXDEFLINES][BUFSIZE], type[SWORD];

  /* Get the definition */
  defpos = getdef(word, defbuf);
  if (defpos <= 0) {
    return FALSE;
  }

  /* Convert the given value to float */
  sscanf(response, "%f", &newvalue);

  /* Determine the type of range */
  sscanf(defbuf[defpos], "%s %f %s %f", type, &aval, type, &bval);
  uppercase(type);
  rtype = NO;
  if (strcmp(type, "OR") == 0) {
    rtype = OR;
  } else if (strcmp(type, "TO") == 0) {
    rtype = TO;
  }

  /* Re-scan the range line based on the range type and */
  /* determine if the given value is within range */
  /* Note: invalid ranges are being treated as warnings for */
  /* the time being. To have invalid ranges treated as truly */
  /* unacceptable, change the message and return FALSE */
  switch (rtype) {
    case OR:
      sscanf(defbuf[defpos], "%s %f %s %f %s %f", 
             type, &aval, type, &bval, type, &cval);
      if (newvalue == aval || newvalue == bval || newvalue == cval) {
        return TRUE;
      } else {
        printf("\n   Warning: invalid range. Acceptable values: ");
        printf("   %3.1f ", aval);
        if (bval != 0) {
          printf("   %3.1f ", bval);
        }
        if (cval != 0) {
          printf("   %3.1f ", cval);
        }
        printf("\n");
        return TRUE;
      }
      break;
    case TO:
      sscanf(defbuf[defpos], "%s %f %s %f", type, &aval, type, &bval);
      if (aval <= newvalue && newvalue <= bval) {
        return TRUE;
      } else { 
        printf("\n   Warning: invalid range. Acceptable range:"); 
        printf(" %7.3f to %7.3f\n", aval, bval);
        return TRUE; 
      } 
      break;
    default:
      printf("\n   No range checking available.\n");
      return TRUE;
      break;
  }
}


/* GETDEF */
int getdef(char *word, char defbuf[][BUFSIZE])
{
  int  done, defpos, i, j;
  char buf[BUFSIZE], term[MWORD], temp[MWORD];

  /* Check that definition file was opened */
  if (deffp == NULL) {
    printf("\n   File containing definitions & ranges was not found.\n");
    return 0;
  }

  /* Initialization */
  done = FALSE;
  defpos = 0;
  rewind(deffp);

  /* Convert the current word to upper case */
  uppercase(word);

  /* Read the header line of the def file */
  fgets(buf, sizeof buf, deffp);

  /* Read the first line of the def file */
  fgets(buf, sizeof buf, deffp);

  while (done == FALSE) {
    /* Get the term from the line */
    sscanf(buf, "%s", temp);
    uppercase(temp);
    /* Add the single quotes to the term */
    i = j = 0;
    term[i++] = '\'';
    while (temp[j] != '\0') {
      term[i++] = temp[j++];
    }
    term[i++] = '\'';
    term[i] = '\0';
    /* If the term matches the word, save the definition */
    if (strcmp(term, word) == 0) {
      strcpy(defbuf[defpos++], "\n");
      strcpy(defbuf[defpos++], buf);
      while (fgets(buf, sizeof buf, deffp) != NULL) {
        if (blankline(buf) == TRUE) {
          break; 
        } else {
          strcpy(defbuf[defpos++], buf);
        }
      }
      done = TRUE;
    } else { 
      /* Read through until next term */
      if (buf[0] != '*') {
        while (fgets(buf, sizeof buf, deffp) != NULL) {
          if (blankline(buf) == TRUE) {
            break;
          }
        }
      }
      if (fgets(buf, sizeof buf, deffp) == NULL) {
        done = TRUE;
      }
    }
  }

  /* Correct defpos to one less */
  --defpos;

  /* Print message if definition not found */
  if (defpos < 0) {
    printf("\n   No definition found.\n");
  }

  return defpos;
}
