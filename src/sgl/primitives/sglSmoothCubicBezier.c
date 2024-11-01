/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSmoothCubicBezier.c
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
  NAME: sglSmoothCubicBezier
  DESCRIPTION:
    Function shall draw a smooth Bezier curve.
  PARAMETERS:
    par_f_x1,par_f_y1 ->  Coordinates of first control point. 
    par_f_x2,par_f_y2 ->  Coordinates of second control point. 
	par_f_x,par_f_y ->  Ending point. 
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglSmoothCubicBezier(SGLfloat par_f_x2, SGLfloat par_f_y2, SGLfloat par_f_x, SGLfloat par_f_y)
{
    if (glob_pr_sglStatemachine->b_path_type != SGL_NO_PATH) {
        SGLfloat loc_f_x1 = glob_pr_sglStatemachine->f_last_path_point_x;
        SGLfloat loc_f_y1 = glob_pr_sglStatemachine->f_last_path_point_y;

        /* Compute reflection point */
        if ((glob_pr_sglStatemachine->b_last_command == OGLX_SCUB_BEZIER)
            || (glob_pr_sglStatemachine->b_last_command == OGLX_CUB_BEZIER)
            || (glob_pr_sglStatemachine->b_last_command == OGLX_QUAD_BEZIER)
            || (glob_pr_sglStatemachine->b_last_command == OGLX_SQUAD_BEZIER)) {
            loc_f_x1 = (2.0F * glob_pr_sglStatemachine->f_last_path_point_x) - glob_pr_sglStatemachine->f_last_path_control_x;
            loc_f_y1 = (2.0F * glob_pr_sglStatemachine->f_last_path_point_y) - glob_pr_sglStatemachine->f_last_path_control_y;
        }

        sglCubicBezier(loc_f_x1, loc_f_y1, par_f_x2, par_f_y2, par_f_x, par_f_y);

        glob_pr_sglStatemachine->b_last_command = OGLX_SCUB_BEZIER;
    }
    return;
}

/* End of File ***************************************************************/
