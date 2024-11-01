/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglMoveTo.c
 DESCRIPTION       : sglMoveTo shall move pen to given point.
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

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglMoveTo
  DESCRIPTION:
    Function shall end last sub-path at last path position and start a new sub-path at given position.
  PARAMETERS:
    par_f_x, par_f_y -> new position of starting sub-path.
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglMoveTo(SGLfloat par_f_x, SGLfloat par_f_y)
{
    if (glob_pr_sglStatemachine->b_path_type != SGL_NO_PATH) {
        glob_pr_sglStatemachine->f_first_path_point_x = par_f_x;
        glob_pr_sglStatemachine->f_first_path_point_y = par_f_y;

        if (glob_pr_sglStatemachine->tr_geometry.l_size == 0) {
            oglxVertex2f(par_f_x, par_f_y);
        }
        else {
            if (glob_pr_sglStatemachine->b_path_type == SGL_OUTLINE) {
                sgl_end_line();
                glob_pr_sglStatemachine->tr_geometry.l_size = 0;
                oglxVertex2f(par_f_x, par_f_y);
            }
            else {
                if (glob_pr_sglStatemachine->tr_geometry.l_size > 2L) {
                    sgl_end_shape();
                }
                glob_pr_sglStatemachine->tr_geometry.l_size = 0;
                oglxVertex2f(par_f_x, par_f_y);
            }
        }

        glob_pr_sglStatemachine->f_last_path_point_x = par_f_x;
        glob_pr_sglStatemachine->f_last_path_point_y = par_f_y;
        glob_pr_sglStatemachine->f_last_path_control_x = par_f_x;
        glob_pr_sglStatemachine->f_last_path_control_y = par_f_y;

        glob_pr_sglStatemachine->b_last_command = OGLX_MOVE_TO;
    }
    return;
}

/* End of File ***************************************************************/
