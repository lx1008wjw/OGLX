/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluWriteRichText.c
 DESCRIPTION       : sgluWriteRichText command shall draw a string
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

/*+ DEFINITION OF CONSTANTS -------------------------------------------------------
 DESCRIPTION       : Definition of constants of sgluWriteRichText.c
 LLR: [OGLX-DDD-sgluWriteRichText_constants]
---------------------------------------------------------------------------- +*/
#define BOLD_WIDTH		0.12F
/*+ END OF DEFINITION OF CONSTANTS -------------------------------------------------------*/

/*+ DEFINITION OF GLOBAL VARIABLES -------------------------------------------------------
 DESCRIPTION       : Definition of global variables of sgluWriteRichText.c
 LLR: [OGLX-DDD-sgluWriteRichText_globals]
---------------------------------------------------------------------------- +*/
static SGLfloat glob_t_line_origin[SGL_MAX_STRING_LINES];
static sgl_rich_text_attr loc_rich_text_attr;
static sgl_char_decoration loc_char_decoration;
static SGLbool b_bold_text;
/*+ END OF DEFINITION OF GLOBAL VARIABLES -------------------------------------------------------*/

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_draw_character_background
  DESCRIPTION:
    Function shall draw the background of a character.
  PARAMETERS:
	par_char_decoration -> Decoration attributes
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_ui_index -> index of the background to draw
    par_f_horiz_advance -> horizontal advance of the character
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_draw_character_background(sgl_char_decoration * par_char_decoration, SGLfloat par_f_x, SGLfloat par_f_y, SGLlong par_ui_index,
                                    SGLfloat par_f_horiz_advance)
{
    SGLlong loc_l_back_color = par_char_decoration->l_back_color[par_ui_index];
    SGLlong loc_l_prev_color = glob_pr_sglStatemachine->l_color_index;
    SGLbool loc_b_prev_reverse = glob_pr_sglStatemachine->b_reverse_color;

    glob_pr_sglStatemachine->b_reverse_color = SGL_FALSE;

    sglIndexColori(loc_l_back_color);
    sglDisable(SGL_POLYGON_SMOOTH);
    sglBegin(SGL_POLYGON);
    sglVertex2f(par_f_x, par_f_y - par_char_decoration->f_max_vert_advance);
    sglVertex2f(par_f_x, par_f_y + par_char_decoration->f_max_height);
    sglVertex2f(par_f_x + par_f_horiz_advance, par_f_y + par_char_decoration->f_max_height);
    sglVertex2f(par_f_x + par_f_horiz_advance, (par_f_y - (par_char_decoration->f_max_vert_advance)));
    sglEnd();

    glob_pr_sglStatemachine->b_reverse_color = loc_b_prev_reverse;

    sglIndexColori(loc_l_prev_color);
    oglxUpdateColor(loc_l_prev_color);
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    oglxEnable((SGLulong) GL_TEXTURE_2D);
#endif
    oglxEnable((SGLulong) GL_BLEND);
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_draw_character_decoration
  DESCRIPTION:
    Function shall draw the decoration of a character.
  PARAMETERS:
	par_char_decoration -> Decoration attributes
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_ui_index -> index of the outline to draw
    par_f_horiz_advance -> horizontal advance of the character
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_draw_character_decoration(sgl_char_decoration * par_char_decoration, SGLfloat par_f_x, SGLfloat par_f_y, SGLlong par_ui_index,
                                    SGLfloat par_f_horiz_advance)
{
    SGLbyte loc_b_decoration = par_char_decoration->b_displayed_lines[par_ui_index];
    SGLlong loc_l_prev_linewidth = glob_pr_sglStatemachine->l_linewidth_index;

    sglIndexLineWidthi(0);

    if ((loc_b_decoration & OGLX_UNDERLINE) == OGLX_UNDERLINE) {
        sglBegin(SGL_LINES);
        sglVertex2f(par_f_x, par_f_y + par_char_decoration->f_underline_position);
        sglVertex2f(par_f_x + par_f_horiz_advance, par_f_y + par_char_decoration->f_underline_position);
        sglEnd();
    }
    else {
        /* Nothing to do */
    }

    if ((loc_b_decoration & OGLX_CROSSED) == OGLX_CROSSED) {
        sglBegin(SGL_LINES);
        sglVertex2f(par_f_x, par_f_y + par_char_decoration->f_crossed_position);
        sglVertex2f(par_f_x + par_f_horiz_advance, par_f_y + par_char_decoration->f_crossed_position);
        sglEnd();
    }
    else {
        /* Nothing to do */
    }

    if ((loc_b_decoration & OGLX_TOP) == OGLX_TOP) {
        sglBegin(SGL_LINES);
        sglVertex2f(par_f_x, par_f_y + par_char_decoration->f_max_height);
        sglVertex2f(par_f_x + par_f_horiz_advance, par_f_y + par_char_decoration->f_max_height);
        sglEnd();
    }
    else {
        /* Nothing to do */
    }

    if ((loc_b_decoration & OGLX_BOTTOM) == OGLX_BOTTOM) {
        sglBegin(SGL_LINES);
        sglVertex2f(par_f_x, par_f_y - par_char_decoration->f_max_vert_advance);
        sglVertex2f(par_f_x + par_f_horiz_advance, (par_f_y - (par_char_decoration->f_max_vert_advance)));
        sglEnd();
    }
    else {
        /* Nothing to do */
    }

    if ((loc_b_decoration & OGLX_LEFT) == OGLX_LEFT) {
        sglBegin(SGL_LINES);
        sglVertex2f(par_f_x, par_f_y - par_char_decoration->f_max_vert_advance);
        sglVertex2f(par_f_x, par_f_y + par_char_decoration->f_max_height);
        sglEnd();
    }
    else {
        /* Nothing to do */
    }

    if ((loc_b_decoration & OGLX_RIGHT) == OGLX_RIGHT) {
        sglBegin(SGL_LINES);
        sglVertex2f(par_f_x + par_f_horiz_advance, (par_f_y - (par_char_decoration->f_max_vert_advance)));
        sglVertex2f(par_f_x + par_f_horiz_advance, par_f_y + par_char_decoration->f_max_height);
        sglEnd();
    }
    else {
        /* Nothing to do */
    }

    sglIndexLineWidthi(loc_l_prev_linewidth);

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_char_decoration
  DESCRIPTION:
    Function shall define the decoration of a character.
  PARAMETERS:
    par_rich_text_attr -> Rich text structure
	par_char_decoration -> Decoration attributes
	par_ui_string_length -> Global string length
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_char_decoration(sgl_rich_text_attr * par_rich_text_attr, sgl_char_decoration * par_char_decoration, SGLulong par_ui_string_length)
{
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_width;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_height;
    const glf_type_font *loc_p_font = par_rich_text_attr->p_current_font;
    const glf_type_character *loc_pr_char;
    SGLushort loc_us_offset;

    par_rich_text_attr->ui_last_char = (GLuint) par_rich_text_attr->ui_counter;
    par_char_decoration->b_displayed_lines[par_rich_text_attr->ui_counter] = par_rich_text_attr->b_activated_lines;
    par_char_decoration->l_back_color[par_rich_text_attr->ui_counter] = par_rich_text_attr->l_back_color;
    par_char_decoration->l_fore_color[par_rich_text_attr->ui_counter] = par_rich_text_attr->l_fore_color;

    if (par_rich_text_attr->b_frame_started == SGL_TRUE) {
        par_char_decoration->b_displayed_lines[par_rich_text_attr->ui_counter] = (SGLbyte) (par_rich_text_attr->b_activated_lines | OGLX_LEFT);
        par_rich_text_attr->b_frame_started = SGL_FALSE;
    }
    else {
        /* Nothing to do */
    }

    if (par_rich_text_attr->b_outline_started && (par_rich_text_attr->ui_counter == 1)) {
        par_char_decoration->b_displayed_lines[par_rich_text_attr->ui_last_char] =
            (SGLbyte) (par_char_decoration->b_displayed_lines[par_rich_text_attr->ui_last_char] | OGLX_RIGHT);
    }
    else {
        /* Nothing to do */
    }

    loc_us_offset = oglx_get_char_index(&(par_rich_text_attr->us_index), par_rich_text_attr->pb_string);
    par_rich_text_attr->pb_string += loc_us_offset;
    par_rich_text_attr->ui_counter -= (SGLlong) loc_us_offset;

    loc_pr_char = GLF_GET_CHAR(loc_p_font, par_rich_text_attr->us_index);

    if ((loc_p_font->f_underline_y * loc_f_ratio_y) > par_rich_text_attr->f_max_underline) {
        par_rich_text_attr->f_max_underline = (loc_p_font->f_underline_y * loc_f_ratio_y);
    }
    else {
        /* Nothing to do */
    }

    if ((loc_p_font->f_stroke_y * loc_f_ratio_y) < par_rich_text_attr->f_min_stroke) {
        par_rich_text_attr->f_min_stroke = loc_p_font->f_stroke_y * loc_f_ratio_y;
    }
    else {
        /* Nothing to do */
    }

    if (par_rich_text_attr->ui_counter == ((GLint) par_ui_string_length - 1L)) {
        /* last character */
        par_rich_text_attr->f_width += loc_pr_char->f_width * loc_f_ratio_x;
    }
    else {
        /* other character */
        par_rich_text_attr->f_width += loc_pr_char->f_horizontal_advance * loc_f_ratio_x;
    }

    if (par_rich_text_attr->f_height < (loc_p_font->f_max_char_height * loc_f_ratio_y)) {
        par_rich_text_attr->f_height = loc_p_font->f_max_char_height * loc_f_ratio_y;
    }
    else {
        /* nothing to do */
    }

    if (par_rich_text_attr->f_max_vert_advance < (loc_p_font->f_max_v_advance * loc_f_ratio_y)) {
        par_rich_text_attr->f_max_vert_advance = loc_p_font->f_max_v_advance * loc_f_ratio_y;
    }
    else {
        /* Nothing to do */
    }

    par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_change_font
  DESCRIPTION:
    Function shall change current font in rich text.
  PARAMETERS:
    par_rich_text_attr -> Rich text structure
	par_us_index -> Font index
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_change_font(sgl_rich_text_attr * par_rich_text_attr, SGLulong par_us_index)
{
    oglxSelectFont(par_us_index);
    par_rich_text_attr->ui_current_font_id = glob_pr_sglStatemachine->ui_current_font_id;
    par_rich_text_attr->p_current_font = glob_pr_sglStatemachine->p_current_font;

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_change_forecolor_deco
  DESCRIPTION:
    Function shall change foreground color for decoration.
  PARAMETERS:
    par_rich_text_attr -> Rich text structure
	par_us_index -> Color index
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_change_forecolor_deco(sgl_rich_text_attr * par_rich_text_attr, SGLulong par_us_index)
{
    if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
        if (par_us_index <= glob_pr_sglStatemachine->ul_nb_colors) {
            if (!(par_rich_text_attr->b_reverse_video)) {
                par_rich_text_attr->l_fore_color = (SGLlong) par_us_index;
            }
            else {
                par_rich_text_attr->l_back_color = (SGLlong) par_us_index;
            }
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        /* Nothing to do */
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_change_backcolor_deco
  DESCRIPTION:
    Function shall change background color for decoration.
  PARAMETERS:
    par_rich_text_attr -> Rich text structure
	par_us_index -> Color index
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_change_backcolor_deco(sgl_rich_text_attr * par_rich_text_attr, SGLulong par_us_index)
{
    if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
        if (par_us_index <= glob_pr_sglStatemachine->ul_nb_colors) {
            if (!(par_rich_text_attr->b_reverse_video)) {
                par_rich_text_attr->l_back_color = (SGLlong) par_us_index;
            }
            else {
                par_rich_text_attr->l_fore_color = (SGLlong) par_us_index;
            }
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        /* Nothing to do */
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_change_outline_deco
  DESCRIPTION:
    Function shall change the outline for decoration.
  PARAMETERS:
    par_rich_text_attr -> Rich text structure
	par_char_decoration -> Decoration attributes
	par_us_index -> Outline index
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_change_outline_deco(sgl_rich_text_attr * par_rich_text_attr, sgl_char_decoration * par_char_decoration, SGLulong par_us_index)
{
    if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
        if (par_rich_text_attr->b_outline_started) {
            par_char_decoration->b_displayed_lines[par_rich_text_attr->ui_last_char] =
                (SGLbyte) (par_char_decoration->b_displayed_lines[par_rich_text_attr->ui_last_char] | OGLX_RIGHT);
            par_rich_text_attr->b_outline_started = SGL_FALSE;
        }
        else {
            /* Nothing to do */
        }

        if ((par_us_index & OGLX_TOP) == OGLX_TOP) {
            par_rich_text_attr->b_activated_lines |= OGLX_TOP;
        }
        else {
            par_rich_text_attr->b_activated_lines &= ~((SGLbyte) OGLX_TOP);
        }

        if ((par_us_index & OGLX_BOTTOM) == OGLX_BOTTOM) {
            par_rich_text_attr->b_activated_lines |= OGLX_BOTTOM;
        }
        else {
            par_rich_text_attr->b_activated_lines &= ~((SGLbyte) OGLX_BOTTOM);
        }

        if ((par_us_index & OGLX_LEFT) == OGLX_LEFT) {
            par_rich_text_attr->b_frame_started = SGL_TRUE;
        }
        else {
            par_rich_text_attr->b_frame_started = SGL_FALSE;
        }

        if ((par_us_index & OGLX_RIGHT) == OGLX_RIGHT) {
            par_rich_text_attr->b_outline_started = SGL_TRUE;
        }
        else {
            par_rich_text_attr->b_outline_started = SGL_FALSE;
        }
    }
    else {
        /* Nothing to do */
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_define_characters_decoration
  DESCRIPTION:
    Function shall define the characters decoration.
  PARAMETERS:
    par_rich_text_attr -> Rich text structure
	par_char_decoration -> Decoration attributes
	par_f_x -> Horizontal origin of string
	par_pb_string -> Original string
	par_ui_string_length -> Global string length
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_define_characters_decoration(sgl_rich_text_attr * par_rich_text_attr, sgl_char_decoration * par_char_decoration, SGLfloat par_f_x,
                                       const SGLbyte * par_pb_string, SGLulong par_ui_string_length)
{
    SGLfloat loc_f_x = par_f_x;
    SGLfloat loc_f_tx;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_y * glob_pr_sglStatemachine->f_scale_y;
    SGLushort loc_us_repeat_number = 0;
    SGLulong loc_ui_repeat_counter = 0;
    const SGLbyte *loc_pb_repeat_string = SGL_NULL;
    SGLlong loc_l_prev_back_color = -1;
    SGLushort loc_us_offset;

    par_rich_text_attr->ui_current_font_id = glob_pr_sglStatemachine->ui_current_font_id;
    par_rich_text_attr->p_current_font = glob_pr_sglStatemachine->p_current_font;

    /* Set length */
    par_rich_text_attr->ui_counter = (GLint) par_ui_string_length;
    par_rich_text_attr->ui_index = 0U;
    (par_rich_text_attr->pb_string) = par_pb_string;
    par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));

    /* First cross of the string to prepare the characters decorations and dimensions */
    while (par_rich_text_attr->ui_counter > 0L) {
        par_char_decoration->b_displayed_lines[par_rich_text_attr->ui_counter] = 0U;
        par_char_decoration->l_back_color[par_rich_text_attr->ui_counter] = -1;
        par_char_decoration->l_fore_color[par_rich_text_attr->ui_counter] = glob_pr_sglStatemachine->l_color_index;

        switch (par_rich_text_attr->us_index) {
        case SGL_K_ESC:
        {
            par_rich_text_attr->ui_counter--;
            (par_rich_text_attr->pb_string)++;
            par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));

            switch (par_rich_text_attr->us_index) {
            case SGL_K_FONT:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                loc_us_offset = oglx_get_char_index(&(par_rich_text_attr->us_index), par_rich_text_attr->pb_string);
                par_rich_text_attr->pb_string += loc_us_offset;
                par_rich_text_attr->ui_counter -= (SGLlong) loc_us_offset;
                oglx_change_font(par_rich_text_attr, par_rich_text_attr->us_index);
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_FORECOLOR:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                loc_us_offset = oglx_get_char_index(&(par_rich_text_attr->us_index), par_rich_text_attr->pb_string);
                par_rich_text_attr->pb_string += loc_us_offset;
                par_rich_text_attr->ui_counter -= (SGLlong) loc_us_offset;
                oglx_change_forecolor_deco(par_rich_text_attr, par_rich_text_attr->us_index);
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;

            case SGL_K_BACKCOLOR:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                loc_us_offset = oglx_get_char_index(&(par_rich_text_attr->us_index), par_rich_text_attr->pb_string);
                par_rich_text_attr->pb_string += loc_us_offset;
                par_rich_text_attr->ui_counter -= (SGLlong) loc_us_offset;
                oglx_change_backcolor_deco(par_rich_text_attr, par_rich_text_attr->us_index);
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;

            case SGL_K_UNDERLINE_BEG:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
                    par_rich_text_attr->b_activated_lines |= OGLX_UNDERLINE;
                }
                else {
                    /* Nothing to do */
                }
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;

            case SGL_K_UNDERLINE_END:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
                    par_rich_text_attr->b_activated_lines &= ~((SGLbyte) OGLX_UNDERLINE);
                }
                else {
                    /* Nothing to do */
                }
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;

            case SGL_K_CROSSED_BEG:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
                    par_rich_text_attr->b_activated_lines |= OGLX_CROSSED;
                }
                else {
                    /* Nothing to do */
                }
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;

            case SGL_K_CROSSED_END:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
                    par_rich_text_attr->b_activated_lines &= ~((SGLbyte) OGLX_CROSSED);
                }
                else {
                    /* Nothing to do */
                }
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_FRAMED_BEG:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
                    par_rich_text_attr->b_activated_lines |= OGLX_TOP;
                    par_rich_text_attr->b_activated_lines |= OGLX_BOTTOM;
                    par_rich_text_attr->b_frame_started = SGL_TRUE;
                }
                else {
                    /* Nothing to do */
                }
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_FRAMED_END:
            {
                SGLbyte loc_b_prev_lines = par_char_decoration->b_displayed_lines[par_rich_text_attr->ui_last_char];
                if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
                    par_rich_text_attr->b_activated_lines &= ~((SGLbyte) OGLX_TOP);
                    par_rich_text_attr->b_activated_lines &= ~((SGLbyte) OGLX_BOTTOM);
                    par_rich_text_attr->b_activated_lines &= ~((SGLbyte) OGLX_LEFT);
                    par_rich_text_attr->b_activated_lines &= ~((SGLbyte) OGLX_RIGHT);
                    par_char_decoration->b_displayed_lines[par_rich_text_attr->ui_last_char] = (SGLbyte) (loc_b_prev_lines | OGLX_RIGHT);
                }
                else {
                    /* Nothing to do */
                }
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_OUTLINE:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
                oglx_change_outline_deco(par_rich_text_attr, par_char_decoration, par_rich_text_attr->us_index);
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_VIDEOINV_BEG:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
                    par_rich_text_attr->b_reverse_video = SGL_TRUE;
                    loc_l_prev_back_color = par_rich_text_attr->l_back_color;
                    par_rich_text_attr->l_back_color = par_rich_text_attr->l_fore_color;
                    par_rich_text_attr->l_fore_color = loc_l_prev_back_color;
                }
                else {
                    /* Nothing to do */
                }
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_VIDEOINV_END:
            {
                SGLlong loc_l_tmp;
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
                    par_rich_text_attr->b_reverse_video = SGL_FALSE;
                    loc_l_tmp = par_rich_text_attr->l_back_color;
                    par_rich_text_attr->l_back_color = loc_l_prev_back_color;
                    par_rich_text_attr->l_fore_color = loc_l_tmp;
                }
                else {
                    /* Nothing to do */
                }
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_REPEAT_BEG:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                loc_us_offset = oglx_get_char_index(&(par_rich_text_attr->us_index), par_rich_text_attr->pb_string);
                par_rich_text_attr->pb_string += loc_us_offset;
                par_rich_text_attr->ui_counter -= (SGLlong) loc_us_offset;

                if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
                    loc_us_repeat_number = par_rich_text_attr->us_index;
                    loc_pb_repeat_string = (par_rich_text_attr->pb_string);
                    loc_ui_repeat_counter = (SGLulong) (par_rich_text_attr->ui_counter);
                }
                else {
                    /* Nothing to do */
                }
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_REPEAT_END:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                if ((glob_pr_sglStatemachine->b_enable_esc_sequence)
                    && (loc_us_repeat_number > 0U)) {
                    loc_us_repeat_number--;
                    par_rich_text_attr->ui_counter = (GLint) loc_ui_repeat_counter;
                    (par_rich_text_attr->pb_string) = loc_pb_repeat_string;
                }
                else {
                    /* Nothing to do */
                }
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            default:
                /*
                   case SGL_K_BOLD_BEG:
                   case SGL_K_BOLD_END:
                 */
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            }

        }
            break;
        case SGL_K_CR:
        {
            par_rich_text_attr->ui_counter--;
            (par_rich_text_attr->pb_string)++;
            loc_f_tx = -glob_pr_sglStatemachine->f_h_alignment * par_rich_text_attr->f_width;

            if (par_rich_text_attr->us_line_number < SGL_MAX_STRING_LINES) {
                glob_t_line_origin[par_rich_text_attr->us_line_number] = loc_f_x + loc_f_tx;
                par_rich_text_attr->us_line_number++;
            }
            else {
                oglxSetError(SGL_ERROR_SGLU_WRITERICHTEXT, 0U);
            }

            par_rich_text_attr->f_width = 0.0F;

            par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
        }
            break;
        case SGL_K_LF:
        {
            par_rich_text_attr->ui_counter--;
            (par_rich_text_attr->pb_string)++;
            par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
        }
            break;
        default:
        {
            oglx_char_decoration(par_rich_text_attr, par_char_decoration, par_ui_string_length);
        }
            break;
        }
    }

    par_char_decoration->f_crossed_position = par_rich_text_attr->f_min_stroke;
    par_char_decoration->f_underline_position = par_rich_text_attr->f_max_underline;
    par_char_decoration->f_max_height = par_rich_text_attr->f_height + (2.0F * loc_f_ratio_y);
    par_char_decoration->f_max_vert_advance = par_rich_text_attr->f_max_vert_advance;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_draw_character
  DESCRIPTION:
    Function shall draw a single character.
  PARAMETERS:
    par_rich_text_attr -> Rich text structure
	par_char_decoration -> Decoration attributes
	par_f_x -> Pointer to horizontal origin of character
	par_f_y -> Vertical origin of character
	par_ui_font_linewidth -> Current font line width
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_draw_character(sgl_rich_text_attr * par_rich_text_attr, sgl_char_decoration * par_char_decoration, SGLfloat * par_f_x, SGLfloat par_f_y,
                         SGLulong par_ui_font_linewidth)
{
    SGLfloat loc_f_x = *par_f_x;
    SGLfloat loc_f_y = par_f_y;
    SGLfloat loc_f_prev_x = *par_f_x;
    SGLfloat loc_f_prev_y = par_f_y;
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_width;
    SGLfloat loc_f_ratio_y = glob_pr_sglStatemachine->f_ratio_scale_height;
    const glf_type_font *loc_p_font = par_rich_text_attr->p_current_font;
    const glf_type_character *loc_pr_char;
    SGLfloat loc_f_tx;
    SGLushort loc_us_offset;

    par_rich_text_attr->b_cr_started = SGL_FALSE;

    oglxUpdateColor(glob_pr_sglStatemachine->l_color_index);

    loc_us_offset = oglx_get_char_index(&(par_rich_text_attr->us_index), par_rich_text_attr->pb_string);
    par_rich_text_attr->pb_string += loc_us_offset;
    par_rich_text_attr->ui_counter -= (SGLlong) loc_us_offset;

    loc_pr_char = GLF_GET_CHAR(loc_p_font, par_rich_text_attr->us_index);

    if (loc_p_font->us_font_kind == GLF_FONT_TYPE_BITMAP) {
        const glf_type_character_bitmap *loc_pr_char_bitmap = GLF_GET_BITMAP_CHAR(loc_pr_char);

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        oglxEnable((SGLulong) GL_TEXTURE_2D);
#endif
        oglxEnable((SGLulong) GL_BLEND);

        if (par_char_decoration->l_back_color[par_rich_text_attr->ui_counter + (SGLlong) loc_us_offset] != -1) {
            oglx_draw_character_background(par_char_decoration, loc_f_x, loc_f_y, par_rich_text_attr->ui_counter + (SGLlong) loc_us_offset,
                                           loc_pr_char->f_horizontal_advance * loc_f_ratio_x);
        }
        else {
            /* Nothing to do */
        }

        oglx_fonts_draw_bitmap_character(loc_p_font, loc_pr_char_bitmap, loc_f_x, loc_f_y, par_ui_font_linewidth, loc_f_ratio_x, loc_f_ratio_y);

        if (b_bold_text) {
            oglx_fonts_draw_bitmap_character(loc_p_font, loc_pr_char_bitmap, (loc_f_x - BOLD_WIDTH), (loc_f_y - BOLD_WIDTH), par_ui_font_linewidth,
                                             loc_f_ratio_x, loc_f_ratio_y);
            oglx_fonts_draw_bitmap_character(loc_p_font, loc_pr_char_bitmap, (loc_f_x - BOLD_WIDTH), (loc_f_y + BOLD_WIDTH), par_ui_font_linewidth,
                                             loc_f_ratio_x, loc_f_ratio_y);
            oglx_fonts_draw_bitmap_character(loc_p_font, loc_pr_char_bitmap, (loc_f_x + BOLD_WIDTH), (loc_f_y + BOLD_WIDTH), par_ui_font_linewidth,
                                             loc_f_ratio_x, loc_f_ratio_y);
            oglx_fonts_draw_bitmap_character(loc_p_font, loc_pr_char_bitmap, (loc_f_x + BOLD_WIDTH), (loc_f_y - BOLD_WIDTH), par_ui_font_linewidth,
                                             loc_f_ratio_x, loc_f_ratio_y);
        }
        else {
            /* Nothing to do */
        }

        loc_f_tx = (loc_pr_char_bitmap->tr_type_character.f_horizontal_advance * loc_f_ratio_x);

        loc_f_x += loc_f_tx;
        *par_f_x = loc_f_x;
        par_rich_text_attr->ui_index++;
    }
    else {
        const glf_type_character_stroke *loc_pr_char_stroke = GLF_GET_STROKE_CHAR(loc_pr_char);

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        oglxDisable((SGLulong) GL_TEXTURE_2D);
#endif
        oglxDisable((SGLulong) GL_BLEND);

        if (par_char_decoration->l_back_color[par_rich_text_attr->ui_counter + (SGLlong) loc_us_offset] != -1) {
            oglx_draw_character_background(par_char_decoration, loc_f_x, loc_f_y, par_rich_text_attr->ui_counter + (SGLlong) loc_us_offset,
                                           loc_pr_char->f_horizontal_advance * loc_f_ratio_x);
        }
        else {
            /* Nothing to do */
        }


        oglx_fonts_draw_stroke_character(loc_p_font, loc_pr_char_stroke, loc_f_x, loc_f_y, loc_f_ratio_x, loc_f_ratio_y,
                                         (GLuint) (glob_pr_sglStatemachine->l_current_font_linewidth_index),
                                         (GLuint) (glob_pr_sglStatemachine->l_current_font_color_index), SGL_FALSE);
        if (b_bold_text) {
            oglx_fonts_draw_stroke_character(loc_p_font, loc_pr_char_stroke, (loc_f_x - BOLD_WIDTH), (loc_f_y - BOLD_WIDTH), loc_f_ratio_x, loc_f_ratio_y,
                                             (GLuint) (glob_pr_sglStatemachine->l_current_font_linewidth_index),
                                             (GLuint) (glob_pr_sglStatemachine->l_current_font_color_index), SGL_FALSE);
            oglx_fonts_draw_stroke_character(loc_p_font, loc_pr_char_stroke, (loc_f_x - BOLD_WIDTH), (loc_f_y + BOLD_WIDTH), loc_f_ratio_x, loc_f_ratio_y,
                                             (GLuint) (glob_pr_sglStatemachine->l_current_font_linewidth_index),
                                             (GLuint) (glob_pr_sglStatemachine->l_current_font_color_index), SGL_FALSE);
            oglx_fonts_draw_stroke_character(loc_p_font, loc_pr_char_stroke, (loc_f_x + BOLD_WIDTH), (loc_f_y + BOLD_WIDTH), loc_f_ratio_x, loc_f_ratio_y,
                                             (GLuint) (glob_pr_sglStatemachine->l_current_font_linewidth_index),
                                             (GLuint) (glob_pr_sglStatemachine->l_current_font_color_index), SGL_FALSE);
            oglx_fonts_draw_stroke_character(loc_p_font, loc_pr_char_stroke, (loc_f_x + BOLD_WIDTH), (loc_f_y - BOLD_WIDTH), loc_f_ratio_x, loc_f_ratio_y,
                                             (GLuint) (glob_pr_sglStatemachine->l_current_font_linewidth_index),
                                             (GLuint) (glob_pr_sglStatemachine->l_current_font_color_index), SGL_FALSE);
        }
        else {
            /* Nothing to do */
        }

        loc_f_tx = (loc_pr_char_stroke->tr_type_character.f_horizontal_advance * loc_f_ratio_x);

        loc_f_x += loc_f_tx;
        *par_f_x = loc_f_x;
        par_rich_text_attr->ui_index++;

    }
    par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));

    if (par_char_decoration->b_displayed_lines[par_rich_text_attr->ui_counter + (SGLlong) loc_us_offset] != 0U) {
        oglx_draw_character_decoration(par_char_decoration, loc_f_prev_x, loc_f_prev_y, par_rich_text_attr->ui_counter + (SGLlong) loc_us_offset, loc_f_tx);
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_change_fore_color
  DESCRIPTION:
    Function shall change foreground color for drawing.
  PARAMETERS:
    par_rich_text_attr -> Rich text structure
	par_us_index -> Foreground color index
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_change_fore_color(sgl_rich_text_attr * par_rich_text_attr, SGLulong par_us_index)
{
    if (glob_pr_sglStatemachine->b_enable_esc_sequence && (par_us_index <= glob_pr_sglStatemachine->ul_nb_colors)) {
        par_rich_text_attr->l_rev_color = (SGLlong) par_us_index;
        if (!(par_rich_text_attr->b_reverse_video)) {
            sglIndexColori((SGLlong) par_us_index);
            oglxUpdateColor((SGLlong) par_us_index);
        }
        else {
            sglIndexColori(par_rich_text_attr->l_back_color);
            oglxUpdateColor(par_rich_text_attr->l_back_color);
        }
    }
    else {
        /* Nothing to do */
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_video_inv
  DESCRIPTION:
    Function shall activate reverse video.
  PARAMETERS:
    par_rich_text_attr -> Rich text structure
	par_char_decoration -> Decoration attributes
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_video_inv(sgl_rich_text_attr * par_rich_text_attr, sgl_char_decoration * par_char_decoration)
{
    SGLlong loc_l_newcolor = par_char_decoration->l_fore_color[par_rich_text_attr->ui_counter - 1];
    par_rich_text_attr->l_rev_color = glob_pr_sglStatemachine->l_color_index;
    if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
        par_rich_text_attr->b_reverse_video = SGL_TRUE;
        if (loc_l_newcolor != -1) {
            sglIndexColori(loc_l_newcolor);
        }
        else {
            sglIndexColori(par_rich_text_attr->l_rev_color);
            glob_pr_sglStatemachine->b_reverse_color = SGL_TRUE;
        }
    }
    else {
        /* Nothing to do */
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_draw_rich_text
  DESCRIPTION:
    Function shall draw a rich text.
  PARAMETERS:
    par_rich_text_attr -> Rich text structure
	par_char_decoration -> Decoration attributes
	par_f_x -> X origin of text
	par_f_y -> Y origin of text
	par_pb_string -> Original string
	par_ui_string_length -> Global string length
	par_ui_font_linewidth -> Current font line width
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_draw_rich_text(sgl_rich_text_attr * par_rich_text_attr, sgl_char_decoration * par_char_decoration, SGLfloat par_f_x, SGLfloat par_f_y,
                         const SGLbyte * par_pb_string, SGLulong par_ui_string_length, SGLulong par_ui_font_linewidth)
{
    SGLfloat loc_f_x = par_f_x;
    SGLfloat loc_f_y = par_f_y;
    SGLushort loc_us_repeat_number = 0;
    SGLulong loc_ui_repeat_counter = 0;
    const SGLbyte *loc_pb_repeat_string = SGL_NULL;
    SGLushort loc_us_offset;
    SGLfloat loc_f_tx;
    SGLfloat loc_f_ty;

    par_rich_text_attr->ui_counter = (GLint) par_ui_string_length;
    (par_rich_text_attr->pb_string) = par_pb_string;
    par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));

    loc_f_tx = -glob_pr_sglStatemachine->f_h_alignment * par_rich_text_attr->f_width;
    if (par_rich_text_attr->us_line_number < SGL_MAX_STRING_LINES) {
        glob_t_line_origin[par_rich_text_attr->us_line_number] = loc_f_x + loc_f_tx;
    }
    else {
        glob_t_line_origin[SGL_MAX_STRING_LINES - 1UL] = loc_f_x + loc_f_tx;
    }

    if (par_rich_text_attr->us_line_number == 0U) {
        loc_f_ty = -(glob_pr_sglStatemachine->f_v_alignment * par_rich_text_attr->f_height);
    }
    else {
        loc_f_ty = -(glob_pr_sglStatemachine->f_v_alignment * par_rich_text_attr->f_height)
            + (((1.0F - glob_pr_sglStatemachine->f_v_alignment) * (par_rich_text_attr->f_height +
                                                                   par_char_decoration->f_max_vert_advance)) * (SGLfloat) (par_rich_text_attr->us_line_number));
    }
    loc_f_y += loc_f_ty;
    /* Set the position */

    if (par_rich_text_attr->us_line_number != 0U) {
        par_rich_text_attr->us_line_number = 0U;
    }
    else {
        /* Nothing to do */
    }

    loc_f_x = glob_t_line_origin[par_rich_text_attr->us_line_number];

    /* Change the coordinate of the first line to match exactly one pixel */
    oglxGetEntirePixelCoordinate(loc_f_x, loc_f_y, &loc_f_x, &loc_f_y);

    /* One line is started */
    par_rich_text_attr->us_line_number++;

    par_rich_text_attr->l_back_color = (SGLlong) (glob_pr_sglStatemachine->ul_nb_colors - 1UL);

    oglx_change_font(par_rich_text_attr, par_rich_text_attr->ui_param_font_id);

    /* Second cross of the string to draw each character */
    while (par_rich_text_attr->ui_counter > 0) {
        switch (par_rich_text_attr->us_index) {
        case SGL_K_ESC:
        {
            par_rich_text_attr->ui_counter--;
            (par_rich_text_attr->pb_string)++;
            par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));

            switch (par_rich_text_attr->us_index) {
            case SGL_K_FONT:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                loc_us_offset = oglx_get_char_index(&(par_rich_text_attr->us_index), par_rich_text_attr->pb_string);
                par_rich_text_attr->pb_string += loc_us_offset;
                par_rich_text_attr->ui_counter -= (SGLlong) loc_us_offset;
                oglx_change_font(par_rich_text_attr, par_rich_text_attr->us_index);
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_FORECOLOR:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                loc_us_offset = oglx_get_char_index(&(par_rich_text_attr->us_index), par_rich_text_attr->pb_string);
                par_rich_text_attr->pb_string += loc_us_offset;
                par_rich_text_attr->ui_counter -= (SGLlong) loc_us_offset;
                oglx_change_fore_color(par_rich_text_attr, par_rich_text_attr->us_index);
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;

            case SGL_K_BACKCOLOR:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                loc_us_offset = oglx_get_char_index(&(par_rich_text_attr->us_index), par_rich_text_attr->pb_string);
                par_rich_text_attr->pb_string += loc_us_offset;
                par_rich_text_attr->ui_counter -= (SGLlong) loc_us_offset;

                if (glob_pr_sglStatemachine->b_enable_esc_sequence && (par_rich_text_attr->us_index <= glob_pr_sglStatemachine->ul_nb_colors)) {
                    par_rich_text_attr->l_back_color = (SGLlong) (par_rich_text_attr->us_index);
                }
                else {
                    /* Nothing to do */
                }

                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_VIDEOINV_BEG:
            {
                oglx_video_inv(par_rich_text_attr, par_char_decoration);
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_VIDEOINV_END:
            {
                par_rich_text_attr->b_reverse_video = SGL_FALSE;
                glob_pr_sglStatemachine->b_reverse_color = SGL_FALSE;
                sglIndexColori(par_rich_text_attr->l_rev_color);
                oglxUpdateColor(par_rich_text_attr->l_rev_color);

                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_OUTLINE:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_BOLD_BEG:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
                    b_bold_text = SGL_TRUE;
                }
                else {
                    /* Nothing to do */
                }
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_BOLD_END:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
                    b_bold_text = SGL_FALSE;
                }
                else {
                    /* Nothing to do */
                }
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_REPEAT_BEG:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;

                loc_us_offset = oglx_get_char_index(&(par_rich_text_attr->us_index), par_rich_text_attr->pb_string);
                par_rich_text_attr->pb_string += loc_us_offset;
                par_rich_text_attr->ui_counter -= (SGLlong) loc_us_offset;
                if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
                    loc_us_repeat_number = par_rich_text_attr->us_index;
                    loc_pb_repeat_string = (par_rich_text_attr->pb_string);
                    loc_ui_repeat_counter = (SGLulong) (par_rich_text_attr->ui_counter);
                }
                else {
                    /* Nothing to do */
                }

                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            case SGL_K_REPEAT_END:
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;

                if (glob_pr_sglStatemachine->b_enable_esc_sequence && (loc_us_repeat_number > 0U)) {
                    loc_us_repeat_number--;
                    par_rich_text_attr->ui_counter = (GLint) loc_ui_repeat_counter;
                    (par_rich_text_attr->pb_string) = loc_pb_repeat_string;
                }
                else {
                    /* Nothing to do */
                }

                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            default:
                /*
                   case SGL_K_UNDERLINE_BEG:
                   case SGL_K_UNDERLINE_END:
                   case SGL_K_CROSSED_BEG:
                   case SGL_K_CROSSED_END:
                   case SGL_K_FRAMED_BEG:
                   case SGL_K_FRAMED_END:
                 */
            {
                par_rich_text_attr->ui_counter--;
                (par_rich_text_attr->pb_string)++;
                par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
            }
                break;
            }

        }
            break;
        case SGL_K_CR:
        {
            par_rich_text_attr->ui_counter--;
            (par_rich_text_attr->pb_string)++;
            if (par_rich_text_attr->us_line_number < SGL_MAX_STRING_LINES) {
                loc_f_x = glob_t_line_origin[par_rich_text_attr->us_line_number];
                par_rich_text_attr->us_line_number++;
            }
            else {
                loc_f_x = glob_t_line_origin[SGL_MAX_STRING_LINES - 1UL];
            }
            loc_f_y -= (par_rich_text_attr->f_height + par_rich_text_attr->f_max_vert_advance);
            /* Adjust text position on text on a new line */
            oglxGetEntirePixelCoordinate(loc_f_x, loc_f_y, &loc_f_x, &loc_f_y);
            par_rich_text_attr->b_cr_started = SGL_TRUE;
            par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
        }
            break;
        case SGL_K_LF:
        {
            par_rich_text_attr->ui_counter--;
            (par_rich_text_attr->pb_string)++;
            if (!(par_rich_text_attr->b_cr_started)) {
                loc_f_y -= (par_rich_text_attr->f_height + par_rich_text_attr->f_max_vert_advance);
            }
            else {
                /* Nothing to do */
            }
            /* Adjust text position on text on a new line */
            oglxGetEntirePixelCoordinate(loc_f_x, loc_f_y, &loc_f_x, &loc_f_y);
            par_rich_text_attr->us_index = (GLushort) (*(par_rich_text_attr->pb_string));
        }
            break;
        default:
        {
            oglx_draw_character(par_rich_text_attr, par_char_decoration, &loc_f_x, loc_f_y, par_ui_font_linewidth);
        }
            break;
        }
    }

    return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_write_attributed_rich_text
  DESCRIPTION:
    Function shall get and draw each character of a string.
  PARAMETERS:
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_pb_string -> string to display
    par_ui_string_length -> length of the string
    par_ui_font_linewidth -> font linewidth
    par_pf_font_color -> font color
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_write_attributed_rich_text(SGLfloat par_f_x, SGLfloat par_f_y, const SGLbyte * par_pb_string, SGLulong par_ui_string_length,
                                     SGLulong par_ui_font_linewidth, const SGLfloat * par_pf_font_color)
{
    SGLbool loc_b_stipple_ok_backup = glob_pr_sglStatemachine->b_stipple_ok;
    SGLlong loc_l_i;
    SGLulong loc_ui_initial_font_id = glob_pr_sglStatemachine->ui_current_font_id;

    loc_char_decoration.f_crossed_position = 0.0F;
    loc_char_decoration.f_underline_position = 0.0F;
    loc_char_decoration.f_max_height = 0.0F;
    loc_char_decoration.f_max_vert_advance = 0.0F;

    for (loc_l_i = 0; loc_l_i < (SGLlong) (SGLU_MAX_STRING_LENGTH + 1U); loc_l_i++) {
        loc_char_decoration.b_displayed_lines[loc_l_i] = 0U;
        loc_char_decoration.l_back_color[loc_l_i] = 0;
        loc_char_decoration.l_fore_color[loc_l_i] = 0;
    }

    loc_rich_text_attr.f_width = 0.0F;
    loc_rich_text_attr.f_height = 0.0F;
    loc_rich_text_attr.ui_counter = 0;
    loc_rich_text_attr.ui_index = 0UL;
    loc_rich_text_attr.l_back_color = -1;
    loc_rich_text_attr.l_fore_color = glob_pr_sglStatemachine->l_color_index;
    loc_rich_text_attr.b_activated_lines = 0U;
    loc_rich_text_attr.ui_last_char = 0U;
    loc_rich_text_attr.b_frame_started = SGL_FALSE;
    loc_rich_text_attr.b_outline_started = SGL_FALSE;
    loc_rich_text_attr.us_index = 0U;
    loc_rich_text_attr.pb_string = SGL_NULL;
    loc_rich_text_attr.f_max_underline = -100.0F;
    loc_rich_text_attr.f_min_stroke = 100.0F;
    loc_rich_text_attr.f_max_vert_advance = 0.0F;
    loc_rich_text_attr.ui_param_font_id = 0U;
    loc_rich_text_attr.b_reverse_video = SGL_FALSE;
    loc_rich_text_attr.us_line_number = 0U;
    loc_rich_text_attr.b_cr_started = SGL_FALSE;
    loc_rich_text_attr.l_rev_color = -1;

    loc_rich_text_attr.ui_current_font_id = glob_pr_sglStatemachine->ui_current_font_id;
    loc_rich_text_attr.p_current_font = glob_pr_sglStatemachine->p_current_font;

    /* Set length */
    loc_rich_text_attr.ui_counter = (GLint) par_ui_string_length;
    loc_rich_text_attr.pb_string = par_pb_string;
    loc_rich_text_attr.us_index = (GLushort) (*loc_rich_text_attr.pb_string);

    glob_pr_sglStatemachine->b_stipple_ok = SGL_FALSE;

    oglxColor4fv(par_pf_font_color);

    /* Initialize default rich text appearance */
    if (glob_pr_sglStatemachine->b_enable_esc_sequence) {
        if (glob_pr_sglStatemachine->l_foreground_color_id >= 0) {
            loc_rich_text_attr.l_fore_color = glob_pr_sglStatemachine->l_foreground_color_id;
            sglIndexColori(loc_rich_text_attr.l_fore_color);
            oglxUpdateColor(loc_rich_text_attr.l_fore_color);
        }
        else {
            /* Nothing to do */
        }

        if (glob_pr_sglStatemachine->l_background_color_id >= 0) {
            loc_rich_text_attr.l_back_color = glob_pr_sglStatemachine->l_background_color_id;
        }
        else {
            /* Nothing to do */
        }

        if ((glob_pr_sglStatemachine->l_outline_value >= 0) && (glob_pr_sglStatemachine->l_outline_value < 256)) {
            loc_rich_text_attr.b_activated_lines = (SGLbyte) (((SGLbyte) (glob_pr_sglStatemachine->l_outline_value)) & (SGLbyte) 0xF);

            if ((loc_rich_text_attr.b_activated_lines & OGLX_LEFT) == OGLX_LEFT) {
                loc_rich_text_attr.b_activated_lines &= ~((SGLbyte) OGLX_LEFT);
                loc_rich_text_attr.b_frame_started = SGL_TRUE;
            }
            else {
                loc_rich_text_attr.b_frame_started = SGL_FALSE;
            }

            if ((loc_rich_text_attr.b_activated_lines & OGLX_RIGHT) == OGLX_RIGHT) {
                loc_rich_text_attr.b_activated_lines &= ~((SGLbyte) OGLX_RIGHT);
                loc_rich_text_attr.b_outline_started = SGL_TRUE;
            }
            else {
                loc_rich_text_attr.b_outline_started = SGL_FALSE;
            }
        }
        else {
            /* Nothing to do */
        }

        if ((glob_pr_sglStatemachine->l_font_id >= 0) && (glob_pr_sglStatemachine->ui_current_font_id != INVALID_FONT_ID)) {
            oglx_change_font(&loc_rich_text_attr, (SGLulong) (glob_pr_sglStatemachine->l_font_id));
            loc_rich_text_attr.ui_param_font_id = loc_rich_text_attr.ui_current_font_id;
            loc_ui_initial_font_id = loc_rich_text_attr.ui_param_font_id;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        /* Nothing to do */
    }

    oglx_define_characters_decoration(&loc_rich_text_attr, &loc_char_decoration, par_f_x, par_pb_string, par_ui_string_length);

    /* Add missing right line if necessary */
    if (loc_rich_text_attr.b_outline_started) {
        loc_char_decoration.b_displayed_lines[loc_rich_text_attr.ui_last_char] =
            (SGLbyte) (loc_char_decoration.b_displayed_lines[loc_rich_text_attr.ui_last_char] | OGLX_RIGHT);
    }
    else {
        /* Nothing to do */
    }

    loc_rich_text_attr.ui_param_font_id = loc_ui_initial_font_id;
    oglx_draw_rich_text(&loc_rich_text_attr, &loc_char_decoration, par_f_x, par_f_y, par_pb_string, par_ui_string_length, par_ui_font_linewidth);


    if (loc_b_stipple_ok_backup) {
        glob_pr_sglStatemachine->b_stipple_ok = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* In case bold text is not closed in input, it shall be closed */
    b_bold_text = SGL_FALSE;
    glob_pr_sglStatemachine->b_reverse_color = SGL_FALSE;

    return;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxWriteAttributedComplexText
  DESCRIPTION:
    Function shall prepare the drawing of a string.
  PARAMETERS:
    par_f_x -> x coordinate of the reference point
    par_f_y -> y coordinate of the reference point
    par_ui_font_identifier -> font identifier
    par_pb_string -> string to display
    par_ui_string_length -> length of the string
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxWriteAttributedComplexText(SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_font_identifier, const SGLbyte * par_pb_string,
                                    SGLulong par_ui_string_length)
{
    const SGLbyte *loc_pb_string;
    SGLulong loc_ui_param_length = par_ui_string_length;
    SGLbool loc_b_end_of_string = SGL_FALSE;
    SGLulong loc_par_ui_string_length = 0U;
    loc_pb_string = par_pb_string;


    while ((!loc_b_end_of_string) && (loc_par_ui_string_length < loc_ui_param_length)) {
        if (loc_pb_string[loc_par_ui_string_length] == 0U) {
            if (loc_par_ui_string_length >= 2U) {
                if (((((SGLbyte) (loc_pb_string[loc_par_ui_string_length - 1UL])) == SGL_K_FONT)
                     || (((SGLbyte) (loc_pb_string[loc_par_ui_string_length - 1UL])) == SGL_K_FORECOLOR)
                     || (((SGLbyte) (loc_pb_string[loc_par_ui_string_length - 1UL])) == SGL_K_BACKCOLOR)
                     || (((SGLbyte) (loc_pb_string[loc_par_ui_string_length - 1UL])) == SGL_K_REPEAT_BEG))
                    && (((SGLbyte) (loc_pb_string[loc_par_ui_string_length - 2UL])) == SGL_K_ESC)) {
                    loc_par_ui_string_length++;
                }
                else {
                    loc_b_end_of_string = SGL_TRUE;
                }
            }
            else {
                loc_b_end_of_string = SGL_TRUE;
            }
        }
        else {
            loc_par_ui_string_length++;
        }
    }

    /* Reduce the string if it is longer than SGLU_MAX_STRING_LENGTH */
    if (loc_par_ui_string_length > SGLU_MAX_STRING_LENGTH) {
        loc_par_ui_string_length = SGLU_MAX_STRING_LENGTH;
    }
    else {
        /* Nothing to do */
    }

    if (loc_par_ui_string_length > 0U) {
        SGLfloat loc_tf_font_color[4];

        loc_tf_font_color[0] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_current_font_color_index].f_red;
        loc_tf_font_color[1] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_current_font_color_index].f_green;
        loc_tf_font_color[2] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_current_font_color_index].f_blue;
        loc_tf_font_color[3] = glob_pr_sglStatemachine->tr_color[glob_pr_sglStatemachine->l_current_font_color_index].f_alpha;

        oglxSelectFont(par_ui_font_identifier);

        if (glob_pr_sglStatemachine->b_haloing_state != (SGLbyte) SGL_FALSE) {
            glob_pr_sglStatemachine->b_haloing_state = SGL_COLORED_PART_ONLY;
        }
        else {
            /* Nothing to do */
        }
        oglx_write_attributed_rich_text(par_f_x, par_f_y, par_pb_string, loc_par_ui_string_length, glob_pr_sglStatemachine->b_current_font_linewidth,
                                        loc_tf_font_color);

        glob_pr_sglStatemachine->tr_geometry.l_size = 0L;
    }
    else {
        /* Nothing to do. String length is zero. */
    }
    return;
}



