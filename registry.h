#ifndef REGISTRY_H
#define REGISTRY_H
#include "xdg_surface.h"
#include "wl_seat.h"
#include "callback.h"
#include <string.h>

void registry_handle_global(
	void *data,
	struct wl_registry *registry,
	uint32_t name,
	const char *interface,
	uint32_t version
);
void registry_handle_global_remove(
	void *data,
	struct wl_registry *registry,
	uint32_t name
);
extern struct wl_registry_listener registry_listener; 

#endif
