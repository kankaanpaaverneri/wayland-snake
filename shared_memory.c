#include "shared_memory.h"

void random_name(char *buffer) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	long r = ts.tv_nsec;
	for(int i = 0; i < 6; i++) {
		buffer[i] = 'A' + (r&15) + (r&16) * 2;	
		r >>= 5;
	}
}

int create_shared_memory_file(void) {
	int retries = 100;
	do {
		char name[] = "/wl_shm-XXXXXX";	
		random_name(name + sizeof(name) - 7);
		retries--;
		int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd >= 0) {
			shm_unlink(name);	
			return fd;
		}
	} while(retries > 0 && errno == EEXIST);
	return -1;
}

int allocate_shared_memory_file(size_t size) {
	int file_descriptor = create_shared_memory_file();
	if (file_descriptor < 0) {
		return -1;	
	}
	int return_value;
	do {
		return_value = ftruncate(file_descriptor, size);	
	} while(return_value < 0 && errno == EINTR);
	if(return_value < 0) {
		close(file_descriptor);	
		return -1;
	}
	return file_descriptor;
}

int map_shared_memory(struct state *state) {
	const int width = SURFACE_WIDTH, height = SURFACE_HEIGHT;
	int stride = width * 4;
	int size = stride * height;
	state->size = size;

	int file_descriptor = allocate_shared_memory_file(size);
	if(!file_descriptor) {
		return -1;	
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
		return -1;
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
	state->data = data;
	state->buffer = buffer;
	state->file_descriptor = file_descriptor;
}

