#include "f_header.inc"

#define MONTHS  12
#define LINELEN 64

/* WEATHER */
void weather(char* file, char* sitefile)
{
  int   i, ioerr;
  int   head, headlines[HEADERS];
  float prec[500][MONTHS], tmin[500][MONTHS], tmax[500][MONTHS],
        precmean[MONTHS], tminmean[MONTHS], tmaxmean[MONTHS],
        precsd[MONTHS], tminsd[MONTHS], tmaxsd[MONTHS],
        precskew[MONTHS], tminskew[MONTHS], tmaxskew[MONTHS];
  char  filename[BWORD];
  char  abbrev[SWORD], descript[BWORD], value[MAXVALS][MWORD],
        name[MAXVALS][MWORD], heading[HEADERS][BWORD];
  FILE *wthrfp;
  strcpy(filename,file);

  /* Open the existing weather file */
  //printf("\n   Enter the name of the actual weather file:\n");
  //scanf("%s", filename);
  i = 0;
  while (i < PREFIX && filename[i] != '.') {
    i++;
    if (filename[i] == '.') {
      i = PREFIX + 1;
    }
  }
  if (i == PREFIX) {
    strcat(filename, ".wth");
  }
  wthrfp = efopen (filename, "r", &ioerr);
  if (ioerr == TRUE) {
    printf("\n   Can't open %s file.\n", filename);
    return;
  }

  /* Read the values from the existing weather file */
  /* Call the function to manipulate the daily weather data into */
  /* monthly weather data format */
  i = process_daily_weather(wthrfp, prec, tmin, tmax);

  fclose(wthrfp);

  /* Generate the mean, sd, and skew values */
  stats(i, prec, precmean, precsd, precskew);
  stats(i, tmin, tminmean, tminsd, tminskew);
  stats(i, tmax, tmaxmean, tmaxsd, tmaxskew);

  /* Open the site file */
  openfiles(SITE, sitefile);

  /* Read in the option */
  readoption(SITE, &head, headlines, abbrev, descript, value,
             name, heading);

  /* Write out the option */
  writewthr(precmean, precsd, precskew, tminmean, tmaxmean, 
            headlines, abbrev, descript, value, name, heading);

  /* Close the files */
  closefiles();

  /* Print Done message */
  printf("\n   Done.\n");
}


/* CHKLEN */
void chklen(int year, char *field, char *buf)
{
  int j;
 
  j = 0;
  while(*buf) {
    buf++;
    j++;
  }
  if (j < LINELEN) {
    printf("\n   Incomplete line in weather file at: %s %d.\n",
           field, year);
    printf("   Repair line and re-run.\n");
    exit(0);
  }
}


/* STATS */
void stats(int numyears, float value[][MONTHS], float mean[],
           float sd[], float skew[])
{
  int goodyears, month, year;
  float sum, ssum, csum, var, testskew;
  float novalue = -99.99f;

  for (month = 0; month < MONTHS; month++) {
    sum = 0.0f;
    ssum = 0.0f;
    csum = 0.0f;
    goodyears = 0;
    for (year = 0; year < numyears; year++) {
      if (value[year][month] != novalue) { 
        goodyears++;
        sum += value[year][month];
        ssum += (float)pow(value[year][month], 2.0);
        csum += (float)pow(value[year][month], 3.0);
      }
    }
    mean[month] = sum / (float)goodyears;
    var = ssum / (float)goodyears - (float)pow(mean[month], 2.0);
    if (var < 0) {
      var = 0.0f;
    }
    sd[month] = (float)sqrt(var);
    testskew = (float)pow(var,1.5);
    if (testskew == 0) {
      printf("Unable to calculate SKEW value; replacing with 0.00\n");
      skew[month] = 0.0f;
    } else {
      skew[month] = (csum / (float)goodyears - 3 * mean[month] *
                     var - (float)pow(mean[month], 3.0)) / testskew;
    }
  }
}
