/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : mth_cos_sin_degree.c
 DESCRIPTION       : This function computes the cosine of an angle in degrees
 COPYRIGHT (C)     : 2008 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl_types.h"

/*+ Protected interfaces +*/
#include "mth.h"

/******************************************************************************
 **                           Local file declarations
 *****************************************************************************/

/*+ DEFINITION OF CONSTANTS -------------------------------------------------------
 DESCRIPTION       : Definition of constants of mth_cos_sin_degree.c
 LLR: [OGLX-DDD-mth_cos_sin_degree_constants]
---------------------------------------------------------------------------- +*/

/* let B be the radix (2 for our purpose) of the floating-point number */
/* system and t the number of base-B digits in the significand of a    */
/* floating-point number (24 for our purpose).                         */

/*
** B * (-t / 2)
*/
#define MTH_EPS 2.44140625E-4F
/*
** 1 / PI
*/
#define MTH_OVER_PI 5.55555556E-3F
#define MTH_90_DEGREE_F 90.0F
/*
** 201 / 64 exact
*/
#define MTH_C1       3.140625F
/*
** representation is crucial
*/
#define MTH_C2       9.676535898E-4F
/* for t <= 24 */
#define MTH_R1       (-0.1666665668F)
#define MTH_R2       0.8333025139E-2F
#define MTH_R3       (-0.1980741872E-3F)
#define MTH_R4       0.2601903036E-5F

#define MTH_TWO_PI_OVER_360     0.017453292F
/*+ END OF DEFINITION OF CONSTANTS -------------------------------------------------------*/

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_cos_sin_degree   
  DESCRIPTION:
    This function shall compute the cosine of a value in degrees. 
    {
    The result of the function shall be equivalent to cos(par_f_x) with a precision of 1.0e-4 if par_f_x is in range [-MTH_ANGLE_MAX,MTH_ANGLE_MAX]. 
    If par_f_x is less than -MTH_ANGLE_MAX the returned value shall be 2.0. 
    If par_f_x is greater than MTH_ANGLE_MAX the returned value shall be 2.0. 
    MTH_ANGLE_MAX is 99999.0 
    }
    Algorithm: 
    From "Software Manual for the Elementary Functions", 
    by William J. Cody Jr. and William Waite; Prentice Hall, 1980 
    pages 125-128 and 134-138. 
    As described in flow chart pages 127-128 the algorithm shall be: 
    {
      sign = 1.0;
      Y = abs(par_f_x) + 90.0;
      N = INTRND(Y / PI);
      if (N is odd) {
        sign = -sign;
      }
      XN = FLOAT(N);
      XN = XN - 0.5;
      f = ((Y * 2*PI/360.0) - (XN * MTH_C1)) - (XN * MTH_C2);
      if (abs(f) < MTH_EPS) {
        Result = sign * f;
      }
      else {
        g = f*f;
        Result = sign * (f + f*R(g));
      }
    }
    MTH_EPS is 2.44140625E-4.
    INTRND shall round the given value to nearest integer.  
    FLOAT shall convert given integer to a float. 	
    R(g) is a rational approximation as described in note 13 p 131-132, 
    it shall be computed as: 
    {
      MTH_R1 = -0.1666665668;
      MTH_R2 = 0.8333025139E-2;
      MTH_R3 = -0.1980741872E-3;
      MTH_R4 = 0.2601903036E-5;
      R(g) = (((MTH_R4 * g + MTH_R3) * g + MTH_R2) * g + MTH_R1) * g;
    }
  PARAMETERS:
    par_f_x -> Input angle in degrees.
	par_pf_cos -> Output value of cosine
	par_pf_sin -> Output value of sine
  RETURN:
    SGLfloat -> Cosine of the argument, range: [-1.0,1.0]  or 2.0 if abs(par_f_x) > MTH_ANGLE_MAX.
