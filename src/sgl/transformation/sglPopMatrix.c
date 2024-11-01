/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglPopMatrix.c
 DESCRIPTION       : sglPopMatrix command shall pop the current modelview matrix
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

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglPopMatrix
  DESCRIPTION:
    Function shall pop the current modelview matrix.
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglPopMatrix(void)
{
    SGLbool loc_b_error_detected = SGL_FALSE;

    /* Check stack is not empty */
    if (glob_pr_sglStatemachine->ul_matrix_stack_index == 0UL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        SGLfloat *loc_pr_f1;
        SGLfloat *loc_pr_f2;
        sgl_stack *loc_pr_stack;

        glob_pr_sglStatemachine->ul_matrix_stack_index--;

        loc_pr_stack = &glob_pr_sglStatemachine->tr_stack[glob_pr_sglStatemachine->ul_matrix_stack_index];

        /* Restore last stored matrix */
        loc_pr_f1 = &glob_pr_sglStatemachine->f_modelview_matrix[0];
        loc_pr_f2 = &loc_pr_stack->tf_modelview_matrix[0];
        loc_pr_f1[0] = loc_pr_f2[0];
        loc_pr_f1[1] = loc_pr_f2[1];
        loc_pr_f1[2] = loc_pr_f2[2];
        loc_pr_f1[3] = loc_pr_f2[3];
        loc_pr_f1[4] = loc_pr_f2[4];
        loc_pr_f1[5] = loc_pr_f2[5];
        loc_pr_f1[6] = loc_pr_f2[6];
        loc_pr_f1[7] = loc_pr_f2[7];
        loc_pr_f1[8] = loc_pr_f2[8];
        loc_pr_f1[9] = loc_pr_f2[9];
        loc_pr_f1[10] = loc_pr_f2[10];
        loc_pr_f1[11] = loc_pr_f2[11];
        loc_pr_f1[12] = loc_pr_f2[12];
        loc_pr_f1[13] = loc_pr_f2[13];
        loc_pr_f1[14] = loc_pr_f2[14];
        loc_pr_f1[15] = loc_pr_f2[15];

        glob_pr_sglStatemachine->f_ratio_x = loc_pr_stack->f_ratio_x;
        glob_pr_sglStatemachine->f_ratio_y = loc_pr_stack->f_ratio_y;
        glob_pr_sglStatemachine->f_scale_x = loc_pr_stack->f_scale_x;
        glob_pr_sglStatemachine->f_scale_y = loc_pr_stack->f_scale_y;
        glob_pr_sglStatemachine->f_ratio_scale_x = loc_pr_stack->f_ratio_x * loc_pr_stack->f_scale_x;
        glob_pr_sglStatemachine->f_ratio_scale_y = loc_pr_stack->f_ratio_y * loc_pr_stack->f_scale_y;
        glob_pr_sglStatemachine->f_ratio_scale_width = glob_pr_sglStatemachine->f_ratio_scale_x * glob_pr_sglStatemachine->f_width_factor;
        glob_pr_sglStatemachine->f_ratio_scale_height = glob_pr_sglStatemachine->f_ratio_scale_y * glob_pr_sglStatemachine->f_height_factor;

        glob_pr_sglStatemachine->f_rotation_angle = loc_pr_stack->f_rotation_angle;

        glob_pr_sglStatemachine->f_current_depth = loc_pr_stack->f_depth;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        glPopMatrix();
#else
        oglxPopMatrix();
#endif
    }
    else {
        oglxSetError(SGL_ERROR_SGL_POPMATRIX, 0U);
    }

    return;
}

/* End of File ***************************************************************/
