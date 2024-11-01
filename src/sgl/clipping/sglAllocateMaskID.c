/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglAllocateMaskID.c
 DESCRIPTION       : sglAllocateMaskID command shall return next available mask id for given type
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
  NAME: sglAllocateMaskID
  DESCRIPTION:
    Function shall return next available mask id for given type.
  PARAMETERS:
    par_b_mask_kind -> kind of mask: SGL_TYPE_CLIP_PLANE, SGL_TYPE_CLIP_BOX or SGL_TYPE_STENCIL
  RETURN:
    SGLulong -> next available mask ID or SGL_NO_MASK
---------------------------------------------------------------------+*/
SGLulong sglAllocateMaskID(SGLbyte par_b_mask_kind)
{
    SGLulong loc_b_return = SGL_NO_MASK;

    switch (par_b_mask_kind) {
#ifndef OGLX_NO_GLCLIPPLANE
    case SGL_TYPE_CLIP_PLANE:
    {
        SGLulong loc_ul_i = SGL_CLIP_PLANE0;
        while (((glob_pr_sglStatemachine->ul_allocated_masks & loc_ul_i) != 0x0UL) && (loc_ul_i <= SGL_CLIP_PLANE5)) {
            loc_ul_i = loc_ul_i << 1U;
        }

        if (loc_ul_i <= SGL_CLIP_PLANE5) {
            loc_b_return = loc_ul_i;
            glob_pr_sglStatemachine->ul_allocated_masks |= loc_b_return;
        }
        else {
            /* Nothing to do */
        }
    }
        break;
#endif
    case SGL_TYPE_CLIP_BOX:
    case SGL_TYPE_STENCIL:
        loc_b_return = oglx_get_mask_id();
        if (loc_b_return != SGL_NO_MASK) {
            glob_pr_sglStatemachine->ul_allocated_masks |= loc_b_return;
        }
        break;
    default:
        oglxSetError(SGL_ERROR_SGL_ALLOCATEMASKID, (SGLulong) par_b_mask_kind);
        break;
    }

    return loc_b_return;
}

/* End of File ***************************************************************/
