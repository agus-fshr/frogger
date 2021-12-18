#include "LEDMatrixEngine.h"

static void render_map(levelptr_t level);
static void render_pause(engineptr_t eng);
static void render_menu(engineptr_t eng);
static void render_death(engineptr_t eng);
static void LEDMatEngine_render(engineptr_t eng);
static void LEDMatEngine_input(engineptr_t eng);
static void disp_write_sanitized(dcoord_t coord, dlevel_t val);

static dcoord_t dcoord;

static void disp_write_sanitized(dcoord_t coord, dlevel_t val) {
    if(coord.x < DISP_MAX_X && coord.y < DISP_MAX_Y) {
        disp_write_sanitized(coord, val);
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
    process_game_state(eng, INPUT_NULL);

    if(eng->state == GAME_STA_EXIT) {
        return 1;
    }
    laneptr_t lane = eng->level->lanes[eng->level->frog->lane];
    if(lane->type == MOB_LOG) {
        int16_t newx = eng->level->frog->x + lane->step;
        if((newx > 0) && (newx+BLOCK_WIDTH < BLOCK_WIDTH*LEVEL_WIDTH)) {
            eng->level->frog->x = newx;
        } else {
            if(newx < 0) eng->level->frog->x = 0;
            else eng->level->frog->x = (LEVEL_WIDTH - 1)*BLOCK_WIDTH;
        }
    }
    LEDMatEngine_input(eng);
    LEDMatEngine_render(eng);
    eng->score += Level_process_collisions(eng->level, eng->volume);
    //sleep(100);
    return 1;
}

static void LEDMatEngine_render(engineptr_t eng) {
    disp_clear();
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

    if(sw = J_PRESS)
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
    dcoord.x = 0;
    dcoord.y = eng->menustate;
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
            for(p = -1; p < (LEVEL_WIDTH / lane->delta) + 2; p++) {
                
                if(lane->type == MOB_CAR) {
                    dcoord.x = Lane_get_elem_x(lane, p);
                    while(dcoord.x < Lane_get_elem_x_end(lane, p)){
                        disp_write_sanitized(dcoord, D_ON);
                        dcoord.x += BLOCK_WIDTH;
                    }
                } else if(lane->type == MOB_LOG) {
                    dcoord.x = 0;
                    while(dcoord.x < DISP_WIDTH) {
                        disp_write_sanitized(dcoord, D_ON);
                        dcoord.x += BLOCK_WIDTH;
                    }
                    dcoord.x = Lane_get_elem_x(lane, p);
                    while(dcoord.x < Lane_get_elem_x_end(lane, p)){
                        disp_write_sanitized(dcoord, D_OFF);
                        dcoord.x += BLOCK_WIDTH;
                    }
                } else if(lane->type == MOB_FINISH) {
                    dcoord.x = Lane_get_elem_x(lane, p);
                    disp_write_sanitized(dcoord, D_ON);
                }
            }
        }
        
    }
    
    uint16_t frogx = level->frog->x;
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