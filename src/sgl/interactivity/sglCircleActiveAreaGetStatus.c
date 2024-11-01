/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglCircleActiveAreaGetStatus.c
 DESCRIPTION       : sglCircleActiveAreaGetStatus command shall return the status 
					on the position of the mouse: SGL_TRUE if the mouse is inside 
					the circle area, SGL_FALSE if the mouse is outside the circle area
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
  NAME: sglCircleActiveAreaGetStatus
  DESCRIPTION:
    Function shall return the status on  the position of 
    the mouse: SGL_TRUE if the mouse is inside the circle area, 
    SGL_FALSE if the mouse is outside the circle area.
  PARAMETERS:  
    par_f_mouse_x  -> x-coordinate for mouse
    par_f_mouse_y  -> y-coordinate for mouse
    par_f_origin_x -> x-coordinate for circle origin
    par_f_origin_y -> y-coordinate for circle origin
    par_f_radius   -> radius for circle            
  RETURN:
    SGLbyte -> SGL_TRUE: the mouse is in the circle
    SGL_FALSE: the mouse is out of the circle  
---------------------------------------------------------------------+*/
SGLbool sglCircleActiveAreaGetStatus(SGLfloat par_f_mouse_x, SGLfloat par_f_mouse_y, SGLfloat par_f_origin_x, SGLfloat par_f_origin_y, SGLfloat par_f_radius)
{
    SGLbool loc_b_return = SGL_FALSE;

    SGLfloat loc_f_mouse_x = 0.0F;
    SGLfloat loc_f_mouse_y = 0.0F;

    SGLfloat loc_f_par_mouse_x = par_f_mouse_x * glob_pr_sglStatemachine->f_width_factor;
    SGLfloat loc_f_par_mouse_y = par_f_mouse_y * glob_pr_sglStatemachine->f_height_factor;

    if (par_f_radius > 0.0F) {
        SGLfloat loc_f_radius_pixel_square_circle, loc_f_radius_pixel_square_position, loc_f_rate;
        sglConvertPixelToPoint(loc_f_par_mouse_x, loc_f_par_mouse_y, &loc_f_mouse_x, &loc_f_mouse_y);

        /* Compute the square radius */
        loc_f_radius_pixel_square_circle = par_f_radius * par_f_radius;

        /* Compute the square distance between center and mouse position */
        loc_f_radius_pixel_square_position = ((loc_f_mouse_x - par_f_origin_x) * (loc_f_mouse_x - par_f_origin_x))
            + ((loc_f_mouse_y - par_f_origin_y) * (loc_f_mouse_y - par_f_origin_y));

        loc_f_rate = mth_sqrtf(SGLfloat_div(loc_f_radius_pixel_square_position, loc_f_radius_pixel_square_circle));

        if (loc_f_rate <= 1.0F) {
            loc_b_return = SGL_TRUE;
        }
    }
    else {
        /* Nothing to do */
    }

    if (loc_b_return) {
        SGLbool loc_b_inside_scissors = sgl_pixel_point_is_inside_scissors(loc_f_par_mouse_x, loc_f_par_mouse_y);
        if (loc_b_inside_scissors != SGL_TRUE) {
            loc_b_return = SGL_FALSE;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        /* Nothing to do */
    }

    return loc_b_return;
}

/* End of File ***************************************************************/
