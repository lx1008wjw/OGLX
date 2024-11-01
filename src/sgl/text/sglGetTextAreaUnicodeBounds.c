/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetTextAreaUnicodeBounds.c
 DESCRIPTION       : sglGetTextAreaUnicodeBounds command shall compute the bounds of text area in unicode format
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
  NAME: sglGetTextAreaUnicodeBounds
  DESCRIPTION:
    Function shall compute the bounds of a text area
  PARAMETERS:
    par_f_x_bottom_left -> x-coordinate of the bottom left point
    par_f_y_bottom_left -> y-coordinate of the bottom left point
    par_f_x_top_right -> x-coordinate of the top right point
    par_f_y_top_right -> y-coordinate of the top right point
    par_pl_string -> string to be drawn
    par_l_string_length -> string length
    par_l_font_id -> font index to use to compute the bounding box
    par_l_hor_alignment -> horizontal alignment
    par_l_vert_alignment -> vertical alignment
  RETURN:
    sgl_bounding_box -> the computed bounding box
---------------------------------------------------------------------+*/
sgl_bounding_box sglGetTextAreaUnicodeBounds(SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left, SGLfloat par_f_x_top_right, SGLfloat par_f_y_top_right,
                                             const SGLlong * par_pl_string, SGLlong par_l_string_length,
                                             SGLlong par_l_font_id, SGLlong par_l_hor_alignment, SGLlong par_l_vert_alignment)
{
    SGLbool loc_b_error_detected = SGL_FALSE;
    SGLulong loc_ul_i = 0UL;
    SGLlong loc_l_string_length = par_l_string_length;

    sgl_bounding_box loc_s_return;

    loc_s_return.f_min_x = par_f_x_bottom_left;
    loc_s_return.f_max_x = par_f_x_top_right;
    loc_s_return.f_min_y = par_f_y_bottom_left;
    loc_s_return.f_max_y = par_f_y_top_right;

    /* Check given string is not NULL */
    if ((par_pl_string == SGL_NULL) || (loc_l_string_length < 0L) || (par_l_font_id < 0L)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        while ((loc_ul_i < (SGLulong) loc_l_string_length) && (par_pl_string[loc_ul_i] != 0L) && (!loc_b_error_detected)) {
            if (par_pl_string[loc_ul_i] < 0L) {
                loc_b_error_detected = SGL_TRUE;
            }
            else {
                /* Nothing to do */
            }
            loc_ul_i++;
        }
    }

    /* Check horizontal alignment */
    if ((par_l_hor_alignment != SGL_ALIGN_LEFT) && (par_l_hor_alignment != SGL_ALIGN_CENTER) && (par_l_hor_alignment != SGL_ALIGN_RIGHT)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check vertical alignment */
    if ((par_l_vert_alignment != SGL_ALIGN_BOTTOM) && (par_l_vert_alignment != SGL_ALIGN_MIDDLE) && (par_l_vert_alignment != SGL_ALIGN_TOP)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if (!loc_b_error_detected) {
        SGLlong loc_i_param_length;
        if (loc_l_string_length > (SGLlong) SGLU_MAX_TEXT_AREA_STRING_LENGTH) {
            loc_l_string_length = (SGLlong) SGLU_MAX_TEXT_AREA_STRING_LENGTH;
        }
        else {
            /* Nothing to do */
        }

        loc_i_param_length = loc_l_string_length;
        loc_l_string_length = 0L;

        while ((loc_l_string_length < loc_i_param_length) && (par_pl_string[loc_l_string_length] != 0L)) {
            glob_us_string[loc_l_string_length] = (GLushort) par_pl_string[loc_l_string_length];
            loc_l_string_length++;
        }

        glob_us_string[loc_l_string_length] = 0U;

        loc_s_return = oglxComputeTextAreaBounds(par_f_x_bottom_left, par_f_y_bottom_left, par_f_x_top_right, par_f_y_top_right,
                                                 loc_l_string_length, par_l_font_id, par_l_vert_alignment);
    }
    else {
        loc_s_return.f_min_x = 0.0F;
        loc_s_return.f_max_x = 0.0F;
        loc_s_return.f_min_y = 0.0F;
        loc_s_return.f_max_y = 0.0F;
        loc_s_return.ul_nb_lines = 0UL;
    }

    return loc_s_return;
}

/* End of File ***************************************************************/
