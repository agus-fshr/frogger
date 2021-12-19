/***************************************************************************//**
  @file     AllegroEngine.h
  @brief    Header de interfaz específica de Allegro
  @author   Grupo 7
 ******************************************************************************/
#ifndef __ALLEGRO_ENGINE_H
#define __ALLEGRO_ENGINE_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "GameEngine.h"
#include "../config.h"
#include "../libs/allegro_sound.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define BLOCK_HEIGHT (64)
#define BLOCK_WIDTH  (64)
#define DISP_HEIGHT (BLOCK_HEIGHT * LEVEL_HEIGHT)
#define DISP_WIDTH (BLOCK_WIDTH * LEVEL_WIDTH)

#define SELECTED_FONT "fonts/Gamer.ttf"

#define BMP_FOLDER "sprites/"
#define BMP_CARRIAGE "mobinit.png"
#define BMP_REINDEER "mobcont.png"
#define BMP_LOG_HEAD "log1_64.png"
#define BMP_LOG_BODY "log2_64.png"
#define BMP_LOG_END  "log3_64.png"
#define BMP_LILYPAD  "lily_64.png"
#define BMP_FROG     "frog_64.png"
#define BMP_HEART    "heart.png"
#define BMP_WATER    "water1.png"
#define BMP_ROAD     "snowy_road.png"
#define BMP_FLOOR    "floor.png"
#define BMP_DEATH_BG "rip.png"

#define BMP_PAUSE_BG  "pause_bg.png"
#define BMP_MAIN_BG   "main_bg.jpg"

#define GET_BMP(s) (BMP_FOLDER s)

#define OPTION_SELECTED '>'
#define OPTION_UNSELECTED ' '

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Inits Allegro interface
 * @param eng Pointer to engine
 * @return 1 if no issues were found
 */ 
int AllegroEngine_init(engineptr_t eng);

/**
 * @brief Destroys Allegro interface
 * @param eng Pointer to engine
 * @return 1 if no issues were found
 */ 
int AllegroEngine_destroy(engineptr_t eng);

/**
 * @brief Computes Allegro-specific core game loop
 * @param eng Pointer to engine
 * @return 1 if game continues
 */ 
int AllegroEngine_gameloop(engineptr_t eng);

#endif