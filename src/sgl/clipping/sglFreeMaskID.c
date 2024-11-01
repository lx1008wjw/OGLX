/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglFreeMaskID.c
 DESCRIPTION       : sglFreeMaskID command shall free given IDs through bitmask
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
  NAME: sglFreeMaskID
  DESCRIPTION:
    Function shall free given IDs through bitmask.
  PARAMETERS:
    par_ul_mask_combination -> IDs to free
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglFreeMaskID(SGLulong par_ul_mask_combination)
{
    SGLulong loc_ul_mask_combination = par_ul_mask_combination;
    if ((loc_ul_mask_combination & SGL_UNUSED_MASK_BITS) != 0x0UL) {
        oglxSetError(SGL_ERROR_SGL_FREEMASKID, (SGLulong) loc_ul_mask_combination);
        loc_ul_mask_combination = loc_ul_mask_combination & (~SGL_UNUSED_MASK_BITS);
    }
    else {
        /* Nothing to do */
    }

    glob_pr_sglStatemachine->ul_allocated_masks &= ~loc_ul_mask_combination;

    sglSetActiveMasks(glob_pr_sglStatemachine->ul_allocated_masks & glob_pr_sglStatemachine->ul_activated_masks);
    return;
}

/* End of File ***************************************************************/
