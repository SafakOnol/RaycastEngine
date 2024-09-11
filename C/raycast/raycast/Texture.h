#ifndef TEXTURES_H
#define TEXTURES_H

#include <stdint.h>
#include "Definitions.h"
#include "upng.h"

upng_t* textures[NUM_TEXTURES];

void LoadTextures();
void FreeTextures();

#endif