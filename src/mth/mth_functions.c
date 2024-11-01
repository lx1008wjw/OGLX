/** FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : mth_functions.c
 DESCRIPTION       : Set of mathematical functions
 COPYRIGHT (C)     : 2008 Esterel Technologies SAS. All Rights Reserved.
 ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS GOVERNED BY ESTEREL TECHNOLOGIES LICENSING CONDITIONS.
---------------------------------------------------------------------------- **/

/*+ Public interfaces +*/
#include "sgl_types.h"
#include "sglConstants.h"
#include "sgl.h"

/*+ Protected interfaces +*/
#include "mth.h"

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_f_round
  DESCRIPTION:
    This function shall round given float to the closest entire float value. 
  PARAMETERS:
    par_f_value -> The value to round.
  RETURN:
    SGLlong -> The result value.
---------------------------------------------------------------------+*/
SGLfloat mth_f_round(SGLfloat par_f_value)
{
    SGLfloat loc_f_result;

    if ((par_f_value < SGLint32_to_float(MTH_MIN_LONG_CONV))
        || (par_f_value > SGLint32_to_float(MTH_MAX_LONG_CONV))) {
        loc_f_result = par_f_value;
    }
    else {
        if (par_f_value > 0.0F) {
            loc_f_result = SGLint32_to_float(mth_f_to_l(par_f_value));
        }
        else {
            loc_f_result = -(SGLint32_to_float(mth_f_to_l(par_f_value)));
        }
    }

    return loc_f_result;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_f_to_l
  DESCRIPTION:
    This function shall round given float to the closest long value and return the absolute value of it. 
    {
    If p_f_value is in range [0.0,MTH_MAX_LONG_CONV], function shall round p_f_value to the closest long value and return it. 
    If p_f_value is in range [MTH_MIN_LONG_CONV,0.0], function shall round p_f_value to the closest long value and return the absolute value of it. 
	If p_f_value is exactly between two integer values (decimal part is .5) the upper long value shall be returned.
    If p_f_value is strictly greater than MTH_MAX_LONG_CONV, function shall return MTH_MAX_LONG_CONV. 
    If p_f_value is strictly less than MTH_MIN_LONG_CONV, function shall return MTH_MAX_LONG_CONV. 
    MTH_MIN_LONG_CONV is -99999.
    MTH_MAX_LONG_CONV is 99999.
    }
  PARAMETERS:
    par_f_value -> The value to convert.
  RETURN:
    SGLlong -> The result long value, range:[0,MTH_MAX_LONG_CONV].
---------------------------------------------------------------------+*/
SGLlong mth_f_to_l(SGLfloat par_f_value)
{
    SGLlong loc_l_return;

    if (par_f_value < SGLint32_to_float(MTH_MIN_LONG_CONV)) {
        loc_l_return = MTH_MAX_LONG_CONV;
    }
    else {
        if (par_f_value > SGLint32_to_float(MTH_MAX_LONG_CONV)) {
            loc_l_return = MTH_MAX_LONG_CONV;
        }
        else {
            if (par_f_value >= 0.0F) {
                loc_l_return = (SGLlong) (par_f_value + 0.5F);
            }
            else {
                loc_l_return = (SGLlong) (-par_f_value + 0.5F);
            }
        }
    }
    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_f_to_l_ceil
  DESCRIPTION:
    This function shall compute the smallest long value that is still bigger than a given float (the ceil value). 
    If par_f_value is equal to an integer with a precision of SGL_MIN_F, this integer shall be returned.
    SGL_MIN_F is 0.000001
    The normal range of par_f_value is [MTH_MIN_LONG_CONV,MTH_MAX_LONG_CONV].
    {
	If par_f_value is less or equal than MTH_MIN_LONG_CONV the returned value shall be MTH_MIN_LONG_CONV.
	If par_f_value is greater or equal than MTH_MAX_LONG_CONV the returned value shall be MTH_MAX_LONG_CONV.
    MTH_MIN_LONG_CONV is -99999.
    MTH_MAX_LONG_CONV is 99999.
    }
  PARAMETERS:
    par_f_value -> The input float.
  RETURN:     
    SGLlong -> The result ceil value, range: [MTH_MIN_LONG_CONV,MTH_MAX_LONG_CONV].
---------------------------------------------------------------------+*/
SGLlong mth_f_to_l_ceil(SGLfloat par_f_value)
{
    SGLlong loc_l_result;

    if (par_f_value <= SGLint32_to_float(MTH_MIN_LONG_CONV)) {
        loc_l_result = MTH_MIN_LONG_CONV;
    }
    else {
        if (par_f_value >= SGLint32_to_float(MTH_MAX_LONG_CONV)) {
            loc_l_result = MTH_MAX_LONG_CONV;
        }
        else {
            SGLlong loc_l_value = mth_f_to_l(par_f_value);
            SGLfloat loc_f_abs_value = mth_f_abs(par_f_value);
            if (mth_f_abs(loc_f_abs_value - ((SGLfloat) loc_l_value)) < SGL_MIN_F) {
                loc_l_result = (SGLlong) par_f_value;
            }
            else {
                if (par_f_value >= 0.0F) {
                    loc_l_result = (SGLlong) par_f_value + 1L;
                }
                else {
                    loc_l_result = (SGLlong) par_f_value;
                }
            }
        }
    }
    return loc_l_result;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_move_in_good_quadrant
  DESCRIPTION:
    This function shall modify given angle according to the 
    quadrant the starting point of an arc is positionned in. 
    The resulted angle shall be computed as follows: 
    {
    - If par_f_deltax is greater or equal to 0.0 and par_f_delta_y is strictly less than 0.0, the returned value shall be -par_f_angle;
    - If par_f_deltax is greater or equal to 0.0 and par_f_delta_y is greater or equal to 0.0, the returned value shall be par_f_angle;
    - If par_f_deltax is strictly less than 0.0 and par_f_delta_y is strictly less than 0.0, the returned value shall be par_f_angle - 180.0;
    - If par_f_deltax is strictly less than 0.0 and par_f_delta_y is greater or equal to 0.0, the returned value shall be 180.0 - par_f_angle;
    }
    {
    If we represent the inputs in four quadrants given by par_f_deltax and par_f_deltay (i.e. two vectors) the algorithm is equivalent to the 
    following scheme where the origin of the landmark 
    is the center of the arc. Each quadrant represents the possible positions 
    of starting point.

                            |											
    180.0 - par_f_angle		|		par_f_angle
    ________________________|__________________________
                            |                          
    par_f_angle - 180.0     |      -par_f_angle
                            |
	}
  PARAMETERS:  
    par_f_angle -> original angle
    par_f_deltax -> difference between x coordinates of starting point and center of the arc
    par_f_deltay -> difference between y coordinates of starting point and center of the arc
  RETURN:
    SGLfloat -> the computed angle.  
---------------------------------------------------------------------+*/
SGLfloat mth_move_in_good_quadrant(SGLfloat par_f_angle, SGLfloat par_f_deltax, SGLfloat par_f_deltay)
{
    SGLfloat loc_f_angle = par_f_angle;

    if (par_f_deltax >= (SGLfloat) 0.0F) {
        if (par_f_deltay < (SGLfloat) 0.0F) {
            loc_f_angle = -loc_f_angle;
        }
        else {
            /* Nothing to do */
        }
    }
    else {
        loc_f_angle = (SGLfloat) 180.0F - loc_f_angle;
        if ((par_f_deltay) < (SGLfloat) 0.0F) {
            loc_f_angle = -loc_f_angle;
        }
        else {
            /* Nothing to do */
        }
    }

    return loc_f_angle;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_integer_to_string
  DESCRIPTION:
    This function shall convert given integer to a string in given string. 
    It shall also fill the length of the string.
  PARAMETERS:
    par_l_value -> the value to convert
    par_b_out_string -> the converted string, it is an output of the function.
    par_l_length -> the length of the output string, it is an output of the function.
  RETURN:	 
    None
---------------------------------------------------------------------+*/
void mth_integer_to_string(SGLlong par_l_value, SGLbyte par_b_out_string[], SGLlong * par_l_length)
{
    SGLlong loc_l_remainer;
    SGLlong loc_l_i;
    SGLlong loc_l_j;
    SGLbyte loc_b_str[32];
    SGLlong loc_l_length;
    SGLlong loc_l_digit;
    SGLlong loc_l_value = par_l_value;

    /* Conversion of the integer into a string */
    if (loc_l_value == 0) {
        /* Default value for zero value */
        loc_l_length = 1;
        par_b_out_string[0] = (SGLbyte) 0x30;
    }
    else {
        /* Absolute value used in case of negative value */
        if (loc_l_value < 0) {
            loc_l_value = -loc_l_value;
        }
        else {
            /* Nothing to do */
        }

        loc_l_remainer = loc_l_value;
        loc_l_length = 0;

        while (loc_l_remainer != 0) {
            loc_l_digit = loc_l_remainer % 10;
            /* Add current digit to the string, 0x30 is character '0' */
            loc_b_str[loc_l_length] = (SGLbyte) (loc_l_digit + 0x30L);
            loc_l_length++;
            loc_l_remainer = loc_l_remainer / 10L;
        }

        /* Revert the content of the string */
        loc_l_j = 0L;
        loc_l_i = loc_l_length;
        while (loc_l_i > 0) {
            loc_l_i--;
            par_b_out_string[loc_l_j] = loc_b_str[loc_l_i];
            loc_l_j++;
        }
    }

    /* Add a null character 0x0 */
    par_b_out_string[loc_l_length] = 0x0U;

    /* Fill the length */
    *par_l_length = loc_l_length;

    return;

}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_exponent
  DESCRIPTION:
    This function shall compute par_l_value^par_l_exponent.
  PARAMETERS:
    par_l_value -> the value
    par_l_exponent -> the exponent
  RETURN:	 
    SGLlong -> par_l_value^par_l_exponent
---------------------------------------------------------------------+*/
SGLlong mth_exponent(SGLlong par_l_value, SGLlong par_l_exponent)
{
    SGLlong loc_l_return = 1L;
    SGLlong loc_l_i;

    if (par_l_value == 0L) {
        if (par_l_exponent == 0L) {
            /* loc_l_return = 1L; */
        }
        else {
            loc_l_return = 0L;
        }
    }
    else {
        if (par_l_exponent >= 0) {
            loc_l_i = par_l_exponent;
        }
        else {
            loc_l_i = -par_l_exponent;
        }

        while (loc_l_i > 0) {
            loc_l_i--;
            if (par_l_exponent >= 0) {
                loc_l_return = loc_l_return * par_l_value;
            }
            else {
                loc_l_return = loc_l_return / par_l_value;
            }
        }
    }

    return loc_l_return;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_strlen
  DESCRIPTION:
    This function shall return the length of given string (the index of first chracter which value is 0).
    If there is no 0 character before par_ul_max_string_length, the function shall return par_ul_max_string_length.
  PARAMETERS:
    par_pus_string -> the string
    par_ul_max_string_length -> the maximum length of the string
  RETURN:	 
    SGLulong -> the length of given string.
---------------------------------------------------------------------+*/
SGLulong mth_strlen(const SGLushort par_pus_string[], SGLulong par_ul_max_string_length)
{
    SGLulong loc_ul_i = 0;

    while ((loc_ul_i < par_ul_max_string_length) && (par_pus_string[loc_ul_i] != (SGLushort) 0x0)) {
        loc_ul_i++;
    }

    return loc_ul_i;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_get_min_value
  DESCRIPTION:
    This function shall return the minimum value of given table
  PARAMETERS:
    par_ul_nb_entries -> Number of entries in the table
	par_pf_values -> Table of values
  RETURN:	 
    SGLfloat -> the minimum value of the table
---------------------------------------------------------------------+*/
SGLfloat mth_get_min_value(SGLulong par_ul_nb_entries, const SGLfloat par_pf_values[])
{
    SGLulong loc_ul_i;
    SGLfloat loc_f_result = par_pf_values[0];

    for (loc_ul_i = 1; loc_ul_i < par_ul_nb_entries; loc_ul_i++) {
        if (par_pf_values[loc_ul_i] < loc_f_result) {
            loc_f_result = par_pf_values[loc_ul_i];
        }
        else {
            /* Nothing to do */
        }
    }

    return loc_f_result;
}

/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_get_max_value
  DESCRIPTION:
    This function shall return the maximum value of given table
  PARAMETERS:
    par_ul_nb_entries -> Number of entries in the table
	par_pf_values -> Table of values
  RETURN:	 
    SGLfloat -> the maximum value of the table
---------------------------------------------------------------------+*/
SGLfloat mth_get_max_value(SGLulong par_ul_nb_entries, const SGLfloat par_pf_values[])
{
    SGLulong loc_ul_i;
    SGLfloat loc_f_result = par_pf_values[0];

    for (loc_ul_i = 1; loc_ul_i < par_ul_nb_entries; loc_ul_i++) {
        if (par_pf_values[loc_ul_i] > loc_f_result) {
            loc_f_result = par_pf_values[loc_ul_i];
        }
        else {
            /* Nothing to do */
        }
    }

    return loc_f_result;
}


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME:  mth_norm
  DESCRIPTION:
    This function shall return the distance between the two given points
  PARAMETERS:
    par_f_x1 -> X coordinate of first point
    par_f_y1 -> Y coordinate of first point
    par_f_x2 -> X coordinate of second point
    par_f_y2 -> Y coordinate of second point
  RETURN:	 
    SGLfloat -> the distance between the two points
---------------------------------------------------------------------+*/
SGLfloat mth_norm(SGLfloat par_f_x1, SGLfloat par_f_y1, SGLfloat par_f_x2, SGLfloat par_f_y2)
{
    SGLfloat loc_f_return;
    SGLfloat loc_f_dx = (par_f_x2 - par_f_x1);
    SGLfloat loc_f_dy = (par_f_y2 - par_f_y1);
    loc_f_return = (loc_f_dx * loc_f_dx) + (loc_f_dy * loc_f_dy);
    loc_f_return = mth_sqrtf(loc_f_return);

    return loc_f_return;
}

#define mth_min(x,y) ((x) < (y) ? (x) : (y))

#define mth_max(x,y) ((x) > (y) ? (x) : (y))


/*+ FUNCTION DESCRIPTION ----------------------------------------------
  NAME: mth_point_in_shape
  DESCRIPTION:
    Function shall return the status of the point position: 
    SGL_TRUE shall be returned if the point is inside the shape 
   , SGL_FALSE shall be returned if the point is 
    outside the shape.
  PARAMETERS:  
    par_f_point_x -> x-coordinate for mouse
    par_f_point_y -> y-coordinate for mouse
    par_ul_nb_points -> number of points of the shape
    par_pf_points -> Points of the shape
  RETURN:
    SGLbyte -> SGL_TRUE: the mouse is in the shape,
    SGL_FALSE: the mouse is out of the shape
---------------------------------------------------------------------+*/
SGLbool mth_point_in_shape(SGLfloat par_f_point_x, SGLfloat par_f_point_y, SGLulong par_ul_nb_points, SGLfloat par_pf_points[][2])
{
    SGLbool loc_b_return = SGL_FALSE;
    SGLlong loc_l_counter = 0L;
    SGLulong loc_ul_i = 0UL;

    /* If the input point is a point of the shape, the function shall return SGL_TRUE */
    while (!loc_b_return && (loc_ul_i < par_ul_nb_points)) {
        if (sgluFloatIsEqual(par_f_point_x, par_pf_points[loc_ul_i][0]) && sgluFloatIsEqual(par_f_point_y, par_pf_points[loc_ul_i][1])) {
            loc_b_return = SGL_TRUE;
        }
        else {
            /* Nothing to do */
        }
        loc_ul_i++;
    }

    if (!loc_b_return) {
        SGLfloat loc_tf_p1[2];
        SGLfloat loc_tf_p2[2];
        SGLfloat loc_f_last_intersection = 0.0F;
        SGLfloat loc_f_first_intersection = 0.0F;
        SGLbool loc_b_intersect_first = SGL_FALSE;

        /* Convert the position in pixel coordinate system */
        loc_tf_p1[0] = par_pf_points[0][0];
        loc_tf_p1[1] = par_pf_points[0][1];

        /*
           Ray casting algorithm:
           Consider a polygon made up of N vertices (xi,yi) where i ranges from 0 to N-1. The last vertex (xN,yN) is assumed to be the same as the first vertex (x0,y0), 
           that is, the polygon is closed. To determine the status of a point (xp,yp) consider a horizontal ray emanating from (xp,yp) and to the right. 
           If the number of times this ray intersects the line segments making up the polygon is even then the point is outside the polygon. Whereas if the number of 
           intersections is odd then the point (xp,yp) lies inside the polygon.
         */

        for (loc_ul_i = 1; loc_ul_i <= par_ul_nb_points; loc_ul_i++) {
            loc_tf_p2[0] = par_pf_points[loc_ul_i % par_ul_nb_points][0];

            loc_tf_p2[1] = par_pf_points[loc_ul_i % par_ul_nb_points][1];

            if (sgluFloatIsEqual(par_f_point_y, loc_tf_p1[1]) || sgluFloatIsEqual(par_f_point_y, loc_tf_p2[1])) {
                par_f_point_y = par_f_point_y + MTH_EPSILON;
            }

            if (par_f_point_y > mth_min(loc_tf_p1[1], loc_tf_p2[1])) {
                if (par_f_point_y < mth_max(loc_tf_p1[1], loc_tf_p2[1])) {
                    if (par_f_point_x <= mth_max(loc_tf_p1[0], loc_tf_p2[0])) {
                        SGLfloat loc_f_x_intersection;

                        loc_f_x_intersection = SGLfloat_div(((par_f_point_y - loc_tf_p1[1]) * (loc_tf_p2[0] - loc_tf_p1[0]))
                                                            , (loc_tf_p2[1] - loc_tf_p1[1])) + loc_tf_p1[0];

                        if ((((loc_tf_p2[0] - loc_tf_p1[0]) < SGL_MIN_F)
                             && ((loc_tf_p2[0] - loc_tf_p1[0]) > -SGL_MIN_F))
                            || (par_f_point_x <= loc_f_x_intersection)) {
                            if (loc_l_counter == 0) {
                                loc_l_counter++;
                                loc_f_last_intersection = loc_f_x_intersection;
                                if (loc_ul_i == 1U) {
                                    loc_f_first_intersection = loc_f_x_intersection;
                                    loc_b_intersect_first = SGL_TRUE;
                                }
                            }
                            else {
                                if (loc_ul_i == par_ul_nb_points) {
                                    /* Check intersection with first segment and last segment */
                                    if (loc_b_intersect_first) {
                                        if ((!sgluFloatIsEqual(loc_f_first_intersection, loc_f_x_intersection))
                                            && (!sgluFloatIsEqual(loc_f_last_intersection, loc_f_x_intersection))) {
                                            loc_l_counter++;
                                            loc_f_last_intersection = loc_f_x_intersection;
                                        }
                                        else {
                                            /* Nothing to do */
                                        }
                                    }
                                    else {
                                        if (!sgluFloatIsEqual(loc_f_last_intersection, loc_f_x_intersection)) {
                                            loc_l_counter++;
                                            loc_f_last_intersection = loc_f_x_intersection;
                                        }
                                        else {
                                            /* Nothing to do */
                                        }
                                    }
                                }
                                else {
                                    /* Check this intersection is not the same as last one */
                                    if (!sgluFloatIsEqual(loc_f_last_intersection, loc_f_x_intersection)) {
                                        loc_l_counter++;
                                        loc_f_last_intersection = loc_f_x_intersection;
                                    }
                                    else {
                                        /* Nothing to do */
                                    }
                                }
                            }
                        }
                        else {
                            /* Nothing to do */
                        }
                    }
                    else {
                        /* Nothing to do */
                    }
                }
                else {
                    /* Nothing to do */
                }
            }
            else {
                /* Nothing to do */
            }

            loc_tf_p1[0] = loc_tf_p2[0];
            loc_tf_p1[1] = loc_tf_p2[1];
        }

        if ((loc_l_counter % 2) != 0) {
            loc_b_return = SGL_TRUE;
        }
        else {
            loc_b_return = SGL_FALSE;
        }
    }
    return loc_b_return;

}

/* End of File ***************************************************************/
