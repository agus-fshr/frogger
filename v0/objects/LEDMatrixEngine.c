#include "LEDMatrixEngine.h"

static void render_map(levelptr_t level);
static void render_pause(engineptr_t eng);
static void render_menu(engineptr_t eng);
static void render_death(engineptr_t eng);

static dcoord_t dcoord;

int LEDMatEngine_init(engineptr_t eng, void* param) {
    joy_init();
    disp_init();
    dcoord.x = 0;
    dcoord.y = 0;
    return 0;
}

int LEDMatEngine_destroy(engineptr_t eng, void* param){
    return 0;
}

int LEDMatEngine_render(engineptr_t eng, void* param) {
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
    return 0;
}

int LEDMatEngine_input(engineptr_t eng, void* param) {
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
        sound_play(SFX_HOP, eng->volume, ALLEGRO_PLAYMODE_ONCE, NULL);
    }

    return 0;
}

static void render_pause(engineptr_t eng) {
    dcoord.x = 0;
    dcoord.y = eng->pausestate;
    disp_write(dcoord, D_ON);
}
static void render_menu(engineptr_t eng) {
    dcoord.x = 0;
    dcoord.y = eng->menustate;
    disp_write(dcoord, D_ON);
}
static void render_death(engineptr_t eng) {
    dcoord.x = 0;
    dcoord.y = eng->deathstate;
    disp_write(dcoord, D_ON);
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
                        disp_write(dcoord, D_ON);
                        dcoord.x += BLOCK_WIDTH;
                    }
                } else if(lane->type == MOB_LOG) {
                    dcoord.x = 0;
                    while(dcoord.x < DISP_WIDTH) {
                        disp_write(dcoord, D_ON);
                        dcoord.x += BLOCK_WIDTH;
                    }
                    dcoord.x = Lane_get_elem_x(lane, p);
                    while(dcoord.x < Lane_get_elem_x_end(lane, p)){
                        disp_write(dcoord, D_OFF);
                        dcoord.x += BLOCK_WIDTH;
                    }
                } else if(lane->type == MOB_FINISH) {
                    dcoord.x = Lane_get_elem_x(lane, p);
                    disp_write(dcoord, D_ON);
                }
            }
        }
        
    }
    
    uint16_t frogx = level->frog->x;
    uint16_t frogy = level->frog->lane;
    dcoord.x = frogx;
    dcoord.y = frogy;

    disp_write(dcoord, D_ON);
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