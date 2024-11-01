/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetLinesSeparators.c
 DESCRIPTION       : sglSetLinesSeparators command shall fill the lines separators table
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
  NAME: sglSetLinesSeparators
  DESCRIPTION:
    Function shall fill the lines separators table.
  PARAMETERS:
    par_pb_separators -> table containing separators
    par_ul_nb_separators -> number of separators
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglSetLinesSeparators(SGLbyte * par_pb_separators, SGLulong par_ul_nb_separators)
{
    if ((par_ul_nb_separators <= SGL_MAX_SEPARATORS) && (par_pb_separators != SGL_NULL)) {
        SGLulong loc_ul_i;
        for (loc_ul_i = 0UL; loc_ul_i < par_ul_nb_separators; loc_ul_i++) {
            glob_pr_sglStatemachine->tb_line_separators[loc_ul_i] = par_pb_separators[loc_ul_i];
        }
        glob_pr_sglStatemachine->ul_nb_line_separators = par_ul_nb_separators;
    }
    else {
        if (par_pb_separators != SGL_NULL) {
            oglxSetError(SGL_ERROR_SGL_SETLINESSEPARATORS, (SGLulong) par_ul_nb_separators);
        }
        else {
            oglxSetError(SGL_ERROR_SGL_SETLINESSEPARATORS, 0UL);
        }
    }
}

/* End of File ***************************************************************/