---------------------------------------------------------------------+*/
void mth_cos_sin_degree(SGLfloat par_f_x, SGLfloat * par_pf_cos, SGLfloat * par_pf_sin)
{
    /* internal data */
    SGLfloat loc_f_result_cos;
    SGLfloat loc_f_result_sin;
    SGLfloat loc_f_y = (SGLfloat) mth_f_abs(par_f_x);

    if (loc_f_y > MTH_ANGLE_MAX) {
        loc_f_result_cos = 2.0F;
        loc_f_result_sin = 2.0F;
    }
    else {
        /*
         * * compute loc_ul_n_cos = round ((Y + MTH_90_DEGREE_F) * MTH_OVER_PI);
         */
        SGLulong loc_ul_n_cos = (SGLulong) (((loc_f_y + MTH_90_DEGREE_F) * MTH_OVER_PI) + 0.5F);
        SGLfloat loc_f_f_cos = ((SGLfloat) loc_ul_n_cos) - 0.5F;
        SGLulong loc_ul_n_sin = (SGLulong) ((loc_f_y * MTH_OVER_PI) + 0.5F);
        SGLfloat loc_f_f_sin = (SGLfloat) loc_ul_n_sin;

        loc_f_f_sin = ((loc_f_y * MTH_TWO_PI_OVER_360) - (loc_f_f_sin * MTH_C1)) - (loc_f_f_sin * MTH_C2);
        loc_f_f_cos = ((loc_f_y * MTH_TWO_PI_OVER_360) - (loc_f_f_cos * MTH_C1)) - (loc_f_f_cos * MTH_C2);
        /*
         * * avoid calling ABS(F)
         */
        if ((loc_f_f_cos < MTH_EPS) && (loc_f_f_cos > -MTH_EPS)) {
            if ((loc_ul_n_cos & 1U) == 1U) {
                loc_f_result_cos = -loc_f_f_cos;
            }
            else {
                loc_f_result_cos = loc_f_f_cos;
            }
        }
        else {
            loc_f_result_cos = loc_f_f_cos * loc_f_f_cos;
            loc_f_result_cos = ((((((MTH_R4 * loc_f_result_cos) + MTH_R3) * loc_f_result_cos) + MTH_R2) * loc_f_result_cos) + MTH_R1) * loc_f_result_cos;
            loc_f_result_cos = (loc_f_f_cos + (loc_f_f_cos * loc_f_result_cos));
            if ((loc_ul_n_cos & 1U) == 1U) {
                loc_f_result_cos = -loc_f_result_cos;
            }
        }

        /*
         * * avoid calling ABS(F)
         */
        if ((loc_f_f_sin < MTH_EPS) && (loc_f_f_sin > -MTH_EPS)) {
            if (par_f_x >= 0.0F) {
                loc_f_result_sin = loc_f_f_sin;
            }
            else {
                loc_f_result_sin = -loc_f_f_sin;
            }
            /* If loc_ul_n_sin is odd */
            if ((loc_ul_n_sin & 1U) == 1U) {
                loc_f_result_sin = -loc_f_result_sin;
            }
        }
        else {
            loc_f_result_sin = loc_f_f_sin * loc_f_f_sin;
            loc_f_result_sin = ((((((MTH_R4 * loc_f_result_sin) + MTH_R3)
                                   * loc_f_result_sin) + MTH_R2) * loc_f_result_sin) + MTH_R1) * loc_f_result_sin;
            loc_f_result_sin = (loc_f_f_sin + (loc_f_f_sin * loc_f_result_sin));
            if (par_f_x < 0.0F) {
                loc_f_result_sin = -loc_f_result_sin;
            }
            /* If loc_ul_n_sin is odd */
            if ((loc_ul_n_sin & 1U) == 1U) {
                loc_f_result_sin = -loc_f_result_sin;
            }
        }
    }

    *par_pf_cos = loc_f_result_cos;
    *par_pf_sin = loc_f_result_sin;
}

/* End of File ***************************************************************/
