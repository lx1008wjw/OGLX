/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetFontValue.c
 DESCRIPTION       : sglGetFontValue command shall return the required font metric.
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
  NAME: sgl_get_font_value_detect_error
  DESCRIPTION:
    Function shall detect parameter errors of sglGetFontValue.
  PARAMETERS:
    par_ul_font_id -> to give the font index
    par_b_value_kind -> to define which metric shall be returned by the function: SGL_CHAR_WIDTH, SGL_CHAR_HEIGHT,
	SGL_VERTICAL_ADVANCE, SGL_HORIZONTAL_ADVANCE, SGL_UNDERLINE, SGL_STROKE, SGL_MAX_CHAR_WIDTH, SGL_MAX_CHAR_HEIGHT, 
	SGL_MAX_CHAR_H_ADVANCE, SGL_MAX_CHAR_V_ADVANCE.
  RETURN:
    SGLbool -> SGL_TRUE if an error is detected, SGL_FALSE otherwise.
---------------------------------------------------------------------+*/
SGLbool sgl_get_font_value_detect_error(SGLulong par_ul_font_id, SGLbyte par_b_value_kind)
{
    SGLbool loc_b_error_detected = SGL_FALSE;

    if (par_ul_font_id >= glob_pr_sglStatemachine->ul_nb_of_fonts) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    if ((par_b_value_kind != SGL_CHAR_WIDTH) && (par_b_value_kind != SGL_CHAR_HEIGHT)
        && (par_b_value_kind != SGL_VERTICAL_ADVANCE) && (par_b_value_kind != SGL_HORIZONTAL_ADVANCE)
        && (par_b_value_kind != SGL_UNDERLINE) && (par_b_value_kind != SGL_STROKE)
        && (par_b_value_kind != SGL_MAX_CHAR_WIDTH) && (par_b_value_kind != SGL_MAX_CHAR_HEIGHT)
        && (par_b_value_kind != SGL_MAX_CHAR_H_ADVANCE) && (par_b_value_kind != SGL_MAX_CHAR_V_ADVANCE)) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    return loc_b_error_detected;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglGetFontValue
  DESCRIPTION:
    Function shall return the required font metric.
  PARAMETERS:
    par_ul_font_id -> to give the font index of the character
    par_ul_char_id -> to give the character index in the font
    par_b_value_kind -> to define which metric shall be returned by the function
  RETURN:
  SGLfloat -> The returned metric value.
---------------------------------------------------------------------+*/
SGLfloat sglGetFontValue(SGLulong par_ul_font_id, SGLulong par_ul_char_id, SGLbyte par_b_value_kind)
{
    SGLfloat loc_f_return = 0.0F;
    SGLbool loc_b_error_detected = sgl_get_font_value_detect_error(par_ul_font_id, par_b_value_kind);

    if (!loc_b_error_detected) {
        const glf_type_font *loc_p_input_font = oglx_get_font(par_ul_font_id);

        if ((par_b_value_kind == SGL_UNDERLINE) || (par_b_value_kind == SGL_STROKE)
            || (par_b_value_kind == SGL_MAX_CHAR_WIDTH) || (par_b_value_kind == SGL_MAX_CHAR_HEIGHT)
            || (par_b_value_kind == SGL_MAX_CHAR_H_ADVANCE) || (par_b_value_kind == SGL_MAX_CHAR_V_ADVANCE)) {
            switch (par_b_value_kind) {
            case SGL_UNDERLINE:
            {
                loc_f_return = loc_p_input_font->f_underline_y;
            }
                break;
            case SGL_STROKE:
            {
                loc_f_return = loc_p_input_font->f_stroke_y;
            }
                break;
            case SGL_MAX_CHAR_WIDTH:
            {
                loc_f_return = loc_p_input_font->f_max_char_width;
            }
                break;
            case SGL_MAX_CHAR_HEIGHT:
            {
                loc_f_return = loc_p_input_font->f_max_char_height;
            }
                break;
            case SGL_MAX_CHAR_H_ADVANCE:
            {
                loc_f_return = loc_p_input_font->f_max_h_advance;
            }
                break;
            default:
                /*case SGL_MAX_CHAR_V_ADVANCE: */
            {
                loc_f_return = loc_p_input_font->f_max_v_advance;
            }
                break;
            }
        }
        else {
            const glf_type_character *loc_pr_char = GLF_GET_CHAR(loc_p_input_font, par_ul_char_id);
            switch (par_b_value_kind) {
            case SGL_CHAR_WIDTH:
            {
                loc_f_return = loc_pr_char->f_width;
            }
                break;
            case SGL_CHAR_HEIGHT:
            {
                loc_f_return = loc_pr_char->f_height;
            }
                break;
            case SGL_VERTICAL_ADVANCE:
            {
                loc_f_return = loc_pr_char->f_vertical_advance;
            }
                break;
            default:
                /*
                   case SGL_HORIZONTAL_ADVANCE:
                 */
            {
                loc_f_return = loc_pr_char->f_horizontal_advance;
            }
                break;
            }
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_GETFONTVALUE, (SGLulong) par_b_value_kind);
    }
    return loc_f_return;
}

/* End of File ***************************************************************/
