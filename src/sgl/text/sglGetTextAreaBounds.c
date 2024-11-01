/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetTextAreaBounds.c
 DESCRIPTION       : sglGetTextAreaBounds command shall compute the bounds of a text area
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
  NAME: oglxComputeTextAreaBounds
  DESCRIPTION:
    Function shall compute the bounds of a text area
  PARAMETERS:
    par_f_x_bottom_left -> x-coordinate of the bottom left point
    par_f_y_bottom_left -> y-coordinate of the bottom left point
    par_f_x_top_right -> x-coordinate of the top right point
    par_f_y_top_right -> y-coordinate of the top right point
    par_l_string_length -> string length
    par_l_font_id -> font to be taken into account
	par_l_hor_alignment -> horizontal alignment
	par_l_vert_alignment -> vertical alignment
  RETURN:
    sgl_bounding_box -> the computed bounding box
---------------------------------------------------------------------+*/
sgl_bounding_box oglxComputeTextAreaBounds(SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left, SGLfloat par_f_x_top_right, SGLfloat par_f_y_top_right,
                                           SGLlong par_l_string_length, SGLlong par_l_font_id, SGLlong par_l_vert_alignment)
{
    SGLfloat loc_f_temp;
    SGLfloat loc_f_char_height;
    SGLfloat loc_f_v_advance;
    SGLfloat loc_f_area_width = mth_norm(par_f_x_bottom_left, par_f_y_bottom_left, par_f_x_top_right, par_f_y_bottom_left);
    SGLfloat loc_f_area_height = mth_norm(par_f_x_bottom_left, par_f_y_bottom_left, par_f_x_bottom_left, par_f_y_top_right);
    SGLulong loc_ul_font_id = (SGLulong) par_l_font_id;
    SGLulong loc_ul_number_of_lines;
    SGLfloat loc_f_x_bottom_left = par_f_x_bottom_left;
    SGLfloat loc_f_y_bottom_left = par_f_y_bottom_left;
    SGLfloat loc_f_x_top_right = par_f_x_top_right;
    SGLfloat loc_f_y_top_right = par_f_y_top_right;
    sgl_bounding_box loc_s_return;
    const glf_type_font *loc_p_font;

    loc_s_return.f_min_x = loc_f_x_bottom_left;
    loc_s_return.f_max_x = loc_f_x_top_right;
    loc_s_return.f_min_y = loc_f_y_bottom_left;
    loc_s_return.f_max_y = loc_f_y_top_right;

    loc_p_font = oglx_get_font(loc_ul_font_id);
    loc_f_char_height = loc_p_font->f_max_char_height * glob_pr_sglStatemachine->f_ratio_scale_height;
    loc_f_v_advance = loc_p_font->f_max_v_advance * glob_pr_sglStatemachine->f_ratio_scale_height;

    if (loc_f_x_bottom_left > loc_f_x_top_right) {
        loc_f_temp = loc_f_x_top_right;
        loc_f_x_top_right = loc_f_x_bottom_left;
        loc_f_x_bottom_left = loc_f_temp;
        loc_s_return.f_min_x = loc_f_x_bottom_left;
        loc_s_return.f_max_x = loc_f_x_top_right;
    }
    else {
        /* Nothing to do */
    }

    if (loc_f_y_bottom_left > loc_f_y_top_right) {
        loc_f_temp = loc_f_y_top_right;
        loc_f_y_top_right = loc_f_y_bottom_left;
        loc_f_y_bottom_left = loc_f_temp;
        loc_s_return.f_min_y = loc_f_y_bottom_left;
        loc_s_return.f_max_y = loc_f_y_top_right;
    }
    else {
        /* Nothing to do */
    }

    loc_ul_number_of_lines = sgl_define_string_lines(glob_us_string, (SGLulong) par_l_string_length, loc_f_area_width);

    switch (par_l_vert_alignment) {
    case SGL_ALIGN_MIDDLE:
        loc_s_return.f_max_y = loc_f_y_bottom_left + (loc_f_area_height * 0.5F)
            + (((SGLfloat) loc_ul_number_of_lines * ((loc_f_char_height + loc_f_v_advance) * 0.5F)) - (loc_f_char_height * 0.5F));
        loc_s_return.f_min_y = loc_s_return.f_max_y - ((SGLfloat) loc_ul_number_of_lines * (loc_f_char_height + loc_f_v_advance));
        break;
    case SGL_ALIGN_BOTTOM:
        loc_s_return.f_min_y = loc_f_y_bottom_left;
        loc_s_return.f_max_y = loc_s_return.f_min_y + ((SGLfloat) loc_ul_number_of_lines * (loc_f_char_height + loc_f_v_advance));
        break;
    default:
        /*
           case SGL_ALIGN_TOP:
         */
        loc_s_return.f_max_y = loc_f_y_top_right;
        loc_s_return.f_min_y = loc_s_return.f_max_y - ((SGLfloat) loc_ul_number_of_lines * (loc_f_char_height + loc_f_v_advance));
        break;
    }

    loc_s_return.ul_nb_lines = loc_ul_number_of_lines;

    return loc_s_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglGetTextAreaBounds
  DESCRIPTION:
    Function shall compute the bounds of a text area
  PARAMETERS:
    par_f_x_bottom_left -> x-coordinate of the bottom left point
    par_f_y_bottom_left -> y-coordinate of the bottom left point
    par_f_x_top_right -> x-coordinate of the top right point
    par_f_y_top_right -> y-coordinate of the top right point
    par_pb_string -> string to be drawn
    par_l_string_length -> string length
    par_l_font_id -> font to be taken into account
	par_l_hor_alignment -> horizontal alignment
	par_l_vert_alignment -> vertical alignment
  RETURN:
    sgl_bounding_box -> the computed bounding box
---------------------------------------------------------------------+*/
sgl_bounding_box sglGetTextAreaBounds(SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left, SGLfloat par_f_x_top_right, SGLfloat par_f_y_top_right,
                                      const SGLbyte * par_pb_string, SGLlong par_l_string_length,
                                      SGLlong par_l_font_id, SGLlong par_l_hor_alignment, SGLlong par_l_vert_alignment)
{
    sgl_bounding_box loc_s_return;
    SGLbool loc_b_error_detected = SGL_FALSE;
    SGLlong loc_l_string_length = par_l_string_length;

    loc_s_return.f_min_x = par_f_x_bottom_left;
    loc_s_return.f_max_x = par_f_x_top_right;
    loc_s_return.f_min_y = par_f_y_bottom_left;
    loc_s_return.f_max_y = par_f_y_top_right;

    /* Check given string is not NULL */
    if (par_pb_string == SGL_NULL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check the size of given string is positive */
    if ((loc_l_string_length < 0L) || (par_l_font_id < 0L)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
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
        SGLlong loc_l_counter;
        SGLulong loc_ui_mblen;
        const GLubyte *loc_pb_string;

        if (loc_l_string_length > (SGLlong) SGLU_MAX_TEXT_AREA_STRING_LENGTH) {
            loc_l_string_length = (SGLlong) SGLU_MAX_TEXT_AREA_STRING_LENGTH;
        }
        else {
            /* Nothing to do */
        }

        loc_l_counter = loc_l_string_length;
        loc_pb_string = par_pb_string;
        loc_l_string_length = 0L;

        while (loc_l_counter > 0) {
            if ((*loc_pb_string) != 0U) {
                GLushort loc_us_index = 0U;
                if (!(sgl_is_line_separator((SGLushort) * loc_pb_string)) && !(sgl_is_word_separator((SGLushort) * loc_pb_string))) {
                    loc_ui_mblen = (GLuint) oglx_get_char_index(&loc_us_index, loc_pb_string);
                    loc_pb_string += loc_ui_mblen;
                    glob_us_string[loc_l_string_length] = loc_us_index;
                }
                else {
                    glob_us_string[loc_l_string_length] = (GLushort) * loc_pb_string;
                    loc_pb_string++;
                }

                loc_l_counter--;
                loc_l_string_length++;
            }
            else {
                loc_l_counter = 0L;
            }
        }

        glob_us_string[loc_l_string_length] = 0U;

        loc_s_return = oglxComputeTextAreaBounds(par_f_x_bottom_left, par_f_y_bottom_left, par_f_x_top_right, par_f_y_top_right,
                                                 loc_l_string_length, par_l_font_id, par_l_vert_alignment);
    }
    else {
        loc_s_return.f_min_x = 0.0F;
        loc_s_return.f_min_y = 0.0F;
        loc_s_return.f_max_x = 0.0F;
        loc_s_return.f_max_y = 0.0F;
        loc_s_return.ul_nb_lines = 0UL;
    }

    return loc_s_return;
}

/* End of File ***************************************************************/
