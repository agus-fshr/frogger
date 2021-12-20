/***************************************************************************//**
  @file     Level.c
  @brief    Implementación del objeto nivel
  @author   Grupo 7
 ******************************************************************************/


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Level.h"
#include<stdio.h>


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static int8_t gen_sign();
static void generate_car_lane(laneptr_t lane, uint8_t diff);
static void generate_log_lane(laneptr_t lane, uint8_t diff);
static void generate_floor_lane(laneptr_t lane);
static uint8_t Level_check_collisions(levelptr_t level, uint8_t* finish);
static void Level_generate(levelptr_t level);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
uint8_t Level_init(levelptr_t level) {
    uint8_t i = 0;
    
    level->finisher_count = 0;
    for(i = 0; i < LVL_FINISHSPOTS; i++) {
        level->finishers[i] = -100;
    }
    level->number = 0;
    level->paused = 0;
    level->score = 0;
    level->lanes = calloc(LEVEL_HEIGHT, sizeof(laneptr_t));
    if(level->lanes == NULL){
        return 0;
    }
    level->frog = malloc(sizeof(frog_t));
    if(level->frog == NULL) {
        free(level->lanes);
        return 0;
    }

    for(i = 0; i < LEVEL_HEIGHT; i++) {
        level->lanes[i] = malloc(sizeof(lane_t));
        if(level->lanes[i] == NULL){
            while(i >= 0){
                free(level->lanes[i--]);
            }
            free(level->lanes);
            free(level->frog);
            return 0;
        }
        level->lanes[i]->delta = 100;
        level->lanes[i]->step = 100;
        level->lanes[i]->x0 = 100;
        level->lanes[i]->mob_length = 1;
    }

    srand((unsigned int) time(NULL));
    return 1;
}


uint8_t Level_delete(levelptr_t level) {
    uint8_t i = 0;
    
    for(i = 0; i < LEVEL_HEIGHT; i++) {        
        free(level->lanes[i]);
    }
    free(level->lanes);
    free(level->frog);
    free(level);
    return 1;
}


uint8_t Level_process_collisions(levelptr_t level, float volume) {
    uint16_t frogy = level->frog->lane;
    uint8_t finish_order;

    uint8_t collided = Level_check_collisions(level, &finish_order);
    uint8_t car_collision = collided && level->lanes[frogy]->type == MOB_CAR;
    uint8_t log_collision = !collided && level->lanes[frogy]->type == MOB_LOG;
    uint8_t finisher_collision = !collided && level->lanes[frogy]->type == MOB_FINISH;
    /*
    #ifdef CHEAT
        car_collision = 0;
        log_collision = 0;
    #endif
    */
    if(car_collision || log_collision || finisher_collision) {
        if(Frog_kill(level->frog) == 1) {
            Frog_move(level->frog, SPAWN_X, SPAWN_Y);
        }
    }

    uint8_t sum = 0;
    if(collided && level->lanes[frogy]->type == MOB_FINISH) {
        if(is_in_array(level->finishers, finish_order, LVL_FINISHSPOTS)){
            Frog_kill(level->frog);
        } else {
            level->finishers[level->finisher_count++] = finish_order;
            sum = 1;
            if(level->finisher_count == LVL_FINISHSPOTS){
                sum = 5*(level->number + 1);
                Level_next(level);
            } else {
                Frog_move(level->frog, SPAWN_X, SPAWN_Y);
            }
        }
    }
    return sum;
}


void Level_next(levelptr_t level) {
    uint8_t i = 0;
    level->finisher_count = 0;
    level->number++;
    Level_generate(level);
    Frog_reset_lives(level->frog);
    Frog_move(level->frog, SPAWN_X, SPAWN_Y);
    for(i = 0; i < LVL_FINISHSPOTS; i++) {
        level->finishers[i] = OUT_OF_BOUNDS;
    }
}


void Level_reset(levelptr_t level) {
    level->number = 0;
    level->score = 0;
    Level_next(level);
}


