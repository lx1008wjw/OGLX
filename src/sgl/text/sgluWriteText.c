/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluWriteText.c
 DESCRIPTION       : sgluWriteText command shall draw a string
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

static GLfloat stc_tf_black_wt[4] = { 0.0F, 0.0F, 0.0F, 1.0F };

static SGLulong ul_line_feed_number;


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_bitmap_text
  DESCRIPTION:
    Function shall get a character and eventually draw a bitmap text.
  PARAMETERS:
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_us_index -> index of character
    par_ui_font_linewidth -> font linewidth
    par_b_haloing -> haloing flag
	par_b_draw-> SGL_TRUE if the text shall be drawn, SGL_FALSE if the character shall be stored.
  RETURN:
    Horizontal advance to next character.
---------------------------------------------------------------------+*/
SGLfloat oglx_bitmap_text(SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_us_index, SGLulong par_ui_font_linewidth, SGLbool par_b_haloing, SGLbool par_b_draw)
{
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_width;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_height;
    const glf_type_font *loc_p_font = glob_pr_sglStatemachine->p_current_font;
    const glf_type_character_bitmap *loc_pr_char = GLF_GET_BITMAP_CHAR(GLF_GET_CHAR(loc_p_font, par_us_index));

    oglx_fonts_draw_bitmap_text(loc_p_font, loc_pr_char, par_f_x, par_f_y, par_ui_font_linewidth, loc_f_ratio_x, loc_f_ratio_y, par_b_haloing, par_b_draw);
    return (loc_pr_char->tr_type_character.f_horizontal_advance * loc_f_ratio_x);
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_draw_stroke_character
  DESCRIPTION:
    Function shall get and draw a stroke character.
  PARAMETERS:
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_us_index -> index of character
    par_b_haloing -> haloing flag
  RETURN:
    Horizontal advance to the next character to draw.
---------------------------------------------------------------------+*/
SGLfloat oglx_draw_stroke_character(SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_us_index, SGLbool par_b_haloing)
{
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_width;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_height;
    const glf_type_font *loc_p_font = glob_pr_sglStatemachine->p_current_font;
    const glf_type_character_stroke *loc_pr_char_stroke = GLF_GET_STROKE_CHAR(GLF_GET_CHAR(loc_p_font, par_us_index));

    oglx_fonts_draw_stroke_character(loc_p_font, loc_pr_char_stroke, par_f_x, par_f_y, loc_f_ratio_x, loc_f_ratio_y,
                                     (GLuint) (glob_pr_sglStatemachine->l_current_font_linewidth_index),
                                     (GLuint) (glob_pr_sglStatemachine->l_current_font_color_index), par_b_haloing);

    return (loc_pr_char_stroke->tr_type_character.f_horizontal_advance * loc_f_ratio_x);
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_compute_character_size
  DESCRIPTION:
    Function shall compute the size of character pointed by current index.
  PARAMETERS:
    par_us_index -> index of character in font
    par_ui_counter -> index of character in string
    par_pf_width -> pointer to the width of character
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_compute_character_size(SGLulong par_us_index, SGLulong par_ui_counter, SGLfloat * par_pf_width)
{
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_width;
    const glf_type_font *loc_p_font = glob_pr_sglStatemachine->p_current_font;
    const glf_type_character *loc_pr_char = GLF_GET_CHAR(loc_p_font, par_us_index);

    if (par_ui_counter == 0UL) {
        /* last character */
        (*par_pf_width) += loc_pr_char->f_width * loc_f_ratio_x;
    }
    else {
        /* other character */
        (*par_pf_width) += loc_pr_char->f_horizontal_advance * loc_f_ratio_x;
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_define_lines
  DESCRIPTION:
    Function shall cut the given string in lines 
  PARAMETERS:
    par_pus_string -> string to be drawn
    par_ul_string_length -> string length
  RETURN:
    The number of lines.
---------------------------------------------------------------------+*/
SGLulong sgl_define_lines(SGLushort * par_pus_string, SGLulong par_ul_string_length)
{
    SGLulong loc_ul_length = 1U;
    SGLulong loc_ul_glob_length = 1U;
    SGLushort *loc_pus_string = (SGLushort *) par_pus_string;
    SGLulong loc_ul_string_length = par_ul_string_length;
    SGLbool loc_b_line_separator_found;
    SGLushort loc_us_index = 0;

    /* Initialize number of lines */
    SGLulong loc_ul_number_of_lines = 0U;

    ul_line_feed_number = 0UL;

    while ((loc_ul_glob_length <= loc_ul_string_length) && (loc_ul_number_of_lines < SGL_MAX_STRING_LINES)) {

        if (loc_pus_string[loc_us_index] == (SGLushort) SGL_K_CR) {
            loc_b_line_separator_found = SGL_TRUE;
        }
        else {
            if (loc_pus_string[loc_us_index] == (SGLushort) SGL_K_LF) {
                ul_line_feed_number++;
            }
            else {
                /* Nothing to do */
            }

            loc_b_line_separator_found = SGL_FALSE;
        }

        if (loc_b_line_separator_found) {
            loc_ul_length = loc_ul_length - 1UL;
            loc_ul_glob_length = loc_ul_glob_length - 1UL;

            /* Fill the lines structure */
            ts_lines[loc_ul_number_of_lines].pb_substring = loc_pus_string;

            /* Store empty lines as strings of size 0 */
            ts_lines[loc_ul_number_of_lines].ul_substring_length = loc_ul_length;

            loc_ul_number_of_lines++;
            loc_pus_string = loc_pus_string + loc_us_index + 1;
            loc_ul_string_length = loc_ul_string_length - (loc_ul_glob_length + 1UL);

            loc_ul_length = 1;
            loc_ul_glob_length = 1;
            loc_us_index = 0U;
        }
        else {
            loc_ul_length++;
            loc_ul_glob_length++;
            loc_us_index++;
        }
    }

    /* Raise an error if maximum number of lines is reached */
    if (loc_ul_number_of_lines >= SGL_MAX_STRING_LINES) {
        oglxSetError(glob_ul_text_error, 0U);
    }
    else {
        /* Write the last line */
        ts_lines[loc_ul_number_of_lines].pb_substring = loc_pus_string;

        /* Store empty lines as strings of size 0 */
        ts_lines[loc_ul_number_of_lines].ul_substring_length = loc_ul_length - 1UL;

        loc_ul_number_of_lines++;
    }

    return loc_ul_number_of_lines;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_draw_lines_up_to_down
  DESCRIPTION:
    Function shall draw text lines from up to down 
  PARAMETERS:
    par_f_x_origin -> x origin of the draw
    par_f_y_origin -> y origin point of the draw
    par_f_y_delta -> y delta of the draw
    par_ui_font_linewidth -> current font line width
    par_pf_font_color -> current font color
	par_b_haloing -> haloing enabled or not
    par_ul_number_of_lines -> number of lines to draw
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_draw_lines_up_to_down(SGLfloat par_f_x_origin, SGLfloat par_f_y_origin, SGLfloat par_f_y_delta,
                               SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color, SGLbool par_b_haloing, SGLulong par_ul_number_of_lines)
{
    SGLlong loc_l_i;
    SGLfloat loc_f_x = par_f_x_origin;
    SGLfloat loc_f_y = par_f_y_origin;

    for (loc_l_i = 0; loc_l_i < (SGLlong) par_ul_number_of_lines; loc_l_i++) {
        if (ts_lines[loc_l_i].ul_substring_length > 0U) {
            loc_f_y =
                oglx_write_attributed_text_multiline(loc_f_x, loc_f_y, ts_lines[loc_l_i].pb_substring,
                                                     ts_lines[loc_l_i].ul_substring_length, par_ui_font_linewidth, par_pf_font_color,
                                                     par_b_haloing, par_f_y_delta);
        }
        else {
            /* Nothing to do */
        }
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_draw_aligned_lines
  DESCRIPTION:
    Function shall draw and align each line
  PARAMETERS:
    par_f_x -> x origin of the draw
    par_f_y -> y origin point of the draw
    par_f_char_height -> height of characters
    par_f_v_advance -> vertical advance
    par_ui_font_linewidth -> current font line width
    par_pf_font_color -> current font color
    par_b_haloing -> haloing enabled or not
    par_ul_number_of_lines -> number of lines to draw
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_draw_aligned_lines(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat par_f_char_height, SGLfloat par_f_v_advance,
                             SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color, SGLbool par_b_haloing, SGLulong par_ul_number_of_lines)
{
    SGLfloat loc_f_y = par_f_y;
    SGLfloat loc_f_ty = par_f_char_height + par_f_v_advance;
    SGLfloat loc_f_v_advance = par_f_v_advance;
    SGLfloat loc_f_line_feed_number = (SGLfloat) (ul_line_feed_number);

    switch (glob_pr_sglStatemachine->b_v_alignment) {
    case (SGLbyte) SGL_ALIGN_MIDDLE:
        sgl_draw_lines_up_to_down(par_f_x, loc_f_y + (((((loc_f_line_feed_number - 1.0F) * loc_f_ty) + loc_f_v_advance) * 0.5F))
                                  , loc_f_ty, par_ui_font_linewidth, par_pf_font_color, par_b_haloing, par_ul_number_of_lines);
        break;
    case (SGLbyte) SGL_ALIGN_BOTTOM:
        sgl_draw_lines_up_to_down(par_f_x, loc_f_y + (loc_f_line_feed_number * loc_f_ty), loc_f_ty, par_ui_font_linewidth, par_pf_font_color, par_b_haloing,
                                  par_ul_number_of_lines);
        break;
    default:
        /*
           case SGL_ALIGN_TOP:
         */
        sgl_draw_lines_up_to_down(par_f_x, loc_f_y - loc_f_ty, loc_f_ty, par_ui_font_linewidth, par_pf_font_color, par_b_haloing, par_ul_number_of_lines);
        break;
    }

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_draw_stroke_string_multiline
  DESCRIPTION:
    Function shall get and draw each character of a stroke string.
  PARAMETERS:
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_pus_string -> string to display
    par_ui_string_length -> length of the string
    par_b_haloing -> haloing flag
    par_f_y_delta -> y delta to apply when a line feed is in the string
  RETURN:
    Y position of next line.
---------------------------------------------------------------------+*/
SGLfloat oglx_draw_stroke_string_multiline(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_ui_string_length,
                                           SGLbool par_b_haloing, SGLfloat par_f_y_delta)
{
    SGLlong loc_l_counter;
    SGLfloat loc_f_width = 0.0F;
    SGLfloat loc_f_x = par_f_x;
    SGLfloat loc_f_y = par_f_y;
    SGLfloat loc_f_tx;
    GLushort loc_us_index = 0U;

    SGLbool loc_b_stipple_ok_backup = glob_pr_sglStatemachine->b_stipple_ok;

    /* Set length */
    loc_l_counter = (SGLlong) par_ui_string_length;

    glob_pr_sglStatemachine->b_stipple_ok = SGL_FALSE;

    while (loc_l_counter > 0) {
        loc_l_counter--;
        if (par_pus_string[loc_us_index] != (SGLushort) SGL_K_LF) {
            oglx_compute_character_size((SGLulong) par_pus_string[loc_us_index], (SGLulong) loc_l_counter, &loc_f_width);
        }
        else {
            /* Nothing to do */
        }
        loc_us_index++;
    }

    /* Apply translation */
    loc_f_tx = -glob_pr_sglStatemachine->f_h_alignment * loc_f_width;

    /* Set the position */
    loc_f_x += loc_f_tx;

    /* Change the coordinate of the first line to match exactly one pixel */
    oglxGetEntirePixelCoordinate(loc_f_x, loc_f_y, &loc_f_x, &loc_f_y);

    loc_l_counter = (GLint) par_ui_string_length;
    loc_us_index = 0U;
    /* For each character of the string */
    while (loc_l_counter > 0) {
        loc_l_counter--;

        if (par_pus_string[loc_us_index] != SGL_K_LF) {
            loc_f_tx = oglx_draw_stroke_character(loc_f_x, loc_f_y, par_pus_string[loc_us_index], par_b_haloing);
            loc_f_x += loc_f_tx;
        }
        else {
            loc_f_y += -par_f_y_delta;
        }
        loc_us_index++;
    }

    if (loc_b_stipple_ok_backup) {
        glob_pr_sglStatemachine->b_stipple_ok = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    return loc_f_y;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_draw_bitmap_string_multiline
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
    par_f_y_delta -> y delta to apply when a line feed is in the string
  RETURN:
    Y position of next line.
---------------------------------------------------------------------+*/
SGLfloat oglx_draw_bitmap_string_multiline(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_ui_string_length,
                                           SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color, SGLbool par_b_haloing, SGLfloat par_f_y_delta)
{
    SGLlong loc_l_counter;
    SGLfloat loc_f_width = 0.0F;
    SGLfloat loc_f_x = par_f_x;
    SGLfloat loc_f_y = par_f_y;
    SGLfloat loc_f_tx;
    GLushort loc_us_index = 0U;

    /* Set length */
    loc_l_counter = (GLint) par_ui_string_length;

    oglxColor4fv(par_pf_font_color);
    while (loc_l_counter > 0) {
        loc_l_counter--;
        if (par_pus_string[loc_us_index] != (SGLushort) SGL_K_LF) {
            oglx_compute_character_size((SGLulong) par_pus_string[loc_us_index], (SGLulong) loc_l_counter, &loc_f_width);
        }
        else {
            /* Nothing to do */
        }
        loc_us_index++;
    }

    /* Apply translation */
    loc_f_tx = -glob_pr_sglStatemachine->f_h_alignment * loc_f_width;

    /* Set the position */
    loc_f_x += loc_f_tx;
    loc_l_counter = (GLint) par_ui_string_length;
    loc_us_index = 0U;

    /* Change the coordinate of the first line to match exactly one pixel */
    oglxGetEntirePixelCoordinate(loc_f_x, loc_f_y, &loc_f_x, &loc_f_y);

    /* For each character of the string */
    while (loc_l_counter > 0) {
        loc_l_counter--;

        if (par_pus_string[loc_us_index] != SGL_K_LF) {
            if ((loc_l_counter == 0) || (par_pus_string[loc_us_index + 1U] == SGL_K_LF)) {
                loc_f_tx = oglx_bitmap_text(loc_f_x, loc_f_y, par_pus_string[loc_us_index], par_ui_font_linewidth, par_b_haloing, SGL_TRUE);
            }
            else {
                loc_f_tx = oglx_bitmap_text(loc_f_x, loc_f_y, par_pus_string[loc_us_index], par_ui_font_linewidth, par_b_haloing, SGL_FALSE);
            }
            loc_f_x += loc_f_tx;
        }
        else {
            loc_f_y += -par_f_y_delta;
        }
        loc_us_index++;
    }

    return loc_f_y;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_write_attributed_text_multiline
  DESCRIPTION:
    Function shall get and draw each chracter of a string.
  PARAMETERS:
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_pus_string -> string to display
    par_ui_string_length -> length of the string
    par_ui_font_linewidth -> font linewidth
    par_pf_font_color -> font color
    par_b_haloing -> haloing flag
    par_f_y_delta -> y delta to apply when a line feed is in the string
  RETURN:
    Y position of next line.
---------------------------------------------------------------------+*/
SGLfloat oglx_write_attributed_text_multiline(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_ui_string_length,
                                              SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color, SGLbool par_b_haloing, SGLfloat par_f_y_delta)
{
    SGLfloat loc_f_return;

    if (glob_pr_sglStatemachine->p_current_font->us_font_kind == GLF_FONT_TYPE_BITMAP) {
        loc_f_return = oglx_draw_bitmap_string_multiline(par_f_x, par_f_y, par_pus_string, par_ui_string_length,
                                                         par_ui_font_linewidth, par_pf_font_color, par_b_haloing, par_f_y_delta);
    }
    else {
        loc_f_return = oglx_draw_stroke_string_multiline(par_f_x, par_f_y, par_pus_string, par_ui_string_length, par_b_haloing, par_f_y_delta);
    }

    return loc_f_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxWriteAttributedTextMultiline
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
void oglxWriteAttributedTextMultiline(SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_font_identifier, SGLushort * par_pus_string,
                                      SGLulong par_ui_string_length)
{
    SGLulong loc_ul_number_of_lines;
    SGLulong loc_ui_length = par_ui_string_length;

    const glf_type_font *loc_p_font = oglx_get_font(par_ui_font_identifier);
    SGLfloat loc_f_char_height = loc_p_font->f_max_char_height * glob_pr_sglStatemachine->f_ratio_scale_height;
    SGLfloat loc_f_v_advance = loc_p_font->f_max_v_advance * glob_pr_sglStatemachine->f_ratio_scale_height;
    GLfloat loc_tf_font_color[4];

    oglxSelectFont(par_ui_font_identifier);

    loc_ul_number_of_lines = sgl_define_lines(par_pus_string, loc_ui_length);

    if (glob_pr_sglStatemachine->b_haloing_state != (SGLbyte) SGL_FALSE) {
        if (glob_pr_sglStatemachine->l_halo_color_index < 0L) {
            oglx_draw_aligned_lines(par_f_x, par_f_y, loc_f_char_height, loc_f_v_advance,
                                    (SGLulong) (glob_pr_sglStatemachine->b_current_font_linewidth + 2U), (const SGLfloat *) stc_tf_black_wt, SGL_TRUE,
                                    loc_ul_number_of_lines);
        }
        else {
            loc_tf_font_color[0] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_halo_color_index].f_red;
            loc_tf_font_color[1] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_halo_color_index].f_green;
            loc_tf_font_color[2] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_halo_color_index].f_blue;
            loc_tf_font_color[3] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_halo_color_index].f_alpha;
            oglx_draw_aligned_lines(par_f_x, par_f_y, loc_f_char_height, loc_f_v_advance,
                                    (SGLulong) (glob_pr_sglStatemachine->b_current_font_linewidth + 2U), (const SGLfloat *) loc_tf_font_color, SGL_TRUE,
                                    loc_ul_number_of_lines);
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
    oglx_draw_aligned_lines(par_f_x, par_f_y, loc_f_char_height, loc_f_v_advance,
                            glob_pr_sglStatemachine->b_current_font_linewidth, (const SGLfloat *) loc_tf_font_color, SGL_FALSE, loc_ul_number_of_lines);
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxGetEntirePixelCoordinate
  DESCRIPTION:
    Function shall fill given pointer with an entire pixel coordinate if it is not called in display list and no rotation is active.
  PARAMETERS:
    par_f_x -> x-coordinate of the reference point
    par_f_y -> y-coordinate of the reference point
    par_pf_x -> pointer to x coordinate to fill
    par_pf_y -> pointer to y coordinate to fill
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxGetEntirePixelCoordinate(SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat * par_pf_x, SGLfloat * par_pf_y)
{
    SGLfloat loc_f_x;
    SGLfloat loc_f_y;
    SGLfloat loc_f_angle = glob_pr_sglStatemachine->f_rotation_angle;

    if (!(glob_pr_sglStatemachine->b_enable_text_adjustment) || (glob_pr_sglStatemachine->b_static_sequence_started) || (!sgluFloatIsEqual(loc_f_angle, 0.0F))) {
        *par_pf_x = par_f_x;
        *par_pf_y = par_f_y;
    }
    else {
        /* Round the position to the closest entire pixel */
        sglConvertPointToPixel(par_f_x, par_f_y, &loc_f_x, &loc_f_y);
        loc_f_x = mth_f_round(loc_f_x);
        loc_f_y = mth_f_round(loc_f_y);
        sglConvertPixelToPoint(loc_f_x, loc_f_y, par_pf_x, par_pf_y);
    }
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxWriteTextMultiline
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
void oglxWriteTextMultiline(SGLfloat par_f_x, SGLfloat par_f_y, SGLushort * par_pus_string, SGLulong par_l_string_length)
{
    SGLulong loc_font_id = glob_pr_sglStatemachine->ul_font_id;
    SGLbyte loc_b_haloing_state_backup = glob_pr_sglStatemachine->b_haloing_state;
    SGLlong loc_l_color_index_backup = glob_pr_sglStatemachine->l_color_index;
    SGLbyte loc_b_transparency_backup = glob_pr_sglStatemachine->b_transparency;

    sglTransparency8((SGLlong) 255U);

    oglxWriteAttributedTextMultiline(par_f_x, par_f_y, loc_font_id, par_pus_string, par_l_string_length);

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
  NAME: sgluWriteText
  DESCRIPTION:
    Function shall draw a string.
  PARAMETERS:
    par_f_x -> x-coordinate of the reference point
    par_f_y -> y-coordinate of the reference point
    par_pb_string -> string to be drawn
    par_l_string_length -> string length
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgluWriteText(SGLfloat par_f_x, SGLfloat par_f_y, const SGLbyte * par_pb_string, SGLlong par_l_string_length)
{
    SGLbool loc_b_error_detected = SGL_FALSE;
    SGLlong loc_l_string_length = par_l_string_length;

    /* Check given string is not NULL */
    if (par_pb_string == SGL_NULL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check the size of given string is positive */
    if (loc_l_string_length < 0L) {
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
        SGLlong loc_l_counter;
        SGLulong loc_ui_mblen;
        const GLubyte *loc_pb_string;
        glob_ul_text_error = SGL_ERROR_SGLU_WRITETEXT;

        glob_pr_sglStatemachine->tr_line_width = (sgl_line_width *) font_line_width;
        glob_pr_sglStatemachine->ul_nb_line_width = FONT_LINE_WIDTH_SIZE;
        glob_pr_sglStatemachine->l_linewidth_index = glob_pr_sglStatemachine->l_current_font_linewidth_index;

        /* Check the length is not too big */
        if (loc_l_string_length > (SGLlong) SGLU_MAX_STRING_LENGTH) {
            loc_l_string_length = (SGLlong) SGLU_MAX_STRING_LENGTH;
            oglxSetError(SGL_ERROR_SGLU_WRITETEXT, (SGLulong) loc_l_string_length);
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
                if ((*loc_pb_string != SGL_K_CR) && (*loc_pb_string != SGL_K_LF)) {
                    loc_ui_mblen = (GLuint) oglx_get_char_index(&loc_us_index, loc_pb_string);
                    loc_pb_string += loc_ui_mblen;
                    glob_us_string[loc_l_string_length] = loc_us_index;
                }
                else {
                    glob_us_string[loc_l_string_length] = (SGLushort) * loc_pb_string;
                    loc_pb_string++;
                    loc_b_multiline = SGL_TRUE;
                }

                loc_l_counter--;
                loc_l_string_length++;
            }
            else {
                loc_l_counter = 0;
            }
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
        oglxSetError(SGL_ERROR_SGLU_WRITETEXT, 0U);
    }

    return;
}

/* End of File ***************************************************************/
