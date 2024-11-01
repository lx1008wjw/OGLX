/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglEllipticalArcBounds.c
 DESCRIPTION       : Function shall compute the bounds of an elliptical arc in a path.
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
  NAME: sglEllipticalArcBounds
  DESCRIPTION:
    Function shall compute the bounds of an elliptical arc in a path.
  PARAMETERS:
	par_f_prev_x, par_f_prev_y -> starting point
    par_f_rx, par_f_ry ->  X and Y radii. 
	par_f_x_axis_rotation ->  Indicates how the ellipse as a whole is rotated relative to the current coordinate system. 
	par_b_large_arc_flag -> Contributes  with par_b_sweep_flag to the automatic calculations and help determine how the arc is drawn.
	par_b_sweep_flag -> Contributes  with par_b_large_arc_flag to the automatic calculations and help determine how the arc is drawn.
	par_f_x, par_f_y -> ending point of the arc.
	par_pf_x1, par_pf_y1, par_pf_x2, par_pf_y2 -> The resulting bounds.
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglEllipticalArcBounds(SGLfloat par_f_prev_x, SGLfloat par_f_prev_y,
                            SGLfloat par_f_rx, SGLfloat par_f_ry, SGLfloat par_f_x_axis_rotation,
                            SGLbyte par_b_large_arc_flag, SGLbyte par_b_sweep_flag, SGLfloat par_f_x, SGLfloat par_f_y,
                            SGLfloat * par_pf_x1, SGLfloat * par_pf_y1, SGLfloat * par_pf_x2, SGLfloat * par_pf_y2)
{
    if ((par_pf_x1 != SGL_NULL) && (par_pf_y1 != SGL_NULL) && (par_pf_x2 != SGL_NULL) && (par_pf_y2 != SGL_NULL)) {
        SGLfloat loc_f_current_x = par_f_prev_x;
        SGLfloat loc_f_current_y = par_f_prev_y;
        SGLbool loc_b_started = SGL_FALSE;

        if (sgluFloatIsEqual(loc_f_current_x, par_f_x) && sgluFloatIsEqual(loc_f_current_y, par_f_y)) {
            *par_pf_x1 = par_f_x;
            *par_pf_y1 = par_f_y;
            *par_pf_x2 = par_f_x;
            *par_pf_y2 = par_f_y;
        }
        else {
            if (sgluFloatIsEqual(par_f_rx, 0.0F) || sgluFloatIsEqual(par_f_ry, 0.0F)) {
                *par_pf_x1 = par_f_x;
                *par_pf_y1 = par_f_y;
                *par_pf_x2 = par_f_x;
                *par_pf_y2 = par_f_y;
            }
            else {
                SGLlong loc_l_i, loc_l_n_segs;
                SGLfloat loc_f_cx, loc_f_cy;
                SGLfloat loc_f_teta1, loc_f_delta_teta;
                SGLfloat loc_f_rx = par_f_rx;
                SGLfloat loc_f_ry = par_f_ry;

                loc_l_n_segs = oglx_convert_endpoint_to_center(loc_f_current_x, loc_f_current_y, &loc_f_rx, &loc_f_ry,
                                                               par_f_x_axis_rotation,
                                                               par_b_large_arc_flag, par_b_sweep_flag,
                                                               par_f_x, par_f_y, &loc_f_teta1, &loc_f_delta_teta, &loc_f_cx, &loc_f_cy);

                /* Limit the number of segments  */
                if (loc_l_n_segs > (SGLlong) SGL_MAX_VERTEX_ARRAY_SIZE) {
                    loc_l_n_segs = (SGLlong) SGL_MAX_VERTEX_ARRAY_SIZE;
                }

                /* loc_l_n_segs defining the elliptical arc */
                for (loc_l_i = 0L; loc_l_i <= loc_l_n_segs; loc_l_i++) {
                    SGLfloat loc_f_x, loc_f_y;
                    oglx_elliptical_arc_point(par_f_x_axis_rotation, loc_f_teta1, loc_f_delta_teta,
                                              loc_f_rx, loc_f_ry, loc_f_cx, loc_f_cy, ((SGLfloat) loc_l_i) / ((SGLfloat) loc_l_n_segs), &loc_f_x, &loc_f_y);

                    if (!loc_b_started) {
                        *par_pf_x1 = loc_f_x;
                        *par_pf_y1 = loc_f_y;
                        *par_pf_x2 = loc_f_x;
                        *par_pf_y2 = loc_f_y;
                        loc_b_started = SGL_TRUE;
                    }
                    else {
                        if (loc_f_x < *par_pf_x1) {
                            *par_pf_x1 = loc_f_x;
                        }

                        if (loc_f_x > *par_pf_x2) {
                            *par_pf_x2 = loc_f_x;
                        }

                        if (loc_f_y < *par_pf_y1) {
                            *par_pf_y1 = loc_f_y;
                        }

                        if (loc_f_y > *par_pf_y2) {
                            *par_pf_y2 = loc_f_y;
                        }
                    }
                }
            }
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_ELLIPTICALARCBOUNDS, 0U);
    }


    return;
}

/* End of File ***************************************************************/
