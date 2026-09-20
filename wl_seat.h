#ifndef WL_SEAT_H
#define WL_SEAT_H
#include "shared_memory.h"
#include <wayland-client.h>
#include "state.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void wl_seat_capabilities(
	void *data,
	struct wl_seat *seat,
	uint32_t capabilities
);
void wl_seat_name(
	void *data,
	struct wl_seat *seat,
	const char *name	
);
extern const struct wl_seat_listener wl_seat_listener;
#endif
