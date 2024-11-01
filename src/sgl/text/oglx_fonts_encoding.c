/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglx_fonts_encoding.c
 DESCRIPTION       : Set of functions used to draw bitmap and stroke fonts
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
#include "glf.h"
#include "mth.h"

/*+ DEFINITION OF CONSTANTS -------------------------------------------------------
 DESCRIPTION       : Definition of constants of oglx_fonts_encoding.c
 LLR: [OGLX-DDD-oglx_fonts_encoding_constants]
---------------------------------------------------------------------------- +*/
#define UTF_8_2_CHAR		0xC0U
#define UTF_8_3_CHAR		0xE0U
#define UTF_8_4_CHAR		0xF0U
#define UTF_8_PREFIX		0x80U
#define GBK_CHAR		0x80U
/*+ END OF DEFINITION OF CONSTANTS -------------------------------------------------------*/

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_get_font
  DESCRIPTION:
    Function shall return the font associated to index, or the default font.
  PARAMETERS:
    par_ul_index -> index of font
  RETURN:
    const glf_type_font * -> Font
---------------------------------------------------------------------+*/
const glf_type_font *oglx_get_font(SGLulong par_ul_index)
{
    const glf_type_font *result;
    if (par_ul_index < glob_pr_sglStatemachine->ul_nb_of_fonts) {
        result = glob_pr_sglStatemachine->pr_font_table + par_ul_index;

        if (result->us_font_kind == GLF_FONT_TYPE_NONE) {
            result = getDefaultFont();
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        result = getDefaultFont();
    }
    return result;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_get_char_index
  DESCRIPTION:
    Function shall convert input character to character index.
  PARAMETERS:
    par_pus_widechar -> Output index of character at the begining of the string
    par_pub_string -> Current string
  RETURN:
    SGLshort -> Offset to next character
---------------------------------------------------------------------+*/
SGLushort oglx_get_char_index(GLushort * par_pus_widechar, const GLubyte * par_pub_string)
{
    GLushort loc_widechar = 0U;
    GLubyte loc_char = par_pub_string[0];
    SGLushort loc_us_return = 0;
    GLushort loc_us_value;

    switch (glob_pr_sglStatemachine->b_charset_encoding) {
    case SGL_FORMAT_UTF8:
    {
        /* Check the UTF-8 character size */
        if ((loc_char & UTF_8_4_CHAR) == UTF_8_4_CHAR) {
            /* Characters coded on 4 bytes cannot be managed by OGLX as font table manages SGLushort indexes (0 to 65535) */
            /* Default character is drawn */
            loc_us_return = 4;
            *par_pus_widechar = 0;
        }
        else {
            if ((loc_char & UTF_8_3_CHAR) == UTF_8_3_CHAR) {
                /* The character is on 3 bytes, the index shall be built from the 3 bytes */
                loc_us_value = (GLushort) (((GLushort) (loc_char & ~UTF_8_3_CHAR)) << 12);
                loc_char = par_pub_string[1];
                loc_us_value = (GLushort) (loc_us_value | (((GLushort) (loc_char & ~UTF_8_PREFIX)) << 6));
                loc_char = par_pub_string[2];
                loc_us_value = (GLushort) (loc_us_value | ((GLushort) (loc_char & ~UTF_8_PREFIX)));
                loc_us_return = 3;
                *par_pus_widechar = loc_us_value;
            }
            else {
                if ((loc_char & UTF_8_2_CHAR) == UTF_8_2_CHAR) {
                    /* The character is on 2 bytes, the index shall be built from the 2 bytes */
                    loc_us_value = (GLushort) (((GLushort) (loc_char & ~UTF_8_2_CHAR)) << 6);
                    loc_char = par_pub_string[1];
                    loc_us_value = (GLushort) (loc_us_value | ((GLushort) (loc_char & ~UTF_8_PREFIX)));
                    loc_us_return = 2;

                    *par_pus_widechar = loc_us_value;
                }
                else {
                    /* The character is on 1 byte, it can be kept as this */
                    *par_pus_widechar = (GLushort) loc_char;
                    loc_us_return = 1;
                }
            }
        }
    }
        break;
    case SGL_FORMAT_GBK:
    {
        /* Get par_string (GBK) */
        if ((loc_char & GBK_CHAR) == GBK_CHAR) {
            loc_widechar = (GLushort) ((par_pub_string[0] * 256U) + par_pub_string[1]);
            *par_pus_widechar = loc_widechar;
            loc_us_return = 2;
        }
        else {
            *par_pus_widechar = (GLushort) loc_char;
            loc_us_return = 1;
        }
    }
        break;
    case SGL_FORMAT_ASCII:
    default:
    {
        /* C99 ASCII with \uXXXX escape sequences, denoting Unicode characters */

        /* First, read \u */
        if ((loc_char != (GLubyte) '\\') || (par_pub_string[1] != (GLubyte) 'u')) {
            *par_pus_widechar = (GLushort) loc_char;
            loc_us_return = 1;
        }
        else {
            SGLushort loc_us_i = 2;

            /* Read the unicode value */
            while ((loc_us_i < 6U) && (loc_us_return == 0U)) {
                loc_char = par_pub_string[loc_us_i];
                if ((loc_char >= (GLubyte) '0') && (loc_char <= (GLubyte) '9')) {
                    loc_char -= (GLubyte) '0';
                }
                else {
                    if ((loc_char >= (GLubyte) 'A') && (loc_char <= (GLubyte) 'F')) {
                        loc_char -= ((GLubyte) 'A' - 10U);
                    }
                    else {
                        if ((loc_char >= (GLubyte) 'a') && (loc_char <= (GLubyte) 'f')) {
                            loc_char -= ((GLubyte) 'a' - 10U);
                        }
                        else {
                            *par_pus_widechar = (GLushort) '\\';
                            loc_us_return = 1;
                        }
                    }
                }

                if (loc_us_return == 0U) {
                    /* Convert unicode value to character index */
                    loc_widechar |= (((GLushort) loc_char) << (4U * (5U - loc_us_i)));
                }
                else {
                    /* Nothing to do */
                }

                loc_us_i++;
            }

            if (loc_us_return == 0U) {
                /* Write the character index to the output index */
                *par_pus_widechar = loc_widechar;
                /* Next character is 6 characters farer */
                loc_us_return = 6;
            }
            else {
                /* Nothing to do */
            }
        }
    }
        break;
    }
    return loc_us_return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_fonts_draw_stroke_character
  DESCRIPTION:
    Function shall draw given stroke character.
  PARAMETERS:
    par_pr_font -> The font to draw
    par_pr_char_stroke -> The character to draw
    par_f_x -> X origin of the character
    par_f_y -> Y origin of the character
    par_f_ratio_x -> X ratio
    par_f_ratio_y -> Y ratio
    par_ui_linewidth_index -> Line width to apply
    par_ui_current_font_color_index -> Index of color to apply
    par_b_haloing -> Haloing or not
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_fonts_draw_stroke_character(const glf_type_font * par_pr_font, const glf_type_character_stroke * par_pr_char_stroke,
                                      SGLfloat par_f_x, SGLfloat par_f_y, SGLfloat par_f_ratio_x, SGLfloat par_f_ratio_y,
                                      SGLulong par_ui_linewidth_index, SGLulong par_ui_current_font_color_index, SGLbool par_b_haloing)
{
    /* Get character description */
    const SGLbyte *loc_char_description;
    SGLulong loc_ui_nb_line_count = 0U;
    SGLulong loc_ui_nb_line;
    SGLfloat loc_f_ratio_x = par_f_ratio_x;
    SGLfloat loc_f_ratio_y = par_f_ratio_y;

    loc_char_description = GLF_GET_STROKE_DATA(par_pr_font) + par_pr_char_stroke->ui_offset_to_description;


    sglIndexLineWidthi((SGLlong) par_ui_linewidth_index);

    if (!par_b_haloing) {
        oglxUpdateColor((SGLlong) par_ui_current_font_color_index);
    }
    else {
        glob_pr_sglStatemachine->b_haloing_state = SGL_HALOED_PART_ONLY;
    }

    /* Read number of lines */
    loc_ui_nb_line = *(const SGLulong *) loc_char_description;
    loc_char_description += sizeof(SGLulong);

    /* Draw each line */
    while (loc_ui_nb_line_count < loc_ui_nb_line) {
        SGLfloat loc_f_point_x;
        SGLfloat loc_f_point_y;
        SGLfloat loc_f_y_minus_1 = 0.0F;
        SGLfloat loc_f_y_plus_1;
        SGLfloat loc_f_y_adjustment = 0.6F * loc_f_ratio_y;
        SGLushort loc_us_nb_vertex_count = 0U;
        SGLushort loc_us_line_type;
        SGLushort loc_us_nb_vertex;

        /* Read line type */
        loc_us_line_type = *(const SGLushort *) loc_char_description;
        loc_char_description += sizeof(SGLushort);

        /* Read number of vertex */
        loc_us_nb_vertex = *(const SGLushort *) loc_char_description;
        loc_char_description += sizeof(SGLushort);

        /* Set current line type */
        sglBegin((SGLbyte) loc_us_line_type);


        /* Process each vertex */
        while (loc_us_nb_vertex_count < loc_us_nb_vertex) {
            const SGLfloat *loc_pr_point = (const SGLfloat *) loc_char_description;
            SGLfloat loc_f_diff;
            loc_f_point_x = loc_pr_point[0] * loc_f_ratio_x;
            loc_f_point_y = loc_pr_point[1] * loc_f_ratio_y;

            /* In case of a line loop, Vertex has not to be adjusted */
            if (loc_us_line_type == GLF_LINE_LOOP) {
                sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_point_y);
            }
            else {
                /* If the vertex is not an extremity of the line */
                if ((loc_us_nb_vertex_count > 0U) && (loc_us_nb_vertex_count < (loc_us_nb_vertex - 1U))) {
                    const SGLfloat *loc_pr_point_plus_1 = loc_pr_point + 2U;
                    loc_f_y_plus_1 = loc_pr_point_plus_1[1] * loc_f_ratio_y;

                    /* If the two consecutive lines change of direction */
                    if ((((loc_f_point_y - loc_f_y_minus_1) * (loc_f_y_plus_1 - loc_f_point_y)) < 0.0F)) {
                        loc_f_diff = loc_f_point_y - loc_f_y_minus_1;
                        /* If current vertex is higher than previous one */
                        if (loc_f_diff > 0.0F) {
                            /* Add a half pixel in Y */
                            sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_y_adjustment + loc_f_point_y);
                        }
                        else {
                            /* Else reduce by a half pixel in Y */
                            sglVertex2f(par_f_x + loc_f_point_x, (par_f_y - loc_f_y_adjustment) + loc_f_point_y);
                        }
                    }
                    else {
                        /* Do not adjust vertex */
                        sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_point_y);
                    }
                }
                else {
                    /* If current vertex is the first of a line */
                    if (loc_us_nb_vertex_count == 0U) {
                        SGLbool loc_b_compare_y_plus_1;
                        const SGLfloat *loc_pr_point_plus_1 = loc_pr_point + 2U;
                        loc_f_y_plus_1 = loc_pr_point_plus_1[1] * loc_f_ratio_y;

                        loc_b_compare_y_plus_1 = sgluFloatIsEqual(loc_f_point_y, loc_f_y_plus_1);
                        /* If current line is horizontal */
                        if (loc_b_compare_y_plus_1) {
                            /* Do not adjust vertex */
                            sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_point_y);
                        }
                        else {
                            loc_f_diff = loc_f_y_plus_1 - loc_f_point_y;
                            /* Else if current line is a raising one */
                            if (loc_f_diff > 0.0F) {
                                /* Reduce Vertex by a half pixel in Y */
                                sglVertex2f(par_f_x + loc_f_point_x, (par_f_y - loc_f_y_adjustment) + loc_f_point_y);
                            }
                            else {
                                /* Else add a half pixel in Y */
                                sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_y_adjustment + loc_f_point_y);
                            }
                        }
                    }
                    else {
                        /* Else current vertex is the last one */
                        SGLbool loc_b_compare_y_minus_1 = sgluFloatIsEqual(loc_f_point_y, loc_f_y_minus_1);
                        /* If current line is horizontal */
                        if (loc_b_compare_y_minus_1) {
                            /* Do not adjust vertex */
                            sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_point_y);
                        }
                        else {
                            loc_f_diff = loc_f_point_y - loc_f_y_minus_1;
                            /* Else if current line is a lowering one */
                            if (loc_f_diff > 0.0F) {
                                /* Add a half pixel in Y */
                                sglVertex2f(par_f_x + loc_f_point_x, par_f_y + loc_f_y_adjustment + loc_f_point_y);
                            }
                            else {
                                /* Else reduce Vertex by a half pixel in Y */
                                sglVertex2f(par_f_x + loc_f_point_x, (par_f_y - loc_f_y_adjustment) + loc_f_point_y);
                            }
                        }
                    }
                }
            }
            loc_char_description += 2U * sizeof(SGLfloat);
            loc_f_y_minus_1 = loc_f_point_y;
            loc_us_nb_vertex_count++;
        }
        sglEnd();

        loc_ui_nb_line_count++;
    }

    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_draw_bitmap_text
  DESCRIPTION:
    Function shall draw an entire bitmap text (a textured shape).
  PARAMETERS:
    par_l_size -> Number of vertices of the shape.
	par_pf_vertices -> Vertex array containing the shape.
	par_pf_texcoord -> Array containing the texture coordinates of the shape
	par_b_haloing -> Haloed part is drawn (SGL_TRUE), or colored part is drawn (SGL_FALSE)
    par_b_halo_width_available -> Halo width is available in bitmap font.
  RETURN:
    none
