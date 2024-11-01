/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetActiveMasks.c
 DESCRIPTION       : sglSetActiveMasks command shall activate given masks
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
  NAME: oglx_set_active_masks
  DESCRIPTION:
    Function shall activate given set of masks.
  PARAMETERS:
    par_ul_mask_combination -> mask combination
  RETURN:
    void
---------------------------------------------------------------------+*/
void oglx_set_active_masks(SGLulong par_ul_mask_combination)
{
    if (par_ul_mask_combination != SGL_NO_MASK) {
        oglxEnable((SGLulong) GL_STENCIL_TEST);

#if !defined(ES2_DEV_ENV) && !defined(SC2_DEV_ENV)
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#else
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
#endif
        glStencilFunc(GL_EQUAL, (GLint) par_ul_mask_combination, (GLuint) par_ul_mask_combination);
    }
    else {
        oglxDisable((SGLulong) GL_STENCIL_TEST);
    }

}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglSetActiveMasks
  DESCRIPTION:
    Function shall activate given set of masks.
  PARAMETERS:
    par_ul_mask_combination -> mask combination
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglSetActiveMasks(SGLulong par_ul_mask_combination)
{
    if ((par_ul_mask_combination & SGL_UNUSED_MASK_BITS) != 0x0UL) {
        oglxSetError(SGL_ERROR_SGL_SETACTIVEMASKS, (SGLulong) par_ul_mask_combination);
    }
    else {
        /* Nothing to do */
    }

    if (glob_pr_sglStatemachine->ul_activated_masks != par_ul_mask_combination) {
        glob_pr_sglStatemachine->ul_activated_masks = par_ul_mask_combination;

#ifndef OGLX_NO_GLCLIPPLANE
        if ((par_ul_mask_combination & SGL_CLIP_PLANE0) == SGL_CLIP_PLANE0) {
            sgl_enable_clip_plane(SGL_CLIP_PLANE0);
        }
        else {
            sgl_disable_clip_plane(SGL_CLIP_PLANE0);
        }

        if ((par_ul_mask_combination & SGL_CLIP_PLANE1) == SGL_CLIP_PLANE1) {
            sgl_enable_clip_plane(SGL_CLIP_PLANE1);
        }
        else {
            sgl_disable_clip_plane(SGL_CLIP_PLANE1);
        }

        if ((par_ul_mask_combination & SGL_CLIP_PLANE2) == SGL_CLIP_PLANE2) {
            sgl_enable_clip_plane(SGL_CLIP_PLANE2);
        }
        else {
            sgl_disable_clip_plane(SGL_CLIP_PLANE2);
        }

        if ((par_ul_mask_combination & SGL_CLIP_PLANE3) == SGL_CLIP_PLANE3) {
            sgl_enable_clip_plane(SGL_CLIP_PLANE3);
        }
        else {
            sgl_disable_clip_plane(SGL_CLIP_PLANE3);
        }

        if ((par_ul_mask_combination & SGL_CLIP_PLANE4) == SGL_CLIP_PLANE4) {
            sgl_enable_clip_plane(SGL_CLIP_PLANE4);
        }
        else {
            sgl_disable_clip_plane(SGL_CLIP_PLANE4);
        }

        if ((par_ul_mask_combination & SGL_CLIP_PLANE5) == SGL_CLIP_PLANE5) {
            sgl_enable_clip_plane(SGL_CLIP_PLANE5);
        }
        else {
            sgl_disable_clip_plane(SGL_CLIP_PLANE5);
        }
#endif

        oglx_set_active_masks(par_ul_mask_combination & SGL_NO_CLIP_PLANE);
    }


    return;
}

/* End of File ***************************************************************/
