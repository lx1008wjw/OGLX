/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglIndexLineWidthi.c
 DESCRIPTION       : sglIndexLineWidthi command shall set the current linewidth 
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
  NAME: sglIndexLineWidthi
  DESCRIPTION:
    Function shall set the current linewidth index.
  PARAMETERS:
    par_l_index -> linewidth index, range: [0,glob_pr_sglStatemachine->ul_nb_line_width-1]
  RETURN:
    void	
---------------------------------------------------------------------+*/
void sglIndexLineWidthi(SGLlong par_l_index)
{
    SGLlong loc_l_index = par_l_index;
    if (loc_l_index >= 0) {
        /* Report an error and set default value if input parameter is invalid */
        if (loc_l_index >= (SGLlong) glob_pr_sglStatemachine->ul_nb_line_width) {
            oglxSetError(SGL_ERROR_SGL_INDEXLINEWIDTHI, (SGLulong) loc_l_index);
            loc_l_index = (SGLlong) (glob_pr_sglStatemachine->ul_nb_line_width - 1UL);
        }
        else {
            /* Nothing to do */
        }

        /* Store index line width in OGLX state machine */
        glob_pr_sglStatemachine->l_linewidth_index = loc_l_index;
    }
    else {
        oglxSetError(SGL_ERROR_SGL_INDEXLINEWIDTHI, (SGLulong) loc_l_index);
    }

    return;
}

/* End of File ***************************************************************/
