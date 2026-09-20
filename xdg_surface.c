#include "xdg_surface.h"

// xdg_surface_listener
void xdg_surface_configure(
	void *data,
	struct xdg_surface *xdg_surface,
	uint32_t serial	
) {
	struct state *state = data;
	xdg_surface_ack_configure(xdg_surface, serial);

	struct wl_buffer *buffer = draw_frame(state);
	wl_surface_attach(state->surface, buffer, 0, 0);
	wl_surface_commit(state->surface);
}

struct xdg_surface_listener xdg_surface_listener = {
	.configure = xdg_surface_configure,
};


// xdg_wm_base_listener
void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial) {
	xdg_wm_base_pong(xdg_wm_base, serial);
}

struct xdg_wm_base_listener xdg_wm_base_listener = {
	.ping = xdg_wm_base_ping,
};

