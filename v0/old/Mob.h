#ifndef MOB_H
#define MOB_H
#include<stdint.h>
#include"../libs/position.h"

typedef enum {MOB_CAR = 1, MOB_LOG, MOB_FINISH} mobtype_t;

typedef struct {
    mobtype_t type;
    uint8_t tickspeed;
    move_t movement;
    uint8_t x;
} mob_t;

typedef mob_t * mobptr_t; 

#endif