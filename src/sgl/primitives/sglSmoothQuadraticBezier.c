/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSmoothQuadraticBezier.c
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
  NAME: sglSmoothQuadraticBezier
  DESCRIPTION:
    Function shall draw a smooth quadratic Bezier curve.
  PARAMETERS:
	par_f_x,par_f_y ->  Ending point. 
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglSmoothQuadraticBezier(SGLfloat par_f_x, SGLfloat par_f_y)
{
    if (glob_pr_sglStatemachine->b_path_type != SGL_NO_PATH) {
        SGLfloat loc_f_x2 = glob_pr_sglStatemachine->f_last_path_point_x;
        SGLfloat loc_f_y2 = glob_pr_sglStatemachine->f_last_path_point_y;

        /* Compute reflection point */
        if ((glob_pr_sglStatemachine->b_last_command == OGLX_SCUB_BEZIER)
            || (glob_pr_sglStatemachine->b_last_command == OGLX_CUB_BEZIER)
            || (glob_pr_sglStatemachine->b_last_command == OGLX_QUAD_BEZIER)
            || (glob_pr_sglStatemachine->b_last_command == OGLX_SQUAD_BEZIER)) {
            loc_f_x2 = (2.0F * glob_pr_sglStatemachine->f_last_path_point_x) - glob_pr_sglStatemachine->f_last_path_control_x;
            loc_f_y2 = (2.0F * glob_pr_sglStatemachine->f_last_path_point_y) - glob_pr_sglStatemachine->f_last_path_control_y;
        }

        sglQuadraticBezier(loc_f_x2, loc_f_y2, par_f_x, par_f_y);

        glob_pr_sglStatemachine->b_last_command = OGLX_SQUAD_BEZIER;
    }
    return;
}

/* End of File ***************************************************************/
