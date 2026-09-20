#ifndef STATE_H
#define STATE_H
#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>
#include "xdg-shell-client-protocol.h"

enum pointer_event_mask {
	POINTER_EVENT_ENTER = 1 << 0,
	POINTER_EVENT_LEAVE = 1 << 1,
	POINTER_EVENT_MOTION = 1 << 2,
	POINTER_EVENT_BUTTON = 1 << 3,
	POINTER_EVENT_AXIS = 1 << 4,
	POINTER_EVENT_AXIS_SOURCE = 1 << 4,
	POINTER_EVENT_AXIS_STOP = 1 << 6,
	POINTER_EVENT_AXIS_DISCRETE = 1 << 7,
};

struct pointer_event {
	uint32_t event_mask;
	wl_fixed_t surface_x, surface_y;
	uint32_t button, state;
	uint32_t time;
	uint32_t serial;
	struct {
		bool valid;
	     	wl_fixed_t value;
   		int32_t discrete;		
	} axes[2];
	uint32_t axis_source;
};

struct snake_position {
	int snake_position_x;
	int snake_position_y;
	int snake_width;
	int snake_height;
};

#define INIT_SNAKE_LENGTH 10 
#define MAX_SNAKE_LENGTH 1000 

#define SURFACE_WIDTH 1366 
#define SURFACE_HEIGHT 768 

#define FRUIT_SIZE 20 

enum Direction {
	Left,
	Right,
	Up,
	Down
};

struct state {
	struct wl_compositor *compositor;
	struct wl_registry *registry;
	struct wl_shm *shared_memory;
	struct wl_surface *surface;
	struct wl_display *display;

	struct xdg_surface *xdg_surface;
	struct xdg_toplevel *xdg_toplevel;
	struct xdg_wm_base *xdg_wm_base;

	struct wl_seat *seat;
	struct wl_keyboard *keyboard;
	struct wl_pointer *pointer;
	
	struct snake_position snake_positions[MAX_SNAKE_LENGTH];
	//int snake_iterator;
	int snake_length;
	enum Direction snake_direction;
	int fruits;
	bool special_fruit_spawned;

	int fruit_x, fruit_y;
	int special_fruit_x, special_fruit_y;

	bool snake_destroy;
	int random_values[MAX_SNAKE_LENGTH];
	bool init_random_values;
	bool init_snake;
		

	uint32_t start_time;
	bool started_time;

	int width, height;
	bool closed;
	struct pointer_event pointer_event;
	struct xkb_state *xkb_state;
	struct xkb_context *xkb_context;
	struct xkb_keymap *xkb_keymap;
};

#endif
