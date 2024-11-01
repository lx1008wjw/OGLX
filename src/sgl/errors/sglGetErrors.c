/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : sglGetErrors.c
 DESCRIPTION       : sglGetErrors command shall fill the error buffer given as 
					parameter
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
  NAME: sglGetErrors
  DESCRIPTION:
    Function shall fill the error buffer given as parameter.
  PARAMETERS:	
    par_pul_error_buffer -> Address to store the array of errors
    par_pul_error_number -> Address to store the number of errors to read
  RETURN:			
    SGLbyte -> state of the returned error stack 
    SGL_NO_ERROR: empty error stack
    SGL_ERROR_STACK_UNDERFLOW: not a full error stack
    SGL_ERROR_STACK_OVERFLOW: a full error stack 
---------------------------------------------------------------------+*/
SGLbyte sglGetErrors(SGLulong * par_pul_error_buffer, SGLulong * par_pul_error_number)
{
    SGLbyte loc_b_return_value = SGL_NO_ERROR;
    SGLulong loc_ul_index;
    SGLbyte loc_b_parameters_ok = 1U;
    SGLulong *loc_pul_error_buffer = par_pul_error_buffer;

    /* Check error buffer is not NULL */
    if (loc_pul_error_buffer == SGL_NULL) {
        loc_b_parameters_ok = 0U;
    }
    else {
        /* Nothing to do */
    }

    /* Check number buffer is not NULL */
    if (par_pul_error_number == SGL_NULL) {
        loc_b_parameters_ok = 0U;
    }
    else {
        /* Nothing to do */
    }

    if (loc_b_parameters_ok != 0U) {
        if (glob_pr_sglStatemachine->ul_error_counter == 0U) {
            /* If there is no error it shall be stated by return value of the function (set to SGL_NO_ERROR) */
            *par_pul_error_number = 0UL;

            for (loc_ul_index = 0UL; loc_ul_index < SGL_ERROR_MAX; loc_ul_index++) {
                *loc_pul_error_buffer = 0UL;
                loc_pul_error_buffer++;
                *loc_pul_error_buffer = 0UL;
                loc_pul_error_buffer++;
            }
            /* Not necessary to delete the error table */
        }
        else {
            if (!(glob_pr_sglStatemachine->b_overflow_flag)) {
                /* Fill the given table with stored errors and set return value to SGL_ERROR_STACK_UNDERFLOW */
                loc_b_return_value = SGL_ERROR_STACK_UNDERFLOW;
                *par_pul_error_number = (SGLulong) glob_pr_sglStatemachine->ul_error_counter;

                for (loc_ul_index = 0UL; loc_ul_index < glob_pr_sglStatemachine->ul_error_counter; loc_ul_index++) {
                    *loc_pul_error_buffer = glob_pr_sglStatemachine->tul_error_table[loc_ul_index][0];
                    loc_pul_error_buffer++;
                    *loc_pul_error_buffer = glob_pr_sglStatemachine->tul_error_table[loc_ul_index][1];
                    loc_pul_error_buffer++;
                }

            }
            else {
                /* Fill the given table with stored errors and set return value to SGL_ERROR_STACK_OVERFLOW */
                loc_b_return_value = SGL_ERROR_STACK_OVERFLOW;
                *par_pul_error_number = (SGLulong) SGL_ERROR_MAX;

                for (loc_ul_index = 0UL; loc_ul_index < SGL_ERROR_MAX; loc_ul_index++) {
                    *loc_pul_error_buffer = glob_pr_sglStatemachine->tul_error_table[loc_ul_index][0];
                    loc_pul_error_buffer++;
                    *loc_pul_error_buffer = glob_pr_sglStatemachine->tul_error_table[loc_ul_index][1];
                    loc_pul_error_buffer++;
                }
            }
        }
    }
    else {
        /* Nothing to do */
    }
    return (loc_b_return_value);

}

/* End of File ***************************************************************/
