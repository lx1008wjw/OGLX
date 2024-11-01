/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxDrawTexVertexArray.c
 DESCRIPTION       : Table command calls for vertices
 COPYRIGHT (C)     : 2010 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/
/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"

#if defined(ES2_DEV_ENV) || defined(SC2_DEV_ENV)
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxSetTextureState
  DESCRIPTION:
    Function shall set texture state in shaders.
  PARAMETERS:
    par_i_state -> the texture state to use in shaders
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxSetTextureState(SGLlong par_i_state)
{
    if (glob_pr_sglStatemachine->i_tex_on_state != par_i_state) {
        glUniform1i((GLint) glob_pr_sglStatemachine->i_tex_on_location, (GLint) par_i_state);
        glob_pr_sglStatemachine->i_tex_on_state = par_i_state;
    }
    else {
        /* Nothing to do */
    }
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxDrawTexVertexArray_es2
  DESCRIPTION:
    Function shall Call OpenGL commands to draw the figure 
    defined by given vertices and drawing mode and applying 
    given texture.
  PARAMETERS:
    par_e_mode -> Drawing mode (GL_TRIANGLE_FAN or GL_TRIANGLE_STRIP)
    par_i_count  -> number of vertices, range: [3, SGL_MAX_VERTEX_ARRAY_SIZE]
    par_pf_vertices -> vertices describing the figure
    par_pf_texcoord -> texture coordinates
    par_b_apply_color -> SGL_TRUE if color shall be applied on texture (for a textured font), SGL_FALSE otherwise
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxDrawTexVertexArray_es2(SGLulong par_e_mode, SGLlong par_i_count, const SGLfloat * par_pf_vertices, SGLbool par_b_apply_color)
{
    /* Set color at index SGL_COLOR */
    if (par_b_apply_color) {
        oglxUpdateColor_es2(glob_pr_sglStatemachine->r_current_color.tf_current_color);
        oglxSetTextureState(4);
    }
    else {
        if (glob_pr_sglStatemachine->b_modulate) {
            oglxUpdateColor_es2(glob_pr_sglStatemachine->r_current_color.tf_current_color);
            oglxSetTextureState(6);
        }
        else {
            oglxSetTextureState(2);
        }
    }

    if (!(glob_pr_sglStatemachine->b_tex_coord_array)) {
        glEnableVertexAttribArray((GLuint) (glob_pr_sglStatemachine->l_texture_array_index));
        glob_pr_sglStatemachine->b_tex_coord_array = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Sets the vertex data to this attribute index */
    if (glob_pr_sglStatemachine->p_current_vertex_array != par_pf_vertices) {
        glVertexAttribPointer((GLuint) (glob_pr_sglStatemachine->l_vertex_array_index), 2, GL_FLOAT, GL_FALSE, 0, par_pf_vertices);
        glob_pr_sglStatemachine->p_current_vertex_array = (const void *) par_pf_vertices;
    }
    else {
        /* Nothing to do */
    }
    glDrawArrays((GLenum) par_e_mode, 0, (GLsizei) par_i_count);
    return;
}
#else

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxDrawTexVertexArray
  DESCRIPTION:
    Function shall Call OpenGL commands to draw the figure 
    defined by given vertices and drawing mode and applying 
    given texture.
  PARAMETERS:
    par_e_mode -> Drawing mode (GL_TRIANGLE_FAN or GL_TRIANGLE_STRIP)
    par_i_count  -> number of vertices, range: [3, SGL_MAX_VERTEX_ARRAY_SIZE]
    par_pf_vertices -> vertices describing the figure
    par_pf_texcoord -> texture coordinates
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxDrawTexVertexArray(SGLulong par_e_mode, SGLlong par_i_count, const SGLfloat * par_pf_vertices, const SGLfloat * par_pf_texcoord)
{
#ifndef OGLX_NO_VERTEX_ARRAY
#ifdef OGLX_DISPLAY_LISTS
    if (!(glob_pr_sglStatemachine->b_static_sequence_started)) {
#endif
        if (!(glob_pr_sglStatemachine->b_tex_coord_array)) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glob_pr_sglStatemachine->b_tex_coord_array = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }

        if (glob_pr_sglStatemachine->p_current_vertex_array != par_pf_vertices) {
            glVertexPointer(2, GL_FLOAT, 0, par_pf_vertices);
            glob_pr_sglStatemachine->p_current_vertex_array = (const void *) par_pf_vertices;
        }
        else {
            /* Nothing to do */
        }
        glDrawArrays((GLenum) par_e_mode, 0, (GLsizei) par_i_count);
#ifdef OGLX_DISPLAY_LISTS
    }
    else {
        SGLlong loc_l_i;
        glBegin((GLenum) par_e_mode);
        for (loc_l_i = 0; loc_l_i < (par_i_count * 2); loc_l_i += 2) {
#ifndef SC101_DEV_ENV
            glTexCoord2f(par_pf_texcoord[loc_l_i], par_pf_texcoord[loc_l_i + 1]);
#else
            glMultiTexCoord2f(GL_TEXTURE0, par_pf_texcoord[loc_l_i], par_pf_texcoord[loc_l_i + 1]);
#endif
            glVertex2f(par_pf_vertices[loc_l_i], par_pf_vertices[loc_l_i + 1]);
        }
        glEnd();
    }
#endif
#else
    SGLlong loc_l_i;
    glBegin((GLenum) par_e_mode);
    for (loc_l_i = 0; loc_l_i < (par_i_count * 2); loc_l_i += 2) {
#ifndef SC101_DEV_ENV
        glTexCoord2f(par_pf_texcoord[loc_l_i], par_pf_texcoord[loc_l_i + 1]);
#else
        glMultiTexCoord2f(GL_TEXTURE0, par_pf_texcoord[loc_l_i], par_pf_texcoord[loc_l_i + 1]);
#endif
        glVertex2f(par_pf_vertices[loc_l_i], par_pf_vertices[loc_l_i + 1]);
    }
    glEnd();
#endif
}
#endif
/* End of File ***************************************************************/
