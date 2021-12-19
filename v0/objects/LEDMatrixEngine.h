/***************************************************************************//**
  @file     LEDMatrixEngine.h
  @brief    Header de interfaz específica de RPi
  @author   Grupo 7
 ******************************************************************************/

#ifndef __LEDMATRIX_ENGINE_H
#define __LEDMATRIX_ENGINE_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "GameEngine.h"
#include "../config.h"
#include "../libs/joydisp/joydrv.h"
#include "../libs/joydisp/disdrv.h"
#include "../libs/LEDlib.h"
#include <unistd.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define BLOCK_HEIGHT (1)
#define BLOCK_WIDTH  (1)
#define DISP_HEIGHT (BLOCK_HEIGHT * LEVEL_HEIGHT)
#define DISP_WIDTH (BLOCK_WIDTH * LEVEL_WIDTH)
#define DEADZONE (90)
#define CENTERED_TEXT_X 1
#define CENTERED_TEXT_Y 6


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Inits RPi interface
 * @param eng Pointer to engine
 * @return 1 if no issues were found
 */ 
int LEDMatEngine_init(engineptr_t eng);

/**
 * @brief Destroys RPi interface
 * @param eng Pointer to engine
 * @return 1 if no issues were found
 */ 
int LEDMatEngine_destroy(engineptr_t eng);

/**
 * @brief Computes RPi-specific core game loop
 * @param eng Pointer to engine
 * @return 1 if game continues
 */ 
int LEDMatEngine_gameloop(engineptr_t eng);

#endif