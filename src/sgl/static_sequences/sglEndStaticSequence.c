/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglEndStaticSequence.c
 DESCRIPTION       : sglEndStaticSequence shall end a sequence of static objects 
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
  NAME: sglEndStaticSequence
  DESCRIPTION:
    Function shall end a sequence of static objects 
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglEndStaticSequence(void)
{
    if (glob_pr_sglStatemachine->b_static_sequence_started) {
#ifdef OGLX_DISPLAY_LISTS
        glEndList();
#endif

#ifdef OGLX_FBO

#ifndef SC2_DEV_ENV
        GLuint loc_ui_framebuffer = (GLuint) glob_pr_sglStatemachine->ul_framebuffer;
#endif

        oglxPopMatrix();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifndef SC2_DEV_ENV
        glDeleteFramebuffers(1, &loc_ui_framebuffer);
#endif

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

        sglDisable(SGL_MODULATE);
#if defined(ES2_DEV_ENV) || defined(SC2_DEV_ENV)
        if (glob_pr_sglStatemachine->b_change_viewport) {
            sglViewport(glob_pr_sglStatemachine->i_prev_x1, glob_pr_sglStatemachine->i_prev_y1, glob_pr_sglStatemachine->i_prev_width,
                        glob_pr_sglStatemachine->i_prev_height);
            sglOrtho(glob_pr_sglStatemachine->f_prev_ortho_left, glob_pr_sglStatemachine->f_prev_ortho_right, glob_pr_sglStatemachine->f_prev_ortho_bottom,
                     glob_pr_sglStatemachine->f_prev_ortho_top);
            glob_pr_sglStatemachine->b_change_viewport = SGL_FALSE;
        }
#endif
        glob_pr_sglStatemachine->b_static_sequence_started = SGL_FALSE;
    }
    return;
}
#endif
/* End of File ***************************************************************/
