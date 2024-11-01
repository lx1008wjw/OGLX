/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglCallStaticSequence.c
 DESCRIPTION       : sglCallStaticSequence shall call the draw of a sequence of static objects 
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

#ifndef ES11_DEV_ENV
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglCallStaticSequence
  DESCRIPTION:
    Function shall call the draw of a sequence of static objects 
  PARAMETERS:
    par_ul_index -> Index of static sequence
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglCallStaticSequence(SGLulong par_ul_index)
{
#ifdef OGLX_DISPLAY_LISTS
#ifndef SC101_DEV_ENV
    glCallList((GLuint) par_ul_index);
#else
    GLuint loc_ui_index = (GLuint) par_ul_index;
    glCallLists(1, GL_UNSIGNED_INT, &loc_ui_index);
#endif
#endif

#ifdef OGLX_FBO
    if ((par_ul_index < SGL_MAX_STATIC_TEXTURE) && (glob_pr_sglStatemachine->s_static_data[par_ul_index].b_defined == SGL_TRUE)) {
        SGLulong loc_ul_index = glob_pr_sglStatemachine->s_static_data[par_ul_index].ul_texture_id;
#ifndef SC2_DEV_ENV
        if (glIsTexture((GLuint) loc_ul_index)) {
#endif
            sgl_vector2d *loc_t_vertex_array = glob_pr_sglStatemachine->tr_geometry.p_vertices;
            sgl_vector2d *loc_t_tex_coord_array = glob_pr_sglStatemachine->tr_geometry.p_tex_coordinates;
            GLint loc_i_texture_width = (GLint) (glob_pr_sglStatemachine->s_static_data[par_ul_index].ul_texture_width);
            GLint loc_i_texture_height = (GLint) (glob_pr_sglStatemachine->s_static_data[par_ul_index].ul_texture_height);
            SGLfloat loc_f_x1 = glob_pr_sglStatemachine->s_static_data[par_ul_index].f_x;
            SGLfloat loc_f_y1 = glob_pr_sglStatemachine->s_static_data[par_ul_index].f_y;
            SGLfloat loc_f_x2 = loc_f_x1 + (SGLfloat) loc_i_texture_width;
            SGLfloat loc_f_y2 = loc_f_y1 + (SGLfloat) loc_i_texture_height;

            loc_t_tex_coord_array[0][0] = 0.0F;
            loc_t_tex_coord_array[0][1] = 0.0F;
            loc_t_tex_coord_array[1][0] = 1.0F;
            loc_t_tex_coord_array[1][1] = 0.0F;
            loc_t_tex_coord_array[2][0] = 1.0F;
            loc_t_tex_coord_array[2][1] = 1.0F;
            loc_t_tex_coord_array[3][0] = 0.0F;
            loc_t_tex_coord_array[3][1] = 1.0F;

            loc_t_vertex_array[0][0] = loc_f_x1;
            loc_t_vertex_array[0][1] = loc_f_y1;
            loc_t_vertex_array[1][0] = loc_f_x2;
            loc_t_vertex_array[1][1] = loc_f_y1;
            loc_t_vertex_array[2][0] = loc_f_x2;
            loc_t_vertex_array[2][1] = loc_f_y2;
            loc_t_vertex_array[3][0] = loc_f_x1;
            loc_t_vertex_array[3][1] = loc_f_y2;
            sglDisable(SGL_MODULATE);

            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

            oglxEnable(GL_BLEND);

            glActiveTexture(GL_TEXTURE0);

            oglxBindTexture((GLuint) loc_ul_index);
            oglxDrawTexVertexArray_es2(GL_TRIANGLE_FAN, 4, &(loc_t_vertex_array[0][0]), SGL_FALSE);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifndef SC2_DEV_ENV
        }
#endif
    }
    else {
        oglxSetError(SGL_ERROR_SGL_CALLSTATICSEQUENCE, par_ul_index);
    }
#endif

    sglRestoreStates();
    return;
}
#endif
/* End of File ***************************************************************/
