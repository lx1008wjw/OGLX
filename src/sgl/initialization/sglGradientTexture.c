/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGradientTexture.c
 DESCRIPTION       : sglGradientTexture shall load a gradient as texture
 COPYRIGHT (C)     : 2014 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/******************************************************************************
 **                           Includes
 *****************************************************************************/

/*+ Public interfaces +*/
#include "sgl.h"

/*+ Protected interfaces +*/
#include "sgl_private.h"
#include "mth.h"

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: sglGradientTexture
  DESCRIPTION:
    Function shall load a gradient as texture.
  PARAMETERS:
    par_l_index -> gradient index
    par_l_width -> texture width
	par_l_height -> texture height
	par_l_pixel_array_size -> size of par_p_pixel
	par_p_pixel -> input texture
	par_b_compressed -> true if input texture is compressed, false if it is not compressed
  RETURN:
    SGLulong -> the index of the texture to draw.
---------------------------------------------------------------------+*/
void sglGradientTexture(SGLlong par_l_index, SGLlong par_l_width, SGLlong par_l_height,
                        SGLlong par_l_pixel_array_size, const void *par_p_pixel, SGLbool par_b_compressed)
{
    SGLlong loc_l_width = mth_next_higher_power2(par_l_width, (SGLlong) SGL_TEXTURE_MIN_WIDTH, (SGLlong) glob_pr_sglStatemachine->ul_max_texture_width);
    SGLlong loc_l_height = mth_next_higher_power2(par_l_height, (SGLlong) SGL_TEXTURE_MIN_HEIGHT, (SGLlong) glob_pr_sglStatemachine->ul_max_texture_height);

    if ((par_l_index >= 0L) && (par_l_index < (SGLlong) glob_pr_sglStatemachine->ul_number_of_gradients)
        && (glob_pr_sglStatemachine->tub_texture_buffer != SGL_NULL)
        && (par_l_width >= 1L) && (par_l_height >= 1L) && (par_p_pixel != SGL_NULL)) {
        SGLulong loc_ul_texture_id = 0UL;
        if (par_b_compressed) {
            const SGLulong *loc_pul_pixel = (const SGLulong *) par_p_pixel;

            SGLbool loc_b_uncompressed = oglxUncompressBitmap(SGL_BITMAP_RGB_TRANSPARENT, par_l_width, par_l_height, par_l_pixel_array_size, loc_pul_pixel);

            if (loc_b_uncompressed) {
                oglxEnlargeTextureKeepDimensions(par_l_width, par_l_height, loc_l_width, loc_l_height, 4);
                loc_ul_texture_id = oglx_load_texture(loc_l_width, loc_l_height);
            }
            else {
                oglxSetError(SGL_ERROR_SGL_GRADIENTTEXTURE, (SGLulong) par_l_index);
            }
        }
        else {
            const SGLbyte *loc_pb_pixel = (const SGLbyte *) par_p_pixel;
            SGLulong loc_l_i;
            for (loc_l_i = 0UL; loc_l_i < (SGLulong) par_l_pixel_array_size; loc_l_i++) {
                glob_pr_sglStatemachine->tub_texture_buffer[loc_l_i] = (GLubyte) (loc_pb_pixel[loc_l_i]);
            }

            oglxEnlargeTextureKeepDimensions(par_l_width, par_l_height, loc_l_width, loc_l_height, 4);
            loc_ul_texture_id = oglx_load_texture(loc_l_width, loc_l_height);
        }
        glob_pr_sglStatemachine->p_gradient_attrib[par_l_index].ui_texture = loc_ul_texture_id;
        glob_pr_sglStatemachine->p_gradient_attrib[par_l_index].ul_textures_dimension[0] = sglInt32ToUInt32(par_l_width);
        glob_pr_sglStatemachine->p_gradient_attrib[par_l_index].ul_textures_dimension[1] = sglInt32ToUInt32(par_l_height);
        glob_pr_sglStatemachine->p_gradient_attrib[par_l_index].ul_dimension_power_2[0] = sglInt32ToUInt32(loc_l_width);
        glob_pr_sglStatemachine->p_gradient_attrib[par_l_index].ul_dimension_power_2[1] = sglInt32ToUInt32(loc_l_height);
        glob_pr_sglStatemachine->p_gradient_attrib[par_l_index].b_texture_specified_state = SGL_TRUE;
    }
    else {
        oglxSetError(SGL_ERROR_SGL_GRADIENTTEXTURE, (SGLulong) par_l_index);
    }

    return;
}

/* End of File ***************************************************************/
