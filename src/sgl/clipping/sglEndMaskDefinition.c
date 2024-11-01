/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglEndMaskDefinition.c
 DESCRIPTION       : sglEndMaskDefinition command shall end a new mask definition
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
  NAME: sglEndMaskDefinition
  DESCRIPTION:
    Function shall end a new mask definition
  PARAMETERS:
    None
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglEndMaskDefinition(void)
{
    /* allow drawing to color */
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glStencilMask(0x0U);

    oglx_set_active_masks(glob_pr_sglStatemachine->ul_activated_masks & SGL_NO_CLIP_PLANE);

    glob_pr_sglStatemachine->ul_mask_definition = SGL_NO_MASK;
    return;
}

/* End of File ***************************************************************/
