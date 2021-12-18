#ifndef FROG_H
#define FROG_H
#include<stdint.h>
#include"../libs/position.h"

#define DEFAULT_LIVES 3

typedef struct {
    uint8_t lives;
    move_t movement;
    uint8_t color;
    uint16_t x;
    uint8_t lane;
} frog_t;

typedef frog_t * frogptr_t;

uint8_t Frog_reset_lives(frogptr_t frog);
uint8_t Frog_add_live(frogptr_t frog);
uint8_t Frog_kill(frogptr_t frog);

void Frog_move(frogptr_t frog, uint16_t x, uint16_t y);
void Frog_move_x(frogptr_t frog, uint16_t x);
void Frog_move_y(frogptr_t frog, uint16_t y);

#endif