---------------------------------------------------------------------+*/
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
void oglx_draw_bitmap_text(SGLlong par_l_size, const SGLfloat * par_pf_vertices, const SGLfloat * par_pf_texcoord, SGLbool par_b_haloing,
                           SGLbool par_b_halo_width_available)
#else
void oglx_draw_bitmap_text(SGLlong par_l_size, const SGLfloat * par_pf_vertices, SGLbool par_b_haloing)
#endif
{
    if (!par_b_haloing) {
        oglxEnable((SGLulong) GL_BLEND);
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        if ((glob_pr_sglStatemachine->b_haloing_state != (SGLbyte) SGL_FALSE) && (!par_b_halo_width_available)) {
#if !defined(SC101_DEV_ENV)
            glEnable(GL_ALPHA_TEST);
#endif
            oglxDrawTexVertexArray((SGLulong) GL_TRIANGLE_STRIP, par_l_size, par_pf_vertices, par_pf_texcoord);
#if !defined(SC101_DEV_ENV)
            glDisable(GL_ALPHA_TEST);
#endif
        }
        else {
            SGLbool loc_b_modulate_backup = glob_pr_sglStatemachine->b_modulate;
            if (!loc_b_modulate_backup) {
                if (glob_pr_sglStatemachine->r_current_color.tf_current_color[3] < 1.0F) {
                    sglEnable(SGL_MODULATE);
                }
            }
            oglxDrawTexVertexArray((SGLulong) GL_TRIANGLE_STRIP, par_l_size, par_pf_vertices, par_pf_texcoord);
            if (!loc_b_modulate_backup) {
                sglDisable(SGL_MODULATE);
            }
        }
#else
        oglxDrawTexVertexArray_es2((SGLulong) GL_TRIANGLE_STRIP, par_l_size, par_pf_vertices, SGL_TRUE);
#endif
    }
    else {
        oglxEnable((SGLulong) GL_BLEND);
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        oglxDrawTexVertexArray((SGLulong) GL_TRIANGLE_STRIP, par_l_size, par_pf_vertices, par_pf_texcoord);
#else
        oglxDrawTexVertexArray_es2((SGLulong) GL_TRIANGLE_STRIP, par_l_size, par_pf_vertices, SGL_TRUE);
#endif
    }
    glob_pr_sglStatemachine->tr_geometry.l_size = 0L;

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_fonts_draw_bitmap_text
  DESCRIPTION:
    Function shall draw a bitmap text or store a bitmap character as part of a text.
  PARAMETERS:
    par_pr_font -> The font to draw
    par_pr_char -> The character to draw
    par_f_x -> X origin of the character
    par_f_y -> Y origin of the character
    par_ui_font_linewidth -> Line width to apply
    par_f_ratio_x -> X ratio
    par_f_ratio_y -> Y ratio
    par_b_haloing -> haloing enabled or not
	par_b_draw -> SGL_TRUE if the text shall be drawn, SGL_FALSE if the character shall be stored.
  RETURN:
    none
---------------------------------------------------------------------+*/
void oglx_fonts_draw_bitmap_text(const glf_type_font * par_pr_font, const glf_type_character_bitmap * par_pr_char,
                                 SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_font_linewidth,
                                 SGLfloat par_f_ratio_x, SGLfloat par_f_ratio_y, SGLbool par_b_haloing, SGLbool par_b_draw)
{
    const glf_type_u_v_couple *loc_pr_char_mapping;

    /* Compute rectangle coordinates */
    SGLfloat loc_f_x1;
    SGLfloat loc_f_y1;
    SGLfloat loc_f_x2;
    SGLfloat loc_f_y2;

    sgl_vector2d *loc_t_tex_coord_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
    sgl_vector2d *loc_p_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
    SGLlong loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;

    SGLbool b_halo_width_available;

    if (par_b_haloing) {
        b_halo_width_available = (SGLbool) (GLF_GET_DELTA_HALO_WIDTH(par_pr_font, (par_ui_font_linewidth - 2UL)) != 0U);
    }
    else {
        b_halo_width_available = (SGLbool) (GLF_GET_DELTA_HALO_WIDTH(par_pr_font, par_ui_font_linewidth) != 0U);
    }

    if (par_b_haloing && b_halo_width_available) {
        loc_pr_char_mapping = &(par_pr_char->tr_u_v_mapping[GLF_GET_DELTA_HALO_WIDTH(par_pr_font, (par_ui_font_linewidth - 2UL))]);
    }
    else {
        if (par_b_haloing) {
            loc_pr_char_mapping = &(par_pr_char->tr_u_v_mapping[GLF_GET_DELTA_WIDTH(par_pr_font, (par_ui_font_linewidth - 2UL))]);
        }
        else {
            loc_pr_char_mapping = &(par_pr_char->tr_u_v_mapping[GLF_GET_DELTA_WIDTH(par_pr_font, par_ui_font_linewidth)]);
        }
    }

    if (((loc_l_size > 0L) && (glob_pr_sglStatemachine->ui_map_index != loc_pr_char_mapping->ui_map_index))
        || ((loc_l_size + 4L) > (SGLlong) SGL_MAX_VERTEX_ARRAY_SIZE)) {
        /* Last index shall not be taken into account when a draw is requested because of a change of texture */
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        oglx_draw_bitmap_text(loc_l_size - 1L, &(loc_p_vertex_array[0][0]), &(loc_t_tex_coord_array[0][0]), par_b_haloing, b_halo_width_available);
#else
        oglx_draw_bitmap_text(loc_l_size - 1L, &(loc_p_vertex_array[0][0]), par_b_haloing);
#endif
    }

    glob_pr_sglStatemachine->ui_map_index = loc_pr_char_mapping->ui_map_index;
    loc_l_size = glob_pr_sglStatemachine->tr_geometry.l_size;

    if (loc_l_size == 0L) {
        oglxSelectFontTexture(loc_pr_char_mapping->ui_map_index);
    }

    loc_f_x1 = par_f_x + ((SGLfloat) loc_pr_char_mapping->s_x * par_f_ratio_x);
    loc_f_y1 = par_f_y + ((SGLfloat) loc_pr_char_mapping->s_y * par_f_ratio_y);
    loc_f_x2 = loc_f_x1 + ((SGLfloat) loc_pr_char_mapping->us_width * par_f_ratio_x);
    loc_f_y2 = loc_f_y1 + ((SGLfloat) loc_pr_char_mapping->us_height * par_f_ratio_y);

    if (loc_l_size > 0L) {
        loc_p_vertex_array[loc_l_size][0] = loc_f_x1;
        loc_p_vertex_array[loc_l_size][1] = loc_f_y1;
        loc_t_tex_coord_array[loc_l_size][0] = loc_pr_char_mapping->f_u1;
        loc_t_tex_coord_array[loc_l_size][1] = loc_pr_char_mapping->f_v1;
        loc_l_size++;
    }

    /* Read texture coordinates */
    loc_t_tex_coord_array[loc_l_size][0] = loc_pr_char_mapping->f_u1;
    loc_t_tex_coord_array[loc_l_size][1] = loc_pr_char_mapping->f_v1;
    loc_t_tex_coord_array[loc_l_size + 1][1] = loc_pr_char_mapping->f_v2;
    loc_t_tex_coord_array[loc_l_size + 1][0] = loc_pr_char_mapping->f_u1;
    loc_t_tex_coord_array[loc_l_size + 2][0] = loc_pr_char_mapping->f_u2;
    loc_t_tex_coord_array[loc_l_size + 2][1] = loc_pr_char_mapping->f_v1;
    loc_t_tex_coord_array[loc_l_size + 3][0] = loc_pr_char_mapping->f_u2;
    loc_t_tex_coord_array[loc_l_size + 3][1] = loc_pr_char_mapping->f_v2;


    /* Fill vertex arrays with a textured rectangle */
    loc_p_vertex_array[loc_l_size][0] = loc_f_x1;
    loc_p_vertex_array[loc_l_size][1] = loc_f_y1;
    loc_p_vertex_array[loc_l_size + 1][0] = loc_f_x1;
    loc_p_vertex_array[loc_l_size + 1][1] = loc_f_y2;
    loc_p_vertex_array[loc_l_size + 2][0] = loc_f_x2;
    loc_p_vertex_array[loc_l_size + 2][1] = loc_f_y1;
    loc_p_vertex_array[loc_l_size + 3][0] = loc_f_x2;
    loc_p_vertex_array[loc_l_size + 3][1] = loc_f_y2;

    loc_l_size += 4L;
    glob_pr_sglStatemachine->tr_geometry.l_size = loc_l_size;

    if (par_b_draw) {
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        oglx_draw_bitmap_text(loc_l_size, &(loc_p_vertex_array[0][0]), &(loc_t_tex_coord_array[0][0]), par_b_haloing, b_halo_width_available);
#else
        oglx_draw_bitmap_text(loc_l_size, &(loc_p_vertex_array[0][0]), par_b_haloing);
#endif
    }
    else {
        loc_p_vertex_array[loc_l_size][0] = loc_f_x2;
        loc_p_vertex_array[loc_l_size][1] = loc_f_y2;

        loc_t_tex_coord_array[loc_l_size][0] = loc_pr_char_mapping->f_u2;
        loc_t_tex_coord_array[loc_l_size][1] = loc_pr_char_mapping->f_v2;

        glob_pr_sglStatemachine->tr_geometry.l_size++;
    }
    return;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_fonts_draw_bitmap_character
  DESCRIPTION:
    Function shall draw a bitmap character according to given index and table.
  PARAMETERS:
    par_pr_font -> The font to draw
    par_pr_char -> The character to draw
    par_f_x -> X origin of the character
    par_f_y -> Y origin of the character
    par_ui_font_linewidth -> Line width to apply
    par_f_ratio_x -> X ratio
    par_f_ratio_y -> Y ratio
  RETURN:
    none
---------------------------------------------------------------------+*/
void oglx_fonts_draw_bitmap_character(const glf_type_font * par_pr_font, const glf_type_character_bitmap * par_pr_char,
                                      SGLfloat par_f_x, SGLfloat par_f_y, SGLulong par_ui_font_linewidth, SGLfloat par_f_ratio_x, SGLfloat par_f_ratio_y)
{
    sgl_vector2d *loc_t_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;

    const glf_type_u_v_couple *loc_pr_char_mapping;

    /* Compute rectangle coordinates */
    SGLfloat loc_f_x1;
    SGLfloat loc_f_y1;
    SGLfloat loc_f_x2;
    SGLfloat loc_f_y2;

    sgl_vector2d *loc_t_tex_coord_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    SGLbool b_halo_width_available = (SGLbool) (GLF_GET_DELTA_HALO_WIDTH(par_pr_font, par_ui_font_linewidth) != 0U);
#endif
    loc_pr_char_mapping = &(par_pr_char->tr_u_v_mapping[GLF_GET_DELTA_WIDTH(par_pr_font, par_ui_font_linewidth)]);

    loc_f_x1 = par_f_x + ((SGLfloat) loc_pr_char_mapping->s_x * par_f_ratio_x);
    loc_f_y1 = par_f_y + ((SGLfloat) loc_pr_char_mapping->s_y * par_f_ratio_y);
    loc_f_x2 = loc_f_x1 + ((SGLfloat) loc_pr_char_mapping->us_width * par_f_ratio_x);
    loc_f_y2 = loc_f_y1 + ((SGLfloat) loc_pr_char_mapping->us_height * par_f_ratio_y);

    oglxSelectFontTexture(loc_pr_char_mapping->ui_map_index);

    /* Read texture coordinates */
    loc_t_tex_coord_array[0][0] = loc_pr_char_mapping->f_u1;
    loc_t_tex_coord_array[0][1] = loc_pr_char_mapping->f_v1;
    loc_t_tex_coord_array[2][0] = loc_pr_char_mapping->f_u2;
    loc_t_tex_coord_array[1][1] = loc_pr_char_mapping->f_v2;
    loc_t_tex_coord_array[1][0] = loc_t_tex_coord_array[0][0];
    loc_t_tex_coord_array[2][1] = loc_t_tex_coord_array[1][1];
    loc_t_tex_coord_array[3][0] = loc_t_tex_coord_array[2][0];
    loc_t_tex_coord_array[3][1] = loc_t_tex_coord_array[0][1];

    /* Fill vertex arrays with a textured rectangle */
    loc_t_vertex_array[0][0] = loc_f_x1;
    loc_t_vertex_array[0][1] = loc_f_y1;
    loc_t_vertex_array[1][0] = loc_f_x1;
    loc_t_vertex_array[1][1] = loc_f_y2;
    loc_t_vertex_array[2][0] = loc_f_x2;
    loc_t_vertex_array[2][1] = loc_f_y2;
    loc_t_vertex_array[3][0] = loc_f_x2;
    loc_t_vertex_array[3][1] = loc_f_y1;

    oglxEnable((SGLulong) GL_BLEND);
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    if ((glob_pr_sglStatemachine->b_haloing_state != (SGLbyte) SGL_FALSE) && (!b_halo_width_available)) {
#if !defined(SC101_DEV_ENV)
        glEnable(GL_ALPHA_TEST);
#endif
        oglxDrawTexVertexArray((SGLulong) GL_TRIANGLE_FAN, 4, &(loc_t_vertex_array[0][0]), &(loc_t_tex_coord_array[0][0]));
#if !defined(SC101_DEV_ENV)
        glDisable(GL_ALPHA_TEST);
#endif
    }
    else {
        SGLbool loc_b_modulate_backup = glob_pr_sglStatemachine->b_modulate;
        if (!loc_b_modulate_backup) {
            if (glob_pr_sglStatemachine->r_current_color.tf_current_color[3] < 1.0F) {
                sglEnable(SGL_MODULATE);
            }
        }
        oglxDrawTexVertexArray((SGLulong) GL_TRIANGLE_FAN, 4, &(loc_t_vertex_array[0][0]), &(loc_t_tex_coord_array[0][0]));
        if (!loc_b_modulate_backup) {
            sglDisable(SGL_MODULATE);
        }
    }
#else
    oglxDrawTexVertexArray_es2((SGLulong) GL_TRIANGLE_FAN, 4, &(loc_t_vertex_array[0][0]), SGL_TRUE);
#endif

    return;
}

/* End of File ***************************************************************/
