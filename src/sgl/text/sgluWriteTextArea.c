/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluWriteTextArea.c
 DESCRIPTION       : sgluWriteTextArea command shall draw a text area
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

static GLfloat stc_tf_black_wta[4] = { 0.0F, 0.0F, 0.0F, 1.0F };

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_define_string_lines
  DESCRIPTION:
    Function shall cut the given string in lines 
  PARAMETERS:
    par_pb_string -> string to be drawn
    par_ul_string_length -> string length
    par_f_area_width -> width of the text area
  RETURN:
    SGLulong -> number of lines
---------------------------------------------------------------------+*/
SGLulong sgl_define_string_lines(SGLushort * par_pus_string, SGLulong par_ul_string_length, SGLfloat par_f_area_width)
{
    SGLulong loc_ul_length = 1U;
    SGLfloat loc_f_temp_width;
    SGLushort *loc_pus_string = par_pus_string;
    SGLulong loc_ul_string_length = par_ul_string_length;
    SGLulong loc_ul_font_id = glob_pr_sglStatemachine->ul_font_id;
    SGLulong loc_ul_separator_index = 0U;
    SGLbool loc_b_word_separator_found = SGL_FALSE;
    SGLbool loc_b_line_separator_found;
    SGLbool loc_b_character_read = SGL_FALSE;
    SGLlong loc_l_line_separator = 0;
    SGLulong loc_ul_number_of_lines = 0U;

    /* Initialize number of lines */

    while ((loc_ul_string_length > 0UL) && (loc_ul_number_of_lines < SGL_MAX_STRING_LINES)) {

        loc_b_line_separator_found = sgl_is_line_separator(loc_pus_string[loc_ul_length - 1UL]);
        if (!loc_b_line_separator_found) {
            loc_f_temp_width = oglx_get_string_width(loc_pus_string, loc_ul_length, loc_ul_font_id, SGL_TRUE);
        }
        else {
            loc_f_temp_width = 0.0F;
        }

        while ((loc_ul_length <= loc_ul_string_length) && (loc_f_temp_width < par_f_area_width)
               && (!loc_b_line_separator_found)) {
            if (sgl_is_word_separator(loc_pus_string[loc_ul_length - 1UL])) {
                loc_ul_separator_index = loc_ul_length - 1UL;
                loc_b_word_separator_found = SGL_TRUE;
            }
            else {
                /* Nothing to do */
            }

            loc_b_character_read = SGL_TRUE;
            loc_ul_length++;
            loc_b_line_separator_found = sgl_is_line_separator(loc_pus_string[loc_ul_length - 1UL]);
            if ((!loc_b_line_separator_found) && (loc_ul_length <= loc_ul_string_length)) {
                loc_f_temp_width = oglx_get_string_width(loc_pus_string, loc_ul_length, loc_ul_font_id, SGL_TRUE);
            }
            else {
                loc_l_line_separator = (SGLlong) loc_pus_string[loc_ul_length - 1UL];
            }
        }


        /* If a separator is met the line shall be cut at the preceding word */
        if (loc_b_word_separator_found && (loc_f_temp_width > par_f_area_width)) {
            loc_ul_length = loc_ul_separator_index + 1U;
        }
        else {
            loc_ul_length = loc_ul_length - 1UL;
        }

        /* Fill the lines structure */
        ts_lines[loc_ul_number_of_lines].pb_substring = loc_pus_string;

        /* Store empty lines as strings of size 0 */
        if (loc_b_line_separator_found && (!loc_b_character_read)) {
            ts_lines[loc_ul_number_of_lines].ul_substring_length = 0U;
        }
        else {
            ts_lines[loc_ul_number_of_lines].ul_substring_length = loc_ul_length;
        }

        loc_ul_number_of_lines++;

        /* Avoid reading line separator twice */
        if (!loc_b_line_separator_found) {
            loc_pus_string = loc_pus_string + loc_ul_length;
            loc_ul_string_length = loc_ul_string_length - loc_ul_length;
        }
        else {
            if ((loc_ul_length + 1UL) < loc_ul_string_length) {
                loc_b_line_separator_found = sgl_is_line_separator(loc_pus_string[loc_ul_length + 1UL]);
                if (loc_b_line_separator_found && ((SGLlong) (loc_pus_string[loc_ul_length + 1UL]) != loc_l_line_separator)) {
                    loc_pus_string = loc_pus_string + (loc_ul_length + 2U);
                    loc_ul_string_length = loc_ul_string_length - (loc_ul_length + 2U);
                }
                else {
                    loc_pus_string = loc_pus_string + (loc_ul_length + 1U);
                    loc_ul_string_length = loc_ul_string_length - (loc_ul_length + 1U);
                }
            }
            else {
                loc_pus_string = loc_pus_string + (loc_ul_length + 1U);
                loc_ul_string_length = loc_ul_string_length - (loc_ul_length + 1U);
            }

        }

        loc_ul_length = 1;
        loc_b_word_separator_found = SGL_FALSE;
        loc_b_character_read = SGL_FALSE;
    }

    if (loc_ul_number_of_lines >= SGL_MAX_STRING_LINES) {
        oglxSetError(glob_ul_textarea_error, 0U);
    }

    return loc_ul_number_of_lines;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_text_lines_up_to_down
  DESCRIPTION:
    Function shall draw text lines from up to down 
  PARAMETERS:
    par_f_x_origin -> x origin of the draw
    par_f_y_origin -> y origin point of the draw
    par_f_y_delta -> y delta of the draw
	par_ul_number_of_lines -> number of lines
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_text_lines_up_to_down(SGLfloat par_f_x_origin, SGLfloat par_f_y_origin, SGLfloat par_f_y_delta, SGLulong par_ul_number_of_lines)
{
    SGLlong loc_l_i;
    SGLfloat loc_f_x;
    SGLfloat loc_f_y;
    SGLulong loc_ul_number_of_lines = par_ul_number_of_lines;

    loc_f_x = par_f_x_origin;
    loc_f_y = par_f_y_origin;

    glob_pr_sglStatemachine->f_v_alignment = 0.0F;
    glob_pr_sglStatemachine->b_v_alignment = (SGLbyte) SGL_ALIGN_BOTTOM;

    for (loc_l_i = 0; loc_l_i < (SGLlong) loc_ul_number_of_lines; loc_l_i++) {
        sgl_line_string loc_s_line = ts_lines[loc_l_i];
        if (loc_s_line.ul_substring_length > 0U) {
            oglxWriteText(loc_f_x, loc_f_y, loc_s_line.pb_substring, loc_s_line.ul_substring_length);
        }
        else {
            /* Nothing to do */
        }
        loc_f_y -= par_f_y_delta;
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_text_lines_down_to_up
  DESCRIPTION:
    Function shall draw text lines from down to up
  PARAMETERS:
    par_f_x_origin -> x origin of the draw
    par_f_y_origin -> y origin point of the draw
    par_f_y_delta -> y delta of the draw
	par_ul_number_of_lines -> number of lines
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_text_lines_down_to_up(SGLfloat par_f_x_origin, SGLfloat par_f_y_origin, SGLfloat par_f_y_delta, SGLulong par_ul_number_of_lines)
{
    SGLlong loc_l_i;
    SGLfloat loc_f_x;
    SGLfloat loc_f_y;
    SGLlong loc_l_number_of_lines = SGLuint32_to_int32(par_ul_number_of_lines);

    loc_f_x = par_f_x_origin;
    loc_f_y = par_f_y_origin;

    glob_pr_sglStatemachine->f_v_alignment = 0.0F;
    glob_pr_sglStatemachine->b_v_alignment = (SGLbyte) SGL_ALIGN_BOTTOM;

    for (loc_l_i = (loc_l_number_of_lines - 1L); loc_l_i >= 0; loc_l_i--) {
        sgl_line_string loc_s_line = ts_lines[loc_l_i];
        if (loc_s_line.ul_substring_length > 0U) {
            oglxWriteText(loc_f_x, loc_f_y, loc_s_line.pb_substring, loc_s_line.ul_substring_length);
        }
        else {
            /* Nothing to do */
        }
        loc_f_y += par_f_y_delta;
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_string_lines
  DESCRIPTION:
    Function shall draw each line of a string.
  PARAMETERS:
    par_f_x_bottom_left -> x-coordinate of the bottom left point
    par_f_y_bottom_left -> y-coordinate of the bottom left point
    par_f_x_top_right -> x-coordinate of the top right point
    par_f_y_top_right -> y-coordinate of the top right point
    par_f_area_height -> area height
	par_ul_font_id -> current font index
	par_ul_number_of_lines -> number of lines to draw
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_string_lines(SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left, SGLfloat par_f_x_top_right, SGLfloat par_f_y_top_right,
                           SGLfloat par_f_area_height, SGLulong par_ul_font_id, SGLulong par_ul_number_of_lines)
{
    const glf_type_font *loc_p_font = oglx_get_font(par_ul_font_id);
    SGLfloat loc_f_char_height = loc_p_font->f_max_char_height * glob_pr_sglStatemachine->f_ratio_scale_height;
    SGLfloat loc_f_v_advance = loc_p_font->f_max_v_advance * glob_pr_sglStatemachine->f_ratio_scale_height;
    SGLulong loc_ul_number_of_lines = par_ul_number_of_lines;
    SGLfloat loc_f_area_height = par_f_area_height;

    /* Manage each kind of alignment */
    switch (glob_pr_sglStatemachine->b_h_alignment) {
    case (SGLbyte) SGL_ALIGN_CENTER:
        switch (glob_pr_sglStatemachine->b_v_alignment) {
        case (SGLbyte) SGL_ALIGN_MIDDLE:
            sgl_draw_text_lines_up_to_down((par_f_x_top_right + par_f_x_bottom_left) * 0.5F, (par_f_y_bottom_left + ((loc_f_area_height + ((((SGLfloat)
                                                                                                                                             (loc_ul_number_of_lines - 1UL)) * (loc_f_char_height + loc_f_v_advance)) - loc_f_char_height)) * 0.5F))
                                           , (loc_f_char_height + loc_f_v_advance), loc_ul_number_of_lines);
            break;
        case (SGLbyte) SGL_ALIGN_BOTTOM:
            sgl_draw_text_lines_down_to_up((par_f_x_top_right + par_f_x_bottom_left) * 0.5F, par_f_y_bottom_left, (loc_f_char_height + loc_f_v_advance),
                                           loc_ul_number_of_lines);
            break;
        case (SGLbyte) SGL_ALIGN_TOP:
        default:
            sgl_draw_text_lines_up_to_down((par_f_x_top_right + par_f_x_bottom_left) * 0.5F, (par_f_y_top_right - (loc_f_char_height + loc_f_v_advance)),
                                           (loc_f_char_height + loc_f_v_advance), loc_ul_number_of_lines);
            break;
        }
        break;
    case (SGLbyte) SGL_ALIGN_RIGHT:
        switch (glob_pr_sglStatemachine->b_v_alignment) {
        case (SGLbyte) SGL_ALIGN_MIDDLE:
            sgl_draw_text_lines_up_to_down(par_f_x_top_right, (par_f_y_bottom_left + ((loc_f_area_height
                                                                                       +
                                                                                       ((((SGLfloat) (loc_ul_number_of_lines - 1UL)) *
                                                                                         (loc_f_char_height + loc_f_v_advance)) -
                                                                                        loc_f_char_height)) * 0.5F)), (loc_f_char_height + loc_f_v_advance),
                                           loc_ul_number_of_lines);
            break;
        case (SGLbyte) SGL_ALIGN_BOTTOM:
            sgl_draw_text_lines_down_to_up(par_f_x_top_right, par_f_y_bottom_left, loc_f_char_height + loc_f_v_advance, loc_ul_number_of_lines);
            break;
        case (SGLbyte) SGL_ALIGN_TOP:
        default:
            sgl_draw_text_lines_up_to_down(par_f_x_top_right, par_f_y_top_right - (loc_f_char_height + loc_f_v_advance), (loc_f_char_height + loc_f_v_advance),
                                           loc_ul_number_of_lines);
            break;
        }
        break;
    case (SGLbyte) SGL_ALIGN_LEFT:
    default:
        switch (glob_pr_sglStatemachine->b_v_alignment) {
        case (SGLbyte) SGL_ALIGN_MIDDLE:
            sgl_draw_text_lines_up_to_down(par_f_x_bottom_left, (par_f_y_bottom_left + ((loc_f_area_height
                                                                                         +
                                                                                         ((((SGLfloat) (loc_ul_number_of_lines - 1UL)) *
                                                                                           (loc_f_char_height + loc_f_v_advance)) -
                                                                                          loc_f_char_height)) * 0.5F)), (loc_f_char_height + loc_f_v_advance),
                                           loc_ul_number_of_lines);
            break;
        case (SGLbyte) SGL_ALIGN_BOTTOM:
            sgl_draw_text_lines_down_to_up(par_f_x_bottom_left, par_f_y_bottom_left, loc_f_char_height + loc_f_v_advance, loc_ul_number_of_lines);
            break;
        case (SGLbyte) SGL_ALIGN_TOP:
        default:
            sgl_draw_text_lines_up_to_down(par_f_x_bottom_left, par_f_y_top_right - (loc_f_char_height + loc_f_v_advance),
                                           (loc_f_char_height + loc_f_v_advance), loc_ul_number_of_lines);
            break;
        }
        break;
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_draw_stroke_string
  DESCRIPTION:
    Function shall get and draw each character of a stroke string.
  PARAMETERS:
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_pus_string -> string to display
    par_ui_string_length -> length of the string
    par_b_haloing -> haloing flag
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_draw_stroke_string(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_ui_string_length, SGLbool par_b_haloing)
{
    SGLlong loc_l_counter;
    const glf_type_font *loc_p_font = glob_pr_sglStatemachine->p_current_font;
    SGLfloat loc_f_width = 0.0F;
    SGLfloat loc_f_height = loc_p_font->f_max_char_height * glob_pr_sglStatemachine->f_ratio_scale_height;
    SGLfloat loc_f_x = par_f_x;
    SGLfloat loc_f_y = par_f_y;
    SGLfloat loc_f_tx;
    SGLfloat loc_f_ty;
    GLushort loc_us_index = 0U;

    SGLbool loc_b_stipple_ok_backup = glob_pr_sglStatemachine->b_stipple_ok;

    /* Set length */
    loc_l_counter = (GLint) par_ui_string_length;

    glob_pr_sglStatemachine->b_stipple_ok = SGL_FALSE;

    while (loc_l_counter > 0) {
        loc_l_counter--;
        oglx_compute_character_size((SGLulong) par_pus_string[loc_us_index], (SGLulong) loc_l_counter, &loc_f_width);
        loc_us_index++;
    }

    /* Apply translation */
    loc_f_tx = -glob_pr_sglStatemachine->f_h_alignment * loc_f_width;
    loc_f_ty = -glob_pr_sglStatemachine->f_v_alignment * loc_f_height;

    /* Set the position */
    loc_f_x += loc_f_tx;
    loc_f_y += loc_f_ty;

    /* Change the coordinate of the first line to match exactly one pixel */
    oglxGetEntirePixelCoordinate(loc_f_x, loc_f_y, &loc_f_x, &loc_f_y);

    loc_l_counter = (SGLlong) par_ui_string_length;
    loc_us_index = 0U;

    /* For each character of the string */
    while (loc_l_counter > 0) {
        loc_l_counter--;
        loc_f_tx = oglx_draw_stroke_character(loc_f_x, loc_f_y, (SGLulong) par_pus_string[loc_us_index], par_b_haloing);
        loc_us_index++;
        loc_f_x += loc_f_tx;
    }

    if (loc_b_stipple_ok_backup) {
        glob_pr_sglStatemachine->b_stipple_ok = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_draw_bitmap_string
  DESCRIPTION:
    Function shall get and draw each character of a bitmap string.
  PARAMETERS:
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_pus_string -> string to display
    par_ui_string_length -> length of the string
    par_ui_font_linewidth -> font linewidth
    par_pf_font_color -> font color
    par_b_haloing -> haloing flag
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_draw_bitmap_string(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_ui_string_length,
                             SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color, SGLbool par_b_haloing)
{
    SGLlong loc_l_counter;
    const glf_type_font *loc_p_font = glob_pr_sglStatemachine->p_current_font;
    SGLfloat loc_f_width = 0.0F;
    SGLfloat loc_f_height = loc_p_font->f_max_char_height * glob_pr_sglStatemachine->f_ratio_scale_height;
    SGLfloat loc_f_x = par_f_x;
    SGLfloat loc_f_y = par_f_y;
    SGLfloat loc_f_tx;
    SGLfloat loc_f_ty;
    GLushort loc_us_index = 0U;

    /* Set length */
    loc_l_counter = (GLint) par_ui_string_length;

    oglxColor4fv(par_pf_font_color);
    while (loc_l_counter > 0) {
        loc_l_counter--;
        oglx_compute_character_size((SGLulong) par_pus_string[loc_us_index], (SGLulong) loc_l_counter, &loc_f_width);
        loc_us_index++;
    }

    /* Apply translation */
    loc_f_tx = -glob_pr_sglStatemachine->f_h_alignment * loc_f_width;
    loc_f_ty = -glob_pr_sglStatemachine->f_v_alignment * loc_f_height;

    /* Set the position */
    loc_f_x += loc_f_tx;
    loc_f_y += loc_f_ty;

    /* Change the coordinate of the first line to match exactly one pixel */
    oglxGetEntirePixelCoordinate(loc_f_x, loc_f_y, &loc_f_x, &loc_f_y);

    loc_l_counter = (GLint) par_ui_string_length;
    loc_us_index = 0U;

    /* For each character of the string */
    while (loc_l_counter > 0) {
        loc_l_counter--;
        if (loc_l_counter == 0) {
            loc_f_tx = oglx_bitmap_text(loc_f_x, loc_f_y, (SGLulong) par_pus_string[loc_us_index], par_ui_font_linewidth, par_b_haloing, SGL_TRUE);
        }
        else {
            loc_f_tx = oglx_bitmap_text(loc_f_x, loc_f_y, (SGLulong) par_pus_string[loc_us_index], par_ui_font_linewidth, par_b_haloing, SGL_FALSE);
        }
        loc_us_index++;
        loc_f_x += loc_f_tx;
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_write_attributed_text
  DESCRIPTION:
    Function shall draw the input string.
  PARAMETERS:
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_pus_string -> string to display
    par_ui_string_length -> length of the string
    par_ui_font_linewidth -> font linewidth
    par_pf_font_color -> font color
    par_b_haloing -> haloing flag
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_write_attributed_text(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_ui_string_length,
                                SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color, SGLbool par_b_haloing)
{
    if (glob_pr_sglStatemachine->p_current_font->us_font_kind == GLF_FONT_TYPE_BITMAP) {
        oglx_draw_bitmap_string(par_f_x, par_f_y, par_pus_string, par_ui_string_length, par_ui_font_linewidth, par_pf_font_color, par_b_haloing);
    }
    else {
        oglx_draw_stroke_string(par_f_x, par_f_y, par_pus_string, par_ui_string_length, par_b_haloing);
    }

    return;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxWriteAttributedText
  DESCRIPTION:
    Function shall prepare the drawing of a string.
  PARAMETERS:
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_ui_font_identifier -> font identifier
    par_pus_string -> string to display
    par_ui_string_length -> length of the string
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxWriteAttributedText(SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_font_identifier, SGLushort * par_pus_string, SGLulong par_ui_string_length)
{
    if (par_ui_string_length > 0U) {
        GLfloat loc_tf_font_color[4];

        oglxSelectFont(par_ui_font_identifier);

        if (glob_pr_sglStatemachine->b_haloing_state != (SGLbyte) SGL_FALSE) {
            if (glob_pr_sglStatemachine->l_halo_color_index < 0L) {
                oglx_write_attributed_text(par_f_x, par_f_y, par_pus_string, par_ui_string_length,
                                           (SGLulong) (glob_pr_sglStatemachine->b_current_font_linewidth + 2U), (const SGLfloat *) stc_tf_black_wta, SGL_TRUE);
            }
            else {
                loc_tf_font_color[0] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_halo_color_index].f_red;
                loc_tf_font_color[1] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_halo_color_index].f_green;
                loc_tf_font_color[2] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_halo_color_index].f_blue;
                loc_tf_font_color[3] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_halo_color_index].f_alpha;
                oglx_write_attributed_text(par_f_x, par_f_y, par_pus_string, par_ui_string_length,
                                           (SGLulong) (glob_pr_sglStatemachine->b_current_font_linewidth + 2U), (const SGLfloat *) loc_tf_font_color, SGL_TRUE);
            }

            glob_pr_sglStatemachine->b_haloing_state = SGL_COLORED_PART_ONLY;
        }
        else {
            /* Nothing to do */
        }

        loc_tf_font_color[0] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_current_font_color_index].f_red;
        loc_tf_font_color[1] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_current_font_color_index].f_green;
        loc_tf_font_color[2] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_current_font_color_index].f_blue;
        loc_tf_font_color[3] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_current_font_color_index].f_alpha;
        oglx_write_attributed_text(par_f_x, par_f_y, par_pus_string, par_ui_string_length, glob_pr_sglStatemachine->b_current_font_linewidth,
                                   (const SGLfloat *) loc_tf_font_color, SGL_FALSE);
    }
    else {
        /* Nothing to do. String length is zero. */
    }
    return;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxWriteText
  DESCRIPTION:
    Function shall write text string.
  PARAMETERS:
    par_f_x -> x-coordinate of the reference point
    par_f_y -> y-coordinate of the reference point
    par_pus_string -> string to be drawn
    par_l_string_length -> string length
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxWriteText(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_l_string_length)
{
    SGLulong loc_font_id = glob_pr_sglStatemachine->ul_font_id;
    SGLbyte loc_b_haloing_state_backup = glob_pr_sglStatemachine->b_haloing_state;
    SGLlong loc_l_color_index_backup = glob_pr_sglStatemachine->l_color_index;
    SGLbyte loc_b_transparency_backup = glob_pr_sglStatemachine->b_transparency;

    sglTransparency8((SGLlong) 255U);

    oglxWriteAttributedText(par_f_x, par_f_y, loc_font_id, par_pus_string, par_l_string_length);

    /* Restore settings changed by the GLF */
    oglxUpdateColor(loc_l_color_index_backup);

    if (loc_b_haloing_state_backup != (SGLbyte) SGL_FALSE) {
        sglEnable(SGL_LINE_HALOING);
    }
    else {
        /* Nothing to do */
    }

    sglTransparency8((SGLlong) loc_b_transparency_backup);

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgluWriteTextArea
  DESCRIPTION:
    Function shall draw a text area in given bounding box.
  PARAMETERS:
    par_f_x_bottom_left -> x-coordinate of the bottom left point
    par_f_y_bottom_left -> y-coordinate of the bottom left point
    par_f_x_top_right -> x-coordinate of the top right point
    par_f_y_top_right -> y-coordinate of the top right point
    par_pb_string -> string to be drawn
    par_l_string_length -> string length
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgluWriteTextArea(SGLfloat par_f_x_bottom_left, SGLfloat par_f_y_bottom_left, SGLfloat par_f_x_top_right, SGLfloat par_f_y_top_right,
                       const SGLbyte * par_pb_string, SGLlong par_l_string_length)
{
    SGLfloat loc_f_temp;
    SGLfloat loc_f_area_width = mth_norm(par_f_x_bottom_left, par_f_y_bottom_left, par_f_x_top_right, par_f_y_bottom_left);
    SGLfloat loc_f_area_height = mth_norm(par_f_x_bottom_left, par_f_y_bottom_left, par_f_x_bottom_left, par_f_y_top_right);
    SGLulong loc_ul_font_id = glob_pr_sglStatemachine->ul_font_id;
    SGLfloat loc_f_x_bottom_left = par_f_x_bottom_left;
    SGLfloat loc_f_y_bottom_left = par_f_y_bottom_left;
    SGLfloat loc_f_x_top_right = par_f_x_top_right;
    SGLfloat loc_f_y_top_right = par_f_y_top_right;
    SGLlong loc_l_string_length = par_l_string_length;

    SGLulong loc_ul_number_of_lines;

    if ((loc_l_string_length >= 0) && (par_pb_string != SGL_NULL)) {
        SGLfloat loc_f_prev_v_alignment = glob_pr_sglStatemachine->f_v_alignment;
        SGLbyte loc_b_prev_v_alignment = glob_pr_sglStatemachine->b_v_alignment;
        const sgl_line_width *loc_prev_line_width = glob_pr_sglStatemachine->tr_line_width;
        SGLulong loc_prev_line_width_size = glob_pr_sglStatemachine->ul_nb_line_width;
        SGLlong loc_l_line_width_backup = glob_pr_sglStatemachine->l_linewidth_index;
        SGLlong loc_l_counter;
        SGLulong loc_ui_mblen;
        const GLubyte *loc_pb_string;

        glob_ul_textarea_error = SGL_ERROR_SGLU_WRITETEXTAREA;

        glob_pr_sglStatemachine->tr_line_width = (sgl_line_width *) font_line_width;
        glob_pr_sglStatemachine->ul_nb_line_width = FONT_LINE_WIDTH_SIZE;
        glob_pr_sglStatemachine->l_linewidth_index = glob_pr_sglStatemachine->l_current_font_linewidth_index;

        /* A text area is written, it is usefull for the checks of sgluWriteText */
        if (loc_l_string_length > ((SGLlong) SGLU_MAX_TEXT_AREA_STRING_LENGTH)) {
            loc_l_string_length = ((SGLlong) SGLU_MAX_TEXT_AREA_STRING_LENGTH);
        }
        else {
            /* Nothing to do */
        }

        loc_l_counter = (GLint) loc_l_string_length;
        loc_pb_string = par_pb_string;
        loc_l_string_length = 0U;

        while (loc_l_counter > 0) {
            if ((*loc_pb_string) != 0U) {
                GLushort loc_us_index = 0U;
                if (!(sgl_is_line_separator((SGLushort) * loc_pb_string)) && !(sgl_is_word_separator((SGLushort) * loc_pb_string))) {
                    loc_ui_mblen = (GLuint) oglx_get_char_index(&loc_us_index, loc_pb_string);
                    loc_pb_string += loc_ui_mblen;
                    glob_us_string[loc_l_string_length] = loc_us_index;
                }
                else {
                    glob_us_string[loc_l_string_length] = (SGLushort) * loc_pb_string;
                    loc_pb_string++;
                }

                loc_l_counter--;
                loc_l_string_length++;
            }
            else {
                loc_l_counter = 0;
            }
        }
        glob_us_string[loc_l_string_length] = 0U;

        if (loc_f_x_bottom_left > loc_f_x_top_right) {
            loc_f_temp = loc_f_x_top_right;
            loc_f_x_top_right = loc_f_x_bottom_left;
            loc_f_x_bottom_left = loc_f_temp;
        }
        else {
            /* Nothing to do */
        }

        if (loc_f_y_bottom_left > loc_f_y_top_right) {
            loc_f_temp = loc_f_y_top_right;
            loc_f_y_top_right = loc_f_y_bottom_left;
            loc_f_y_bottom_left = loc_f_temp;
        }
        else {
            /* Nothing to do */
        }

        loc_ul_number_of_lines = sgl_define_string_lines(glob_us_string, (SGLulong) loc_l_string_length, loc_f_area_width);

        sgl_draw_string_lines(loc_f_x_bottom_left, loc_f_y_bottom_left, loc_f_x_top_right, loc_f_y_top_right, loc_f_area_height,
                              loc_ul_font_id, loc_ul_number_of_lines);

        glob_pr_sglStatemachine->f_v_alignment = loc_f_prev_v_alignment;
        glob_pr_sglStatemachine->b_v_alignment = loc_b_prev_v_alignment;

        glob_pr_sglStatemachine->tr_line_width = loc_prev_line_width;
        glob_pr_sglStatemachine->ul_nb_line_width = loc_prev_line_width_size;
        glob_pr_sglStatemachine->l_linewidth_index = loc_l_line_width_backup;
    }
    else {
        oglxSetError(SGL_ERROR_SGLU_WRITETEXTAREA, 0U);
    }

    return;
}

/* End of File ***************************************************************/
