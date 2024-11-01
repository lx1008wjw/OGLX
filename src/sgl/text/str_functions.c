/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : str_functions.c
 DESCRIPTION       : This file provides utilitary functions for strings
 COPYRIGHT (C)     : 2008 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Protected interfaces +*/
#include "sgl.h"
#include "sgl_private.h"

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_is_word_separator
  DESCRIPTION:
    Function shall check if given character is a word separator 
  PARAMETERS:
    par_us_char -> the character to check
  RETURN:
    SGLbool -> SGL_TRUE if given character is a separator, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool sgl_is_word_separator(SGLushort par_us_char)
{
    SGLulong loc_ul_i = 0;
    SGLbool loc_b_return = SGL_FALSE;

    while ((loc_ul_i < glob_pr_sglStatemachine->ul_nb_word_separators) && (!loc_b_return)) {
        if ((SGLushort) (glob_pr_sglStatemachine->tb_word_separators[loc_ul_i]) == par_us_char) {
            loc_b_return = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }
        loc_ul_i++;
    }

    return loc_b_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgl_is_line_separator
  DESCRIPTION:
    Function shall check if given character is a line separator 
  PARAMETERS:
    par_b_char -> the character to check
  RETURN:
    SGLbool -> SGL_TRUE if given character is a separator, SGL_FALSE otherwise
---------------------------------------------------------------------+*/
SGLbool sgl_is_line_separator(SGLushort par_us_char)
{
    SGLulong loc_ul_i = 0;
    SGLbool loc_b_return = SGL_FALSE;

    while ((loc_ul_i < glob_pr_sglStatemachine->ul_nb_line_separators) && (!loc_b_return)) {
        if ((SGLushort) (glob_pr_sglStatemachine->tb_line_separators[loc_ul_i]) == par_us_char) {
            loc_b_return = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }
        loc_ul_i++;
    }

    return loc_b_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_get_string_width
  DESCRIPTION:
    Function shall compute the width of a line.
  PARAMETERS:
    par_pus_string -> the input string
    par_ui_string_length -> string length
    par_ui_font_id -> font index
	par_b_count_correction -> correction taken into account or not
  RETURN:
    SGLfloat -> the computed width
---------------------------------------------------------------------+*/
SGLfloat oglx_get_string_width(SGLushort * par_pus_string, SGLulong par_ui_string_length, SGLulong par_ui_font_id, SGLbool par_b_count_correction)
{
    SGLulong loc_l_counter;
    const glf_type_font *loc_p_font = oglx_get_font(par_ui_font_id);

    /* fonts are scaleable */
    SGLfloat loc_f_ratio_x = glob_pr_sglStatemachine->f_ratio_scale_width;
    SGLfloat loc_f_width = 0.0F;
    SGLfloat loc_f_correction = 0.0F;

    loc_l_counter = 0UL;

    /* For each character */
    while (loc_l_counter < par_ui_string_length) {
        const glf_type_character *loc_pr_char;
        /* Add a character to the box */

        loc_pr_char = GLF_GET_CHAR(loc_p_font, par_pus_string[loc_l_counter]);

        loc_f_width += (loc_pr_char->f_horizontal_advance * loc_f_ratio_x);
        loc_f_correction = (loc_pr_char->f_horizontal_advance * loc_f_ratio_x) - (loc_pr_char->f_width * loc_f_ratio_x);

        loc_l_counter++;
    }

    if (par_b_count_correction) {
        loc_f_width -= loc_f_correction;
    }
    else {
        /* Nothing to do */
    }

    return loc_f_width;
}

/* End of File ***************************************************************/
