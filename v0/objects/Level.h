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
#define SCORE_MULTIPLIER_LEVEL_COMPLETE (5)

#define SPAWN_X (LEVEL_WIDTH*REFERENCE_WIDTH/2)
#define SPAWN_Y (LEVEL_HEIGHT-1)

#define START_LANE (SPAWN_Y)
#define REST_LANE (8)
#define FINISH_LANE (0)

#define OUT_OF_BOUNDS (-10)   // For placing in finishers array



/** DIFFICULTY SETTINGS **/

#define CUT_OFF_DIFFICULTY    (15)
#define MIN_DELTA(lane)       ((lane)->mob_length + 2)

#define MAX_VEHICLE_LENGTH    (3)
#define MIN_VEHICLE_LENGTH    (1)
#define VEHICLE_DELTA_PER_STEP (0.1)
#define MIN_VEHICLE_STEP      (2)
#define DIFF_SCALING_VEHICLE_DELTA(d) (2*(CUT_OFF_DIFFICULTY - (d)))
#define DIFF_SCALING_VEHICLE_STEP(d)  (4+(d))

#define MIN_LOG_LENGTH    (2)
#define MAX_LOG_LENGTH    (7)
#define MAX_LOG_EXTRA_DELTA (4)
#define MIN_LOG_STEP      (3)
#define DIFF_SCALING_LOG_LENGTH(d)  (MAX_LOG_LENGTH - MIN_LOG_LENGTH - (d)/3)
#define DIFF_SCALING_LOG_STEP(d)    (8 + (d))

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
    uint8_t number;
    laneptr_t * lanes;
    frogptr_t frog;
    int16_t finishers[LVL_FINISHSPOTS];
    uint8_t finisher_count;
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
 * @param extra_score returns earned score if it applies
 * @return 1 if collision ocurred
 */ 
uint8_t Level_process_collisions(levelptr_t level, uint16_t* extra_score);

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