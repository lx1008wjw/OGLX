/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglCircleActiveAreaGetRate.c
 DESCRIPTION       : sglCircleActiveAreaGetRate command shall return the ratio 
					of the distance of the mouse coordinates and the radius of 
					the circle active area radius.
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
  NAME: sglCircleActiveAreaGetRate
  DESCRIPTION:
    Function shall return the ratio of  the distance of 
    the mouse coordinates and the radius of the circle 
    active area radius.
  PARAMETERS:  
    par_f_mouse_x  -> x-coordinate for mouse
    par_f_mouse_y  -> y-coordinate for mouse
    par_f_origin_x -> x-coordinate for circle origin
    par_f_origin_y -> y-coordinate for circle origin
    par_f_radius   -> radius for circle            
  RETURN:
    SGLfloat -> Rate between mouse position relativeto center and radius, range: [0.0F,1.0F]
---------------------------------------------------------------------+*/
SGLfloat sglCircleActiveAreaGetRate(SGLfloat par_f_mouse_x, SGLfloat par_f_mouse_y, SGLfloat par_f_origin_x, SGLfloat par_f_origin_y, SGLfloat par_f_radius)
{
    SGLfloat loc_f_return;

    SGLfloat loc_f_radius_pixel_square_circle;
    SGLfloat loc_f_radius_pixel_square_position;

    SGLfloat loc_f_mouse_x = 0.0F;
    SGLfloat loc_f_mouse_y = 0.0F;

    SGLfloat loc_f_par_mouse_x = par_f_mouse_x * glob_pr_sglStatemachine->f_width_factor;
    SGLfloat loc_f_par_mouse_y = par_f_mouse_y * glob_pr_sglStatemachine->f_height_factor;

    /* Check if radius is positive */
    if (par_f_radius > 0.0F) {
        sglConvertPixelToPoint(loc_f_par_mouse_x, loc_f_par_mouse_y, &loc_f_mouse_x, &loc_f_mouse_y);

        /* Compute the square radius */
        loc_f_radius_pixel_square_circle = par_f_radius * par_f_radius;

        /* Compute the square distance between center and mouse position */
        loc_f_radius_pixel_square_position = ((loc_f_mouse_x - par_f_origin_x) * (loc_f_mouse_x - par_f_origin_x))
            + ((loc_f_mouse_y - par_f_origin_y) * (loc_f_mouse_y - par_f_origin_y));

        loc_f_return = mth_sqrtf(SGLfloat_div(loc_f_radius_pixel_square_position, loc_f_radius_pixel_square_circle));
    }
    else {
        loc_f_return = 0.0F;
    }

    return loc_f_return;
}

/* End of File ***************************************************************/
