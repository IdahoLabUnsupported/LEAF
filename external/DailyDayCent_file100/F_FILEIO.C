#include <stdlib.h>
#include "f_header.inc"

/* Globals */
extern FILE *infp, *outfp, *deffp;

/* OPENFILES    */
int openfiles(int type, char* file)
{
  int  i, ioerr;
  char buf[BUFSIZE], defname[BWORD], filename[BWORD], newname[BWORD],
       string[BUFSIZE];

  
  /* Get the original file name */
  if (type == SITE || type == SOIL) {
    for (i = 0; i <= PREFIX; i++) {
      filename[i] = ' ';
    }
    filename[i] = '\0';
    strcpy(filename, file);
    //printf("\n   Enter the site file name:\n");
    //scanf("%s", filename);
    i = 0;
    while ((i < PREFIX) && (filename[i] != '.')) {
      i++;
      if (filename[i] == '.') {
        i = PREFIX + 1;
      }
    }
    if (i == PREFIX) {
      strcat(filename, ".100");
    }
  } else {
    strcpy(filename, fstr[type]);
    strcat(filename, ".100");
  }

  /* Open the original file to read from */
  /* and the temporary file to write to */
  infp = efopen(filename, "r", &ioerr);
  if (ioerr == TRUE) {
    printf("\n   Can't open %s file.\n", filename);
    return FALSE;
  }
  outfp = efopen("XXXX.100", "w", &ioerr);

  /* Copy the original file into the temporary file */
  while (fgets(buf, sizeof buf, infp) != NULL) {
    fprintf(outfp, "%s", buf);
  }

  /* Close both files */
  fclose(infp);
  fclose(outfp);

  /* Get a new site file name */
  if (type == SOIL) {
    strcpy(filename, "soils.in");
  } else if (type == SITE) {
    /*printf("\n   Enter a new site file name to save changes\n");
    printf("   to or a <return> to save to %s:\n", filename);
    getres(newname, BWORD, FALSE);
    if (strcmp(newname, NULLSTR) != 0) {
      strcpy(filename, newname);
      strcat(filename, ".100");
    }*/
  }

  /* Open the temporary file to read from */
  /* and the original file to overwrite */
  infp = efopen("XXXX.100", "r", &ioerr);
  outfp = efopen(filename, "w", &ioerr);

  if (ioerr == TRUE) {
    sprintf(string, "chmod 666 %s", filename);
    system(string);
    outfp = efopen(filename, "w", &ioerr);
    printf("\n   Allowing write permission to %s.\n", filename);
  }

  /* Determine the name of the definition file and open */
  if (type != SOIL) {
    if (type == SITE) {
      strcpy(defname, "site");
    } else {
      strcpy(defname, fstr[type]);
    }
    strcat(defname, ".def");
    deffp = efopen(defname, "r", &ioerr);
    if (ioerr == TRUE) {
      deffp = NULL;
    }
  }
}


/* CLOSEFILES */
void closefiles()
{
/*  char string[BUFSIZE]; */

  fclose(infp);
  fclose(outfp);
  if (deffp != NULL) {
    fclose(deffp);
  }
  unlink("XXXX.100");
}
