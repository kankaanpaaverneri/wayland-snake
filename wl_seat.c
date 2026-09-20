#include "wl_seat.h"

// Mouse
void wl_pointer_enter(
	void *data,
	struct wl_pointer *wl_pointer,
	uint32_t serial,
	struct wl_surface *surface,
	wl_fixed_t surface_x,
	wl_fixed_t surface_y	
) {
	struct state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_ENTER;
	state->pointer_event.serial = serial;
	state->pointer_event.surface_x = surface_x;
	state->pointer_event.surface_y = surface_y;
}

void wl_pointer_leave(
	void *data,
	struct wl_pointer *wl_pointer,
	uint32_t serial,
	struct wl_surface *surface
) {
	struct state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_LEAVE;
	state->pointer_event.serial = serial;
}

void wl_pointer_motion(
	void *data,
	struct wl_pointer *wl_pointer,
	uint32_t time,
	wl_fixed_t surface_x,
	wl_fixed_t surface_y	
) {
	struct state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_MOTION;
	state->pointer_event.time = time;
	state->pointer_event.surface_x = surface_x;
	state->pointer_event.surface_y = surface_y;
}

void wl_pointer_button(
	void *data,
	struct wl_pointer *wl_pointer,
	uint32_t serial,
	uint32_t time,
	uint32_t button,
	uint32_t state_integer
) {
	struct state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_BUTTON;
	state->pointer_event.time = time;
	state->pointer_event.serial = serial;
	state->pointer_event.button = button;
	state->pointer_event.state = state_integer;
}

void wl_pointer_axis(
	void *data,
	struct wl_pointer *wl_pointer,
	uint32_t time,
	uint32_t axis,
	wl_fixed_t value
) {
	struct state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_AXIS;
	state->pointer_event.time = time;
	state->pointer_event.axes[axis].valid = true;
	state->pointer_event.axes[axis].value = value;
}

void wl_pointer_axis_source(
	void *data,
	struct wl_pointer *wl_pointer,
	uint32_t axis_source
) {
	struct state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_AXIS_SOURCE;
	state->pointer_event.axis_source = axis_source;
}

void wl_pointer_axis_stop(
	void *data,
	struct wl_pointer *wl_pointer,
	uint32_t time,
	uint32_t axis
) {
	struct state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_AXIS_STOP;
	state->pointer_event.time = time;
	state->pointer_event.axes[axis].valid = true;
}

void wl_pointer_axis_discrete(
	void *data,
	struct wl_pointer *wl_pointer,
	uint32_t axis,
	int32_t discrete
) {
	struct state *state = data;
	state->pointer_event.event_mask |= POINTER_EVENT_AXIS_DISCRETE;
	state->pointer_event.axes[axis].valid = true;
	state->pointer_event.axes[axis].discrete = discrete;
}

void wl_pointer_frame(
	void *data,
	struct wl_pointer *wl_pointer	
) {
	struct state *state = data;	
	struct pointer_event *event = &state->pointer_event;
	//printf("Entered at: %d\n", event->time);

	if(event->event_mask & POINTER_EVENT_ENTER) {
		/*
		fprintf(stdout, "x: %f, y: %f \n",
				wl_fixed_to_double(event->surface_x),
				wl_fixed_to_double(event->surface_y)
		);
		*/
	}

	if(event->event_mask & POINTER_EVENT_LEAVE) {
		//fprintf(stdout, "leave");	
	}

	if(event->event_mask & POINTER_EVENT_MOTION) {
		/*
		fprintf(stdout, "motion, x: %f, y: %f\n",
				wl_fixed_to_double(event->surface_x),
				wl_fixed_to_double(event->surface_y)
		);	
		*/
	}

	if(event->event_mask & POINTER_EVENT_BUTTON) {
		char *state = event->state == WL_POINTER_BUTTON_STATE_RELEASED ?
		       	"released": "pressed";
		//fprintf(stdout, "button: %d %s\n", event->button, state);	
	}
	memset(event, 0, sizeof(*event));
}

struct wl_pointer_listener wl_pointer_listener = {
	.enter = wl_pointer_enter,
	.leave = wl_pointer_leave,
	.motion = wl_pointer_motion,
	.button = wl_pointer_button,
	.axis = wl_pointer_axis,
	.frame = wl_pointer_frame,
	.axis_source = wl_pointer_axis_source,
	.axis_stop = wl_pointer_axis_stop,
	.axis_discrete = wl_pointer_axis_discrete,
};

// Keyboard

void wl_keyboard_keymap(
	void *data,
	struct wl_keyboard *wl_keyboard,
	uint32_t format,
	int32_t fd,
	uint32_t size
) {
	struct state *state = data;

	if(format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1) {
		fprintf(stderr, "Not correct keyboard keymap format\n");
		return;	
	}

	char *map_shm = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);
	if(map_shm == MAP_FAILED) {
		fprintf(stderr, "Map Shared memory failed\n");
		return;	
	}

	struct xkb_keymap *xkb_keymap = xkb_keymap_new_from_string(
		state->xkb_context,
		map_shm,
		XKB_KEYMAP_FORMAT_TEXT_V1,
		XKB_KEYMAP_COMPILE_NO_FLAGS		
	);
	munmap(map_shm, size);
	close(fd);

	struct xkb_state *xkb_state = xkb_state_new(xkb_keymap);
	xkb_keymap_unref(state->xkb_keymap);
	xkb_state_unref(state->xkb_state);
	state->xkb_keymap = xkb_keymap;
	state->xkb_state = xkb_state;
}

