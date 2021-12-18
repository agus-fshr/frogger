#include "Level.h"
#include<stdio.h>

#ifdef LEDMAT
    #include "LEDMatrixEngine.h"
#else
    #include "AllegroEngine.h"
#endif

static int8_t gen_sign();
static void generate_car_lane(laneptr_t lane, uint8_t diff);
static void generate_log_lane(laneptr_t lane, uint8_t diff);
static void generate_floor_lane(laneptr_t lane);
static uint8_t Level_check_collisions(levelptr_t level, uint8_t* finish);
static uint8_t is_in_array(int16_t *arr, int16_t elem, int16_t len);
static void Level_generate(levelptr_t level);


uint8_t Level_init(levelptr_t level) {
    uint8_t i = 0, p = 0;
    
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

    return 1;
}

uint8_t Level_delete(levelptr_t level) {
    uint8_t i = 0, p = 0;
    
    for(i = 0; i < LEVEL_HEIGHT; i++) {        
        free(level->lanes[i]);
    }
    free(level->lanes);

    free(level->frog);
    free(level);
    return 1;
}


uint8_t Level_process_collisions(levelptr_t level, float volume) {
    uint16_t frogx = level->frog->x;
    uint16_t frogy = level->frog->lane;
    uint8_t finish_order;
    uint8_t collided = Level_check_collisions(level, &finish_order);
    uint8_t car_collision = collided && level->lanes[frogy]->type == MOB_CAR;
    uint8_t log_collision = !collided && level->lanes[frogy]->type == MOB_LOG;
    uint8_t finisher_collision = !collided && level->lanes[frogy]->type == MOB_FINISH;
    uint8_t done = 0;
    #ifdef CHEAT
        car_collision = 0;
        log_collision = 0;
    #endif

    if(car_collision || log_collision || finisher_collision) {
        if(Frog_kill(level->frog) == 0) {
            done = 1;
        }
        else {
            Frog_move(level->frog, SPAWN_X, SPAWN_Y);
        }
    }
    if(car_collision) {
        sound_play(SFX_SQUASH, volume, ALLEGRO_PLAYMODE_ONCE, NULL);
    } else if(log_collision) {
        sound_play(SFX_PLUNK, volume, ALLEGRO_PLAYMODE_ONCE, NULL);
    } else if(finisher_collision) {
        sound_play(SFX_SQUASH, volume, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    /*
    for(int i = 0; i < LVL_FINISHSPOTS; i++) {
        printf("%d ", level->finishers[i]);
    }
    printf("\n");
    */
   uint8_t sum;
    if(collided && level->lanes[frogy]->type == MOB_FINISH) {
        if(is_in_array(level->finishers, finish_order, LVL_FINISHSPOTS)){
            if(Frog_kill(level->frog) == 0){
                done = 1;
            } else {
                Frog_move(level->frog, SPAWN_X, SPAWN_Y);
            }
        } else{
            level->finishers[level->finisher_count++] = finish_order;
            //level->score += 1;
            sum = 1;
            if(level->finisher_count == LVL_FINISHSPOTS){
                //level->score += 5*(level->number + 1);
                sum = 5*(level->number + 1);
                Level_next(level);
            } else {
                Frog_move(level->frog, SPAWN_X, SPAWN_Y);
            }
            //printf("%d\n", level->score);
        }
    }
    //return done;
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
        level->finishers[i] = -10;
    }
}

void Level_reset(levelptr_t level) {
    level->number = 0;
    level->score = 0;
    Level_next(level);
}

static uint8_t Level_check_collisions(levelptr_t level, uint8_t* finish) {
    float frog_x = ((float) level->frog->x) / BLOCK_WIDTH;
    uint8_t frog_y = level->frog->lane;
    int8_t i = 0, p = 0; // iterator
    laneptr_t lane = level->lanes[frog_y];
    uint8_t collided = 0;
    if(lane->delta != 0) {   
        for(p = -1; p < LEVEL_WIDTH / lane->delta + 2; p++) {    
            float start = ((float) lane->x0)/BLOCK_WIDTH + p*lane->delta;
            //printf("%d %f %f\n", frog_y, frog_x, start);
            if(((frog_x+1) > start) && (frog_x < (start + lane->mob_length))) {
                *finish = p;
                return 1;
            }
        }
    }
    return 0;
}

// a a
static int8_t gen_sign() {
    return 1 - 2 * (rand() % 2);
}

static void generate_car_lane(laneptr_t lane, uint8_t diff) {
    lane->ticks = rand() % 500;
    lane->type = MOB_CAR;
    //lane->step = gen_sign() * ((20-diff) + rand() % (36 - diff));
    lane->step = gen_sign() * (2 + rand() % (5 + diff));
    if(lane->step == 0) {
        lane->step = gen_sign();
    }
    lane->mob_length = 1 + rand() % 3;
    lane->delta = lane->mob_length + 2 + abs(lane->step)/10;
    
    if(diff < 10) 
        lane->delta += (rand() % (20 - diff*2));
    lane->x0 = 10*(rand() % 6);
}

static void generate_log_lane(laneptr_t lane, uint8_t diff) {    
    lane->ticks = rand() % 500;
    lane->type = MOB_LOG;
    lane->step = gen_sign() * (1 + rand() % (8+diff));

    if(diff < 15)
        lane->mob_length = 2 + (rand() % (5 - (diff/3)));
    else
        lane->mob_length = 2;
    lane->delta = lane->mob_length + 2 + (rand() % 4);
    lane->x0 = rand() % BLOCK_WIDTH;
}

static void generate_floor_lane(laneptr_t lane) {
    lane->ticks = 0;
    lane->type = MOB_FLOOR;
    lane->step = 0;
    lane->mob_length = 0;
    lane->delta = 1;
    lane->x0 = 0;
}

uint8_t is_in_array(int16_t *arr, int16_t elem, int16_t len) {
    uint8_t i = 0;
    for(i = 0; i < len; i++) {
        if(arr[i] == elem) return 1;
    }
    return 0;
}

static void Level_generate(levelptr_t level) {
    uint8_t i = 0;
    for(i = 0; i < LEVEL_HEIGHT; i++) {
        if(i == 0) {
            level->lanes[0]->type = MOB_FINISH;
            level->lanes[0]->delta = 4;
            level->lanes[0]->step = 0;
            level->lanes[0]->x0 = 50;
            level->lanes[0]->mob_length = 1;
        } else if(i == 8 || i == 15) {
            generate_floor_lane(level->lanes[i]);
        }else if(i < 8){
            generate_log_lane(level->lanes[i], level->number);
        } else {
            generate_car_lane(level->lanes[i], level->number);
        }
    }
}