#ifndef GAME_H
#define GAME_H
#include <stdlib.h>
#include "state.h"
#include <time.h>

void init_state(struct state *state);
bool is_valid_position(int fruit_x, int fruit_y, struct state *state);
void spawn_special_fruit(struct state *state);
void spawn_fruit(struct state *state);
void extend_snake(struct state *state, int amount);
#endif
