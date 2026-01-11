#define _DEFAULT_SOURCE
#include "../include/physics.h"
#include "../include/render.h"
#include "utils.h"

#include <getopt.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

enum {
  INVALID_ARGS,
  INVALID_TERM,
  INVALID_COLOR_ARG,
  INVALID_INTEN_ARG,
  ALLOC_ERR,
} ErrorCodes;

static const char *HELP_TEXT =
    "Usage: crain [OPTION]...\n"
    "Render CLI rain animation.\n"
    "\nThe options below may be used to select render parametrs.\n"
    "-cx, --color=name [g, b, c, w, m, g, r] select drop color;\n"
    "-ix, --i=NUM      (0.1, 1.0)        select rain intensity;\n"
    "-h   --help                    display this help and exit;\n"
    "-v   --version        output version information and exit;";

static const char *VERSION_TEXT = "crain 1.0.0\n"
                                  "Written by nwpm.";

void init_rain(Scene *s);
void recalculate_pos(Scene *s);

bool select_color(RainConfig *conf);
bool parse_options(const struct option *long_options, RainConfig *conf,
                   int argc, char **argv);
bool select_intensity(RainConfig *conf);

int main(int argc, char **argv) {

  RainConfig rain_conf = {0};
  struct option long_options[] = {{"color", required_argument, NULL, 'c'},
                                  {"intensity", required_argument, NULL, 'i'},
                                  {"help", no_argument, NULL, 'h'},
                                  {"version", no_argument, NULL, 'v'}};

  if (!parse_options(long_options, &rain_conf, argc, argv)) {
    endwin();
    return INVALID_ARGS;
  }

  srand(time(NULL));
  initscr();

  if (!has_colors()) {
    endwin();
    fprintf(stderr, "Terminal doesn't support colors.\n");
    return INVALID_TERM;
  }

  start_color();
  if (!select_color(&rain_conf)) {
    endwin();
    return INVALID_COLOR_ARG;
  }

  if (!select_intensity(&rain_conf)) {
    endwin();
    return INVALID_INTEN_ARG;
  }

  int x_max = 0;
  int y_max = 0;
  getmaxyx(stdscr, y_max, x_max);
  int max_drops = x_max;
  int active_drops = rain_conf.ival * max_drops;

  Raindrop *drops = malloc(sizeof(Raindrop) * active_drops);
  if (!drops) {
    endwin();
    return ALLOC_ERR;
  }
  Scene s = {max_drops, active_drops,          y_max,
             x_max,     rain_conf.rainbow_mod, drops};

  init_rain(&s);
  nodelay(stdscr, TRUE);

  while (true) {
    int key = getch();
    if (key == 'q') {
      break;
    } else if (key == KEY_RESIZE) {
      int new_x, new_y;
      getmaxyx(stdscr, new_y, new_x);

      s.height = new_y;
      s.width = new_x;
      s.max_drops = new_x;
      clear();
      recalculate_pos(&s);
    }
    update(&s);
    render(&s);
    usleep(50000);
  }

  free(drops);
  endwin();

  return 0;
}

bool parse_options(const struct option *long_options, RainConfig *conf,
                   int argc, char **argv) {

  int c;

  while ((c = getopt_long(argc, argv, "c:i:hv", long_options, NULL)) != -1) {
    switch (c) {
    case 'c':
      conf->cflag = true;
      conf->cflag_param = optarg[0];
      break;
    case 'i':
      conf->iflag = true;
      conf->iflag_param = optarg;
      break;
    case 'h':
      conf->info_flag = true;
      puts(HELP_TEXT);
      exit(EXIT_SUCCESS);
    case 'v':
      conf->info_flag = true;
      puts(VERSION_TEXT);
      exit(EXIT_SUCCESS);
    case '?':
      fprintf(stderr, "Try --help for more information.\n");
      return false;
    }
  }

  return true;
}

bool select_color(RainConfig *conf) {

  if (conf->cflag) {
    switch (conf->cflag_param) {
    case 'g':
      init_pair(1, COLOR_GREEN, COLOR_BLACK);
      break;
    case 'c':
      init_pair(1, COLOR_CYAN, COLOR_BLACK);
      break;
    case 'b':
      init_pair(1, COLOR_BLUE, COLOR_BLACK);
      break;
    case 'm':
      init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
      break;
    case 'r':
      init_pair(1, COLOR_RED, COLOR_BLACK);
      break;
    case 'y':
      init_pair(1, COLOR_YELLOW, COLOR_BLACK);
      break;
    case 'w':
      init_pair(1, COLOR_WHITE, COLOR_BLACK);
      break;
    case 'h':
      conf->rainbow_mod = true;
      init_pair(1, COLOR_WHITE, COLOR_BLACK);
      init_pair(2, COLOR_GREEN, COLOR_BLACK);
      init_pair(3, COLOR_YELLOW, COLOR_BLACK);
      init_pair(4, COLOR_RED, COLOR_BLACK);
      init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
      init_pair(6, COLOR_BLUE, COLOR_BLACK);
      init_pair(7, COLOR_CYAN, COLOR_BLACK);
      break;
    default:
      fprintf(stderr, "Incorrect color argument.\n");
      return false;
    }
  } else {
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
  }

  return true;
}

bool select_intensity(RainConfig *conf) {

  if (conf->iflag) {
    float intensity = atof(conf->iflag_param);
    if (intensity < 0.f || intensity > 1.f) {
      return false;
    }
    conf->ival = intensity;
  } else {
    conf->ival = 0.5f;
  }

  return true;
}

void init_rain(Scene *s) {
  for (int i = 0; i < s->active_drops; ++i) {
    s->drops[i].mass = get_random_float(0.05, 0.4);
    s->drops[i].pos_y = get_random_float(-s->height, 0);
    s->drops[i].pos_x = get_random(0, s->width);
    s->drops[i].symbol = '|';
    s->drops[i].v_y = s->drops[i].mass;
    s->drops[i].color = s->rainbow_mod ? get_random(1, 7) : 1;
  }
}

void recalculate_pos(Scene *s) {
  for (int i = 0; i < s->active_drops; ++i) {
    s->drops[i].pos_y = get_random_float(-s->height, 0);
    s->drops[i].pos_x = get_random(0, s->width);
  }
}
