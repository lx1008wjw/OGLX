/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sgluLoadFonts.c
 DESCRIPTION       : sgluLoadFonts command shall load the data structure 
					containing the fonts description
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
  NAME: sgl_set_font_texture_parameters
  DESCRIPTION:
    Function shall set font texture parameters.
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgl_set_font_texture_parameters(void)
{
#if defined(ES2_DEV_ENV) || defined(SC101_DEV_ENV) || defined(ES11_DEV_ENV) || defined(SC2_DEV_ENV)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) GL_CLAMP_TO_EDGE);
#else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) GL_CLAMP);
#endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint) GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint) GL_LINEAR);
    return;
}

static SGLbyte tub_expand_alpha[16] = { 0x00U, 0x11U, 0x22U, 0x33U, 0x44U, 0x55U, 0x66U, 0x77U, 0x88U, 0x99U, 0xAAU, 0xBBU, 0xCCU, 0xDDU, 0xEEU, 0xFFU };

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sgluLoadFont
  DESCRIPTION:
    Function shall load the data structure containing the font description.
  PARAMETERS:
    par_ul_index -> index in font table
    par_ps_font -> pointer to font structure
  RETURN:
    None
---------------------------------------------------------------------+*/
void sgluLoadFonts(const void *par_pv_fonts)
{
    SGLbool loc_b_error_detected = SGL_FALSE;

    /* Sanity check on version */
    if (par_pv_fonts != SGL_NULL) {
        const glf_type_font_table_header *loc_header = (const glf_type_font_table_header *) par_pv_fonts;
        if (loc_header->ui_version != GLF_FORMAT_VERSION) {
            loc_b_error_detected = SGL_TRUE;
        }
    }

    if (!loc_b_error_detected) {
        SGLulong loc_ui_index;

        /* Reset current font to default font */
        glob_pr_sglStatemachine->ui_current_font_id = INVALID_FONT_ID;
        glob_pr_sglStatemachine->p_current_font = oglx_get_font(INVALID_FONT_ID);

        if (par_pv_fonts == SGL_NULL) {
            glob_pr_sglStatemachine->pr_font_table = SGL_NULL;
            glob_pr_sglStatemachine->ul_nb_of_fonts = 0UL;
            for (loc_ui_index = 0U; loc_ui_index < GLF_MAX_NUMBER_OF_TEXTURE; loc_ui_index++) {
                glob_pr_sglStatemachine->tui_texture_id[loc_ui_index] = 0U;
            }
        }
        else {
            const glf_type_font_table_header *loc_header = (const glf_type_font_table_header *) par_pv_fonts;
            SGLulong loc_ul_font_number = loc_header->ui_fonts_number;
            SGLulong loc_ul_texture_number = loc_header->ui_textures_number;
            SGLulong loc_ul_maps_offset = ((SGLulong) sizeof(glf_type_font_table_header)) + (loc_ul_font_number * ((SGLulong) sizeof(glf_type_font)));
            const glf_type_map_property *loc_map_property = (const glf_type_map_property *) (((const SGLbyte *) par_pv_fonts) + loc_ul_maps_offset);

            glob_pr_sglStatemachine->pr_font_table = (const glf_type_font *) (((const SGLbyte *) par_pv_fonts) + sizeof(glf_type_font_table_header));
            glob_pr_sglStatemachine->ul_nb_of_fonts = loc_ul_font_number;

            for (loc_ui_index = 0U; loc_ui_index < GLF_MAX_NUMBER_OF_TEXTURE; loc_ui_index++) {
                if (loc_ui_index >= loc_ul_texture_number) {
                    glob_pr_sglStatemachine->tui_texture_id[loc_ui_index] = 0U;
                }
                else {
                    if ((loc_map_property->ui_map_width <= glob_pr_sglStatemachine->ul_max_texture_width)
                        && (loc_map_property->ui_map_height <= glob_pr_sglStatemachine->ul_max_texture_height)
                        && (glob_pr_sglStatemachine->tub_texture_buffer != SGL_NULL)) {
                        SGLulong loc_ul_index = 0UL;
                        GLuint loc_ui_texture;
                        SGLbyte *loc_pub_map = loc_map_property->ptub_map_data;
                        SGLbyte *loc_pub_map_end = loc_pub_map + loc_map_property->ui_map_size;

                        /* Replace alpha value */
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
                        oglxTexEnvi((SGLulong) GL_TEXTURE_ENV, (SGLulong) GL_TEXTURE_ENV_MODE, (SGLlong) GL_REPLACE);
#endif
                        /* Allocate texture */
                        glGenTextures(1, &loc_ui_texture);
                        glob_pr_sglStatemachine->tui_texture_id[loc_ui_index] = (SGLulong) loc_ui_texture;
                        /* Bind the texture  */
                        glBindTexture(GL_TEXTURE_2D, loc_ui_texture);
                        sgl_set_font_texture_parameters();
                        /* Uncompress the texture */
                        while (loc_pub_map < loc_pub_map_end) {
                            SGLbyte loc_ub_component = *loc_pub_map;
                            SGLulong loc_ul_repeat = 1U;
                            loc_pub_map++;
                            if ((loc_pub_map < loc_pub_map_end) && (loc_ub_component == *loc_pub_map)) {
                                loc_pub_map++;
                                loc_ul_repeat++;
                                if ((loc_pub_map + 2U) <= loc_pub_map_end) {
                                    loc_ul_repeat += (((SGLulong) (*loc_pub_map)) << 8U);
                                    loc_pub_map++;
                                    loc_ul_repeat += ((SGLulong) (*loc_pub_map));
                                    loc_pub_map++;
                                }
                            }
                            for (; loc_ul_repeat > 0UL; loc_ul_repeat--) {
                                glob_pr_sglStatemachine->tub_texture_buffer[loc_ul_index] = tub_expand_alpha[(loc_ub_component >> 4U) & 0xFU];
                                loc_ul_index++;
                                glob_pr_sglStatemachine->tub_texture_buffer[loc_ul_index] = tub_expand_alpha[loc_ub_component & 0xFU];
                                loc_ul_index++;
                            }
                        }
                        /* Fill the texture */
#ifndef SC2_DEV_ENV
                        glTexImage2D(GL_TEXTURE_2D, 0, (GLint) GL_ALPHA, (GLsizei) loc_map_property->ui_map_width,
                                     (GLsizei) loc_map_property->ui_map_height, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
                                     &(glob_pr_sglStatemachine->tub_texture_buffer[0]));
#else
                        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, (GLsizei) loc_map_property->ui_map_width, (GLsizei) loc_map_property->ui_map_height);
                        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, (GLsizei) loc_map_property->ui_map_width,
                                        (GLsizei) loc_map_property->ui_map_height, GL_RED, GL_UNSIGNED_BYTE,
                                        (const void *) &(glob_pr_sglStatemachine->tub_texture_buffer[0]));
#endif
                        /* Advance */
                        loc_map_property++;
                    }
                    else {
                        oglxSetError(SGL_ERROR_SGLU_LOADFONTS, 2UL);
                    }
                }
            }
        }

        /* Reset bound texture in case this function is called after a draw */
        glob_pr_sglStatemachine->ui_ogl_texture_bound = NO_OGL_TEXTURE_BOUND;
    }
    else {
        oglxSetError(SGL_ERROR_SGLU_LOADFONTS, 1UL);
    }

    return;
}

/* End of File ***************************************************************/
