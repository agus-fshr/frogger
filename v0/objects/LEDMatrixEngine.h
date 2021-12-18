#ifndef __LEDMATRIX_ENGINE_H
#define __LEDMATRIX_ENGINE_H

#include "GameEngine.h"
#include "../config.h"
#include "../libs/sound.h"
#include "../libs/joydisp/joydrv.h"
#include "../libs/joydisp/disdrv.h"

#define BLOCK_HEIGHT (1)
#define BLOCK_WIDTH  (1)
#define DISP_HEIGHT (BLOCK_HEIGHT * LEVEL_HEIGHT)
#define DISP_WIDTH (BLOCK_WIDTH * LEVEL_WIDTH)

#define SPAWN_X (LEVEL_WIDTH*BLOCK_WIDTH/2)
#define SPAWN_Y (15)

#define DEADZONE (90)

int LEDMatEngine_init(engineptr_t, void*);
int LEDMatEngine_render(engineptr_t, void*);
int LEDMatEngine_destroy(engineptr_t, void*);
int LEDMatEngine_input(engineptr_t, void*);

#endif