/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglEllipticalArc.c
 DESCRIPTION       : Function shall draw an elliptical arc in a path.
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
  NAME: oglx_angle_between
  DESCRIPTION:
    Function shall compute the angle in radian between two vectors.
  PARAMETERS:
    par_f_x1, par_f_y1 -> First vector.
	par_f_x2, par_f_y2 -> Second vector.
  RETURN:
    The angle between the two vectors.
---------------------------------------------------------------------+*/
SGLfloat oglx_angle_between(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2)
{
    /* Dot product of the two vectors */
    SGLfloat loc_f_num = (par_f_x1 * par_f_x2) + (par_f_y1 * par_f_y2);
    /* Norms of the two vectors */
    SGLfloat loc_f_norm_v1 = mth_sqrtf((par_f_x1 * par_f_x1) + (par_f_y1 * par_f_y1));
    SGLfloat loc_f_norm_v2 = mth_sqrtf((par_f_x2 * par_f_x2) + (par_f_y2 * par_f_y2));

    /* Eventually change the orientation of the angle */
    SGLfloat loc_f_sign = (((par_f_x1 * par_f_y2) - (par_f_y1 * par_f_x2)) < 0.0F) ? -1.0F : 1.0F;

    SGLfloat loc_f_angle = SGLfloat_div(loc_f_num, loc_f_norm_v1 * loc_f_norm_v2);

    return loc_f_sign * mth_acos(loc_f_angle);
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_elliptical_arc_point
  DESCRIPTION:
    Function shall define a point on an elliptical arc.
  PARAMETERS:
	par_f_x_axis_rotation ->  Indicates how the ellipse as a whole is rotated relative to the current coordinate system. 
	par_f_start_angle -> starting angle on the arc.
	par_f_delta_teta -> total angle on the arc.
    par_f_rx, par_f_ry ->  X and Y radii.
	par_f_cx, par_f_cy -> center of the arc
	par_f_t -> percentage of the position of the point to compute
	par_pf_x, par_pf_y -> resulting point.
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_elliptical_arc_point(SGLfloat par_f_x_axis_rotation, SGLfloat par_f_start_angle, SGLfloat par_f_delta_teta,
                               SGLfloat par_f_rx, SGLfloat par_f_ry,
                               SGLfloat par_f_cx, SGLfloat par_f_cy, SGLfloat par_f_t, SGLfloat * par_pf_x, SGLfloat * par_pf_y)
{
    /* F.6.3 Parameterization alternatives */
    SGLfloat loc_f_angle = par_f_start_angle + (par_f_delta_teta * par_f_t);
    SGLfloat loc_f_ellipse_x = par_f_rx * mth_cos_degree(loc_f_angle);
    SGLfloat loc_f_ellipse_y = par_f_ry * mth_sin_degree(loc_f_angle);
    SGLfloat loc_f_cos_phi = mth_cos_degree(par_f_x_axis_rotation);
    SGLfloat loc_f_sin_phi = mth_sin_degree(par_f_x_axis_rotation);

    *par_pf_x = (loc_f_cos_phi * loc_f_ellipse_x) - (loc_f_sin_phi * loc_f_ellipse_y) + par_f_cx;
    *par_pf_y = (loc_f_sin_phi * loc_f_ellipse_x) + (loc_f_cos_phi * loc_f_ellipse_y) + par_f_cy;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_convert_endpoint_to_center
  DESCRIPTION:
    Function shall convert end point parameterization to center parameterization of the arc.
  PARAMETERS:
	par_f_current_x, par_f_current_y ->  Last path position. 
	par_f_rx, par_f_ry -> Pointers to radiix
	par_f_x_axis_rotation ->  Indicates how the ellipse as a whole is rotated relative to the current coordinate system. 
	par_b_large_arc_flag -> Contributes  with par_b_sweep_flag to the automatic calculations and help determine how the arc is drawn.
	par_b_sweep_flag -> Contributes  with par_b_large_arc_flag to the automatic calculations and help determine how the arc is drawn.
	par_f_x, par_f_y -> ending point of the arc.
	par_f_teta1 -> Pointer to starting angle on the arc.
	par_f_delta_teta -> Pointer to total angle on the arc.
	par_f_cx, par_f_cy -> Pointers to center coordinates
  RETURN:
    The number of points on the elliptical arc.
---------------------------------------------------------------------+*/
SGLlong oglx_convert_endpoint_to_center(SGLfloat par_f_current_x, SGLfloat par_f_current_y,
                                        SGLfloat * par_f_rx, SGLfloat * par_f_ry,
                                        SGLfloat par_f_x_axis_rotation,
                                        SGLbyte par_b_large_arc_flag, SGLbyte par_b_sweep_flag,
                                        SGLfloat par_f_x, SGLfloat par_f_y,
                                        SGLfloat * par_f_teta1, SGLfloat * par_f_delta_teta, SGLfloat * par_f_cx, SGLfloat * par_f_cy)
{
    /*
       This implementation follows www.w3.org/TR/SVG/implnote.html#ArcImplementationNotes
     */
    SGLfloat loc_f_cos_phi, loc_f_sin_phi;
    SGLfloat loc_f_x_factor, loc_f_y_factor;
    SGLfloat loc_f_x1_prime, loc_f_y1_prime;
    SGLfloat loc_f_rx, loc_f_ry;
    SGLfloat loc_f_rx2, loc_f_ry2;
    SGLfloat loc_f_x1_prime2, loc_f_y1_prime2;
    SGLfloat loc_f_num, loc_f_den;
    SGLfloat loc_f_square, loc_f_c_factor;
    SGLfloat loc_f_cx_prime, loc_f_cy_prime;
    SGLfloat loc_f_radii_check;
    SGLfloat loc_f_teta1, loc_f_delta_teta;
    SGLfloat loc_f_start_vector_x, loc_f_start_vector_y;
    SGLfloat loc_f_end_vector_x, loc_f_end_vector_y;

    /* Force positive radiix */
    loc_f_rx = mth_f_abs(*par_f_rx);
    loc_f_ry = mth_f_abs(*par_f_ry);

    /* F.6.5 Conversion from endpoint to center parameterization */
    loc_f_cos_phi = mth_cos_degree(par_f_x_axis_rotation);
    loc_f_sin_phi = mth_sin_degree(par_f_x_axis_rotation);
    loc_f_x_factor = 0.5F * (par_f_current_x - par_f_x);
    loc_f_y_factor = 0.5F * (par_f_current_y - par_f_y);

    /* Step 1: Compute (X1',Y1') */
    loc_f_x1_prime = (loc_f_cos_phi * loc_f_x_factor) + (loc_f_sin_phi * loc_f_y_factor);
    loc_f_y1_prime = (-loc_f_sin_phi * loc_f_x_factor) + (loc_f_cos_phi * loc_f_y_factor);
    loc_f_x1_prime2 = loc_f_x1_prime * loc_f_x1_prime;
    loc_f_y1_prime2 = loc_f_y1_prime * loc_f_y1_prime;

    /* F.6.6 Correction of out-of-range radii */
    loc_f_rx2 = loc_f_rx * loc_f_rx;
    loc_f_ry2 = loc_f_ry * loc_f_ry;
    loc_f_radii_check = (loc_f_x1_prime2 / loc_f_rx2) + (loc_f_y1_prime2 / loc_f_ry2);
    if (loc_f_radii_check > 1.0F) {
        loc_f_radii_check = mth_sqrtf(loc_f_radii_check);
        loc_f_rx = loc_f_radii_check * loc_f_rx;
        loc_f_ry = loc_f_radii_check * loc_f_ry;
        loc_f_rx2 = loc_f_rx * loc_f_rx;
        loc_f_ry2 = loc_f_ry * loc_f_ry;
    }

    /* Step 2: Compute (CX',CY') */
    loc_f_num = (loc_f_rx2 * loc_f_ry2) - (loc_f_rx2 * loc_f_y1_prime2) - (loc_f_ry2 * loc_f_x1_prime2);
    loc_f_den = (loc_f_rx2 * loc_f_y1_prime2) + (loc_f_ry2 * loc_f_x1_prime2);
    loc_f_square = SGLfloat_div(loc_f_num, loc_f_den);
    loc_f_c_factor = mth_sqrtf(loc_f_square);

    loc_f_c_factor = loc_f_c_factor * ((par_b_large_arc_flag != par_b_sweep_flag) ? 1.0F : -1.0F);

    loc_f_cx_prime = loc_f_c_factor * ((loc_f_rx * loc_f_y1_prime) / loc_f_ry);
    loc_f_cy_prime = loc_f_c_factor * (-((loc_f_ry * loc_f_x1_prime) / loc_f_rx));

    /* Step 3: Compute (Cx,Cy), coordinates of the center of the ellipse */
    *par_f_cx = (loc_f_cos_phi * loc_f_cx_prime) + (-loc_f_sin_phi * loc_f_cy_prime) + ((par_f_current_x + par_f_x) / 2.0F);
    *par_f_cy = (loc_f_sin_phi * loc_f_cx_prime) + (loc_f_cos_phi * loc_f_cy_prime) + ((par_f_current_y + par_f_y) / 2.0F);

    /* Step4: Compute angles */

    /* Compute Teta1 */
    loc_f_start_vector_x = (loc_f_x1_prime - loc_f_cx_prime) / loc_f_rx;
    loc_f_start_vector_y = (loc_f_y1_prime - loc_f_cy_prime) / loc_f_ry;

    loc_f_teta1 = oglx_angle_between(1.0F, 0.0F, loc_f_start_vector_x, loc_f_start_vector_y);
    *par_f_teta1 = loc_f_teta1 * MTH_180_OVER_PI;

    /* Compute DeltaTeta */
    loc_f_end_vector_x = (-loc_f_x1_prime - loc_f_cx_prime) / loc_f_rx;
    loc_f_end_vector_y = (-loc_f_y1_prime - loc_f_cy_prime) / loc_f_ry;

    loc_f_delta_teta = oglx_angle_between(loc_f_start_vector_x, loc_f_start_vector_y, loc_f_end_vector_x, loc_f_end_vector_y);

    loc_f_delta_teta = loc_f_delta_teta * MTH_180_OVER_PI;
    /* The angle shall be modulo 360 */
    loc_f_delta_teta = sgluModuloAngle(loc_f_delta_teta);

    if ((par_b_sweep_flag == (SGLbyte) SGL_FALSE) && (loc_f_delta_teta > 0.0F)) {
        loc_f_delta_teta = loc_f_delta_teta - 360.0F;
    }
    else {
        /* Nothing to do if par_b_sweep_flag is SGL_TRUE */
        /* loc_f_delta_teta is positive here as requested by detailed design as sgluModuloAngle returns in range [0.0,360.0] */
    }
    *par_f_delta_teta = loc_f_delta_teta;

    *par_f_rx = loc_f_rx;
    *par_f_ry = loc_f_ry;

    return mth_f_to_l_ceil(mth_f_abs(loc_f_delta_teta * ((loc_f_rx + loc_f_ry) / 2.0F) * 0.01F));
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglEllipticalArc
  DESCRIPTION:
    Function shall draw an elliptical arc.
  PARAMETERS:
    par_f_rx, par_f_ry ->  X and Y radii. 
	par_f_x_axis_rotation ->  Indicates how the ellipse as a whole is rotated relative to the current coordinate system. 
	par_b_large_arc_flag -> Contributes  with par_b_sweep_flag to the automatic calculations and help determine how the arc is drawn.
	par_b_sweep_flag -> Contributes  with par_b_large_arc_flag to the automatic calculations and help determine how the arc is drawn.
	par_f_x, par_f_y -> ending point of the arc.
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglEllipticalArc(SGLfloat par_f_rx, SGLfloat par_f_ry,
                      SGLfloat par_f_x_axis_rotation, SGLbyte par_b_large_arc_flag, SGLbyte par_b_sweep_flag, SGLfloat par_f_x, SGLfloat par_f_y)
{
    if (glob_pr_sglStatemachine->b_path_type != SGL_NO_PATH) {
        if ((glob_pr_sglStatemachine->tr_geometry.l_size > 0) || (glob_pr_sglStatemachine->b_last_command == OGLX_MOVE_TO)) {
            SGLfloat loc_f_current_x = glob_pr_sglStatemachine->f_last_path_point_x;
            SGLfloat loc_f_current_y = glob_pr_sglStatemachine->f_last_path_point_y;

            if (sgluFloatIsEqual(loc_f_current_x, par_f_x) && sgluFloatIsEqual(loc_f_current_y, par_f_y)) {
                /* Nothing to do */
            }
            else {
                if (sgluFloatIsEqual(par_f_rx, 0.0F) || sgluFloatIsEqual(par_f_ry, 0.0F)) {
                    sglLineTo(par_f_x, par_f_y);
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

                    /* The first point of the curve is already in table as last point of last command */
                    for (loc_l_i = 1L; loc_l_i <= loc_l_n_segs; loc_l_i++) {
                        SGLfloat loc_f_x, loc_f_y;
                        oglx_elliptical_arc_point(par_f_x_axis_rotation, loc_f_teta1, loc_f_delta_teta,
                                                  loc_f_rx, loc_f_ry, loc_f_cx, loc_f_cy, ((SGLfloat) loc_l_i) / ((SGLfloat) loc_l_n_segs), &loc_f_x, &loc_f_y);
                        oglxVertex2f(loc_f_x, loc_f_y);
                    }
                }
            }
        }

        glob_pr_sglStatemachine->f_last_path_point_x = par_f_x;
        glob_pr_sglStatemachine->f_last_path_point_y = par_f_y;
        glob_pr_sglStatemachine->f_last_path_control_x = par_f_x;
        glob_pr_sglStatemachine->f_last_path_control_y = par_f_y;

        glob_pr_sglStatemachine->b_last_command = OGLX_ELL_ARC;
    }
    return;
}

/* End of File ***************************************************************/
