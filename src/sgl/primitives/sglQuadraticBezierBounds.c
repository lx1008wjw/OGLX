/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglQuadraticBezierBounds.c
 DESCRIPTION       : Function shall compute the bounds of a bezier curve.
 COPYRIGHT (C)     : 2016 Esterel Technologies SAS. All Rights Reserved.
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
  NAME: sglQuadraticBezierBounds
  DESCRIPTION:
    Function shall compute the bounds of a quadratic Bezier curve.
  PARAMETERS:
	par_f_prev_x, par_f_prev_y -> Starting point.
    par_f_x1,par_f_y1 ->  Coordinates of control point. 
	par_f_x,par_f_y ->  Ending point. 
	par_pf_x1, par_pf_y1, par_pf_x2, par_pf_y2 -> The resulting bounds.
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglQuadraticBezierBounds(SGLfloat par_f_prev_x, SGLfloat par_f_prev_y,
                              SGLfloat par_f_x1, SGLfloat par_f_y1,
                              SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat * par_pf_x1, SGLfloat * par_pf_y1, SGLfloat * par_pf_x2, SGLfloat * par_pf_y2)
{
    if ((par_pf_x1 != SGL_NULL) && (par_pf_y1 != SGL_NULL) && (par_pf_x2 != SGL_NULL) && (par_pf_y2 != SGL_NULL)) {
        SGLulong loc_ul_i;
        SGLulong loc_ul_nb_segments;

        sgl_point loc_p_p0;
        sgl_point loc_p_p1;
        sgl_point loc_p_p2;

        sgl_point loc_p_p_temp;
        SGLfloat loc_f_t;
        SGLbool loc_b_started = SGL_FALSE;

        loc_p_p0.f_x = par_f_prev_x;
        loc_p_p0.f_y = par_f_prev_y;

        loc_p_p1.f_x = par_f_x1;
        loc_p_p1.f_y = par_f_y1;

        loc_p_p2.f_x = par_f_x;
        loc_p_p2.f_y = par_f_y;

        loc_ul_nb_segments = oglx_quad_bezier_points(&loc_p_p0, &loc_p_p1, &loc_p_p2);

        /* Limit the number of segments  */
        if (loc_ul_nb_segments > SGL_MAX_VERTEX_ARRAY_SIZE) {
            loc_ul_nb_segments = SGL_MAX_VERTEX_ARRAY_SIZE;
        }

        for (loc_ul_i = 0UL; loc_ul_i <= loc_ul_nb_segments; loc_ul_i++) {
            loc_f_t = SGLfloat_div((SGLfloat) loc_ul_i, (SGLfloat) loc_ul_nb_segments);
            oglxCalculateQuadBezierPoint(loc_f_t, &loc_p_p_temp, &loc_p_p0, &loc_p_p1, &loc_p_p2);

            if (!loc_b_started) {
                *par_pf_x1 = loc_p_p_temp.f_x;
                *par_pf_y1 = loc_p_p_temp.f_y;
                *par_pf_x2 = loc_p_p_temp.f_x;
                *par_pf_y2 = loc_p_p_temp.f_y;
                loc_b_started = SGL_TRUE;
            }
            else {
                if (loc_p_p_temp.f_x < *par_pf_x1) {
                    *par_pf_x1 = loc_p_p_temp.f_x;
                }

                if (loc_p_p_temp.f_x > *par_pf_x2) {
                    *par_pf_x2 = loc_p_p_temp.f_x;
                }

                if (loc_p_p_temp.f_y < *par_pf_y1) {
                    *par_pf_y1 = loc_p_p_temp.f_y;
                }

                if (loc_p_p_temp.f_y > *par_pf_y2) {
                    *par_pf_y2 = loc_p_p_temp.f_y;
                }
            }
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_QUADBEZIERBOUNDS, 0U);
    }

    return;
}

/* End of File ***************************************************************/
