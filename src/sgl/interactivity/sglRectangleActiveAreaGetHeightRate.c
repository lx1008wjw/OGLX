/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglRectangleActiveAreaGetHeightRate.c
 DESCRIPTION       : sglRectangleActiveAreaGetHeightRate command shall return 
					the ratio for the distance between the mouse y-coordinate 
					with the y-coordinate of the low left corner of the rectangle 
					active area with the height of the rectangle active area
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
  NAME: sglRectangleActiveAreaGetHeightRate
  DESCRIPTION:
    Function shall return the ratio for the distance between the mouse 
    y-coordinate with the y-coordinate of the low left corner of the 
    rectangle active area with the height of the rectangle active area.
  PARAMETERS:
    par_f_mouse_x -> x-coordinate for mouse
    par_f_mouse_y -> y-coordinate for mouse
    par_f_x1 -> x-coordinate for first point
    par_f_y1 -> y-coordinate for first point
    par_f_x2 -> x-coordinate for second point
    par_f_y2 -> y-coordinate for second point            
  RETURN:
    SGLfloat -> Vertical rate between mouse position and lower point 
    of rectangle for interactivity, range: [0.0F,1.0F]  
---------------------------------------------------------------------+*/
SGLfloat sglRectangleActiveAreaGetHeightRate(SGLfloat par_f_mouse_x, SGLfloat par_f_mouse_y, SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2,
                                             SGLfloat par_f_y2)
{
    SGLfloat loc_f_return;

    if ((((par_f_x2 - par_f_x1) > -SGL_MIN_F) && ((par_f_x2 - par_f_x1) < SGL_MIN_F))
        || (((par_f_y2 - par_f_y1) > -SGL_MIN_F) && ((par_f_y2 - par_f_y1) < SGL_MIN_F))) {
        loc_f_return = 0.0F;
    }
    else {
        SGLfloat loc_f_min_y;
        SGLfloat loc_f_max_y;
        SGLfloat loc_f_par_mouse_x = par_f_mouse_x * glob_pr_sglStatemachine->f_width_factor;
        SGLfloat loc_f_par_mouse_y = par_f_mouse_y * glob_pr_sglStatemachine->f_height_factor;
        SGLfloat loc_f_mouse_x = 0.0F;
        SGLfloat loc_f_mouse_y = 0.0F;

        sglConvertPixelToPoint(loc_f_par_mouse_x, loc_f_par_mouse_y, &loc_f_mouse_x, &loc_f_mouse_y);

        /* Compute upper and lower rectangle position */
        if (par_f_y1 < par_f_y2) {
            loc_f_min_y = par_f_y1;
            loc_f_max_y = par_f_y2;
        }
        else {
            loc_f_min_y = par_f_y2;
            loc_f_max_y = par_f_y1;
        }
        /* If rectangle is large enough, compute the rate between mouse position and rectangle height */
        loc_f_return = SGLfloat_div(loc_f_mouse_y - loc_f_min_y, loc_f_max_y - loc_f_min_y);
    }

    return loc_f_return;
}

/* End of File ***************************************************************/
