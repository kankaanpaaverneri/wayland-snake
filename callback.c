#include "callback.h"

void wl_surface_frame_done(void *data, struct wl_callback *callback, uint32_t time) {
	// Destroy this callback
	wl_callback_destroy(callback);

	// Request another frame
	struct state *state = data;
	callback = wl_surface_frame(state->surface);
	wl_callback_add_listener(callback, &wl_surface_frame_listener, state);
	struct wl_buffer *buffer = draw_frame(state);
	wl_surface_attach(state->surface, buffer, 0, 0);
	wl_surface_damage_buffer(state->surface, 0, 0, INT32_MAX, INT32_MAX);
	wl_surface_commit(state->surface);

	
	if(state->snake_destroy) {
		return;
	}


	for (int i = state->snake_length - 1; i > 0; i--) {
		state->snake_positions[i] = state->snake_positions[i - 1];
	}
	if(state->special_fruit_spawned == true && state->started_time == false) {
		state->start_time = time;
		state->started_time = true;
	}
	if(state->init_snake == true && state->started_time == false) {
		state->start_time = time;
		state->started_time = true;
	}
	int elapsed = time - state->start_time;
	if(elapsed > 5000) {
		state->started_time = false;	
		state->special_fruit_spawned = false;
		state->start_time = 0;
	}

	if(elapsed > 1000 && state->init_snake == true) {
		state->started_time == false;
		state->init_snake = false;
		state->start_time = 0;
	}

	// Update state
	const int move_amount = 3;
	switch (state->snake_direction) {
		case Left:
			state->snake_positions[0].snake_position_x -= move_amount;
			state->snake_positions[0].snake_width -= move_amount;
			break;
		case Right:
			state->snake_positions[0].snake_position_x += move_amount;
			state->snake_positions[0].snake_width += move_amount;
			break;
		case Up:
			state->snake_positions[0].snake_position_y -= move_amount;
			state->snake_positions[0].snake_height -= move_amount;
			break;
		case Down:
			state->snake_positions[0].snake_position_y += move_amount;
			state->snake_positions[0].snake_height += move_amount;
			break;
	}
	
}

const struct wl_callback_listener wl_surface_frame_listener = {
	.done = wl_surface_frame_done,
};
