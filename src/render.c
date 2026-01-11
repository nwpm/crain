#include "../include/render.h"
#include <ncurses.h>

void render(Scene *s) {
  werase(stdscr);

  for (int i = 0; i < s->active_drops; ++i) {
    Raindrop *current_drop = &s->drops[i];
    float x_pos = current_drop->pos_x;
    float y_pos = current_drop->pos_y;
    int color   = current_drop->color;

    if (y_pos < 0) {
      continue;
    }

    if ((int)y_pos == (s->height - 1)) {
      int left_splash = (int)x_pos - 1;
      int right_splash = (int)x_pos + 1;

      if (left_splash >= 0) {
        mvaddch((int)y_pos, left_splash, '\\' | COLOR_PAIR(color));
      }
      if (right_splash < s->width) {
        mvaddch((int)y_pos, right_splash, '/' | COLOR_PAIR(color));
      }

    } else {
      mvaddch((int)y_pos, (int)x_pos, current_drop->symbol | COLOR_PAIR(color));
    }
  }
  refresh();
}
