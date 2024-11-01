/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : animation.c
 DESCRIPTION       : Animation of SCADE Display plugs
 COPYRIGHT (C)     : 2008 by Esterel Technologies. All Rights Reserved.
 UNAUTHORIZED ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS PROHIBITED.
---------------------------------------------------------------------------- +*/

#ifndef _ANIMATION_H
#define _ANIMATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Interface to animation functions */

    void init_PN_AIRSPEED(float *p_data);
    void animate_PN_AIRSPEED(float *p_data);

    void init_PN_ROLL_ANGLE(float *p_data);
    void animate_PN_ROLL_ANGLE(float *p_data);

    void init_PN_ALTI(float *p_data);
    void animate_PN_ALTI(float *p_data);

    void init_PN_PITCH_ANGLE(float *p_data);
    void animate_PN_PITCH_ANGLE(float *p_data);

#ifdef __cplusplus
}
#endif
#endif                          /* _ANIMATION_H */
