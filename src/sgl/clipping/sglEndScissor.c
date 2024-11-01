/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglEndScissor.c
 DESCRIPTION       : sglEndScissor command shall restore previous scissor or disable scissor test
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
  NAME: sglEndScissor
  DESCRIPTION:
    Function shall restore previous scissor or disable scissor test.
  PARAMETERS:
    None
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglEndScissor(void)
{
    if (glob_pr_sglStatemachine->ul_number_of_scissors > 0U) {
        glob_pr_sglStatemachine->ul_number_of_scissors--;

        if (glob_pr_sglStatemachine->ul_number_of_scissors > 0U) {
            SGLulong loc_ul_index = glob_pr_sglStatemachine->ul_number_of_scissors - 1U;

            /* Activate last scissor on stack */
            glScissor((GLint) (glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_index].i_x),
                      (GLint) (glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_index].i_y),
                      (GLsizei) (glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_index].s_width),
                      (GLsizei) (glob_pr_sglStatemachine->ts_scissor_stack[loc_ul_index].s_height));
        }
        else {
            /* Disable scissor test if there is no more scissor on stack */
            oglxDisable((SGLulong) GL_SCISSOR_TEST);
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_ENDSCISSOR, 0);
    }
}

/* End of File ***************************************************************/
