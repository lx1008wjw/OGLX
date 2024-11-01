/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluArcCircleLinePointsList2f.c
 DESCRIPTION       : sgluArcCircleLinePointsList2f command shall generate list 
					of vertices for a 2D Arc
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

/*+ DEFINITION OF CONSTANTS -------------------------------------------------------
 DESCRIPTION       : Definition of constants of sgluArcCircleLinePointsList2f.c
 LLR: [OGLX-DDD-sgluArcCircleLinePointsList2f_constants]
---------------------------------------------------------------------------- +*/
/* Epsilon value for a radius */
#define SGL_EPSILON_RADIUS		0.1F
/* Epsilon value for a positive float */
#define SGL_EPSILON_ABS			0.0001F
/*+ END OF DEFINITION OF CONSTANTS -------------------------------------------------------*/


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_simple_arc
  DESCRIPTION:
    Function shall compute list of vertices for arcs or 
    sectors.
  PARAMETERS:
    par_f_centerx -> x-coordinate of the origin of the arc or sector
    par_f_centery -> y-coordinate of the origin of the arc or sector
    par_f_startx -> x-coordinate of the arc or sector
    par_f_starty -> y-coordinate of the arc or sector
    par_f_clockwise -> clockwise mode (-1.0F or 1.0F)
    par_f_angle_ok -> angle of the arc or sector
    par_f_x_radius -> X radius
	par_f_y_radius -> Y radius
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_simple_arc(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx,
                     SGLfloat par_f_starty, SGLfloat par_f_clockwise, SGLfloat par_f_angle_ok, SGLfloat par_f_x_radius, SGLfloat par_f_y_radius)
{
    SGLlong loc_l_end_index = 1L;
    SGLfloat loc_f_tmpx = par_f_startx;
    SGLfloat loc_f_tmpy = par_f_starty;

    SGLfloat loc_f_deltax = (loc_f_tmpx - par_f_centerx);
    SGLfloat loc_f_deltay = (loc_f_tmpy - par_f_centery);

    SGLfloat loc_f_angle_end;
    /*
     ** radius calculation
     */
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_x;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_y;
    SGLfloat loc_f_sin = 0.0F;
    SGLfloat loc_f_cos = 0.0F;
    SGLfloat loc_f_sin_prev;
    SGLfloat loc_f_cos_prev;
    SGLfloat loc_f_sin_inc = 0.0F;
    SGLfloat loc_f_cos_inc = 0.0F;

    SGLfloat loc_f_distance = (SGLfloat_div(par_f_x_radius, loc_f_ratio_x) + SGLfloat_div(par_f_y_radius, loc_f_ratio_y)) * 0.5F;
    /*
     ** Local Variable ( set forward)
     */
    SGLlong loc_l_n;
    SGLfloat loc_f_n;
    SGLfloat loc_f_angle_inc;
    SGLfloat loc_f_angle;

    loc_f_angle = mth_acos_degree(loc_f_deltax, par_f_x_radius);

    if (loc_f_distance < MTH_RADIUS_VERY_SMALL_MAX) {
        loc_f_angle_inc = MTH_DELTA_I_VERY_SMALL;
        loc_f_n = SGLfloat_div(par_f_angle_ok, loc_f_angle_inc);
        loc_l_n = ((SGLlong) loc_f_n) + 1L;
    }
    else {
        /*
         ** Only if the radius superior to MTH_RADIUS_VERY_SMALL_MAX
         */
        loc_f_angle_inc = mth_get_angle_from_radius(loc_f_distance);
        loc_f_n = SGLfloat_div(par_f_angle_ok, loc_f_angle_inc);
        loc_l_n = mth_f_to_l_ceil(loc_f_n);
    }

    if (loc_f_n > SGL_EPSILON_ABS) {
        loc_f_angle_inc = SGLfloat_div(par_f_angle_ok, loc_f_n) * par_f_clockwise;
    }
    else {
        loc_f_angle_inc = loc_f_angle_inc * par_f_clockwise;
    }
    loc_f_angle = mth_move_in_good_quadrant(loc_f_angle, loc_f_deltax, loc_f_deltay);

    loc_f_angle_end = loc_f_angle + (par_f_angle_ok * par_f_clockwise);

    /* If the arc is drawn after a call to sglVertex2f, there is no need to draw first vertex a second time */
    if (!(glob_pr_sglStatemachine->b_new_vertex)) {
        oglxVertex2f(par_f_startx, par_f_starty);
    }
    else {
        /* Nothing to do */
    }

    /* Avoid last arc point too close from first point of next line */
    if ((loc_f_distance < MTH_RADIUS_MEDIUM_SMALL_MAX) && (glob_pr_sglStatemachine->b_drawing_mode == SGL_MODE_LINE)
        && (mth_f_abs(loc_f_angle) < 110.0F) && (!(glob_pr_sglStatemachine->b_draw_circle))) {
        loc_l_end_index = 2L;
    }
    else {
        /* Nothing to do */
    }
    mth_cos_sin_degree(loc_f_angle, &loc_f_cos, &loc_f_sin);
    mth_cos_sin_degree(loc_f_angle_inc, &loc_f_cos_inc, &loc_f_sin_inc);
    loc_f_sin_prev = loc_f_sin;
    loc_f_cos_prev = loc_f_cos;
    while (loc_l_n > loc_l_end_index) {
        loc_l_n--;
        loc_f_cos = (loc_f_cos_prev * loc_f_cos_inc) - (loc_f_sin_prev * loc_f_sin_inc);
        loc_f_sin = (loc_f_sin_prev * loc_f_cos_inc) + (loc_f_cos_prev * loc_f_sin_inc);
        loc_f_sin_prev = loc_f_sin;
        loc_f_cos_prev = loc_f_cos;
        loc_f_tmpx = par_f_centerx + (par_f_x_radius * loc_f_cos);
        loc_f_tmpy = par_f_centery + (par_f_y_radius * loc_f_sin);
        oglxVertex2f(loc_f_tmpx, loc_f_tmpy);
    }

    if ((par_f_angle_ok >= (SGLfloat) 360.0F) && !(glob_pr_sglStatemachine->b_close_line_loop)) {
        /* In the case of 360 degree, the last point is the first */
        oglxVertex2f(par_f_startx, par_f_starty);
    }
    else {
        /*
         ** Drawing the N-1 line
         ** Calculate the last coordinate for the last
         ** point
         */
        mth_cos_sin_degree(loc_f_angle_end, &loc_f_cos, &loc_f_sin);
        loc_f_tmpx = par_f_centerx + (par_f_x_radius * loc_f_cos);
        loc_f_tmpy = par_f_centery + (par_f_y_radius * loc_f_sin);

        /* Avoid drawing the last point if it is too close for the first point */
        if (glob_pr_sglStatemachine->b_close_line_loop) {
            if (((mth_f_abs(loc_f_tmpx - glob_pr_sglStatemachine->f_line_loop_first_vertex_x) > SGL_EPSILON_ABS)
                 || (mth_f_abs(loc_f_tmpy - glob_pr_sglStatemachine->f_line_loop_first_vertex_y) > SGL_EPSILON_ABS))) {
                /* Set the last point */
                oglxVertex2f(loc_f_tmpx, loc_f_tmpy);
            }
        }
        else {
            oglxVertex2f(loc_f_tmpx, loc_f_tmpy);
        }
    }

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_compute_arc_distance
  DESCRIPTION:
    Function shall compute the radius of an arc
  PARAMETERS:
    par_f_centerx -> x-coordinate of the origin of the arc or sector
    par_f_centery -> y-coordinate of the origin of the arc or sector
    par_f_startx -> x-coordinate of the arc or sector
    par_f_starty -> y-coordinate of the arc or sector
  RETURN:
    SGLfloat -> Radius of the arc
---------------------------------------------------------------------+*/
SGLfloat oglx_compute_arc_distance(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty)
{
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_x;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_y;

    SGLfloat loc_f_distance =
        ((par_f_startx - par_f_centerx) * (par_f_startx - par_f_centerx)) + ((par_f_starty - par_f_centery) * (par_f_starty - par_f_centery));
    loc_f_distance = mth_sqrtf(loc_f_distance);

    loc_f_distance = (SGLfloat_div(loc_f_distance, loc_f_ratio_x) + SGLfloat_div(loc_f_distance, loc_f_ratio_y)) * 0.5F;

    if (loc_f_distance < SGL_EPSILON_RADIUS) {
        loc_f_distance = SGL_EPSILON_RADIUS;
    }
    else {
        /* Nothing to do */
    }

    return loc_f_distance;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgluArcCircleLinePointsList2f
  DESCRIPTION:
    Function shall generate a list of vertices for a 2D Arc.
  PARAMETERS:
    par_f_centerx -> x-coordinate of the origin of the arc or sector
    par_f_centery -> y-coordinate of the origin of the arc or sector
    par_f_startx -> x-coordinate of the arc or sector
    par_f_starty -> y-coordinate of the arc or sector
	par_f_x_radius -> X radius
	par_f_y_radius -> Y radius
    par_b_clockwise -> orientation of angle
    par_f_angle -> angle in degrees of the arc or sector
  RETURN:
    None  
---------------------------------------------------------------------+*/
void sgluArcCircleLinePointsList2f(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty,
                                   SGLfloat par_f_x_radius, SGLfloat par_f_y_radius, SGLbool par_b_clockwise, SGLfloat par_f_angle)
{
    SGLfloat loc_f_clockwise;
    SGLfloat loc_f_angle_ok;

    /* Local variables used to calculate the arc radius */
    SGLfloat loc_f_distance;

    SGLbool loc_b_error_detected = SGL_FALSE;

    if (par_b_clockwise) {
        loc_f_clockwise = -1.0F;
    }
    else {
        loc_f_clockwise = 1.0F;
    }


    if (par_f_angle >= 360.0F) {
        loc_f_angle_ok = 360.0F;
    }
    else {
        loc_f_angle_ok = par_f_angle;
    }

    /* Check angle is positive */
    if (loc_f_angle_ok <= (SGLfloat) 0.0F) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        loc_f_distance = oglx_compute_arc_distance(par_f_centerx, par_f_centery, par_f_startx, par_f_starty);
        if (loc_f_distance <= MTH_RADIUS_TOO_SMALL_MAX) {
            oglxVertex2f(par_f_centerx, par_f_centery);
            oglxVertex2f(par_f_centerx, par_f_centery);
        }
        else {
            oglx_simple_arc(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, loc_f_clockwise, loc_f_angle_ok, par_f_x_radius, par_f_y_radius);
        }
    }
    else {
        /* Nothing to do */
    }
    return;
}

/* End of File ***************************************************************/
