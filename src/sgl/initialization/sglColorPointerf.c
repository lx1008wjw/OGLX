/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglColorPointerf.c
 DESCRIPTION       : sglColorPointerf shall define the colormap 
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
  NAME: sglColorPointerf
  DESCRIPTION:
    Function shall store the given colormap.
  PARAMETERS:		
    par_pb_colormap -> Colormap            
  RETURN:
    void	
---------------------------------------------------------------------+*/
void sglColorPointerf(const sgl_color * par_ps_colormap, SGLulong par_ul_nb_colors)
{
    if ((par_ps_colormap != SGL_NULL) && (par_ul_nb_colors != 0UL)) {
        glob_pr_sglStatemachine->tr_color = par_ps_colormap;
        glob_pr_sglStatemachine->ul_nb_colors = par_ul_nb_colors;
    }
    else {
        oglxSetError(SGL_ERROR_SGL_COLORPOINTERF, 0U);
    }

    return;
}

/* End of File ***************************************************************/
