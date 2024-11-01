/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglShapeActiveAreaGetStatus.c
 DESCRIPTION       : sglShapeActiveAreaGetStatus command shall return the status 
					of the mouse position: SGL_TRUE shall be returned if the mouse 
					is inside the shape active area, SGL_FALSE shall be returned if 
					the mouse is outside the shape active area
 COPYRIGHT (C)     : 2008 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl.h"
#include "mth.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglShapeActiveAreaGetStatus
  DESCRIPTION:
    Function shall return the status of the mouse position: 
    SGL_TRUE shall be returned if the mouse is inside the shape 
    active area, SGL_FALSE shall be returned if the mouse is 
    outside the shape active area.
  PARAMETERS:  
    par_f_mouse_x -> x-coordinate for mouse
    par_f_mouse_y -> y-coordinate for mouse
    par_ul_nb_points -> number of points of the shape
    par_pf_points -> Points of the shape
  RETURN:
    SGLbyte -> SGL_TRUE: the mouse is in the shape,
    SGL_FALSE: the mouse is out of the shape
---------------------------------------------------------------------+*/
SGLbool sglShapeActiveAreaGetStatus(SGLfloat par_f_mouse_x, SGLfloat par_f_mouse_y, SGLulong par_ul_nb_points, SGLfloat par_pf_points[][2])
{
    SGLbool loc_b_return;
    SGLfloat loc_f_mouse_x, loc_f_mouse_y;

    SGLfloat loc_f_par_mouse_x = par_f_mouse_x * glob_pr_sglStatemachine->f_width_factor;
    SGLfloat loc_f_par_mouse_y = par_f_mouse_y * glob_pr_sglStatemachine->f_height_factor;

    sglConvertPixelToPoint(loc_f_par_mouse_x, loc_f_par_mouse_y, &loc_f_mouse_x, &loc_f_mouse_y);

    loc_b_return = mth_point_in_shape(loc_f_mouse_x, loc_f_mouse_y, par_ul_nb_points, par_pf_points);

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
