/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglQuadraticBezier.c
 DESCRIPTION       : Function shall draw a bezier curve.
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
  NAME: oglxCalculateQuadBezierPoint
  DESCRIPTION:
    Function shall compute the coordinate of a point on a quadratic Bezier curve.
  PARAMETERS:
    par_f_t ->  Step corresponding to the point to compute 
    par_p_p ->  The point to compute. 
	par_p_p0 -> Starting point of the curve. 
	par_p_p1 -> Control point of the curve.
	par_p_p2 -> Ending point of the curve.
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxCalculateQuadBezierPoint(SGLfloat par_f_t, sgl_point * par_p_p, const sgl_point * par_p_p0, const sgl_point * par_p_p1, const sgl_point * par_p_p2)
{
    SGLfloat loc_f_u = 1.0F - par_f_t;
    SGLfloat loc_f_tt = par_f_t * par_f_t;
    SGLfloat loc_f_uu = loc_f_u * loc_f_u;
    SGLfloat loc_f_2_u_t = 2.0F * loc_f_u * par_f_t;

    par_p_p->f_x = loc_f_uu * par_p_p0->f_x;
    par_p_p->f_y = loc_f_uu * par_p_p0->f_y;

    par_p_p->f_x = par_p_p->f_x + (loc_f_2_u_t * par_p_p1->f_x);
    par_p_p->f_y = par_p_p->f_y + (loc_f_2_u_t * par_p_p1->f_y);

    par_p_p->f_x = par_p_p->f_x + (loc_f_tt * par_p_p2->f_x);
    par_p_p->f_y = par_p_p->f_y + (loc_f_tt * par_p_p2->f_y);

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_quad_bezier_points
  DESCRIPTION:
    Function shall compute the number of points to draw a quadratic bezier curve.
  PARAMETERS:
	par_p_p0 -> Starting point of the curve. 
	par_p_p1 -> Control point of the curve.
	par_p_p2 -> Ending point of the curve.
  RETURN:
    The number of points on the curve.
---------------------------------------------------------------------+*/
SGLulong oglx_quad_bezier_points(const sgl_point * par_p_p0, const sgl_point * par_p_p1, const sgl_point * par_p_p2)
{
    sgl_point loc_p_p0_pix;
    sgl_point loc_p_p1_pix;
    sgl_point loc_p_p2_pix;
    SGLfloat loc_f_norm_segment;
    SGLfloat loc_f_norm_p0_p1;
    SGLfloat loc_f_norm_p1_p2;

    sglConvertPointToPixel(par_p_p0->f_x, par_p_p0->f_y, &(loc_p_p0_pix.f_x), &(loc_p_p0_pix.f_y));
    sglConvertPointToPixel(par_p_p1->f_x, par_p_p1->f_y, &(loc_p_p1_pix.f_x), &(loc_p_p1_pix.f_y));
    sglConvertPointToPixel(par_p_p2->f_x, par_p_p2->f_y, &(loc_p_p2_pix.f_x), &(loc_p_p2_pix.f_y));

    loc_f_norm_segment = mth_norm(loc_p_p0_pix.f_x, loc_p_p0_pix.f_y, loc_p_p2_pix.f_x, loc_p_p2_pix.f_y);
    loc_f_norm_p0_p1 = mth_norm(loc_p_p0_pix.f_x, loc_p_p0_pix.f_y, loc_p_p1_pix.f_x, loc_p_p1_pix.f_y);
    loc_f_norm_p1_p2 = mth_norm(loc_p_p2_pix.f_x, loc_p_p2_pix.f_y, loc_p_p1_pix.f_x, loc_p_p1_pix.f_y);

    return SGLfloat_to_uint32((loc_f_norm_segment + loc_f_norm_p0_p1 + loc_f_norm_p1_p2) / 10.0F);
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglQuadraticBezier
  DESCRIPTION:
    Function shall draw a quadratic Bezier curve.
  PARAMETERS:
    par_f_x1,par_f_y1 ->  Coordinates of control point. 
	par_f_x,par_f_y ->  Ending point. 
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglQuadraticBezier(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x, SGLfloat par_f_y)
{
    if (glob_pr_sglStatemachine->b_path_type != SGL_NO_PATH) {
        SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;

        /* The size can be 0 after a sglMoveTo */
        if ((loc_l_size > 0) || (glob_pr_sglStatemachine->b_last_command == OGLX_MOVE_TO)) {
            SGLulong loc_ul_i;
            SGLulong loc_ul_nb_segments;

            sgl_point loc_p_p0;
            sgl_point loc_p_p1;
            sgl_point loc_p_p2;

            sgl_point loc_p_p_temp;
            SGLfloat loc_f_t;

            loc_p_p0.f_x = glob_pr_sglStatemachine->f_last_path_point_x;
            loc_p_p0.f_y = glob_pr_sglStatemachine->f_last_path_point_y;

            loc_p_p1.f_x = par_f_x1;
            loc_p_p1.f_y = par_f_y1;

            loc_p_p2.f_x = par_f_x;
            loc_p_p2.f_y = par_f_y;

            loc_ul_nb_segments = oglx_quad_bezier_points(&loc_p_p0, &loc_p_p1, &loc_p_p2);

            /* Limit the number of segments  */
            if (loc_ul_nb_segments > SGL_MAX_VERTEX_ARRAY_SIZE) {
                loc_ul_nb_segments = SGL_MAX_VERTEX_ARRAY_SIZE;
            }

            /* The first point of the curve is already in table as last point of last command */
            for (loc_ul_i = 1; loc_ul_i <= loc_ul_nb_segments; loc_ul_i++) {
                loc_f_t = SGLfloat_div((SGLfloat) loc_ul_i, (SGLfloat) loc_ul_nb_segments);
                oglxCalculateQuadBezierPoint(loc_f_t, &loc_p_p_temp, &loc_p_p0, &loc_p_p1, &loc_p_p2);
                oglxVertex2f(loc_p_p_temp.f_x, loc_p_p_temp.f_y);
            }
        }

        glob_pr_sglStatemachine->f_last_path_point_x = par_f_x;
        glob_pr_sglStatemachine->f_last_path_point_y = par_f_y;
        glob_pr_sglStatemachine->f_last_path_control_x = par_f_x1;
        glob_pr_sglStatemachine->f_last_path_control_y = par_f_y1;

        glob_pr_sglStatemachine->b_last_command = OGLX_QUAD_BEZIER;
    }
    return;
}

/* End of File ***************************************************************/
