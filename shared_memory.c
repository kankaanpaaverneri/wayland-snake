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

