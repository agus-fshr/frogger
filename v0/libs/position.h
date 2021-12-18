#ifndef POSITION_H
#define POSITION_H
#include<stdint.h>


typedef struct {
    uint16_t x;
    uint16_t y;
} coord_t;

typedef enum {STOP, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT} move_t;

#endif