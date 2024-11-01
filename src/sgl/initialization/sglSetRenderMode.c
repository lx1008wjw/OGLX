/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetRenderMode.c
 DESCRIPTION       : sglSetRenderMode command shall select the rendering mode 
					for line drawing: smooth lines or raw lines
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
  NAME: sglSetRenderMode
  DESCRIPTION:
    Function shall select the rendering mode for line drawing: smooth lines or raw lines.
  PARAMETERS:
    par_b_mode -> Mode to draw OpenGL lines (SGL_RAW_OPENGL_LINES or SGL_SMOOTH_LINES)
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglSetRenderMode(SGLbyte par_b_mode)
{
    /* Store new render mode in global OGLX context */
    if (par_b_mode == SGL_RAW_OPENGL_LINES) {
        glob_pr_sglStatemachine->b_line_rendering_mode = SGL_RAW_OPENGL_LINES;
    }
    else {
        if (par_b_mode == SGL_SMOOTH_LINES) {
            glob_pr_sglStatemachine->b_line_rendering_mode = SGL_SMOOTH_LINES;
            glob_pr_sglStatemachine->f_last_ogl_linewidth = 0.0F;
        }
        else {
            /* Nothing to do */
        }
    }

    return;
}

/* End of File ***************************************************************/
