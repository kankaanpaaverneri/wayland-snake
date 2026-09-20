#ifndef SHARED_MEMORY
#define SHARED_MEMORY

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include "state.h"

int allocate_shared_memory_file(size_t size); 
//int map_shared_memory(struct state *state);
#endif
