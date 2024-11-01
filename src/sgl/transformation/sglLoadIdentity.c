/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglLoadIdentity.c
 DESCRIPTION       : sglLoadIdentity command shall replace the current modelview 
					matrix with the orthographic matrix
 COPYRIGHT (C)     : 2008 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "mth.h"
#include "sgl_private.h"

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglLoadIdentity
  DESCRIPTION:
    Function shall replace the current modelview matrix with the orthographic matrix.
  PARAMETERS:
    None        
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglLoadIdentity(void)
{

    SGLfloat loc_f_ratio_x;
    SGLfloat loc_f_ratio_y;

    SGLulong loc_ul_i;

    /* Restore the modelview matrix */
    for (loc_ul_i = 0UL; loc_ul_i < 16UL; loc_ul_i++) {
        glob_pr_sglStatemachine->f_modelview_matrix[loc_ul_i]
            = glob_pr_sglStatemachine->tf_ortho_matrix[loc_ul_i];
    }

    /* Compute the X ratio */
    loc_f_ratio_x = (glob_pr_sglStatemachine->f_ortho_right - glob_pr_sglStatemachine->f_ortho_left);

    loc_f_ratio_x = SGLfloat_div(loc_f_ratio_x, 2.0F * glob_pr_sglStatemachine->f_viewport_width_div_2);

    if (loc_f_ratio_x < 0.0F) {
        loc_f_ratio_x = -loc_f_ratio_x;
    }
    else {
        /* Nothing to do */
    }

    /* Compute the Y ratio */
    loc_f_ratio_y = (glob_pr_sglStatemachine->f_ortho_top - glob_pr_sglStatemachine->f_ortho_bottom);

    loc_f_ratio_y = SGLfloat_div(loc_f_ratio_y, 2.0F * glob_pr_sglStatemachine->f_viewport_height_div_2);

    if (loc_f_ratio_y < 0.0F) {
        loc_f_ratio_y = -loc_f_ratio_y;
    }
    else {
        /* Nothing to do */
    }


    glob_pr_sglStatemachine->f_ratio_x = loc_f_ratio_x;
    glob_pr_sglStatemachine->f_ratio_y = loc_f_ratio_y;
    glob_pr_sglStatemachine->f_scale_x = 1.0F;
    glob_pr_sglStatemachine->f_scale_y = 1.0F;
    glob_pr_sglStatemachine->f_ratio_scale_x = loc_f_ratio_x;
    glob_pr_sglStatemachine->f_ratio_scale_y = loc_f_ratio_y;
    glob_pr_sglStatemachine->f_ratio_scale_width = loc_f_ratio_x * glob_pr_sglStatemachine->f_width_factor;
    glob_pr_sglStatemachine->f_ratio_scale_height = loc_f_ratio_y * glob_pr_sglStatemachine->f_height_factor;

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    glLoadMatrixf(glob_pr_sglStatemachine->f_modelview_matrix);
#endif

    return;
}

/* End of File ***************************************************************/
