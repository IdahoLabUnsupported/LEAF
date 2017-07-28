#include <stdlib.h>
#include "f_header.inc"

#undef  EOF
#define EOF     '\n'

/* BLANKLINE */
int blankline(char *buf)
{
  if (*buf == '\n') {
    return TRUE;
  }
  while (*buf && *buf != '\n') {
    if ((*buf >= 'A' && *buf <= 'Z') ||
        (*buf >= 'a' && *buf <= 'z') ||
        (*buf >= '!' && *buf <= '/') ||
        (*buf >= '[' && *buf <= '_') ||
        (*buf >= '{' && *buf <= '~')) {
      return FALSE;
    } else {
      buf++;
    }
  }
  return TRUE;
}


/* CHKANS */
int chkans(char *ch)
{
  float value;

  value = (float) atof(ch);
  if (value == 0) {
    if (strcmp(ch, "0.0") == 0 || strcmp(ch, "0.") == 0 ||
        strcmp(ch, "0") == 0) {
      return TRUE;
    } else {
      printf("\n   You must enter a number or a command.\n");
      return FALSE;
    }
  } else {
    return TRUE;
  }
}


/* CMESG */
void cmesg()
{
  printf("\n\n   Ctrl-C is not an acceptable way \n");
  printf("   to exit from the program.\n");
}


/* EFOPEN  */
FILE *efopen(char *file, char *mode, int *err)
{
  FILE *fp;

  *err = FALSE;
  if ((fp = fopen(file, mode)) != NULL) {
    return fp;
  }
  *err = TRUE;
}


/* GETRES */
/* Gets a response from the keyboard, including just a return */
void getres(char *ptr, int len, int up)
{
  int num;
  char c;
 
  num = 0;
  setbuf(stdin, NULL);
  strcpy(ptr, NULLSTR);
/*  signal(SIGINT, cmesg); */
  while ((c = getc(stdin)) != EOF && num <= len) {
    *ptr = c;
    if (up == TRUE && islower(*ptr)) {
      *ptr = toupper(*ptr);
    }
    ptr++;
    num++;
  }
  *ptr = '\0';
}


/* GETINT */
/* Gets an integer reply from the keyboard */
int getint()
{
  char cword[SWORD];

  getres(cword, SWORD, FALSE);
  if (strcmp(cword, NULLSTR) == 0) {
    return 0;
  } else {
    return (atoi(cword));
  }
}


/* HELP */
void help()
{
  printf("\n   Valid responses are:\n");
  printf("   D (def)   display the definition for the value\n");
  printf("   F (find)  find a name in that option\n");
  printf("   H (help)  see this help message\n");
  printf("   L (list)  list the next 12 names and values\n");
  printf("   Q (quit)  for this and the remaining values\n");
  printf("             in this option, take the old values\n");
  printf("   <return>  take the old value\n");
  printf("   <a new value>\n");
}


/* UPPERCASE */
void uppercase(char *ptr)
{
  while (*ptr) {
    if (islower(*ptr)) {
      *ptr = toupper(*ptr);
    }
    ptr++;
  }
}
