#include "utils.h"
#include <stdlib.h>

int get_random(int min, int max) { return min + rand() % (max - min + 1); }

float get_random_float(float min, float max) {
  return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}
