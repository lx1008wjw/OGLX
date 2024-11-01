/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglIndexLineStipplei.c
 DESCRIPTION       : sglIndexLineStipplei command shall set the current linestipple 
					index
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
  NAME: sglIndexLineStipplei
  DESCRIPTION:
    Function shall set the current linestipple index.
  PARAMETERS:
    par_l_index -> line stipple index, range: [0,glob_pr_sglStatemachine->ul_nb_line_stipple-1]
  RETURN:
    void
---------------------------------------------------------------------+*/
void sglIndexLineStipplei(SGLlong par_l_index)
{
    SGLlong loc_l_index = par_l_index;
    if (loc_l_index >= 0) {
        /* Report an OGLX error if the input parameter has an incorrect value */
        if (((SGLulong) loc_l_index) < glob_pr_sglStatemachine->ul_nb_line_stipple) {
            /* Nothing to do */
        }
        else {
            oglxSetError(SGL_ERROR_SGL_INDEXLINESTIPPLE, (SGLulong) loc_l_index);
            /* set the index to the last index by default */
            loc_l_index = (SGLlong) (glob_pr_sglStatemachine->ul_nb_line_stipple - 1U);
        }

        glob_pr_sglStatemachine->pr_stipple = &(glob_pr_sglStatemachine->tr_linestipple[loc_l_index]);

        if (glob_pr_sglStatemachine->pr_stipple->b_all_on == 0U) {
            glob_pr_sglStatemachine->b_stipple_ok = SGL_TRUE;
        }
        else {
            glob_pr_sglStatemachine->b_stipple_ok = SGL_FALSE;
        }
    }
    else {
        oglxSetError(SGL_ERROR_SGL_INDEXLINESTIPPLE, (SGLulong) loc_l_index);
    }

    return;
}

/* End of File ***************************************************************/
