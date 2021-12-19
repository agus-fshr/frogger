/***************************************************************************//**
  @file     Level.h
  @brief    Header del objeto nivel
  @author   Grupo 7
 ******************************************************************************/

#ifndef __LEVEL_H
#define __LEVEL_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include<stdint.h>
#include<stdlib.h>
#include <time.h>
#include"../config.h"
#include"Frog.h"
#include"Lane.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SPAWN_X (LEVEL_WIDTH*REFERENCE_WIDTH/2)
#define SPAWN_Y (LEVEL_HEIGHT-1)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
    uint8_t number;
    laneptr_t * lanes;
    frogptr_t frog;
    int16_t finishers[LVL_FINISHSPOTS];
    uint8_t finisher_count;
    uint8_t paused;
    uint32_t score;
} level_t;

typedef level_t * levelptr_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Reserves memory and initializes values
 * @param level Pointer to level
 */ 
uint8_t Level_init(levelptr_t level);

/**
 * @brief Passes to the next level and regenerates
 * @param level Pointer to level
 */ 
void Level_next(levelptr_t level);

/**
 * @brief Resets and regenerates level
 * @param level Pointer to level
 */ 
void Level_reset(levelptr_t level);


/**
 * @brief Checks collisions on level's current state
 * @param level Pointer to level
 * @param volume Volume of sound (if such a colission occurs)
 * @return Score earned if collides with target pads
 */ 
uint8_t Level_process_collisions(levelptr_t level, float volume);

/**
 * @brief Destroys pointer and frees memory
 * @param level Pointer to level
 * @return 1 if all good
 */ 
uint8_t Level_delete(levelptr_t level);


/**
 * @brief Checks if an element is already in an array
 * @param arr Array
 * @param elem Value to look for
 * @param len Length of array to search
 * @return 1 if it exists
 */ 
uint8_t is_in_array(int16_t *arr, int16_t elem, int16_t len);


#endif