void wl_keyboard_enter(
	void *data,
	struct wl_keyboard *wl_keyboard,
	uint32_t serial,
	struct wl_surface *surface,
	struct wl_array *keys	
) {
	struct state *state = data;
	//fprintf(stdout, "Keyboard enter: keys pressed are: \n");
	uint32_t *key;
	wl_array_for_each(key, keys) {
		char buffer[128];
		xkb_keysym_t sym = xkb_state_key_get_one_sym(
			state->xkb_state, *key + 8		
		);
		xkb_keysym_get_name(sym, buffer, sizeof(buffer));
		//fprintf(stdout, "sym: %-12s (%d), ", buffer, sym);
		xkb_state_key_get_utf8(state->xkb_state, *key + 8, buffer, sizeof(buffer));
		//fprintf(stdout, "utf8: %s\n", buffer);
	};
}

void wl_keyboard_key(
	void *data,
	struct wl_keyboard *wl_keyboard,
	uint32_t serial,
	uint32_t time,
	uint32_t key,
	uint32_t state_integer	
) {
	struct state *state = data;
	char buffer[128];
	uint32_t keycode = key + 8;
	xkb_keysym_t sym = xkb_state_key_get_one_sym(state->xkb_state, keycode);
	xkb_keysym_get_name(sym, buffer, sizeof(buffer));

	const char *action = state_integer == WL_KEYBOARD_KEY_STATE_PRESSED ?
		"pressed" : "released";
	if(strcmp(buffer, "Left") == 0 && strcmp(action, "pressed") == 0) {
		if(state->snake_direction != Right) {
			state->snake_direction = Left;
		}
	}
	if(strcmp(buffer, "Right") == 0 && strcmp(action, "pressed") == 0) {
		if(state->snake_direction != Left) {
			state->snake_direction = Right;
		}
	}
	if(strcmp(buffer, "Up") == 0 && strcmp(action, "pressed") == 0) {
		if(state->snake_direction != Down) {
			state->snake_direction = Up;
		}
	}
	if(strcmp(buffer, "Down") == 0 && strcmp(action, "pressed") == 0) {
		if(state->snake_direction != Up) {
			state->snake_direction = Down;
		}
	}
	//fprintf(stdout, "key: %s, sym: %-12s (%d), ", action, buffer, sym);
	xkb_state_key_get_utf8(state->xkb_state, keycode, buffer, sizeof(buffer));
	//fprintf(stdout, "utf8: %s\n", buffer);
}

void wl_keyboard_leave(
	void *data,
	struct wl_keyboard *wl_keyboard,
	uint32_t serial,
	struct wl_surface *surface
) {
	//fprintf(stdout, "Keyboard leave\n");
}

void wl_keyboard_modifiers(
	void *data,
	struct wl_keyboard *wl_keyboard,
	uint32_t serial,
	uint32_t mods_depressed,
	uint32_t mods_latched,
	uint32_t mods_locked,
	uint32_t group
) {
	struct state *state = data;
	xkb_state_update_mask(
		state->xkb_state,
		mods_depressed,
		mods_latched,
		mods_locked,
		0,
		0,
		group
	);
}

void wl_keyboard_repeat_info(
	void *data,
	struct wl_keyboard *wl_keyboard,
	int32_t rate,
	int32_t delay
) {
	// Left as an exercise for the reader
}

const struct wl_keyboard_listener wl_keyboard_listener = {
	.keymap = wl_keyboard_keymap,
	.enter = wl_keyboard_enter,
	.leave = wl_keyboard_leave,
	.key = wl_keyboard_key,
	.modifiers = wl_keyboard_modifiers,
	.repeat_info = wl_keyboard_repeat_info,
};

void wl_seat_capabilities(
	void *data,
	struct wl_seat *seat,
	uint32_t capabilities
) {
	struct state *state = data;

	// Mouse
	bool have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER;

	if(have_pointer && state->pointer == NULL) {
		state->pointer = wl_seat_get_pointer(state->seat);	
		wl_pointer_add_listener(state->pointer, &wl_pointer_listener, state);
	} else if(!have_pointer && state->pointer != NULL) {
		wl_pointer_release(state->pointer);	
		state->pointer = NULL;
	}

	// Keyboard
	bool have_keyboard = capabilities & WL_SEAT_CAPABILITY_KEYBOARD;	

	if(have_keyboard && state->keyboard == NULL) {
		state->keyboard = wl_seat_get_keyboard(state->seat);	
		wl_keyboard_add_listener(state->keyboard, &wl_keyboard_listener, state);
	} else if(!have_keyboard && state->keyboard != NULL) {
		wl_keyboard_release(state->keyboard);	
		state->keyboard = NULL;
	}
}

void wl_seat_name(
	void *data,
	struct wl_seat *seat,
	const char *name	
) {
	//fprintf(stdout, "seat name: %s\n", name);
}

const struct wl_seat_listener wl_seat_listener = {
	.capabilities = wl_seat_capabilities,
	.name = wl_seat_name
};
