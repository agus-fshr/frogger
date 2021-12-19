/***************************************************************************//**
  @file     Lane.c
  @brief    Implementación del objeto carril
  @author   Grupo 7
 ******************************************************************************/


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Lane.h"


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void Lane_tick(laneptr_t lane) {
    lane->ticks++;
    lane->x0 += lane->step;
    if((lane->x0 >= REFERENCE_WIDTH*lane->delta) || (lane->x0 <= -REFERENCE_WIDTH*lane->delta)) {
        lane->x0 = 0;
    }
}

int16_t Lane_get_elem_x(laneptr_t lane, int8_t elem) {
    return lane->x0 + elem*lane->delta*REFERENCE_WIDTH;
}

int16_t Lane_get_elem_x_end(laneptr_t lane, int8_t elem) {
    return lane->x0 + (elem * lane->delta + lane->mob_length)*REFERENCE_WIDTH;
}
