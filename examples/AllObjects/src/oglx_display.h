/*+ FILE DESCRIPTION -------------------------------------------------------
 FILENAME          : oglx_display.h 
 DESCRIPTION       : Management of display
 COPYRIGHT (C)     : 2015 by Esterel Technologies. All Rights Reserved.
 UNAUTHORIZED ACCESS, USE, REPRODUCTION OR DISTRIBUTION IS PROHIBITED.
---------------------------------------------------------------------------- +*/

#ifndef __oglx_display__
#define __oglx_display__

#include "sdy_events.h"

void oglx_display__initialise(int par_i_rotate);
void oglx_display__update(void);
void oglx_display_start_cycle(void);

sdy_keyboard_event_t oglx_display__get_keyboard_event();
sdy_pointer_event_t oglx_display__get_mouse_event();
double oglx_display__get_mouse_posx();
double oglx_display__get_mouse_posy();

void oglx_display_enter_io_mutex();
void oglx_display_end_io_mutex();

void record_mouse_event(int button, int pressed, int released, int modifiers);
void record_mouse_pos(double posx, double posy);
void record_keyboard_event(int key, int pressed, int released, int modifiers);

void start_drawing_thread();
void stop_drawing_thread();

void start_dispatch_thread();
void stop_dispatch_thread();
int open_window();
#endif
