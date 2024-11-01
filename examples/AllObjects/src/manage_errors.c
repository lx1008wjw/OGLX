/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : manage_errors.c
 DESCRIPTION       : Management of errors in the application
 COPYRIGHT (C)     : 2015 by Esterel Technologies. All Rights Reserved.
 UNAUTHORIZED ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS PROHIBITED.
---------------------------------------------------------------------------- +*/

/******************************************************************************
 **                           Includes
 *****************************************************************************/
#include "sgl.h"
#include "oglxReadErrors.h"

/* Clear OGLX errors buffer */
void ClearErrors()
{
    /* OGLX interface sglClearErrors clears the OGLX internal error buffer, next call to sglGetErrors would return no error */
    sglClearErrors();
}

/* Set errors parameters */
void ReadErrors(SGLuint32 * nb_errors, SGLuint32 * last_error_id, SGLuint8 * buffer_status, SGLuint8(*error_name)[255UL])
{
    SGLuint32 loc_errors_number;
    SGLuint32 loc_errors[SGL_ERROR_MAX * 2];
    oglx_error *loc_decoded_errors;
    oglx_error loc_last_error;
    SGLulong loc_decoded_number;

    /*  OGLX interface sglGetErrors fills the input table with the 32 first errors since the start of the application or since last call to sgl ClearErrors */
    SGLbyte loc_b_status = sglGetErrors(loc_errors, &loc_errors_number);
    *nb_errors = loc_errors_number;

    /* Fill the last error index only if there is at least one error */
    if (loc_b_status != SGL_NO_ERROR) {
        SGLulong i = 0;
        *last_error_id = loc_errors[2 * (loc_errors_number - 1)];

        /* oglxReadErrors utility interface provide a table of errors as texts to understand easily which errors are produced */
        /* oglxReadErrors interface is provided in extras/utils */
        loc_decoded_errors = oglxReadErrors(&loc_decoded_number);
        loc_last_error = loc_decoded_errors[loc_decoded_number - 1];

        while (loc_last_error.s_error_name[i] != 0 && i < 255) {
            (*error_name)[i] = loc_last_error.s_error_name[i];
            i++;
        }
        (*error_name)[i] = 0;
    }
    else {
        *last_error_id = 0;
        (*error_name)[0] = 0;
    }
    *buffer_status = loc_b_status;
}
