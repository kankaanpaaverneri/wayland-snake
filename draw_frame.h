#ifndef DRAW_FRAME_H
#define DRAW_FRAME_H
#include "shared_memory.h"
#include "state.h"
#include <stdlib.h>
#include "game.h"

extern const struct wl_buffer_listener wl_buffer_listener; 
void wl_buffer_release(void *data, struct wl_buffer *buffer);
struct wl_buffer *draw_frame(struct state *state);
#endif
