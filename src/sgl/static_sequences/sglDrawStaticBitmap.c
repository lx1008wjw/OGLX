/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglDrawStaticBitmap.c
 DESCRIPTION       : sglDrawStaticBitmap shall draw a static bitmap
 COPYRIGHT (C)     : 2014 Esterel Technologies SAS. All Rights Reserved.
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
  NAME: sglDrawStaticBitmap
  DESCRIPTION:
    Function shall draw a static bitmap
  PARAMETERS:
    par_ul_index -> OpenGL texture index
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglDrawStaticBitmap(SGLulong par_ul_index)
{
    if (par_ul_index < SGL_MAX_STATIC_TEXTURE) {
        if (glob_pr_sglStatemachine->s_static_data[par_ul_index].b_defined) {
            SGLbool loc_b_prev_modulate = SGL_FALSE;
            SGLulong loc_ul_index = glob_pr_sglStatemachine->s_static_data[par_ul_index].ul_texture_id;
            sgl_vector2d *loc_t_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
            sgl_vector2d *loc_t_tex_coord_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
            SGLulong loc_ul_width = glob_pr_sglStatemachine->s_static_data[par_ul_index].ul_texture_width;
            SGLulong loc_ul_height = glob_pr_sglStatemachine->s_static_data[par_ul_index].ul_texture_height;
            SGLlong loc_ul_width_p2 =
                mth_next_higher_power2((SGLlong) loc_ul_width, (SGLlong) SGL_TEXTURE_MIN_WIDTH, (SGLlong) glob_pr_sglStatemachine->ul_max_texture_width);
            SGLlong loc_ul_height_p2 = mth_next_higher_power2((SGLlong) loc_ul_height, (SGLlong) SGL_TEXTURE_MIN_HEIGHT,
                                                              (SGLlong) glob_pr_sglStatemachine->ul_max_texture_height);

            SGLulong loc_i_texture_width = loc_ul_width;
            SGLulong loc_i_texture_height = loc_ul_height;
            SGLfloat loc_f_x1 = glob_pr_sglStatemachine->s_static_data[par_ul_index].f_x;
            SGLfloat loc_f_y1 = glob_pr_sglStatemachine->s_static_data[par_ul_index].f_y;
            SGLfloat loc_f_x2 = loc_f_x1 + ((SGLfloat) loc_i_texture_width * glob_pr_sglStatemachine->f_ratio_scale_width);
            SGLfloat loc_f_y2 = loc_f_y1 + ((SGLfloat) loc_i_texture_height * glob_pr_sglStatemachine->f_ratio_scale_height);

            loc_t_tex_coord_array[0][0] = 0.0F;
            loc_t_tex_coord_array[0][1] = 0.0F;
            loc_t_tex_coord_array[1][0] = SGLfloat_div((SGLfloat) loc_ul_width, (SGLfloat) loc_ul_width_p2);
            loc_t_tex_coord_array[1][1] = 0.0F;
            loc_t_tex_coord_array[2][0] = SGLfloat_div((SGLfloat) loc_ul_width, (SGLfloat) loc_ul_width_p2);
            loc_t_tex_coord_array[2][1] = SGLfloat_div((SGLfloat) loc_ul_height, (SGLfloat) loc_ul_height_p2);
            loc_t_tex_coord_array[3][0] = 0.0F;
            loc_t_tex_coord_array[3][1] = SGLfloat_div((SGLfloat) loc_ul_height, (SGLfloat) loc_ul_height_p2);

            loc_t_vertex_array[0][0] = loc_f_x1;
            loc_t_vertex_array[0][1] = loc_f_y1;
            loc_t_vertex_array[1][0] = loc_f_x2;
            loc_t_vertex_array[1][1] = loc_f_y1;
            loc_t_vertex_array[2][0] = loc_f_x2;
            loc_t_vertex_array[2][1] = loc_f_y2;
            loc_t_vertex_array[3][0] = loc_f_x1;
            loc_t_vertex_array[3][1] = loc_f_y2;
            if (glob_pr_sglStatemachine->b_modulate) {
                loc_b_prev_modulate = SGL_TRUE;
                sglDisable(SGL_MODULATE);
            }

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
            oglxEnable((SGLulong) GL_TEXTURE_2D);
#endif
            oglxEnable((SGLulong) GL_BLEND);

            oglxBindTexture((GLuint) loc_ul_index);
#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
            oglxDrawTexVertexArray((SGLulong) GL_TRIANGLE_FAN, 4, &(loc_t_vertex_array[0][0]), &(loc_t_tex_coord_array[0][0]));
#else
            oglxDrawTexVertexArray_es2((SGLulong) GL_TRIANGLE_FAN, 4, &(loc_t_vertex_array[0][0]), SGL_FALSE);
#endif
            if (loc_b_prev_modulate) {
                sglEnable(SGL_MODULATE);
            }
        }
        else {
            oglxSetError(SGL_ERROR_SGL_DRAWSTATICBITMAP, par_ul_index);
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_DRAWSTATICBITMAP, par_ul_index);
    }
    return;
}

/* End of File ***************************************************************/
