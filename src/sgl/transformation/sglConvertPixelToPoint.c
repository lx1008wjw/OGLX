/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglConvertPixelToPoint.c
 DESCRIPTION       : sglConvertPixelToPoint command shall convert the given pixel coordinates to user coordinates
 COPYRIGHT (C)     : 2008 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"
#include "mth.h"


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_projection
  DESCRIPTION:
    Projection of point D on line (AC) in parallel to line (AB).
  PARAMETERS:
    par_f_x,par_f_y -> Coordinates of point D to project 
    par_f_x1,par_f_y1 -> Coordinates of point A
    par_f_x2,par_f_y2 -> Coordinates of point B
    par_f_x3, par_f_y3 -> Coordinates of C
	*par_f_x_prime, *par_f_y_prime -> Result of the projection
  RETURN:
    void
---------------------------------------------------------------------+*/
void oglx_projection(SGLfloat par_f_x, SGLfloat par_f_y,
                     SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2,
                     SGLfloat par_f_x3, SGLfloat par_f_y3, SGLfloat * par_f_x_prime, SGLfloat * par_f_y_prime)
{
    /* Compute v1 vector */
    SGLfloat loc_v1_x = par_f_x2 - par_f_x1;
    SGLfloat loc_v1_y = par_f_y2 - par_f_y1;

    SGLfloat loc_p2_x = par_f_x + loc_v1_x;
    SGLfloat loc_p2_y = par_f_y + loc_v1_y;

    if (!sgluFloatIsEqual(loc_p2_x, par_f_x)) {
        /* Denominator is checked in previous test */
        SGLfloat loc_D_Dp_a = (loc_p2_y - par_f_y) / (loc_p2_x - par_f_x);
        SGLfloat loc_D_Dp_b = par_f_y - (((loc_p2_y - par_f_y) * par_f_x) / (loc_p2_x - par_f_x));

        if (!sgluFloatIsEqual(par_f_x3, par_f_x1)) {
            /* Denominator is checked in previous test */
            SGLfloat loc_A_C_a = (par_f_y3 - par_f_y1) / (par_f_x3 - par_f_x1);
            SGLfloat loc_A_C_b = par_f_y1 - (((par_f_y3 - par_f_y1) * par_f_x1) / (par_f_x3 - par_f_x1));

            /* Compute the intersection of the two lines */
            *par_f_x_prime = SGLfloat_div((loc_A_C_b - loc_D_Dp_b), (loc_D_Dp_a - loc_A_C_a));
            *par_f_y_prime = SGLfloat_div(((loc_A_C_b * loc_D_Dp_a) - (loc_A_C_a * loc_D_Dp_b))
                                          , (loc_D_Dp_a - loc_A_C_a));
        }
        else {
            *par_f_x_prime = par_f_x3;
            *par_f_y_prime = par_f_y;
        }
    }
    else {
        *par_f_x_prime = par_f_x;
        *par_f_y_prime = par_f_y3;
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglConvertPixelToPoint
  DESCRIPTION:
    Function shall convert the given pixel coordinates to user coordinates
  PARAMETERS:
    par_f_x -> X-coordinate in pixels
    par_f_y -> Y-coordinate in pixels
    par_pf_user_x -> X-coordinate in user units
    par_pf_user_y -> Y-coordinate in user units
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglConvertPixelToPoint(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat * par_pf_user_x, SGLfloat * par_pf_user_y)
{
    if ((par_pf_user_x != SGL_NULL) && (par_pf_user_y != SGL_NULL)) {
        SGLfloat loc_f_x_origin, loc_f_y_origin;

        SGLfloat loc_f_x_100_0, loc_f_y_100_0;
        SGLfloat loc_f_x_0_100, loc_f_y_0_100;

        SGLfloat loc_f_x_proj_x, loc_f_y_proj_x;
        SGLfloat loc_f_x_proj_y, loc_f_y_proj_y;

        SGLfloat loc_f_norm_pixel_100_0, loc_f_norm_pixel_0_100, loc_f_norm_prime_0_x, loc_f_norm_prime_0_y;

        SGLfloat loc_f_x_sign, loc_f_y_sign;
        SGLfloat loc_f_dot_x, loc_f_dot_y;

        sglConvertPointToPixel(0.0F, 0.0F, &loc_f_x_origin, &loc_f_y_origin);
        sglConvertPointToPixel(0.0F, 100.0F, &loc_f_x_0_100, &loc_f_y_0_100);
        sglConvertPointToPixel(100.0F, 0.0F, &loc_f_x_100_0, &loc_f_y_100_0);

        /* Projection on X axis parallel to Y axis */
        oglx_projection(par_f_x, par_f_y, loc_f_x_origin, loc_f_y_origin, loc_f_x_0_100, loc_f_y_0_100, loc_f_x_100_0, loc_f_y_100_0, &loc_f_x_proj_x,
                        &loc_f_y_proj_x);

        /* Projection on Y axis parallel to X axis */
        oglx_projection(par_f_x, par_f_y, loc_f_x_origin, loc_f_y_origin, loc_f_x_100_0, loc_f_y_100_0, loc_f_x_0_100, loc_f_y_0_100, &loc_f_x_proj_y,
                        &loc_f_y_proj_y);

        /* compute the sign of x coordinate with dot product */
        loc_f_dot_x =
            (((loc_f_x_origin - loc_f_x_proj_x) * (loc_f_x_origin - loc_f_x_100_0)) + ((loc_f_y_origin - loc_f_y_proj_x) * (loc_f_y_origin - loc_f_y_100_0)));
        loc_f_dot_y =
            (((loc_f_x_origin - loc_f_x_proj_y) * (loc_f_x_origin - loc_f_x_0_100)) + ((loc_f_y_origin - loc_f_y_proj_y) * (loc_f_y_origin - loc_f_y_0_100)));

        loc_f_x_sign = loc_f_dot_x > 0.0F ? 1.0F : -1.0F;
        loc_f_y_sign = loc_f_dot_y > 0.0F ? 1.0F : -1.0F;

        loc_f_norm_pixel_100_0 = mth_norm(loc_f_x_origin, loc_f_y_origin, loc_f_x_100_0, loc_f_y_100_0);
        loc_f_norm_pixel_0_100 = mth_norm(loc_f_x_origin, loc_f_y_origin, loc_f_x_0_100, loc_f_y_0_100);

        loc_f_norm_prime_0_x = mth_norm(loc_f_x_origin, loc_f_y_origin, loc_f_x_proj_x, loc_f_y_proj_x);
        loc_f_norm_prime_0_y = mth_norm(loc_f_x_origin, loc_f_y_origin, loc_f_x_proj_y, loc_f_y_proj_y);

        /* Norms are not null as it define a 100 user units length */
        *par_pf_user_x = loc_f_x_sign * (loc_f_norm_prime_0_x * 100.0F) / loc_f_norm_pixel_100_0;
        *par_pf_user_y = loc_f_y_sign * (loc_f_norm_prime_0_y * 100.0F) / loc_f_norm_pixel_0_100;
    }
    else {
        oglxSetError(SGL_ERROR_SGL_CONVERTPIXELTOPOINT, 0U);
    }

    return;
}

/* End of File ***************************************************************/
