/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluArcEllipse2f.c
 DESCRIPTION       : sgluCrown2f command shall draw a 2D Ellipse
 VERSION           : $Revision$
 MODIFICATION DATE : $Date$
 COPYRIGHT (C)     : 2008-2014 Esterel Technologies SAS. All Rights Reserved.
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
  NAME: sgl_draw_filled_ellipse_sector
  DESCRIPTION:
    Function shall draw a filled Ellipse or Ellipse Sector.
  PARAMETERS:
    par_f_centerx -> x-coordinate of the center of the arc or sector
    par_f_centery -> y-coordinate of the center of the arc or sector
    par_f_startx -> x-coordinate of the start of the arc or sector
    par_f_starty -> y-coordinate of the start of the arc or sector
	par_f_x_radius -> X radius
	par_f_y_radius -> Y radius
    par_b_clockwise	-> clockwise or not
    par_f_angle -> angle of the arc or sector, range: [0.0F,360.0F]
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_filled_ellipse_sector(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty,
                                    SGLfloat par_f_x_radius, SGLfloat par_f_y_radius, SGLbool par_b_clockwise, SGLfloat par_f_angle)
{
    SGLbool loc_b_draw_outline = SGL_TRUE;

    if (glob_pr_sglStatemachine->b_texture_state) {
        loc_b_draw_outline = SGL_FALSE;
    }
    else {
        if (glob_pr_sglStatemachine->b_enable_gradient) {
            loc_b_draw_outline = SGL_FALSE;
        }
        else {
            /* Nothing to do */
        }
    }

    if (loc_b_draw_outline) {
        SGLbool loc_b_polygon_smooth_backup = glob_pr_sglStatemachine->b_polygon_smooth;
        sgl_polygon_smooth_backup loc_struct_backup;
        /* Backup polygon smooth, linestipple, haloing and linewidth */
        sgl_backup_for_polygon_smooth(&loc_struct_backup);

        /* Treat polygon smooth by drawing first the outline */
        if ((loc_b_polygon_smooth_backup) && (glob_pr_sglStatemachine->b_transparency == SGL_MAX_ALPHA_8)) {
            if (par_f_angle < 360.0F) {
                sglBegin(SGL_LINE_LOOP);
                oglxVertex2f(par_f_centerx, par_f_centery);
                sgluArcCircleLinePointsList2f(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, par_f_x_radius, par_f_y_radius, par_b_clockwise,
                                              par_f_angle);
                sglEnd();
            }
            else {
                sglBegin(SGL_LINE_LOOP);
                sgluArcCircleLinePointsList2f(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, par_f_x_radius, par_f_y_radius, par_b_clockwise,
                                              par_f_angle);
                sglEnd();
            }
        }
        else {
            /* Nothing to do */
        }

        sglBegin(SGL_POLYGON);
        oglxVertex2f(par_f_centerx, par_f_centery);
        sgluArcCircleLinePointsList2f(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, par_f_x_radius, par_f_y_radius, par_b_clockwise, par_f_angle);
        sglEnd();

        /* Restore polygon smooth, linestipple, haloing and linewidth */
        sgl_restore_after_polygon_smooth(&loc_struct_backup);
    }
    else {
        sglBegin(SGL_POLYGON);
        oglxVertex2f(par_f_centerx, par_f_centery);
        sgluArcCircleLinePointsList2f(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, par_f_x_radius, par_f_y_radius, par_b_clockwise, par_f_angle);
        sglEnd();
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_outline_ellipse_sector
  DESCRIPTION:
    Function shall draw an outline Ellipse or Ellipse Sector.
  PARAMETERS:
    par_f_centerx -> x-coordinate of the center of the arc or sector
    par_f_centery -> y-coordinate of the center of the arc or sector
    par_f_startx -> x-coordinate of the start of the arc or sector
    par_f_starty -> y-coordinate of the start of the arc or sector
	par_f_x_radius -> X radius
	par_f_y_radius -> Y radius
    par_b_clockwise	-> clockwise or not
    par_f_angle -> angle of the arc or sector, range: [0.0F,360.0F]
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_outline_ellipse_sector(SGLfloat par_f_centerx, SGLfloat par_f_centery, SGLfloat par_f_startx, SGLfloat par_f_starty,
                                     SGLfloat par_f_x_radius, SGLfloat par_f_y_radius, SGLbool par_b_clockwise, SGLfloat par_f_angle)
{
    sglBegin(SGL_LINE_STRIP);
    if (glob_pr_sglStatemachine->b_stipple_ok) {
        /* Switch to SGL_LINES to draw stipple */
        glob_pr_sglStatemachine->b_switch_to_sgl_lines = SGL_TRUE;
        glob_pr_sglStatemachine->ul_currentAction = SGL_LINES;
        glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINES;
    }

    sgluArcCircleLinePointsList2f(par_f_centerx, par_f_centery, par_f_startx, par_f_starty, par_f_x_radius, par_f_y_radius, par_b_clockwise, par_f_angle);
    sglEnd();

    return;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglu_arc_ellipse_2f_detect_error
  DESCRIPTION:
    Function shall check parameters of sgluArcEllipse2f.
  PARAMETERS:
    par_f_x_radius -> x radius of the ellipse
    par_f_y_radius -> y radius of the ellipse
	par_b_clockwise -> Clockwise or not
	par_f_start_angle -> start angle
    par_f_end_angle -> end angle

  RETURN:
    SGLbool -> Error detected (SGL_TRUE) or not (SGL_FALSE)
---------------------------------------------------------------------+*/
SGLbool sglu_arc_ellipse_2f_detect_error(SGLbool par_b_clockwise, SGLfloat par_f_start_angle, SGLfloat par_f_end_angle, SGLbyte par_b_mode)
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

    if ((par_b_mode != SGL_ELLIPSE_FILLED) && (par_b_mode != SGL_ELLIPSE_OUTLINE)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_error_detected;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgluArcEllipse2f
  DESCRIPTION:
    Function shall draw a 2D Ellipse.
  PARAMETERS:
    par_f_centerx -> x-coordinate of the center
    par_f_centery -> y-coordinate of the center
    par_f_x_radius -> horizontal radius
    par_f_y_radius -> vertical radius
    par_f_start_angle -> start angle
    par_f_end_angle -> end angle
    par_b_clockwise -> clockwise orientation (SGL_TRUE or SGL_FALSE)
    par_b_mode	-> ellipse mode (SGL_ELLIPSE_FILLED or SGL_ELLIPSE_OUTLINE)
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgluArcEllipse2f(SGLfloat par_f_centerx, SGLfloat par_f_centery,
                      SGLfloat par_f_x_radius, SGLfloat par_f_y_radius,
                      SGLfloat par_f_start_angle, SGLfloat par_f_end_angle, SGLbool par_b_clockwise, SGLbyte par_b_mode)
{
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_width;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_height;
    SGLbool loc_b_error_detected = sglu_arc_ellipse_2f_detect_error(par_b_clockwise, par_f_start_angle, par_f_end_angle, par_b_mode);
    SGLfloat loc_f_par_x_radius = par_f_x_radius;
    SGLfloat loc_f_par_y_radius = par_f_y_radius;

    if (loc_f_par_x_radius < 0.0F) {
        loc_f_par_x_radius = 0.0F;
    }

    if (loc_f_par_y_radius < 0.0F) {
        loc_f_par_y_radius = 0.0F;
    }

    if (!loc_b_error_detected) {

        if (!(sgluFloatIsEqual(loc_f_par_x_radius, 0.0F)) && !(sgluFloatIsEqual(loc_f_par_y_radius, 0.0F))) {
            SGLfloat loc_f_start_x;
            SGLfloat loc_f_start_y;
            SGLfloat loc_f_radius = loc_f_par_x_radius * loc_f_par_y_radius;
            SGLfloat loc_f_x_radius = SGLfloat_div(loc_f_par_x_radius, loc_f_ratio_x);
            SGLfloat loc_f_y_radius = SGLfloat_div(loc_f_par_y_radius, loc_f_ratio_y);

            if ((loc_f_x_radius <= MTH_RADIUS_TOO_SMALL_MAX) || (loc_f_y_radius <= MTH_RADIUS_TOO_SMALL_MAX)) {
                if ((loc_f_x_radius <= MTH_RADIUS_TOO_SMALL_MAX) && (loc_f_y_radius <= MTH_RADIUS_TOO_SMALL_MAX)) {
                    /* Draw a single point when radius is too small */
                    sgl_vector2d *loc_t_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
                    const void *loc_p_vertex_array;
                    loc_f_radius = (loc_f_x_radius + loc_f_y_radius) / 2.0F;
                    loc_t_vertex_array[0][0] = par_f_centerx;
                    loc_t_vertex_array[0][1] = par_f_centery;

                    loc_p_vertex_array = loc_t_vertex_array;

                    oglxUpdateColor(glob_pr_sglStatemachine->l_color_index);
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
                    glPointSize(loc_f_radius * 2.0F);
                    oglxDrawVertexArray((SGLulong) GL_POINTS, 1L, loc_p_vertex_array);
#else
                    oglxDrawVertexArray_es2((SGLulong) GL_POINTS, 1L, loc_p_vertex_array);
#endif
                }
                else {
                    sgl_polygon_smooth_backup loc_struct_backup;
                    sgl_backup_for_polygon_smooth(&loc_struct_backup);

                    if (loc_f_x_radius <= MTH_RADIUS_TOO_SMALL_MAX) {
                        /* Draw a single line when radius is too small */
                        sglBegin(SGL_LINES);
                        sglVertex2f(par_f_centerx, par_f_centery - loc_f_par_y_radius);
                        sglVertex2f(par_f_centerx, par_f_centery + loc_f_par_y_radius);
                        sglEnd();
                    }
                    else {
                        /* Draw a single line when radius is too small */
                        sglBegin(SGL_LINES);
                        sglVertex2f(par_f_centerx - loc_f_par_x_radius, par_f_centery);
                        sglVertex2f(par_f_centerx + loc_f_par_x_radius, par_f_centery);
                        sglEnd();
                    }

                    sgl_restore_after_polygon_smooth(&loc_struct_backup);
                }
            }
            else {

                SGLfloat loc_f_tmp = ((loc_f_par_y_radius * mth_cos_degree(par_f_start_angle)) * (loc_f_par_y_radius * mth_cos_degree(par_f_start_angle)))
                    + ((loc_f_par_x_radius * mth_sin_degree(par_f_start_angle)) * (loc_f_par_x_radius * mth_sin_degree(par_f_start_angle)));
                SGLfloat loc_f_angle;
                SGLfloat loc_f_end_x;
                SGLfloat loc_f_end_y;
                SGLfloat loc_f_start_angle;
                SGLfloat loc_f_end_angle;

                loc_f_tmp = mth_sqrtf(loc_f_tmp);
                loc_f_radius = SGLfloat_div(loc_f_radius, loc_f_tmp);
                loc_f_start_x = par_f_centerx + (loc_f_radius * mth_cos_degree(par_f_start_angle));
                loc_f_start_y = par_f_centery + (loc_f_radius * mth_sin_degree(par_f_start_angle));
                loc_f_start_angle = mth_acos_degree(loc_f_start_x - par_f_centerx, loc_f_par_x_radius);

                loc_f_start_angle = mth_move_in_good_quadrant(loc_f_start_angle, loc_f_start_x - par_f_centerx, loc_f_start_y - par_f_centery);

                loc_f_radius = loc_f_par_x_radius * loc_f_par_y_radius;
                loc_f_tmp = ((loc_f_par_y_radius * mth_cos_degree(par_f_end_angle)) * (loc_f_par_y_radius * mth_cos_degree(par_f_end_angle)))
                    + ((loc_f_par_x_radius * mth_sin_degree(par_f_end_angle)) * (loc_f_par_x_radius * mth_sin_degree(par_f_end_angle)));
                loc_f_tmp = mth_sqrtf(loc_f_tmp);
                loc_f_radius = SGLfloat_div(loc_f_radius, loc_f_tmp);
                loc_f_end_x = par_f_centerx + (loc_f_radius * mth_cos_degree(par_f_end_angle));
                loc_f_end_y = par_f_centery + (loc_f_radius * mth_sin_degree(par_f_end_angle));
                loc_f_end_angle = mth_acos_degree(loc_f_end_x - par_f_centerx, loc_f_par_x_radius);

                loc_f_end_angle = mth_move_in_good_quadrant(loc_f_end_angle, loc_f_end_x - par_f_centerx, loc_f_end_y - par_f_centery);

                if (sgluModuloAngle(par_f_end_angle - par_f_start_angle) > 355.0F) {
                    loc_f_end_angle = par_f_end_angle;
                    loc_f_start_angle = par_f_start_angle;
                }

                if (par_b_clockwise == SGL_FALSE) {
                    loc_f_angle = sgluModuloAngle(loc_f_end_angle - loc_f_start_angle);
                }
                else {
                    loc_f_angle = sgluModuloAngle(loc_f_start_angle - loc_f_end_angle);
                }

                if (sgluFloatIsEqual(360.0F, loc_f_angle)) {
                    glob_pr_sglStatemachine->b_draw_circle = SGL_TRUE;
                }
                else {
                    /* Nothing to do */
                }

                glob_pr_sglStatemachine->b_draw_ellipse = SGL_TRUE;



                if (par_b_mode == SGL_ELLIPSE_FILLED) {
                    sgl_draw_filled_ellipse_sector(par_f_centerx, par_f_centery, loc_f_start_x, loc_f_start_y, loc_f_par_x_radius, loc_f_par_y_radius,
                                                   par_b_clockwise, loc_f_angle);
                }
                else {          /*SGL_ELLIPSE_OUTLINE */
                    sgl_draw_outline_ellipse_sector(par_f_centerx, par_f_centery, loc_f_start_x, loc_f_start_y, loc_f_par_x_radius, loc_f_par_y_radius,
                                                    par_b_clockwise, loc_f_angle);
                }

                glob_pr_sglStatemachine->b_draw_circle = SGL_FALSE;
                glob_pr_sglStatemachine->b_draw_ellipse = SGL_FALSE;
            }
        }
        else {
            sgl_polygon_smooth_backup loc_struct_backup;
            sgl_backup_for_polygon_smooth(&loc_struct_backup);

            if (!(sgluFloatIsEqual(loc_f_par_x_radius, 0.0F))) {
                /* Draw a single line when radius is too small */
                sglBegin(SGL_LINES);
                sglVertex2f(par_f_centerx - loc_f_par_x_radius, par_f_centery);
                sglVertex2f(par_f_centerx + loc_f_par_x_radius, par_f_centery);
                sglEnd();
            }
            else {
                /* Draw a single line when radius is too small */
                sglBegin(SGL_LINES);
                sglVertex2f(par_f_centerx, par_f_centery - loc_f_par_y_radius);
                sglVertex2f(par_f_centerx, par_f_centery + loc_f_par_y_radius);
                sglEnd();
            }
            sgl_restore_after_polygon_smooth(&loc_struct_backup);
        }

        glob_pr_sglStatemachine->tr_geometry.l_size = 0L;
    }
    else {
        oglxSetError(SGL_ERROR_SGLU_ARCELLIPSE2F, 0U);
    }

    return;
}


/* End of File ***************************************************************/
