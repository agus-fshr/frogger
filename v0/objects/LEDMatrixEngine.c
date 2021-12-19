#include "LEDMatrixEngine.h"
#include<stdio.h>

static void render_map(levelptr_t level);
static void render_pause(engineptr_t eng);
static void render_menu(engineptr_t eng);
static void render_death(engineptr_t eng);
static void LEDMatEngine_render(engineptr_t eng);
static void LEDMatEngine_input(engineptr_t eng);
static void disp_write_sanitized(dcoord_t coord, dlevel_t val);

static dcoord_t dcoord;

static void disp_write_sanitized(dcoord_t coord, dlevel_t val) {
    if(coord.x <= DISP_MAX_X && coord.y <= DISP_MAX_Y) {
        disp_write(coord, val);
    }
}
int LEDMatEngine_init(engineptr_t eng) {
    joy_init();
    disp_init();
    dcoord.x = 0;
    dcoord.y = 0;
    return 0;
}

int LEDMatEngine_destroy(engineptr_t eng){
    return 0;
}

int LEDMatEngine_gameloop(engineptr_t eng) {
    uint8_t i = 0;

    process_game_state(eng, INPUT_NULL);
    if(eng->state == GAME_STA_PLAY) {
        for(i = 0; i < LEVEL_HEIGHT; i++) {
            Lane_tick(eng->level->lanes[i]);
        }
    }
    //printf("%d %d %d %d %d\n", eng->state, eng->playstate, eng->pausestate, eng->menustate, eng->deathstate);
    if(eng->state == GAME_STA_EXIT) {
        return 1;
    }
    laneptr_t lane = eng->level->lanes[eng->level->frog->lane];
    if(lane->type == MOB_LOG) {
        int16_t newx = eng->level->frog->x + lane->step;
        if((newx > 0) && (newx+REFERENCE_WIDTH < REFERENCE_WIDTH*LEVEL_WIDTH)) {
            eng->level->frog->x = newx;
        } else {
            if(newx < 0) eng->level->frog->x = 0;
            else eng->level->frog->x = (LEVEL_WIDTH - 1)*REFERENCE_WIDTH;
        }
    }
    
    LEDMatEngine_input(eng);
    LEDMatEngine_render(eng);
    eng->score += Level_process_collisions(eng->level, eng->volume);
    
    usleep(1000000/60);
    
    return 0;
}

static void LEDMatEngine_render(engineptr_t eng) {
    disp_clear();
    //printf("A\n");
    //printf("%d\n", eng->state);
    switch(eng->state) {
        case GAME_STA_MENU:
            render_menu(eng);
            break;

        case GAME_STA_PLAY:
            render_map(eng->level);
            break;
        
        case GAME_STA_PAUSE:
            render_pause(eng);
            break;
        
        case GAME_STA_DEATH:
            render_death(eng);
            break;
    }
    disp_update();
}

static void LEDMatEngine_input(engineptr_t eng) {
    levelptr_t level = eng->level;
    joy_update();
    jcoord_t coord = joy_get_coord();
    jswitch_t sw = joy_get_switch();

    uint8_t joyup = coord.y > DEADZONE;
    uint8_t joydown = coord.y < -DEADZONE;
    uint8_t joyright = coord.x > DEADZONE;
    uint8_t joyleft = coord.x < -DEADZONE;
    
    if(joyup)
        process_game_state(eng, INPUT_UP);
    else if(joydown)
        process_game_state(eng, INPUT_DOWN);
    else if(joyleft)
        process_game_state(eng, INPUT_LEFT);
    else if(joyright)
        process_game_state(eng, INPUT_RIGHT);

    if(sw == J_PRESS)
        process_game_state(eng, INPUT_ENTER);
        
    if(joyup || joydown || joyleft || joyright) {
        //sound_play(SFX_HOP, eng->volume, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}

static void render_pause(engineptr_t eng) {
    dcoord.x = 0;
    dcoord.y = eng->pausestate;
    disp_write_sanitized(dcoord, D_ON);
}
static void render_menu(engineptr_t eng) {
    dcoord.x = 8;
    dcoord.y = 8 + eng->menustate;
    disp_write_sanitized(dcoord, D_ON);
}
static void render_death(engineptr_t eng) {
    dcoord.x = 0;
    dcoord.y = eng->deathstate;
    disp_write_sanitized(dcoord, D_ON);
}
static void render_map(levelptr_t level) {
    int16_t i=0, p=0, x=0;
    for(i = 0; i < LEVEL_HEIGHT; i++) {
        laneptr_t lane = level->lanes[i];
        dcoord.y = i;
        
        if(lane->delta != 0) {
            if(lane->type == MOB_LOG) {
                dcoord.x = 0;
                while(dcoord.x < DISP_WIDTH) {
                    disp_write_sanitized(dcoord, D_ON);
                    dcoord.x += BLOCK_WIDTH;
                }
            }
            for(p = -1; p < (LEVEL_WIDTH / lane->delta) + 2; p++) {
                
                if(lane->type == MOB_CAR) {
                    dcoord.x = scale_width(Lane_get_elem_x(lane, p), BLOCK_WIDTH);
                    printf("%d %d\n", Lane_get_elem_x(lane, p), dcoord.x);
                    uint8_t limit_x = scale_width(Lane_get_elem_x_end(lane, p), BLOCK_WIDTH);
                    while(dcoord.x < limit_x){
                        disp_write_sanitized(dcoord, D_ON);
                        dcoord.x += BLOCK_WIDTH;
                    }
                } else if(lane->type == MOB_LOG) {
                    dcoord.x = scale_width(Lane_get_elem_x(lane, p), BLOCK_WIDTH);
                    uint8_t limit_x = scale_width(Lane_get_elem_x_end(lane, p), BLOCK_WIDTH);
                    while(dcoord.x < limit_x){
                        disp_write_sanitized(dcoord, D_OFF);
                        dcoord.x += BLOCK_WIDTH;
                    }
                } else if(lane->type == MOB_FINISH) {
                    dcoord.x = scale_width(Lane_get_elem_x(lane, p), BLOCK_WIDTH);
                    disp_write_sanitized(dcoord, D_ON);
                }
            }
        }
        
    }
    
    uint16_t frogx = scale_width(level->frog->x, BLOCK_WIDTH);
    uint16_t frogy = level->frog->lane;
    dcoord.x = frogx;
    dcoord.y = frogy;

    disp_write_sanitized(dcoord, D_ON);
    /*
    laneptr_t finisherlane = level->lanes[0];
    for(i = 0; i < LVL_FINISHSPOTS; i++) {
        al_draw_filled_rectangle(
            Lane_get_elem_x(finisherlane, level->finishers[i]),
            0,
            Lane_get_elem_x_end(finisherlane, level->finishers[i]),
            BLOCK_HEIGHT,
            al_map_rgb(255, 255, 0)); 
    }
    */

}
