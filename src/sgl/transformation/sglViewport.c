/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglViewport.c
 DESCRIPTION       : sglViewport command shall set the viewport
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
  NAME: sglViewport
  DESCRIPTION:
    Function shall set the viewport.
  PARAMETERS:
    par_l_x -> X-coordinate in pixels of the upper left corner of the viewport in the screen
    par_l_y -> Y-coordinate in pixels of the upper left corner of the viewport in the screen
    par_l_width -> Width in pixels of the viewport
    par_l_height -> Height in pixels of the viewport
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglViewport(SGLlong par_l_x, SGLlong par_l_y, SGLlong par_l_width, SGLlong par_l_height)
{
    SGLbool loc_b_error_detected = SGL_FALSE;

    /* Check matrix sack is empty */
    if (glob_pr_sglStatemachine->ul_matrix_stack_index > 0U) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        SGLlong loc_l_x;
        SGLlong loc_l_y;
        SGLlong loc_l_width;
        SGLlong loc_l_height;
        SGLfloat loc_f_ratio_x;
        SGLfloat loc_f_ratio_y;

        /* Check width is at least 1 user unit */
        if (par_l_width < 1L) {
            loc_b_error_detected = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }

        /* Check height is at least 1 user unit */
        if (par_l_height < 1L) {
            loc_b_error_detected = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }

        /* Initialize viewport data */
        if (loc_b_error_detected) {
            loc_l_x = (SGLuint32_to_int32(glob_pr_sglStatemachine->ul_screen_w)) / 4L;
            loc_l_y = (SGLuint32_to_int32(glob_pr_sglStatemachine->ul_screen_h)) / 4L;
            loc_l_width = (SGLuint32_to_int32(glob_pr_sglStatemachine->ul_screen_w)) / 2L;
            loc_l_height = (SGLuint32_to_int32(glob_pr_sglStatemachine->ul_screen_h)) / 2L;
            oglxSetError(SGL_ERROR_SGL_VIEWPORT, 0U);
        }
        else {
            loc_l_x = par_l_x;
            loc_l_y = par_l_y;
            loc_l_width = par_l_width;
            loc_l_height = par_l_height;
        }

        glViewport((GLint) loc_l_x, (GLint) loc_l_y, (GLsizei) loc_l_width, (GLsizei) loc_l_height);

        glob_pr_sglStatemachine->f_viewport_x1 = (SGLfloat) loc_l_x;
        glob_pr_sglStatemachine->f_viewport_y1 = (SGLfloat) loc_l_y;
        glob_pr_sglStatemachine->f_viewport_width_div_2 = (SGLfloat) loc_l_width *0.5F;
        glob_pr_sglStatemachine->f_viewport_height_div_2 = (SGLfloat) loc_l_height *0.5F;

        glob_pr_sglStatemachine->f_width_factor = SGLfloat_div((SGLfloat) loc_l_width, (SGLfloat) glob_pr_sglStatemachine->ul_screen_w);
        glob_pr_sglStatemachine->f_height_factor = SGLfloat_div((SGLfloat) loc_l_height, (SGLfloat) glob_pr_sglStatemachine->ul_screen_h);

        /* Compute the X ratio */
        loc_f_ratio_x = (glob_pr_sglStatemachine->f_ortho_right - glob_pr_sglStatemachine->f_ortho_left);

        loc_f_ratio_x = SGLfloat_div(loc_f_ratio_x, 2.0F * glob_pr_sglStatemachine->f_viewport_width_div_2);

        /* Compute the Y ratio */
        loc_f_ratio_y = (glob_pr_sglStatemachine->f_ortho_top - glob_pr_sglStatemachine->f_ortho_bottom);

        loc_f_ratio_y = SGLfloat_div(loc_f_ratio_y, 2.0F * glob_pr_sglStatemachine->f_viewport_height_div_2);

        glob_pr_sglStatemachine->f_ratio_x = mth_f_abs(loc_f_ratio_x);
        glob_pr_sglStatemachine->f_ratio_y = mth_f_abs(loc_f_ratio_y);
        glob_pr_sglStatemachine->f_ratio_scale_x = glob_pr_sglStatemachine->f_ratio_x * glob_pr_sglStatemachine->f_scale_x;
        glob_pr_sglStatemachine->f_ratio_scale_y = glob_pr_sglStatemachine->f_ratio_y * glob_pr_sglStatemachine->f_scale_y;
        glob_pr_sglStatemachine->f_ratio_scale_width = glob_pr_sglStatemachine->f_ratio_scale_x * glob_pr_sglStatemachine->f_width_factor;
        glob_pr_sglStatemachine->f_ratio_scale_height = glob_pr_sglStatemachine->f_ratio_scale_y * glob_pr_sglStatemachine->f_height_factor;

    }
    else {
        /* Raise error */
        oglxSetError(SGL_ERROR_SGL_VIEWPORT, 0U);
    }

    return;
}

/* End of File ***************************************************************/
