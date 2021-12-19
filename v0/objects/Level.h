#ifndef LEVEL_H
#define LEVEL_H
#include<stdint.h>
#include<stdlib.h>
#include"Frog.h"
#include"Lane.h"
#include"../config.h"

#define REFERENCE_WIDTH (64.0)
#define SPAWN_X (LEVEL_WIDTH*REFERENCE_WIDTH/2)
#define SPAWN_Y (LEVEL_HEIGHT-1)

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


uint8_t Level_init(levelptr_t level);
uint8_t Level_end(levelptr_t level);
void Level_next(levelptr_t level);
void Level_reset(levelptr_t level);
//void Level_gen(levelptr_t level);
uint8_t Level_process_collisions(levelptr_t level, float volume);
uint8_t Level_delete(levelptr_t level);
uint8_t is_in_array(int16_t *arr, int16_t elem, int16_t len);

#endif