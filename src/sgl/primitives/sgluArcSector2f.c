/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluArcSector2f.c
 DESCRIPTION       : sgluArcSector2f command shall draw a 2D Arc or Sector
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
  NAME: sglu_arc_sector_2f_detect_error
  DESCRIPTION:
    Function shall check parameters and context of sgluArcSector2f.
  PARAMETERS:
    par_b_mode -> mode of the arc or sector (SGL_ARC_FILLED,SGL_ARC_OUTLINE or SGL_ARC_CIRCLE_LINE)
  RETURN:
    SGLbool: Error detected (SGL_TRUE) or not (SGL_FALSE)
---------------------------------------------------------------------+*/
SGLbool sglu_arc_sector_2f_detect_error(SGLbyte par_b_mode, SGLbool par_b_clockwise, SGLfloat par_f_start_angle, SGLfloat par_f_end_angle)
{
    SGLbool loc_b_error_detected = SGL_FALSE;

    if ((par_f_start_angle > MTH_ANGLE_MAX) || (par_f_start_angle < -MTH_ANGLE_MAX) || (par_f_end_angle > MTH_ANGLE_MAX) || (par_f_end_angle < -MTH_ANGLE_MAX)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if ((par_b_clockwise != SGL_TRUE) && (par_b_clockwise != SGL_FALSE)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if ((par_b_mode != SGL_ARC_FILLED) && (par_b_mode != SGL_ARC_OUTLINE) && (par_b_mode != SGL_ARC_CIRCLE_LINE)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_error_detected;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_outline_arc_sector
  DESCRIPTION:
    Function shall draw an outline Arc or Sector.
  PARAMETERS:
    par_f_centerx -> x-coordinate of the center of the arc or sector
    par_f_centery -> y-coordinate of the center of the arc or sector
    par_f_startx -> x-coordinate of the start of the arc or sector
    par_f_starty -> y-coordinate of the start of the arc or sector
    par_b_clockwise	-> clockwise or not
    par_f_angle -> angle of the arc or sector, range: [0.0F,360.0F]
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_outline_arc_sector(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty,
                                 SGLfloat par_f_radius, SGLbool par_b_clockwise, SGLfloat par_f_angle)
{
    /* Do not use optmization for arcs, else the last frame does not appear */
    sglBegin(SGL_LINE_LOOP);
    if (glob_pr_sglStatemachine->b_stipple_ok) {
        /* Switch to SGL_LINES to draw stipple */
        glob_pr_sglStatemachine->b_switch_to_sgl_lines = SGL_TRUE;
        glob_pr_sglStatemachine->ul_currentAction = SGL_LINES;
        glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINES;
    }

    if (par_f_angle < 360.0F) {
        oglxVertex2f(par_f_centerx, par_f_centery);
    }
    else {
        /* Nothing to do */
    }
    sgluArcCircleLinePointsList2f(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, par_f_radius, par_f_radius, par_b_clockwise, par_f_angle);
    sglEnd();

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglu_arc_sector_2f
  DESCRIPTION:
    Function shall draw a 2D Arc or Sector.
  PARAMETERS:
    par_f_centerx -> x-coordinate of the center of the arc or sector
    par_f_centery -> y-coordinate of the center of the arc or sector
    par_f_startx -> x-coordinate of the start of the arc or sector
    par_f_starty -> y-coordinate of the start of the arc or sector
    par_b_clockwise	-> clockwise or not
    par_f_angle -> angle of the arc or sector, range: [0.0F,360.0F]
    par_b_mode -> mode of the arc or sector (SGL_ARC_FILLED,SGL_ARC_OUTLINE or SGL_ARC_CIRCLE_LINE)
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglu_arc_sector_2f(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty, SGLfloat par_f_radius,
                        SGLbool par_b_clockwise, SGLfloat par_f_angle, SGLbyte par_b_mode)
{
    switch (par_b_mode) {
    case SGL_ARC_FILLED:
    {
        sgl_draw_filled_ellipse_sector(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, par_f_radius, par_f_radius, par_b_clockwise, par_f_angle);
    }
        break;
    case SGL_ARC_OUTLINE:
    {
        sgl_draw_outline_arc_sector(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, par_f_radius, par_b_clockwise, par_f_angle);
    }
        break;
    default:
        /*
           case SGL_ARC_CIRCLE_LINE:
         */
    {
        sgl_draw_outline_ellipse_sector(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, par_f_radius, par_f_radius, par_b_clockwise, par_f_angle);
    }
        break;
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgluArcSector2f
  DESCRIPTION:
    Function shall draw a 2D Arc or Sector.
  PARAMETERS:
    par_f_centerx -> x-coordinate of the center of the arc or sector
    par_f_centery -> y-coordinate of the center of the arc or sector
    par_f_radius -> radius of the arc sector
    par_f_start_angle -> start angle of the arc sector
    par_f_end_angle -> end angle of the arc sector
    par_b_clockwise -> clockwise orientation (SGL_TRUE or SGL_FALSE)
    par_b_mode -> mode of the arc or sector (SGL_ARC_FILLED,SGL_ARC_OUTLINE or SGL_ARC_CIRCLE_LINE)
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgluArcSector2f(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_radius,
                     SGLfloat par_f_start_angle, SGLfloat par_f_end_angle, SGLbool par_b_clockwise, SGLbyte par_b_mode)
{
    SGLbool loc_b_error_detected = sglu_arc_sector_2f_detect_error(par_b_mode, par_b_clockwise, par_f_start_angle, par_f_end_angle);
    SGLfloat loc_f_par_radius = par_f_radius;

    if (loc_f_par_radius < 0.0F) {
        loc_f_par_radius = 0.0F;
    }

    if (!loc_b_error_detected) {
        SGLfloat loc_f_start_x = par_f_centerx + (loc_f_par_radius * mth_cos_degree(par_f_start_angle));
        SGLfloat loc_f_start_y = par_f_centery + (loc_f_par_radius * mth_sin_degree(par_f_start_angle));
        SGLfloat loc_f_angle;

        if (!par_b_clockwise) {
            loc_f_angle = sgluModuloAngle(par_f_end_angle - par_f_start_angle);
        }
        else {
            loc_f_angle = sgluModuloAngle(par_f_start_angle - par_f_end_angle);
        }

        if (sgluFloatIsEqual(360.0F, loc_f_angle)) {
            glob_pr_sglStatemachine->b_draw_circle = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }

        if (!sgluFloatIsEqual(loc_f_angle, 0.0F)) {
            SGLfloat loc_f_ratio = (glob_pr_sglStatemachine->f_ratio_scale_x + glob_pr_sglStatemachine->f_ratio_scale_y) * 0.5F;
            SGLfloat loc_f_radius = SGLfloat_div(loc_f_par_radius, loc_f_ratio);

            if (loc_f_radius <= MTH_RADIUS_TOO_SMALL_MAX) {
                /* Draw a single point when radius is too small */
                sgl_vector2d *loc_t_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
                const void *loc_p_vertex_array;
                loc_t_vertex_array[0][0] = par_f_centerx;
                loc_t_vertex_array[0][1] = par_f_centery;

                loc_p_vertex_array = loc_t_vertex_array;

                oglxUpdateColor(glob_pr_sglStatemachine->l_color_index);
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
                glPointSize(loc_f_radius * 2.0F);
                oglxDrawVertexArray((SGLulong) GL_POINTS, 1, loc_p_vertex_array);
#else
                oglxDrawVertexArray_es2((SGLulong) GL_POINTS, 1, loc_p_vertex_array);
#endif
            }
            else {
                sglu_arc_sector_2f(par_f_centerx, par_f_centery, loc_f_start_x, loc_f_start_y, loc_f_par_radius, par_b_clockwise, loc_f_angle, par_b_mode);
            }
            glob_pr_sglStatemachine->b_draw_circle = SGL_FALSE;

            glob_pr_sglStatemachine->tr_geometry.l_size = 0L;
        }
        else {
            /* Nothing to do */
        }

    }
    else {
        oglxSetError(SGL_ERROR_SGLU_ARCSECTOR2F, 0U);
    }

    return;
}

/* End of File ***************************************************************/
