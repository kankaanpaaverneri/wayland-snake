#include "registry.h"

void registry_handle_global(
		void *data,
		struct wl_registry *registry,
		uint32_t name,
		const char *interface,
		uint32_t version
		) {
	struct state *state = data;
	if(strcmp(interface, wl_compositor_interface.name) == 0) {
		state->compositor = wl_registry_bind(
			registry, name, &wl_compositor_interface, 4		
		);	
	} else if(strcmp(interface, wl_shm_interface.name) == 0) {
		state->shared_memory = wl_registry_bind(
			registry, name, &wl_shm_interface, 1
		);
	} else if(strcmp(interface, xdg_wm_base_interface.name) == 0) {
		state->xdg_wm_base = wl_registry_bind(
			registry, name, &xdg_wm_base_interface, 1
		);
	} else if(strcmp(interface, wl_seat_interface.name) == 0) {
		state->seat = wl_registry_bind(
			registry, name, &wl_seat_interface, 7			
		);	
		wl_seat_add_listener(state->seat, &wl_seat_listener, state);
	}
}

void registry_handle_global_remove(
		void *data,
		struct wl_registry *registry,
		uint32_t name
		) {
	// Leave this blank
}

struct wl_registry_listener registry_listener = {
	.global = registry_handle_global,
	.global_remove = registry_handle_global_remove 
};
