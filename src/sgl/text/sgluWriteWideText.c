/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluWriteWideText.c
 DESCRIPTION       : sgluWriteWideText command shall draw a string
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
  NAME: sgluWriteWideText
  DESCRIPTION:
    Function shall draw a string.
  PARAMETERS:
    par_f_x -> x-coordinate of the reference point
    par_f_y -> y-coordinate of the reference point
    par_pus_string -> string to be drawn
    par_l_string_length -> string length
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgluWriteWideText(SGLfloat par_f_x, SGLfloat par_f_y, const SGLushort * par_pus_string, SGLlong par_l_string_length)
{
    SGLbool loc_b_error_detected = SGL_FALSE;
    SGLlong loc_l_string_length = par_l_string_length;

    /* Check given string is not NULL */
    if ((par_pus_string == SGL_NULL) || (loc_l_string_length < 0L)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* If no error is detected */
    if (!loc_b_error_detected) {
        const sgl_line_width *loc_prev_line_width = glob_pr_sglStatemachine->tr_line_width;
        SGLulong loc_prev_line_width_size = glob_pr_sglStatemachine->ul_nb_line_width;
        SGLlong loc_l_line_width_backup = glob_pr_sglStatemachine->l_linewidth_index;
        SGLbool loc_b_multiline = SGL_FALSE;
        SGLulong loc_ui_param_length = sglInt32ToUInt32(loc_l_string_length);
        glob_ul_text_error = SGL_ERROR_SGLU_WRITEWIDETEXT;

        glob_pr_sglStatemachine->tr_line_width = (sgl_line_width *) font_line_width;
        glob_pr_sglStatemachine->ul_nb_line_width = FONT_LINE_WIDTH_SIZE;
        glob_pr_sglStatemachine->l_linewidth_index = glob_pr_sglStatemachine->l_current_font_linewidth_index;

        /* Check the length is not too big */
        if (loc_l_string_length > ((SGLlong) SGLU_MAX_STRING_LENGTH)) {
            loc_ui_param_length = ((SGLlong) SGLU_MAX_STRING_LENGTH);
            oglxSetError(SGL_ERROR_SGLU_WRITEWIDETEXT, (SGLulong) loc_l_string_length);
        }
        else {
            /* Nothing to do */
        }

        loc_l_string_length = 0U;

        while (((SGLulong) loc_l_string_length < loc_ui_param_length) && (par_pus_string[loc_l_string_length] != 0U)) {
            glob_us_string[loc_l_string_length] = par_pus_string[loc_l_string_length];
            if ((glob_us_string[loc_l_string_length] == (SGLushort) SGL_K_CR) || (glob_us_string[loc_l_string_length] == (SGLushort) SGL_K_LF)) {
                loc_b_multiline = SGL_TRUE;
            }
            else {
                /* Nothing to do */
            }

            loc_l_string_length++;
        }
        glob_us_string[loc_l_string_length] = 0U;

        if (loc_b_multiline) {
            oglxWriteTextMultiline(par_f_x, par_f_y, glob_us_string, (SGLulong) loc_l_string_length);
        }
        else {
            oglxWriteText(par_f_x, par_f_y, glob_us_string, (SGLulong) loc_l_string_length);
        }

        glob_pr_sglStatemachine->tr_line_width = loc_prev_line_width;
        glob_pr_sglStatemachine->ul_nb_line_width = loc_prev_line_width_size;
        glob_pr_sglStatemachine->l_linewidth_index = loc_l_line_width_backup;
    }
    else {
        oglxSetError(SGL_ERROR_SGLU_WRITEWIDETEXT, 0U);
    }

    return;
}

/* End of File ***************************************************************/
