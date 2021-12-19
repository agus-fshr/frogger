/***************************************************************************//**
  @file     GameEngine.h
  @brief    Header de la abstracción de núcleo de juego
  @author   Grupo 7
 ******************************************************************************/
#ifndef __GAME_ENGINE_H
#define __GAME_ENGINE_H


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Level.h"
#include <stdio.h>


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum {
    GAME_STA_MENU = 1,
    GAME_STA_PLAY,
    GAME_STA_DEATH,
    GAME_STA_PAUSE,
    GAME_STA_EXIT,
} gamestate_t;

typedef enum {
    MENU_STA_OP_1 = 1,
    MENU_STA_OP_2,
} menustate_t;

typedef enum {
    PLAY_STA_INIT = 1,
    PLAY_STA_1,
    PLAY_STA_2,
} playstate_t;

typedef enum {
    DEATH_STA_MENU_OP_1 = 1,
    DEATH_STA_MENU_OP_2,
} deathstate_t;


typedef enum {
    PAUSE_STA_OP_1 = 1,
    PAUSE_STA_OP_2,
    PAUSE_STA_OP_3,
} pausestate_t;

typedef enum {
    EXIT_STA_OP_1 = 1,
    EXIT_STA_OP_2,
} exitstate_t;

typedef enum {
    INPUT_NULL = 0,
    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_NEXTLEVEL,
    INPUT_PREVLEVEL,
    INPUT_PAUSE,
    INPUT_ENTER,
} input_t;


struct engineT;

typedef struct engineT engine_t;

typedef int (*engine_fun_ptr_t)(engine_t* engine);

struct engineT {
    engine_fun_ptr_t init;
    engine_fun_ptr_t destroy;
    engine_fun_ptr_t gameloop;
    levelptr_t level;
    uint32_t score;
    float volume;
    gamestate_t state;
    menustate_t menustate;
    pausestate_t pausestate;
    playstate_t playstate;
    deathstate_t deathstate;
    exitstate_t exitstate;
};

typedef  engine_t * engineptr_t; 



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initializes FSMs and volume
 * @param eng Pointer to engine
 */ 
void initialize_game_status(engineptr_t eng);

/**
 * @brief Initializes abstract engine and specific implementation
 * @param eng Pointer to engine
 */ 
void engine_init_wrapper(engineptr_t eng);


/**
 * @brief Destroys abstract engine and specific implementation
 * @param eng Pointer to engine
 */ 
void engine_destroy_wrapper(engineptr_t eng);


/**
 * @brief Processes general game FSM
 * @param eng Pointer to engine
 */ 
void process_game_state(engineptr_t eng, input_t input);


/**
 * @brief Fetches highscore from file
 * @return Internal score value
 */ 
uint32_t get_highscore(void);


/**
 * @brief Transforms internal width to display width
 * @param width Width to transform
 * @param block_width Block width of graphical coordinate system
 * @return Equivalent display width
 */ 
float scale_width(int16_t width, int16_t block_width);

#endif