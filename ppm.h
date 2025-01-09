// RICCARDO SAMARITAN SM3201396

#ifndef PPM_H
#define PPM_H

#include <stdbool.h>
#include "scene.h"

bool savePpm(const char *filename, const Pixel *image, int width, int height);

#endif