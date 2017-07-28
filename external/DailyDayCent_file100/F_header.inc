#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <math.h>

/* Type enumerations */
enum {CROP = 1, CULT, FERT, FIRE, FIX, GRAZ, HARV, IRRI, OMAD,
      TREE, TREM, SITE, WTHR, SOIL, NUMTYPES};
enum {REV = 1, ADD, CHG, DEL, DIFF, ONECHG};

/* Defines */
#define NULLSTR "\0"
#define MAXVALS 250
#define FALSE   0
#define TRUE    1
#define QUIT    0
#define PREFIX  8
#define HEADERS 15
#define MAXDIFF 5

#define BUFSIZE 132
#define MWORD   25
#define BWORD   65
#define SWORD   6

static char *fstr[] = {
  "quit",
  "crop",
  "cult",
  "fert",
  "fire",
  "fix",
  "graz",
  "harv",
  "irri",
  "omad",
  "tree",
  "trem",
  "<site>",
  "weather statistics",
  "create soils.in"
};

/* Number of parameters, does not include header line */
static int lines[] = {
   0, /* quit */
 110, /* crop */
  11, /* cult */
   8, /* fert */
  20, /* fire */
 214, /* fix */
  11, /* graz */
   6, /* harv */
   4, /* irri */
   7, /* omad */ 
 139, /* tree */
  20, /* trem */
 244, /* site */
 244, /* site for weather statistics */
 244  /* site for creating soils.in file */
};

/* Prototypes */
int  blankline     (char *);
int  chkans        (char *);
void chklen        (int, char *, char *);
int  chkrange      (char *, char *);
void closefiles    ();
void cmesg         ();
void create_soil   (int);
void define        (char *);
void dooption      (int, int, int, int, int [], char [], char [],
                    char [][MWORD], char [][MWORD], char [][BWORD]);
int  dovalue       (int, int, int *, char [][MWORD],
                    char [][MWORD], char [][MWORD]);
FILE *efopen       (char *, char *, int *);
void find          (int, int *, char *, char [][MWORD]);
void getabbrevs    (int, int *, int [], char [], char [],
                    char [][MWORD], char [][MWORD], char [][BWORD]);
int  getdef        (char *, char[][BUFSIZE]);
int  getint        ();
void getres        (char *, int, int);
void get_soilvalues(float, float, float *, float *, float *);
void help          ();
int  openfiles     (int, char*);
void printabbrevs  ();
int  readoption    (int, int *, int [], char [], char [],
                    char [][MWORD], char [][MWORD], char [][BWORD]);
void savevalues    (int, int, char [], char [], char [][MWORD],
                    char [], char [], char [][MWORD]);
void stats         (int, float [][12], float [], float [], float[]);
void submenu       (int);
int  uniqueabbrev  (char *);
void uppercase     (char *);
void weather       (char*, char*);
void writefloat    (int, float *, char [][MWORD]);
void writeoption   (int, int, int [], char [], char descript[],
                    char [][MWORD], char [][MWORD], char [][BWORD]);
void writewthr     (float *, float *, float *, float *, float *,
                    int [], char [], char [],
                    char [][MWORD], char [][MWORD], char [][BWORD]);

int  process_daily_weather(FILE *, float[][12], float[][12], float[][12]);