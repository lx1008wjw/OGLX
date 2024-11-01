/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglIndexHaloColori.c
 DESCRIPTION       : sglIndexHaloColori command shall set the current halo color with 
					color set at the specified index
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
  NAME: sglIndexHaloColori
  DESCRIPTION:
    Function shall set the current halo color with color set 
    at the specified index.
  PARAMETERS:
    par_l_index -> color index, range: [0,glob_pr_sglStatemachine->ul_nb_colors-1]
  RETURN:
    void  
---------------------------------------------------------------------+*/
void sglIndexHaloColori(SGLlong par_l_index)
{

    SGLbool loc_b_error_detected = SGL_FALSE;
    SGLlong loc_l_index = par_l_index;

    if (loc_l_index >= (SGLlong) glob_pr_sglStatemachine->ul_nb_colors) {
        loc_b_error_detected = SGL_TRUE;
    }
    else {
        /* Nothing to do */
    }

    /* Check if input parameter has a valid value */
    if (!loc_b_error_detected) {
        /* Nothing to do */
    }
    else {
        oglxSetError(SGL_ERROR_SGL_INDEXHALOCOLORI, (SGLulong) loc_l_index);
        loc_l_index = (SGLlong) (glob_pr_sglStatemachine->ul_nb_colors - 1U);
    }

    glob_pr_sglStatemachine->l_halo_color_index = loc_l_index;
    glob_pr_sglStatemachine->b_drawing_mode = SGL_MODE_UNDEFINED;

    return;
}

/* End of File ***************************************************************/
