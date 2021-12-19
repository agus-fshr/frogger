#ifndef __LEDMATRIX_ENGINE_H
#define __LEDMATRIX_ENGINE_H

#include "GameEngine.h"
#include "../config.h"
#include "../libs/joydisp/joydrv.h"
#include "../libs/joydisp/disdrv.h"
#include <unistd.h>

#define BLOCK_HEIGHT (1)
#define BLOCK_WIDTH  (1)
#define DISP_HEIGHT (BLOCK_HEIGHT * LEVEL_HEIGHT)
#define DISP_WIDTH (BLOCK_WIDTH * LEVEL_WIDTH)

#define DEADZONE (90)

int LEDMatEngine_init(engineptr_t);
int LEDMatEngine_destroy(engineptr_t);
int LEDMatEngine_gameloop(engineptr_t);

#endif