#include "draw_frame.h"

void wl_buffer_release(void *data, struct wl_buffer *buffer) {
	// Sent by the compositor when it's no longer using this buffer
	wl_buffer_destroy(buffer);
}

const struct wl_buffer_listener wl_buffer_listener = {
	.release = wl_buffer_release,
};

struct wl_buffer *draw_frame(struct state *state) {
	const int width = SURFACE_WIDTH, height = SURFACE_HEIGHT;
	int stride = width * 4;
	int size = stride * height;

	int file_descriptor = allocate_shared_memory_file(size);
	if(!file_descriptor) {
		return NULL;	
	}
	uint32_t *data = mmap(
		NULL,
		size,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		file_descriptor,
		0
	);
	if(data == MAP_FAILED) {
		close(file_descriptor);	
		return NULL;
	}

	struct wl_shm_pool *pool = wl_shm_create_pool(
		state->shared_memory,
		file_descriptor,
		size
	);
	struct wl_buffer *buffer = wl_shm_pool_create_buffer(
		pool,
		0,
		width,
		height,
		stride,
		WL_SHM_FORMAT_XRGB8888
	);
	wl_shm_pool_destroy(pool);
	close(file_descriptor);



	int snake_head_pos_x = state->snake_positions[0].snake_position_x; 
	int snake_head_pos_y = state->snake_positions[0].snake_position_y; 
	int snake_head_width = state->snake_positions[0].snake_width; 
	int snake_head_height = state->snake_positions[0].snake_height; 

	// Wall collision
	for(int y = snake_head_pos_y; y < snake_head_height; y++) {
		for(int x = snake_head_pos_x; x < snake_head_width; x++) {
			if(y >= 0 && y <= 10 || y > height-10) {
				//fprintf(stdout, "Top bottom wall crash\n");
				state->snake_destroy = true;
				//exit(0);	
			}
			if(x >= 0 && x <= 10 || x > width-10) {
				//fprintf(stdout, "Side wall crash\n");
				state->snake_destroy = true;
				//exit(0);	
			}
		
		}
	}
	// Own tail colition
	for(int i = 5; i < state->snake_length; i++) {
		int snake_pos_y = state->snake_positions[i].snake_position_y;
		int snake_pos_x = state->snake_positions[i].snake_position_x;
		int snake_height = state->snake_positions[i].snake_height;
		int snake_width = state->snake_positions[i].snake_width;
		for(int y = snake_pos_y; y < snake_height; y++) {
			for(int x = snake_pos_x; x < snake_width; x++) {
				if(snake_pos_y == snake_head_pos_y && snake_pos_x == snake_head_pos_x) {
					if(state->init_snake == false) {
						state->snake_destroy = true;	
					}
				}
			}	
		}
			
	}


	// Eat fruit
	for(int y = snake_head_pos_y; y < snake_head_height; y++) {
		for(int x = snake_head_pos_x; x < snake_head_width; x++) {
			if(y >= state->fruit_y && y < state->fruit_y + FRUIT_SIZE
					&& x >= state->fruit_x && x < state->fruit_x + FRUIT_SIZE) {
				spawn_fruit(state);
				extend_snake(state, 1);
				state->fruits++;
				printf("fruits eaten: %d\n", state->fruits);
				if(state->fruits % 5 == 0 && state->fruits > 0 && state->special_fruit_spawned == false) {
					spawn_special_fruit(state);
					state->special_fruit_spawned = true;
				}
			}
			if(y >= state->special_fruit_y && y < state->special_fruit_y + FRUIT_SIZE
					&& x >= state->special_fruit_x && x < state->special_fruit_x + FRUIT_SIZE) {
				state->fruits += 20;
				extend_snake(state, 20);	
				printf("fruits eaten: %d\n", state->fruits);
				state->special_fruit_spawned = false;
				state->special_fruit_x = 0;
				state->special_fruit_y = 0;
				state->started_time = false;
			}
		}	
	}

	// Ground
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			if(data[y * width + x] != 0x1500FF){
				data[y * width + x] = 0x05ED39;
			}	
		}
	}

	// Fruit
	for(int y = state->fruit_y; y < state->fruit_y + FRUIT_SIZE; y++) {
		for(int x = state->fruit_x; x < state->fruit_x + FRUIT_SIZE; x++) {
			data[y * width + x] = 0xFF0000;	
		}	
	}

	// Special Fruit
	if(state->special_fruit_spawned == true) {
		for(int y = state->special_fruit_y; y < state->special_fruit_y + FRUIT_SIZE; y++) {
			for(int x = state->special_fruit_x; x < state->special_fruit_x + FRUIT_SIZE; x++) {
					// Special fruit
					data[y * width + x] = 0xFFF200;
			}	
		}
	}

	// Snake
	for (int i = 0; i < state->snake_length; i++) {
	    int snake_pos_x = state->snake_positions[i].snake_position_x;
	    int snake_pos_y = state->snake_positions[i].snake_position_y;
	    int snake_width = state->snake_positions[i].snake_width;
	    int snake_height = state->snake_positions[i].snake_height;
	    
	    for (int y = snake_pos_y; y < snake_height && y < height; y++) {
		for (int x = snake_pos_x; x < snake_width && x < width; x++) {
		    data[y * width + x] = 0x1500FF;
		}
	    }
	}

	// Walls
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			if(y >= 0 && y <= 10 || y > height-10) {
				data[y * width + x] = 0xFFFFFF;
			}
			if(x >= 0 && x <= 10 || x > width-10) {
				data[y * width + x] = 0xFFFFFF;
			}
		}
	}

	munmap(data, size);
	wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
	return buffer;
}
