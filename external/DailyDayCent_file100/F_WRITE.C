#include <stdlib.h>
#include "f_header.inc"

/* Globals */
extern FILE *outfp;

/* WRITEOPTION   */
void writeoption(int type, int head, int headlines[], char abbrev[], 
                 char descript[], char value[][MWORD], char name[][MWORD], 
                 char heading[][BWORD])
{
  int headnum, i;

  uppercase(abbrev);
  strcat(descript, "\n\0");
  fprintf(outfp, "%-5s %s", abbrev, descript);

  for (i = 0, headnum = 1; i < lines[type]; i++) {
    if (head == TRUE && i == headlines[headnum]) {
      fprintf(outfp, "*** %-s\n", heading[headnum++]);
    }
    if (strcmp(name[i], name[i-1]) != 0 &&
        strcmp(name[i], NULLSTR) != 0) {
      fprintf(outfp, "%-10s        %-12s\n", value[i], name[i]);
    }
  }
}


/* WRITEWTHR */
void writewthr(float *precmean, float *precsd, float *precskew, 
               float *tminmean, float *tmaxmean,
               int headlines[], char abbrev[], char descript[], 
               char value[][MWORD], char name[][MWORD], char heading [][BWORD])
{
  int headnum, i;

  uppercase(abbrev);
  strcat(descript, "\n\0");
  fprintf(outfp, "%-5s %s", abbrev, descript);

  headnum = 1;
  fprintf(outfp, "*** %-s\n", heading[headnum++]);
  writefloat(0,  precmean, name);
  writefloat(12, precsd, name);
  writefloat(24, precskew, name);
  writefloat(36, tminmean, name);
  writefloat(48, tmaxmean, name);

  for (i = 60; i < lines[SITE]; i++) {
    if (i == headlines[headnum]) {
      fprintf(outfp, "*** %-s\n", heading[headnum++]);
    }
    if (strcmp(name[i], name[i-1]) != 0 && 
        strcmp(name[i], NULLSTR) != 0) {
      fprintf(outfp, "%-10s        %-12s\n", value[i], name[i]);
    }
  }
}


/* WRITEFLOAT */
void writefloat(int pos, float *invalue, char name[][MWORD])
{
  int i;

  for (i = 0; i < 12; i++, invalue++, pos++) {
    fprintf(outfp, "%-10.4f        %-12s\n", *invalue, name[pos]);
  }
}