/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxWriteRichText
  DESCRIPTION:
    Function shall write text string.
  PARAMETERS:
    par_f_x -> x-coordinate of the reference point
    par_f_y -> y-coordinate of the reference point
    par_pb_string -> string to be drawn
    par_l_string_length -> string length
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxWriteRichText(SGLfloat par_f_x, SGLfloat par_f_y, const SGLbyte * par_pb_string, SGLulong par_l_string_length)
{
    SGLulong loc_font_id = glob_pr_sglStatemachine->ul_font_id;
    SGLbyte loc_b_haloing_state_backup = glob_pr_sglStatemachine->b_haloing_state;
    SGLlong loc_l_linewidth_index_backup = glob_pr_sglStatemachine->l_linewidth_index;
    SGLlong loc_l_color_index_backup = glob_pr_sglStatemachine->l_color_index;
    SGLbyte loc_b_transparency_backup = glob_pr_sglStatemachine->b_transparency;
    SGLbool loc_b_texture_state_backup = glob_pr_sglStatemachine->b_texture_state;

    sglTransparency8((SGLlong) 255U);
    sglDisable(SGL_TEXTURE_2D);

    oglxWriteAttributedComplexText((GLfloat) par_f_x, (GLfloat) par_f_y, (GLuint) loc_font_id, par_pb_string, (GLuint) par_l_string_length);

    /* Restore settings changed by the GLF */
    oglxUpdateColor(loc_l_color_index_backup);
    sglIndexLineWidthi(loc_l_linewidth_index_backup);

    if (loc_b_haloing_state_backup != (SGLbyte) SGL_FALSE) {
        sglEnable(SGL_LINE_HALOING);
    }
    else {
        /* Nothing to do */
    }

    if (loc_b_texture_state_backup) {
        sglEnable(SGL_TEXTURE_2D);
    }
    else {
        sglDisable(SGL_TEXTURE_2D);
    }

    sglTransparency8((SGLlong) loc_b_transparency_backup);

    return;
}




