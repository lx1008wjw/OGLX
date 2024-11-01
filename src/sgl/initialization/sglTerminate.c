/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglTerminate.c
 DESCRIPTION       : sglTerminate command shall delete loaded OpenGL textures 
					to free memory
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

#ifdef ES2_DEV_ENV

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglTerminate_es2
  DESCRIPTION:
    Function shall delete OpenGL ES2 program and shaders
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglTerminate_es2(void)
{
    /* Frees the OpenGL handles for the program and the 2 shaders */
    glDeleteProgram((GLuint) (glob_pr_sglStatemachine->ui_program_object));
    glDeleteShader((GLuint) (glob_pr_sglStatemachine->ui_frag_shader));
    glDeleteShader((GLuint) (glob_pr_sglStatemachine->ui_vert_shader));

    return;
}
#endif

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglTerminate
  DESCRIPTION:
    Function shall delete loaded OpenGL textures.
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglTerminate(void)
{
#if !defined(SC101_DEV_ENV) && !defined(SC2_DEV_ENV)
    SGLulong loc_ul_i;
    SGLulong loc_ul_number_of_textures = glob_pr_sglStatemachine->ul_number_of_textures;

    /* GLF */
    /* Simple Font */
    for (loc_ul_i = 0UL; loc_ul_i < GLF_MAX_NUMBER_OF_TEXTURE; loc_ul_i++) {
        if (glob_pr_sglStatemachine->tui_texture_id[loc_ul_i] != 0U) {
            GLuint loc_ui_id = (GLuint) (glob_pr_sglStatemachine->tui_texture_id[loc_ul_i]);
            glDeleteTextures(1, &loc_ui_id);
            glob_pr_sglStatemachine->tui_texture_id[loc_ul_i] = 0U;
        }
        else {
            /* Nothing to do */
        }
    }

    /* SGL */
    /* Delete SGL textures */
    for (loc_ul_i = 0UL; loc_ul_i < loc_ul_number_of_textures; loc_ul_i++) {
        /* Check texture attribute buffer is not NULL */
        if (glob_pr_sglStatemachine->p_texture_attrib != SGL_NULL) {
            if (glob_pr_sglStatemachine->p_texture_attrib[loc_ul_i].ui_texture != 0U) {
                GLuint loc_ui_texture = (GLuint) (glob_pr_sglStatemachine->p_texture_attrib[loc_ul_i].ui_texture);
                glDeleteTextures(1, &loc_ui_texture);
                glob_pr_sglStatemachine->p_texture_attrib[loc_ul_i].ui_texture = 0U;
                glob_pr_sglStatemachine->p_texture_attrib[loc_ul_i].b_texture_specified_state = SGL_FALSE;
            }
            else {
                /* Nothing to do */
            }
        }
    }

#endif

#ifdef ES2_DEV_ENV
    sglTerminate_es2();
#endif

    return;
}

/* End of File ***************************************************************/
