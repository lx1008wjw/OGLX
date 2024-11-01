/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglBeginPath.c
 DESCRIPTION       : sglBeginPath shall begin the definition of a path.
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
  NAME: sglBeginPath
  DESCRIPTION:
    Function shall begin the definition of a path.
  PARAMETERS:
    par_b_mode ->  Type of the path. 
	Values: SGL_OUTLINE, SGL_FILL
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglBeginPath(SGLbyte par_b_mode)
{
    SGLlong loc_l_last_texture_bound_index = glob_pr_sglStatemachine->l_last_texture_bound_index;
    SGLbool loc_b_texture_alpha = SGL_FALSE;
    glob_pr_sglStatemachine->b_new_vertex = SGL_FALSE;

    if (glob_pr_sglStatemachine->b_texture_state) {
        if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
            if (glob_pr_sglStatemachine->p_texture_attrib[loc_l_last_texture_bound_index].b_texture_format == SGL_BITMAP_ALPHA) {
                loc_b_texture_alpha = SGL_TRUE;
            }
        }
    }

    /* Nothing shall be drawn if a texture of type SGL_BITMAP_ALPHA is defined and if modulate is inactive */
    switch (par_b_mode) {
    case SGL_OUTLINE:
    {
        /* Switch to line mode */
        sgl_set_line_mode();

        if (glob_pr_sglStatemachine->b_stipple_ok) {
            /* Switch to SGL_LINES to draw stipple */
            glob_pr_sglStatemachine->b_switch_to_sgl_lines = SGL_TRUE;
            glob_pr_sglStatemachine->ul_currentAction = SGL_LINES;
            glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINES;
        }
        else {
            glob_pr_sglStatemachine->tr_geometry.b_type = SGL_LINE_STRIP;
        }

        glob_pr_sglStatemachine->b_path_type = par_b_mode;
    }
        break;
    case SGL_FILL:
    {
        if ((glob_pr_sglStatemachine->b_texture_state) && (loc_b_texture_alpha) && (!glob_pr_sglStatemachine->b_modulate)) {
            /* Nothing to do */
        }
        else {
            sgl_set_fill_mode();
            glob_pr_sglStatemachine->b_path_type = par_b_mode;
        }
    }
        break;
    default:
    {
        /* In default case, no drawing mode is required */
        glob_pr_sglStatemachine->b_path_type = SGL_NO_PATH;
        oglxSetError(SGL_ERROR_SGL_BEGINPATH, (SGLulong) par_b_mode);
    }
        break;
    }
    glob_pr_sglStatemachine->tr_geometry.l_size = 0;
    glob_pr_sglStatemachine->b_last_command = OGLX_NO_COMMAND;

    return;
}

/* End of File ***************************************************************/