uint8_t is_in_array(int16_t *arr, int16_t elem, int16_t len) {
    uint8_t i = 0;
    for(i = 0; i < len; i++) {
        if(arr[i] == elem) return 1;
    }
    return 0;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static uint8_t Level_check_collisions(levelptr_t level, uint8_t* finish) {
    float frog_x = ((float) level->frog->x) / REFERENCE_WIDTH;
    uint8_t frog_y = level->frog->lane;
    int8_t p = 0; // iterator
    laneptr_t lane = level->lanes[frog_y];
    if(lane->delta != 0) {   
        // For some extra elements of the lane, calculates
        // initial x and final x values, comparing to player's x

        // This could use Lane_get_elem_x and Lane_get_elem_x_end
        // but we had time issues to make it work nicely
        for(p = -1; p < LEVEL_WIDTH / lane->delta + 2; p++) {    
            float start = ((float) lane->x0)/REFERENCE_WIDTH + p*lane->delta;
            if(((frog_x+1) > start) && (frog_x < (start + lane->mob_length))) {
                *finish = p;
                return 1;
            }
        }
    }
    return 0;
}


static int8_t gen_sign() {
    return 1 - 2 * (rand() % 2);
}

static void generate_car_lane(laneptr_t lane, uint8_t diff) {
    lane->ticks = 0;
    lane->type = MOB_CAR;
    lane->step = gen_sign() * (MIN_VEHICLE_STEP + rand() % DIFF_SCALING_VEHICLE_STEP(diff));
    if(lane->step == 0) {
        lane->step = gen_sign();
    }
    lane->mob_length = MIN_VEHICLE_LENGTH + rand() % MAX_VEHICLE_LENGTH;
    lane->delta = MIN_DELTA(lane) + abs(lane->step)*VEHICLE_DELTA_PER_STEP;
    
    if(diff < CUT_OFF_DIFFICULTY) 
        lane->delta += (rand() % DIFF_SCALING_VEHICLE_DELTA(diff));
    lane->x0 = rand() % ((uint16_t) REFERENCE_WIDTH);
}

static void generate_log_lane(laneptr_t lane, uint8_t diff) {  
    lane->ticks = 0;
    lane->type = MOB_LOG;
    lane->step = gen_sign() * (MIN_LOG_STEP + rand() % DIFF_SCALING_LOG_STEP(diff));
    lane->mob_length = MIN_LOG_LENGTH;
    if(diff < CUT_OFF_DIFFICULTY)
         lane->mob_length += (rand() % ((MAX_LOG_LENGTH - MIN_LOG_LENGTH) + DIFF_SCALING_LOG_LENGTH(diff)));

    lane->delta = MIN_DELTA(lane) + (rand() % MAX_LOG_EXTRA_DELTA);
    lane->x0 = rand() % ((uint16_t) REFERENCE_WIDTH);
}

static void generate_floor_lane(laneptr_t lane) {
    lane->ticks = 0;
    lane->type = MOB_FLOOR;
    lane->step = 0;
    lane->mob_length = 0;
    lane->delta = 1;    // To prevent errors
    lane->x0 = 0;
}


static void Level_generate(levelptr_t level) {
    uint8_t i = 0;
    for(i = 0; i < LEVEL_HEIGHT; i++) {
        if(i == FINISH_LANE) {
            level->lanes[FINISH_LANE]->type = MOB_FINISH;
            level->lanes[FINISH_LANE]->delta = LEVEL_WIDTH/LVL_FINISHSPOTS;
            level->lanes[FINISH_LANE]->step = 0;
            // The next line tries to center the objective pads
            level->lanes[FINISH_LANE]->x0 = REFERENCE_WIDTH * (level->lanes[FINISH_LANE]->delta / 2);
            level->lanes[FINISH_LANE]->mob_length = 1;
        } else if(i == START_LANE || i == REST_LANE) {
            generate_floor_lane(level->lanes[i]);
        }else if(i < REST_LANE){
            generate_log_lane(level->lanes[i], level->number);
        } else {
            generate_car_lane(level->lanes[i], level->number);
        }
    }
}