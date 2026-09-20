#ifndef CALLBACK_H
#define CALLBACK_H
#include "state.h"
#include "draw_frame.h"

void wl_surface_frame_done(void *data, struct wl_callback *callback, uint32_t time);
extern const struct wl_callback_listener wl_surface_frame_listener; 

#endif
