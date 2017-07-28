
#include <stdio.h>
#include "f_header.inc"

    int process_daily_weather(FILE *wthrfp, float prec[500][12],
                               float tmin[500][12], float tmax[500][12])
    {

      char  buf[BUFSIZE];
      int   ii, jj;
      int   day_of_month;
      int   month;
      int   year;
      int   julian_day;
      float maxtemp;
      float mintemp;
      float precip;
      int   month_max_count, month_min_count, month_ppt_count;
      float month_max_total, month_min_total, month_ppt_total;
      float month_max_avg, month_min_avg;
      int   current_year = 0;
      int   current_month = 1;

      /* Initializations */
      for (ii=0; ii<500; ii++) {
        for (jj=0; jj<12; jj++) {
          prec[ii][jj] = 0.0f;
          tmin[ii][jj] = 0.0f;
          tmax[ii][jj] = 0.0f;
        }
      }
      month_max_count = 0;
      month_min_count = 0;
      month_ppt_count = 0;
      month_max_total = 0.0f;
      month_min_total = 0.0f;
      month_ppt_total = 0.0f;

      /* Read the values from the existing weather file one line at a time */
      while (fgets(buf, sizeof buf, wthrfp) != NULL) {
        sscanf(buf, "%d %d %d %d %f %f %f", &day_of_month, &month, &year,
               &julian_day, &maxtemp, &mintemp, &precip);
        if (month == current_month) {
          if (maxtemp >= -98.0) {
            month_max_count++;
            month_max_total += maxtemp;
          }
          if (mintemp >= -98.0) {
            month_min_count++;
            month_min_total += mintemp;
          }
          if (precip >= -98.0) {
            month_ppt_count++;
            month_ppt_total += precip;
          }
        } else {
          /* We are at the end of the month, store the values */
          if (month_max_count > 1) {
            month_max_avg = month_max_total / month_max_count;
          } else {
            month_max_avg = -99.99f;
          }
          if (month_min_count > 1) {
            month_min_avg = month_min_total / month_min_count;
          } else {
            month_min_avg = -99.99f;
          }
          if (month_ppt_count < 1) {
            month_ppt_total = -99.99f;
          }
          tmax[current_year][current_month-1] = month_max_avg;
          tmin[current_year][current_month-1] = month_min_avg;
          prec[current_year][current_month-1] = month_ppt_total;
          /* Reset counters and accumulators */
          if (month == 12) {
            current_year++;
          }
          current_month = month;
          if (maxtemp >= -98.0) {
            month_max_count = 1;
            month_max_total = maxtemp;
          } else {
            month_max_count = 0;
            month_max_total = 0.0f;
          }
          if (mintemp >= -98.0) {
            month_min_count = 1;
            month_min_total = mintemp;
          } else {
            month_min_count = 0;
            month_min_total = 0.0f;
          }
          if (precip >= -98.0) {
            month_ppt_count = 1;
            month_ppt_total = precip;
          } else {
            month_ppt_count = 0;
            month_ppt_total = 0.0f;
          }
        }
      }
      /* Write out the final month's values */
      if (month_max_count > 1) {
        month_max_avg = month_max_total / month_max_count;
      } else {
        month_max_avg = -99.99f;
      }
      if (month_min_count > 1) {
        month_min_avg = month_min_total / month_min_count;
      } else {
        month_min_avg = -99.99f;
      }
      if (month_ppt_count < 1) {
        month_ppt_total = -99.99f;
      }
      tmax[current_year][current_month-1] = month_max_avg;
      tmin[current_year][current_month-1] = month_min_avg;
      prec[current_year][current_month-1] = month_ppt_total;
      
/* Add error checking ??? */
      return(current_year);
    }
