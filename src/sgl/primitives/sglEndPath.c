/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglEndPath.c
 DESCRIPTION       : sglEndPath shall end the definition of a path.
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
  NAME: sglEndPath
  DESCRIPTION:
    Function shall end the definition of a path.
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglEndPath(void)
{
    oglxUpdateColor(glob_pr_sglStatemachine->l_color_index);

    switch (glob_pr_sglStatemachine->b_path_type) {
    case SGL_OUTLINE:
    {
        sgl_end_line();
    }
        break;
        /*case SGL_FILL: */
    default:
    {

        if (glob_pr_sglStatemachine->b_last_command == OGLX_CLOSE_PATH) {

            glob_pr_sglStatemachine->tr_geometry.l_size = glob_pr_sglStatemachine->tr_geometry.l_size - 1L;
        }
        if (glob_pr_sglStatemachine->tr_geometry.l_size > 2L) {
            sgl_end_shape();
        }
    }
        break;
    }

    glob_pr_sglStatemachine->tr_geometry.l_size = 0L;
    glob_pr_sglStatemachine->tr_geometry.l_size_array_tex_coord = 0;

    /* Disable line stipple */
    glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_STRIP;
    glob_pr_sglStatemachine->ul_currentAction = SGL_NO_VERTEX;
    glob_pr_sglStatemachine->b_switch_to_sgl_lines = SGL_FALSE;

    /* End path */
    glob_pr_sglStatemachine->b_path_type = SGL_NO_PATH;
    glob_pr_sglStatemachine->b_last_command = OGLX_NO_COMMAND;
    glob_pr_sglStatemachine->ul_last_stipple_index = 0UL;
    glob_pr_sglStatemachine->f_last_stipple_length = 0.0F;
    return;
}

/* End of File ***************************************************************/
