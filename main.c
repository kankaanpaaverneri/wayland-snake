#include <stdio.h>
#include "registry.h"
#include "game.h"

int main(int argc, char *argv[]) {
	srand(time(NULL));
	struct state state = { 0 };
	init_state(&state);

	// Initializing wayland display
	state.display = wl_display_connect(NULL);
	if (!state.display) {
		fprintf(stderr, "Cannot connect to wl_display.\n");	
		return 1;
	}

	// Binding to globals 
	state.registry = wl_display_get_registry(state.display);
	wl_registry_add_listener(state.registry, &registry_listener, &state);

	// Initializing xkb_context
	state.xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	wl_display_roundtrip(state.display);

	// Creating surface
	state.surface = wl_compositor_create_surface(state.compositor);
	if (map_shared_memory(&state) == -1) {
		fprintf(stderr, "Mapping shared memory failed.\n");	
		return 1;
	}

	// Creating xdg_surface
	state.xdg_surface = xdg_wm_base_get_xdg_surface(
		state.xdg_wm_base, state.surface
	);
	xdg_surface_add_listener(state.xdg_surface, &xdg_surface_listener, &state);

	// Creating xdg_toplevel
	state.xdg_toplevel = xdg_surface_get_toplevel(state.xdg_surface);
	xdg_toplevel_set_title(state.xdg_toplevel, "MATOPELI");

	wl_surface_commit(state.surface);

	// Creating Callback
	struct wl_callback *callback = wl_surface_frame(state.surface);
	wl_callback_add_listener(callback, &wl_surface_frame_listener, &state);


	while(wl_display_dispatch(state.display)) {
	
	}
	munmap(state.data, state.size);
	
	return 0;
}
