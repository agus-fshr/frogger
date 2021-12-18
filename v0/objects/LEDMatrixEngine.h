#ifndef __LEDMATRIX_ENGINE_H
#define __LEDMATRIX_ENGINE_H

#include "GameEngine.h"
#include "../config.h"

#define BLOCK_HEIGHT (1)
#define BLOCK_WIDTH  (1)
#define DISP_HEIGHT (BLOCK_HEIGHT * LEVEL_HEIGHT)
#define DISP_WIDTH (BLOCK_WIDTH * LEVEL_WIDTH)

void LedMat_render(levelptr_t level);

#endif