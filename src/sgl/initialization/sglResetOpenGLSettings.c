/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglResetOpenGLSettings.c
 DESCRIPTION       : sglResetOpenGLSettings shall reset specific OpenGL settings
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
  NAME: sglResetOpenGLSettings
  DESCRIPTION:
    Function shall reset specific OpenGL settings
  PARAMETERS:		
    par_ul_settings -> SGL_RESET_ALL_SETTINGS to reset all settings or bitmask between values:
					SGL_DISABLE_BLENDING
					SGL_DISABLE_TEXTURE_2D
					SGL_DISABLE_SCISSOR_TEST
					SGL_DISABLE_DEPTH_TEST
					SGL_DISABLE_CULL_FACE
					SGL_RESET_BLEND_FUNC
					SGL_DISABLE_LIGHTING
					SGL_ENABLE_LINE_SMOOTH
					SGL_ENABLE_VERTEX_ARRAY
					SGL_ENABLE_TEXTURE_ARRAY
					SGL_RESET_TEXTURE_ENV
  RETURN:
    void	
---------------------------------------------------------------------+*/
void sglResetOpenGLSettings(SGLulong par_ul_settings)
{
    if ((par_ul_settings & SGL_DISABLE_BLENDING) == SGL_DISABLE_BLENDING) {
        glDisable(GL_BLEND);
        glob_pr_sglStatemachine->r_ogl_state.b_state_blend = SGL_FALSE;
    }

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    if ((par_ul_settings & SGL_DISABLE_TEXTURE_2D) == SGL_DISABLE_TEXTURE_2D) {
        glDisable(GL_TEXTURE_2D);
        glob_pr_sglStatemachine->r_ogl_state.b_state_texture_2d = SGL_FALSE;
    }
#endif

    if ((par_ul_settings & SGL_DISABLE_SCISSOR_TEST) == SGL_DISABLE_SCISSOR_TEST) {
        glDisable(GL_SCISSOR_TEST);
        glob_pr_sglStatemachine->r_ogl_state.b_state_scissor_test = SGL_FALSE;
    }

    if ((par_ul_settings & SGL_DISABLE_DEPTH_TEST) == SGL_DISABLE_DEPTH_TEST) {
        glDisable(GL_DEPTH_TEST);
        glob_pr_sglStatemachine->r_ogl_state.b_state_depth_test = SGL_FALSE;
    }

    if ((par_ul_settings & SGL_DISABLE_CULL_FACE) == SGL_DISABLE_CULL_FACE) {
        glDisable(GL_CULL_FACE);
    }

    if ((par_ul_settings & SGL_RESET_BLEND_FUNC) == SGL_RESET_BLEND_FUNC) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
    if ((par_ul_settings & SGL_DISABLE_LIGHTING) == SGL_DISABLE_LIGHTING) {
        glDisable(GL_LIGHTING);
    }

    if ((par_ul_settings & SGL_ENABLE_LINE_SMOOTH) == SGL_ENABLE_LINE_SMOOTH) {
        glEnable(GL_LINE_SMOOTH);
    }

    if ((par_ul_settings & SGL_ENABLE_VERTEX_ARRAY) == SGL_ENABLE_VERTEX_ARRAY) {
        glEnableClientState(GL_VERTEX_ARRAY);
    }

    if ((par_ul_settings & SGL_ENABLE_TEXTURE_ARRAY) == SGL_ENABLE_TEXTURE_ARRAY) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if ((par_ul_settings & SGL_RESET_TEXTURE_ENV) == SGL_RESET_TEXTURE_ENV) {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLint) GL_REPLACE);
        glob_pr_sglStatemachine->r_ogl_state.ul_texture_target = ((SGLulong) GL_TEXTURE_ENV);
        glob_pr_sglStatemachine->r_ogl_state.ul_texture_pname = ((SGLulong) GL_TEXTURE_ENV_MODE);
        glob_pr_sglStatemachine->r_ogl_state.l_texture_param = ((SGLlong) GL_REPLACE);

    }
#else

    if ((par_ul_settings & SGL_ENABLE_VERTEX_ARRAY) == SGL_ENABLE_VERTEX_ARRAY) {
        glEnableVertexAttribArray((GLuint) (glob_pr_sglStatemachine->l_vertex_array_index));
    }

    if ((par_ul_settings & SGL_ENABLE_TEXTURE_ARRAY) == SGL_ENABLE_TEXTURE_ARRAY) {
        glEnableVertexAttribArray((GLuint) (glob_pr_sglStatemachine->l_texture_array_index));
    }

#endif

    return;
}

/* End of File ***************************************************************/
