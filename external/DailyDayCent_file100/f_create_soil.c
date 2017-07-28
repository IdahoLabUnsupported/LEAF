
/*              Copyright 2002 Colorado State University                    */
/*                      All Rights Reserved                                 */

/*****************************************************************************
**
**  FILE:      create_soil.c
**
**  FUNCTION:  void create_soil()
**
**  PURPOSE:   Create a DayCent 4.5 SOILS.IN file using the information from
**             an existing Century 4.0 <SITE>.100 file.
**
**             This utility assumes the ADEP(*) values in the fix.100 file are
**             parameterized using default values as follows:
**               ADEP(1)           15.00000
**               ADEP(2)           15.00000
**               ADEP(3)           15.00000
**               ADEP(4)           15.00000
**               ADEP(5)           30.00000
**               ADEP(6)           30.00000
**               ADEP(7)           30.00000
**               ADEP(8)           30.00000
**               ADEP(9)           30.00000
**               ADEP(10)          30.00000
**             ADEP(10) is used for flow out of the bottom of the soil profile
**             in Century so the SOILS.IN file only needs to go down to 
**             ADEP(9) for a 210 cm depth.
**
**             The layer depths used as the template to create the SOILS.IN
**             DayCent input file, are defined as follows:
**               layer  1 =   0.0 to   1.0 cm
**               layer  2 =   1.0 to   4.0 cm
**               layer  3 =   4.0 to  15.0 cm
**               layer  4 =  15.0 to  30.0 cm
**               layer  5 =  30.0 to  45.0 cm
**               layer  6 =  45.0 to  60.0 cm
**               layer  7 =  60.0 to  75.0 cm
**               layer  8 =  75.0 to  90.0 cm
**               layer  9 =  90.0 to 105.0 cm
**               layer 10 = 105.0 to 120.0 cm
**               layer 11 = 120.0 to 150.0 cm
**               layer 12 = 150.0 to 180.0 cm
**               layer 13 = 180.0 to 210.0 cm
**
**             The NLAYER value from the <SITE>.100 file will be used to
**             determine the depth of the soil profile that will be created
**             for the SOILS.IN file as follows:
**               NLAYER = 1        SOILS.IN depth =  15 cm, layers 1 -  3
**               NLAYER = 2        SOILS.IN depth =  30 cm, layers 1 -  4
**               NLAYER = 3        SOILS.IN depth =  45 cm, layers 1 -  5
**               NLAYER = 4        SOILS.IN depth =  60 cm, layers 1 -  6
**               NLAYER = 5        SOILS.IN depth =  90 cm, layers 1 -  8
**               NLAYER = 6        SOILS.IN depth = 120 cm, layers 1 - 10 
**               NLAYER = 7        SOILS.IN depth = 150 cm, layers 1 - 11
**               NLAYER = 8        SOILS.IN depth = 180 cm, layers 1 - 12 
**               NLAYER = 9        SOILS.IN depth = 210 cm, layers 1 - 13
**
**             Any value for NLAYER greater than 9 is an invalid value for
**             running the Century model and will generate an error with no
**             SOILS.IN file being created.
**
**             The format of the SOILS.IN file to be created is as follows:
**               Column  1 - Minimum depth of soil layer (cm)
**               Column  2 - Maximum depth of soil layer (cm)
**               Column  3 - Bulk density of soil layer (g/cm^3)
**               Column  4 - Field capacity of soil layer, volumetric
**               Column  5 - Wilting point of soil layer, volumetric
**               Column  6 - Evaporation coefficient for soil layer (currently
**                           not being used)
**               Column  7 - Percentage of roots in soil layer, these values
**                           must sum to 1.0
**               Column  8 - Fraction of sand in soil layer, 0.0 - 1.0
**               Column  9 - Fraction of clay in soil layer, 0.0 - 1.0
**               Column 10 - Organic matter in soil layer, fraction 0.0 - 1.0
**               Column 11 - Minimum volumetric soil water content below
**                           wilting point for soil layer, soil water content
**                           will not be allowed to drop below this value
**               Column 12 - Saturated hydraulic conductivity of soil layer in
**                           centimeters per second
**               Column 13 - pH of soil layer
** 
**             The field capacity, wilting point, and saturated hydraulic
**             conductivity computed for the SOILS.IN file will be based on
**             the soil texture, percent sand (SAND) and percent clay (CLAY)
**             values, read from the indicated <SITE>.100 file.  The equations
**             used to compute these values are modified equations obtained
**             from the "Soil texture triangle hydraulic properties
**             calculator" available online at:
**               http://www.bsyse.wsu.edu/saxton/soilwater/soilwater.htm
**             Source:
**               Dr. K. E. Saxton
**               USDA/ARS
**               Pullman, WA 99164-6120
**               Phone: (509) 335-2724
**               FAX: (509) 335-7786
**               E-mail: ksaxton@wsu.edu
**               Homepage:http://www.bsyse.wsu.edu/saxton
**             Reference:
**               K.E. Saxton et al., 1986
**               Estimating generalized soil-water characteristics from
**               texture.  Soil Sci. Soc. Amer. J. 50(4):1031-1036
**
**  INPUTS:
**    type - indicates file type
**
**  GLOBAL VARIABLES:
**    BWORD   - maximum string size (65)
**    FALSE   - false (0)
**    HEADERS - maximum number of headers (15)
**    MAXVALS - maximum number of parameter values (250)
**    MWORD   - maximum medimum string size (25)
**    SWORD   - maximum short string size (6)
**    TRUE    - true (1)
**
**  EXTERNAL VARIABLES:
**    infp  - file pointer to the old parameter file
**    outfp - file pointer to the new parameter file
**
**  LOCAL VARIABLES:
**    abbrev[]         - parameter option abbreviation
**    bulkd            - BULKD as read from <SITE>.100 file
**    descript[]       - parameter option description
**    error            - set to TRUE if file being processed has invalid
**                       format, otherwise FALSE
**    evap_coeff[]     - array to hold evaporation coefficient values by layer
**                       for SOILS.IN file (all values must sum to 1.0)
**    fieldc           - computed field capacity for SOILS.IN file
**                       (volumetric)
**    head             - flag, set to TRUE when processing a <SITE>.100 file,
**                       otherwise FALSE
**    heading[][]      - array of site subheading strings
**    headlines[]      - array of site subheadings line numbers
**    ii               - loop indices
**    ksat             - computed saturated hydraulic conductivity of soil
**                       layer (cm/second)
**    layers           - NLAYERS as read from <SITE>.100 file
**    max_depth[]      - array to hold maximum depth values by layer for
**                       SOILS.IN file (cm)
**    min_depth[]      - array to hold minimum depth values by layer for
**                       SOILS.IN file (cm)
**    min_vswc         - minimum volumetric soil water content for layer for
**                       SOILS.IN file
**    name[][]         - list of parameter names read
**    newlayers        - number of layers in SOILS.IN file
**    organic_matter[] - array to hold organic matter values by layer for
**                       SOILS.IN file (fraction 0.0 - 1.0)
**    pctclay          - CLAY as read from <SITE>.100 file
**    pctsand          - SAND as read from <SITE>.100 file
**    pctsilt          - SILT as read from <SITE>.100 file
**    pH               - PH as read from <SITE>.100 file
**    sum              - summation of sand, silt, and clay values used for
**                       normalizing sand, silt, and clay if necessary
**    trans_coeff[]    - array to hold transpiration coefficient values by
**                       layer for SOILS.IN file (all values must sum to 1.0)
**    value[][]        - list of parameter values as read from <SITE>.100 file
**    wiltpt           - computed wilting point for SOILS.IN file (volumetric)
**
**  OUTPUTS:
**    None
**
**  CALLED BY:
**    main
**
**  CALLS:
**    closefiles     - closes all open files
**    get_soilvalues - get the computed field capacity, wilting point and
**                     saturated hydraulic conductivity values for a soil
**                     layer
**    openfiles      - open the parameter files
**
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "f_header.inc"

/* Globals */
extern FILE *infp, *outfp;

    void create_soil(int type)
    { 
      /* Arrays for SOILS.IN template values */
	  static float min_depth[14] = {0.0f, 2.0f, 5.0f, 10.0f, 20.0f, 30.0f,
                                    45.0f, 60.0f, 75.0f, 90.0f, 105.0f,
                                    120.0f, 150.0f, 180.0f};
      static float max_depth[14] = {2.0f, 5.0f, 10.0f, 20.0f, 30.0f, 45.0f,
                                    60.0f, 75.0f, 90.0f, 105.0f, 120.0f,
                                    150.0f, 180.0f, 210.0f};
      static float evap_coeff[14] = {0.8f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
      static float trans_coeff[14] = {0.01f, 0.04f, 0.25f, 0.30f, 0.10f,
                                      0.05f, 0.04f, 0.03f, 0.02f, 0.01f, 0.0f,
                                      0.0f, 0.0f, 0.0f};
      static float organic_matter[14] = {0.01f, 0.01f, 0.01f, 0.01f, 0.01f,
                                         0.01f, 0.01f, 0.01f, 0.01f, 0.01f,
                                         0.01f, 0.01f, 0.01f, 0.01f};
      /* Values read from <SITE>.100 file */
      int   layers;
      float pctsand, pctsilt, pctclay, bulkd, pH;
      /* Computed values for SOILS.IN file */
      float fieldc, wiltpt, ksat, min_vswc;

      int   error = FALSE;
      int   head, ii, newlayers;
      int   headlines[HEADERS];
      float sum;
      char  abbrev[SWORD];
      char  descript[BWORD];
      char  value[MAXVALS][MWORD];
      char  name[MAXVALS][MWORD];
      char  heading[HEADERS][BWORD];
      
      /* Open the files, returning if an error is found */
      if (openfiles(type,"null") == FALSE) {
        return;
      }

      /* Read the parameter values from the <SITE>.100 file */
      while (readoption(type, &head, headlines, abbrev, descript,
                        value, name, heading) == FALSE) {

        /* Get the percent sand, silt, clay, bulk density, number of */
        /* layers, and pH values from the <SITE>.100 file parameter */
        /* values read above */
        pctsand = (float)atof(value[64]);
        pctsilt = (float)atof(value[65]);
        pctclay = (float)atof(value[66]);
        bulkd   = (float)atof(value[68]);
        layers  = atoi(value[69]);
        pH      = (float)atof(value[97]);

        /* Determine number of layers to create for SOILS.IN file based on */
        /* layer value read from the site file */
        switch (layers) {
          case 1:
            newlayers = 3;
            break;
          case 2:
            newlayers = 5;
            break;
          case 3:
            newlayers = 6;
            break;
          case 4:
            newlayers = 7;
            break;
          case 5:
            newlayers = 9;
            break;
          case 6:
            newlayers = 11;
            break;
          case 7:
            newlayers = 12;
            break;
          case 8:
            newlayers = 13;
            break;
          case 9:
            newlayers = 14;
            break;
          default:
            printf("\n Invalid number of soil layers in <SITE>.100 file! \n");
            printf(" Valid number for soil layers is >= 1 or <= 9. \n");
            printf(" Conversion aborted! \n");
            printf(" Correct <SITE>.100 file and try again.\n");
            error = TRUE;
            goto ex;
            break;
        }

        /* Normalize to make sure that the SAND, SILT, and CLAY values sum */
        /* to 1.0 if necessary */
        if ((pctsand+pctsilt+pctclay) != 1.00000) {
/*          printf("\n Sand, silt and clay content in <SITE>.100 file does\n");
          printf(" not sum to 1.0.  These values have been normalized. \n"); */
          sum = pctsand + pctsilt + pctclay;
          pctsand = pctsand / sum;
          pctsilt = pctsilt / sum;
          pctclay = pctclay / sum;
        }

        /* Create SOILS.IN file */
        for (ii = 0; ii < newlayers; ii++) {

          /* Compute values for wilting point, field capacity, and */
          /* saturated hydraulic conductivity values based on the soil */
          /* texture values read from the <SITE>.100 file */
          get_soilvalues(pctsand*100, pctclay*100, &wiltpt, &fieldc, &ksat);

          /* Compute minimum volumetric soil water content for soil layer */
          /* based on wilting point and current depth */
          switch (ii) {
            case 0:
              min_vswc = 0.8f * wiltpt;
              break;
            case 1:
              min_vswc = 0.6f * wiltpt;
              break;
            case 2:
              min_vswc = 0.4f * wiltpt;
              break;
            case 3:
              min_vswc = 0.1f * wiltpt;
              break;
            default:
              min_vswc = 0.0f;
              break;
          }

          /* Write the values for the current layer to the output file */
          /* including the computed values for field capacity, wilting */
          /* point, and saturated hydraulic conductivity */
          fprintf(outfp,"%5.1f %5.1f %5.2f %8.5f %8.5f %5.2f %5.2f",
                  min_depth[ii], max_depth[ii], bulkd, fieldc, wiltpt,
                  evap_coeff[ii], trans_coeff[ii]);
          fprintf(outfp," %5.2f %5.2f %5.2f %5.2f %8.5f %5.2f\n",
                  pctsand, pctclay, organic_matter[ii], min_vswc,
                  ksat, pH);
        }
      } /* while */

      /* Print Done message */ 
      printf("\n   Done creating SOILS.IN file.\n");

      /* Close all files */
ex:   closefiles(error);

      return;
    }


/*****************************************************************************
**
**  FILE:      create_soil.c
**
**  FUNCTION:  void get_soilvalues()
**
**  PURPOSE:   Compute wilting point, field capacity, saturated hydraulic
**             conductivity and bulk density values for a soil layer based on
**             the soil texture using the equations from Soil texture triangle
**             hydraulic properties calculator, see above
**
**  INPUTS:
**    pctclay  - CLAY as read from <SITE>.100 file
**    pctsand  - SAND as read from <SITE>.100 file
**
**  GLOBAL VARIABLES:
**    None
**
**  EXTERNAL VARIABLES:
**    None
**
**  LOCAL VARIABLES:
**    acoef    - first coefficient computed for field capacity and wilting
**               point equations
**    bcoef    - second coefficient computed for field capacity and wilting
**               point equations
**    bd_raw   - bulk density value as computed using Saxton equations without
**               correction
**    bulkd    - bulk density value as computed using Saxton equations with
**               correction, currently not being used but calculations
**               retained for possible future reference (g/cm^3)
**    sat      - computed coefficient for saturated hydraulic conductivity
**               equations
**    wp_raw   - wilting point value as computed using Saxton equations
**               without correction
**    fc_raw   - field capacity value as computed using Saxton equations
**               without correction
**    ksat_raw - saturated hydraulic conductivity value as computed using
**               Saxton equations without correction
**
**  OUTPUTS:
**    fieldc - field capacity value as computed using Saxton equations with
**             correction (volumetric)
**    ksat   - saturated hydraulic conductivity value as computed using Saxton
**             equations with correction (cm/second)
**    wiltpt - wilting point value as computed using Saxton equations with
**             correction (volumetric)
**
**  CALLED BY:
**    create_soil
**
**  CALLS:
**    None
**
*****************************************************************************/

    void get_soilvalues(float pctsand, float pctclay, float *wiltpt,
                        float *fieldc, float *ksat)
    {
      float acoef;
      float bcoef;
      float sat;
      float wp_raw;
      float fc_raw;
      float ksat_raw;
      float bd_raw;
      float bulkd;

      /* Equations from Soil texture triangle hydraulic properties */
      /* calculator, see above */
      /* Note:  I have retained the bulk density calculations although */
      /*        they are not being used at this time - cak, 11/01/01 */
      acoef = (float)(exp(-4.396 - 0.0715 * pctclay - 4.88e-4 *
              pow(pctsand, 2) - 4.285e-5 * pow(pctsand, 2) * pctclay));
      bcoef = (float)(-3.14 - 0.00222 * pow(pctclay, 2) - 3.484e-5 *
              pow(pctsand, 2) * pctclay);
      sat = (float)(0.332 - 7.251e-4 * pctsand + 0.1276 * log10(pctclay));

      /* Corrections from Steve Del Grosso */    
      *wiltpt = wp_raw + (-0.15f * wp_raw);
      *fieldc = fc_raw + (0.07f * fc_raw);
      *ksat = ksat_raw / 1500;
       bulkd = bd_raw + (-0.08f * bd_raw);
    }
