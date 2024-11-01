/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : animation.c
 DESCRIPTION       : Animation of SCADE Display generated plugs
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 UNAUTHORIZED ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS PROHIBITED.
---------------------------------------------------------------------------- +*/

/* Includes */

#include "animation.h"

/* Animation functions for plug PN_AIRSPEED */

float _STEP_PN_AIRSPEED;

void init_PN_AIRSPEED(float *p_data)
{
    _STEP_PN_AIRSPEED = (float) 1.0;
    *p_data = (float) 1.0;
}

void animate_PN_AIRSPEED(float *p_data)
{
    *p_data += _STEP_PN_AIRSPEED;
    if (*p_data >= 200.0) {
        *p_data = (float) 200.0;
        _STEP_PN_AIRSPEED = -_STEP_PN_AIRSPEED;
    }
    else if (*p_data <= 0.0) {
        *p_data = (float) 0.0;
        _STEP_PN_AIRSPEED = -_STEP_PN_AIRSPEED;
    }
}


/* Animation functions for plug PN_ROLL_ANGLE */

float _STEP_PN_ROLL_ANGLE;

void init_PN_ROLL_ANGLE(float *p_data)
{
    _STEP_PN_ROLL_ANGLE = (float) 0.3;
    *p_data = (float) 0.3;
}

void animate_PN_ROLL_ANGLE(float *p_data)
{
    *p_data += _STEP_PN_ROLL_ANGLE;
    if (*p_data >= 10.0) {
        *p_data = (float) 10.0;
        _STEP_PN_ROLL_ANGLE = -_STEP_PN_ROLL_ANGLE;
    }
    else if (*p_data <= -10.0) {
        *p_data = (float) -10.0;
        _STEP_PN_ROLL_ANGLE = -_STEP_PN_ROLL_ANGLE;
    }
}


/* Animation functions for plug PN_ALTI */

float _STEP_PN_ALTI;

void init_PN_ALTI(float *p_data)
{
    _STEP_PN_ALTI = (float) 1.0;
    *p_data = (float) 1.0;
}

void animate_PN_ALTI(float *p_data)
{
    *p_data += _STEP_PN_ALTI;
    if (*p_data >= 600.0) {
        *p_data = (float) 600.0;
        _STEP_PN_ALTI = -_STEP_PN_ALTI;
    }
    else if (*p_data <= 0.0) {
        *p_data = (float) 0.0;
        _STEP_PN_ALTI = -_STEP_PN_ALTI;
    }
}


/* Animation functions for plug PN_PITCH_ANGLE */

float _STEP_PN_PITCH_ANGLE;

void init_PN_PITCH_ANGLE(float *p_data)
{
    _STEP_PN_PITCH_ANGLE = (float) 0.2;
    *p_data = (float) 0.2;
}

void animate_PN_PITCH_ANGLE(float *p_data)
{
    *p_data += _STEP_PN_PITCH_ANGLE;
    if (*p_data >= 15.0) {
        *p_data = (float) 15.0;
        _STEP_PN_PITCH_ANGLE = -_STEP_PN_PITCH_ANGLE;
    }
    else if (*p_data <= -15.0) {
        *p_data = (float) -15.0;
        _STEP_PN_PITCH_ANGLE = -_STEP_PN_PITCH_ANGLE;
    }
}
