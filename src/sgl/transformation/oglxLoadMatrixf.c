/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglxLoadMatrixf.c
 DESCRIPTION       : oglxLoadMatrixf command shall load given matrix.
 COPYRIGHT (C)     : 2008 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"

#if defined(ES2_DEV_ENV) || defined(SC2_DEV_ENV)
/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: oglxLoadMatrixf
  DESCRIPTION:
    Function shall load given matrix.
  PARAMETERS:
    par_pf_matrix -> the matrix to load
  RETURN:
    None
---------------------------------------------------------------------+*/
void oglxLoadMatrixf(const SGLfloat * par_pf_matrix)
{
    glUniformMatrix4fv((GLint) (glob_pr_sglStatemachine->i_matrix_location), 1, GL_FALSE, par_pf_matrix);
    return;
}
#endif

/* End of File ***************************************************************/
