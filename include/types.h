#ifndef RAIN_TYPES_H
#define RAIN_TYPES_H

#include <stdbool.h>

typedef struct {
  float pos_x;
  float pos_y;

  float v_y;

  float mass;
  int color;
  char symbol;
} Raindrop;

typedef struct {

  int max_drops;
  int active_drops;
  int height;
  int width;
  bool rainbow_mod;

  Raindrop *drops;
} Scene;

typedef struct {
  bool cflag;
  bool rainbow_mod;
  char cflag_param;

  bool iflag;
  const char *iflag_param;
  float ival;

  bool info_flag;
} RainConfig;

#endif
