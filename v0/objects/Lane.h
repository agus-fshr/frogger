/***************************************************************************//**
  @file     Lane.h
  @brief    Header del objeto carril
  @author   Grupo 7
 ******************************************************************************/
#ifndef __LANE_H
#define __LANE_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include<stdint.h>
#include"../config.h"

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum {MOB_CAR = 1, MOB_LOG, MOB_FINISH, MOB_FLOOR} mobtype_t;

typedef struct {
    mobtype_t type;
    uint16_t ticks;
    int16_t step;
    int16_t x0;
    uint8_t delta;
    uint8_t mob_length;
} lane_t;

typedef lane_t * laneptr_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Advances lane state
 * @param lane Pointer to lane
 */ 
void Lane_tick(laneptr_t lane);

/**
 * @brief Calculates an element's start internal x value in a lane
 * @param lane Pointer to lane
 * @param elem Number of element to calculate
 */ 
int16_t Lane_get_elem_x(laneptr_t lane, int8_t elem);

/**
 * @brief Calculates an element's end internal x value in a lane
 * @param lane Pointer to lane
 * @param elem Number of element to calculate
 */ 
int16_t Lane_get_elem_x_end(laneptr_t lane, int8_t elem);


#endif