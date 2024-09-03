#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "Definitions.h"

bool InitializeWindow(void);
void DestroyWindow(void);
void ClearColorBuffer(color_t color);
void RenderColorBuffer(void);
void DrawPixel(int x, int y, color_t color);
void DrawRect(int x, int y, int width, int height, color_t color);
void DrawLine(int x1, int y1, int x2, int y2, color_t color);

#endif