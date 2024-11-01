/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglBeginMaskDefinition.c
 DESCRIPTION       : sglBeginMaskDefinition command shall start a new mask definition
 COPYRIGHT (C)     : 2016 Esterel Technologies SAS. All Rights Reserved.
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
  NAME: oglx_select_mask
  DESCRIPTION:
    Function shall select active mask before its definition.
  PARAMETERS:
    par_ul_mask -> The mask to select.
	par_b_visible -> The visible side of the mask (SGL_VISIBLE_INSIDE or SGL_VISIBLE_OUTSIDE)
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglx_select_mask(SGLulong par_ul_mask, SGLbyte par_b_visible)
{
    glob_pr_sglStatemachine->ul_allocated_masks |= par_ul_mask;

    /* Switch on the stencil test (maybe off due to no active box or stencil) */
    oglxEnable((SGLulong) GL_STENCIL_TEST);

    /* Prevent OpenGL from writing to the color and depth buffer */
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    /* Open the corresponding stencil bits for writing the stencil */
    glStencilMask((GLuint) par_ul_mask);

    /* Clear the old stencil */
    if (par_b_visible == SGL_VISIBLE_INSIDE) {
        glClearStencil(0x0000);
        glClear(GL_STENCIL_BUFFER_BIT);

        glStencilFunc(GL_ALWAYS, OGLX_MASK_FFFF, (GLuint) par_ul_mask);
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    }
    else {
        glClearStencil(OGLX_MASK_FFFF);
        glClear(GL_STENCIL_BUFFER_BIT);

        glStencilFunc(GL_ALWAYS, 0x0, (GLuint) par_ul_mask);
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    }

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglx_get_mask_id
  DESCRIPTION:
    Function shall return a non allocated mask id.
  PARAMETERS:
    None
  RETURN:
    The allocated mask id or SGL_NO_MASK is none is available.
---------------------------------------------------------------------+*/
SGLulong oglx_get_mask_id(void)
{
    SGLulong loc_ul_return = SGL_NO_MASK;
    SGLulong loc_ul_i = 0UL;
    SGLbool loc_b_new_mask = SGL_FALSE;

    while ((loc_ul_i < SGL_MAX_MASKS_NUMBER) && (!loc_b_new_mask)) {
        SGLulong loc_ul_mask = (SGLulong) (glob_pr_sglStatemachine->ul_allocated_masks >> loc_ul_i);
        if ((loc_ul_mask & 0x1UL) == 0x0UL) {
            loc_ul_return = (SGLulong) (0x1UL << loc_ul_i);
            loc_b_new_mask = SGL_TRUE;
        }

        loc_ul_i++;
    }

    return loc_ul_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglBeginMaskDefinition
  DESCRIPTION:
    Function shall start a new mask definition
  PARAMETERS:
    par_b_visible -> To define the visible part of the mask (SGL_VISIBLE_INSIDE or SGL_VISIBLE_OUTSIDE)
  RETURN:
    Index of the mask in range [0,7]
---------------------------------------------------------------------+*/
SGLulong sglBeginMaskDefinition(SGLbyte par_b_visible)
{
    SGLulong loc_ul_return = SGL_NO_MASK;
    if ((par_b_visible == SGL_VISIBLE_INSIDE) || (par_b_visible == SGL_VISIBLE_OUTSIDE)) {
        loc_ul_return = oglx_get_mask_id();

        if (loc_ul_return != SGL_NO_MASK) {
            oglx_select_mask(loc_ul_return, par_b_visible);
            glob_pr_sglStatemachine->ul_mask_definition = loc_ul_return;
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_BEGINMASKDEFINITION, 0UL);
    }
    return loc_ul_return;
}

/* End of File ***************************************************************/