/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgluWriteRichText
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
void sgluWriteRichText(SGLfloat par_f_x, SGLfloat par_f_y, const SGLbyte * par_pb_string, SGLlong par_l_string_length)
{
    SGLbool loc_b_error_detected = SGL_FALSE;
    SGLulong loc_ul_i;

    b_bold_text = SGL_FALSE;

    for (loc_ul_i = 0; loc_ul_i < SGL_MAX_STRING_LINES; loc_ul_i++) {
        glob_t_line_origin[loc_ul_i] = 0.0F;
    }

    /* Check given string is not NULL */
    if (par_pb_string == SGL_NULL) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check the size of given string is positive */
    if (par_l_string_length < 0L) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* If no error is detected */
    if (loc_b_error_detected == SGL_FALSE) {
        /* Store first font text attributes */
        SGLlong loc_l_prev_color = glob_pr_sglStatemachine->l_color_index;
        SGLulong loc_ul_prev_font_id = glob_pr_sglStatemachine->ul_font_id;

        const sgl_line_width *loc_prev_line_width = glob_pr_sglStatemachine->tr_line_width;
        SGLulong loc_prev_line_width_size = glob_pr_sglStatemachine->ul_nb_line_width;
        SGLlong loc_l_line_width_backup = glob_pr_sglStatemachine->l_linewidth_index;

        glob_pr_sglStatemachine->tr_line_width = (sgl_line_width *) font_line_width;
        glob_pr_sglStatemachine->ul_nb_line_width = FONT_LINE_WIDTH_SIZE;
        glob_pr_sglStatemachine->l_linewidth_index = glob_pr_sglStatemachine->l_current_font_linewidth_index;

        /* Check the length is not too big */
        if (par_l_string_length > (SGLlong) SGLU_MAX_STRING_LENGTH) {
            oglxSetError(SGL_ERROR_SGLU_WRITERICHTEXT, (SGLulong) par_l_string_length);
        }
        else {
            /* Nothing to do */
        }

        oglxWriteRichText(par_f_x, par_f_y, par_pb_string, (SGLulong) par_l_string_length);

        /* Restore first font text attributes */
        glob_pr_sglStatemachine->tr_line_width = loc_prev_line_width;
        glob_pr_sglStatemachine->ul_nb_line_width = loc_prev_line_width_size;
        glob_pr_sglStatemachine->l_linewidth_index = loc_l_line_width_backup;

        sglIndexColori(loc_l_prev_color);
        sglIndexFonti((SGLlong) loc_ul_prev_font_id);
        sglIndexLineWidthi(loc_l_line_width_backup);
    }
    else {
        oglxSetError(SGL_ERROR_SGLU_WRITERICHTEXT, 0U);
    }

    return;
}

/* End of File ***************************************************************/
