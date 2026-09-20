#ifndef XDG_SURFACE_H
#define XDG_SURFACE_H
#include "draw_frame.h"
#include "xdg-shell-client-protocol.h"

// xdg_surface_listener
void xdg_surface_configure(
	void *data,
	struct xdg_surface *xdg_surface,
	uint32_t serial	
);
extern struct xdg_surface_listener xdg_surface_listener; 

// xdg_wm_base_ping
void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial);
extern struct xdg_wm_base_listener xdg_wm_base_listener; 

#endif
