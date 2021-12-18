#ifndef LANE_H
#define LANE_H
#include<stdint.h>
#include"../libs/position.h"

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

void Lane_tick(laneptr_t lane);
int16_t Lane_get_elem_x(laneptr_t lane, int8_t elem);
int16_t Lane_get_elem_x_end(laneptr_t lane, int8_t elem);


#endif