/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglSetTextureInternalFormat.c
 DESCRIPTION       : sglSetTextureInternalFormat command shall select the internal 
					texture mode to store texture with transparency and without 
					transparency in the OpenGL driver
 *  COPYRIGHT (C)     : 2010 Esterel Technologies SAS. All Rights Reserved.
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
  NAME: sglSetTextureInternalFormat
  DESCRIPTION:
    Function shall select the internal texture mode to 
    store texture with transparency and without transparency 
    in the OpenGL driver.
  PARAMETERS:		
    par_l_tex_format_without_alpha -> to select the internal texture format mode for the OpenGL function glTexImage2D 
	  (parameter internalFormat) when texture without transparency is loaded.
	  The possible values for the internal texture format mode depend on the OpenGL driver (the most common one is GL_RGB).
    par_l_tex_format_with_alpha -> to select the internal texture format mode for the OpenGL function glTexImage2D 
	  (parameter internalFormat) when texture with transparency is loaded. 
	  The possible values for the internal texture format mode depend on the OpenGL driver (the most common one is GL_RGBA).
  RETURN:
    None
---------------------------------------------------------------------+*/
void sglSetTextureInternalFormat(SGLlong par_l_tex_format_without_alpha, SGLlong par_l_tex_format_with_alpha)
{
    /* Store texture internal format in OGLX context */
    glob_pr_sglStatemachine->i_texture_internal_format = (GLint) par_l_tex_format_without_alpha;
    glob_pr_sglStatemachine->i_alpha_texture_internal_format = (GLint) par_l_tex_format_with_alpha;

    return;
}

/* End of File ***************************************************************/
