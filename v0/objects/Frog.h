/***************************************************************************//**
  @file     Frog.h
  @brief    Header del objeto rana
  @author   Grupo 7
 ******************************************************************************/
#ifndef __FROG_H
#define __FROG_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include "../config.h"


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum {STOP, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT} move_t;

typedef struct {
    uint8_t lives;
    move_t movement;
    uint8_t color;
    uint16_t x;
    uint8_t lane;
} frog_t;

typedef frog_t * frogptr_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Resets lives to default value
 * @param frog Pointer to frog
 */ 
uint8_t Frog_reset_lives(frogptr_t frog);

/**
 * @brief Adds a live to the counter
 * @param frog Pointer to frog
 */ 
uint8_t Frog_add_live(frogptr_t frog);

/**
 * @brief Advances lane state
 * @param frog Pointer to frog
 */ 
uint8_t Frog_kill(frogptr_t frog);

/**
 * @brief Moves frog to position
 * @param frog Pointer to frog
 * @param x Desired reference x value
 * @param y Desired reference y value
 */ 
void Frog_move(frogptr_t frog, uint16_t x, uint16_t y);

/**
 * @brief Moves frog to position
 * @param frog Pointer to frog
 * @param x Desired reference x value
 */ 
void Frog_move_x(frogptr_t frog, uint16_t x);

/**
 * @brief Moves frog to position
 * @param frog Pointer to frog
 * @param y Desired reference y value
 */ 
void Frog_move_y(frogptr_t frog, uint16_t y);

#endif