#include "../include/physics.h"
#include "utils.h"

void update(Scene *s) {
  for (int i = 0; i < s->active_drops; ++i) {
    Raindrop *current_drop = &s->drops[i];
    if (current_drop->pos_y > s->height) {
      current_drop->pos_y = get_random_float(-s->height, 0.f);
      current_drop->pos_x = get_random(0, s->width);
    }
    current_drop->pos_y += current_drop->v_y;
  }
}
