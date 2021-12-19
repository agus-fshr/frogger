/***************************************************************************//**
  @file     Frog.h
  @brief    Implementación del objeto rana
  @author   Grupo 7
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Frog.h"



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void Frog_reset_lives(frogptr_t frog) {
    frog->lives = LVL_LIVES;
}


void Frog_add_live(frogptr_t frog) {
    frog->lives++;
}


uint8_t Frog_kill(frogptr_t frog) {
    if(frog->lives > 0) {
        frog->lives--;
        return 1;
    }
    return 0;
}


void Frog_move(frogptr_t frog, uint16_t x, uint16_t y) {
    frog->x = x;
    frog->lane = y;
}


void Frog_move_x(frogptr_t frog, uint16_t x) {
    frog->x = x;
}


void Frog_move_y(frogptr_t frog, uint16_t y) {
    frog->lane = y;
}