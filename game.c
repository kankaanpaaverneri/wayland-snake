#include "game.h"

void init_state(struct state *state) {
	int x = 250;
	int y = 250;
	int width = x + 20;
	int height = y + 20;
	state->snake_length = INIT_SNAKE_LENGTH;
	state->snake_direction = Left;
	state->fruits = 0;
	state->special_fruit_spawned = false;
	state->special_fruit_x = 0;
	state->special_fruit_y = 0;
	state->fruit_x = 0;
	state->fruit_y = 0;
	state->started_time = false;
	state->snake_destroy = false;
	state->init_random_values = true;
	state->init_snake = true;


	for(int i = 0; i < state->snake_length; i++) {
		state->snake_positions[i].snake_position_x = x;
		state->snake_positions[i].snake_position_y = y;
		state->snake_positions[i].snake_width = width;
		state->snake_positions[i].snake_height = height;
		x += 100;
		width += 20;
	}
	// Spawn initial fruit
	spawn_fruit(state);
}

bool is_valid_position(int fruit_x, int fruit_y, struct state *state) {
	bool is_valid = false;
	if(fruit_x < 50) {
		return false;
	}
	if(fruit_x > SURFACE_WIDTH - 50) {
		return false;
	}
	if(fruit_y < 50) {
		return false;	
	}

	if(fruit_y > SURFACE_HEIGHT - 50) {
		return false;
	}
	for (int i = 0; i < state->snake_length; i++) {
	    int snake_pos_x = state->snake_positions[i].snake_position_x;
	    int snake_pos_y = state->snake_positions[i].snake_position_y;
	    int snake_width = state->snake_positions[i].snake_width;
	    int snake_height = state->snake_positions[i].snake_height;
	    
	    for (int y = snake_pos_y; y < snake_height; y++) {
		for (int x = snake_pos_x; x < snake_width; x++) {
			if(y != fruit_y) {
				is_valid = true;		
			} else {
				is_valid = false;	
			}
			if(x != fruit_x) {
				is_valid = true;	
			} else {
				is_valid = false;	
			}
		}
	    }
	}
	return is_valid;
}

void spawn_special_fruit(struct state *state) {
	int fruit_x = rand() % SURFACE_WIDTH + 1;
	int fruit_y = rand() % SURFACE_HEIGHT + 1;
	while(!is_valid_position(fruit_x, fruit_y, state)) {
		fruit_x = rand() % SURFACE_WIDTH + 1;
		fruit_y = rand() % SURFACE_HEIGHT + 1;
	}
	state->special_fruit_x = fruit_x;
	state->special_fruit_y = fruit_y;
}

void spawn_fruit(struct state *state) {
	int fruit_x = rand() % SURFACE_WIDTH + 1;
	int fruit_y = rand() % SURFACE_HEIGHT + 1;
	while(!is_valid_position(fruit_x, fruit_y, state)) {
		fruit_x = rand() % SURFACE_WIDTH + 1;
		fruit_y = rand() % SURFACE_HEIGHT + 1;
	}
	state->fruit_x = fruit_x;
	state->fruit_y = fruit_y;
}

void extend_snake(struct state *state, int amount) {
	if(state->snake_length >= MAX_SNAKE_LENGTH) {
		fprintf(stdout, "Max snake length\nGAME WON\n");	
		exit(0);
	}
	int length = state->snake_length;
	for(int i = 0; i < amount; i++) {
		state->snake_positions[length].snake_position_x = state->snake_positions[length-1].snake_position_x;
		state->snake_positions[length].snake_position_y = state->snake_positions[length-1].snake_position_y;
		state->snake_positions[length].snake_width = state->snake_positions[length-1].snake_width;
		state->snake_positions[length].snake_height = state->snake_positions[length-1].snake_height;
		state->snake_length += 1;
	
	}
}

