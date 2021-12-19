/***************************************************************************//**
  @file     LEDMatrixEngine.c
  @brief    Implementación de la interfaz específica RPi
  @author   Grupo 7
 ******************************************************************************/
#include "LEDMatrixEngine.h"


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include<stdio.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void render_map(levelptr_t level);
static void render_pause(engineptr_t eng);
static void render_menu(engineptr_t eng);
static void render_death(engineptr_t eng);
static void LEDMatEngine_render(engineptr_t eng);
static input_t LEDMatEngine_input(engineptr_t eng);
static void show_menu_score(uint32_t score);
static void draw_menu_arrows();
static void show_menu_lives(uint8_t lives);



/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static dcoord_t dcoord;

const dlevel_t arrow_up[BMP_WIDTH*BMP_HEIGHT] = {
    D_OFF, D_OFF, D_ON, D_OFF, D_OFF,
    D_OFF, D_ON, D_ON, D_ON, D_OFF,
    D_ON, D_ON, D_ON, D_ON, D_ON,
    D_OFF, D_OFF, D_OFF, D_OFF, D_OFF,
    D_OFF, D_OFF, D_OFF, D_OFF, D_OFF
};
    
const dlevel_t arrow_down[BMP_WIDTH*BMP_HEIGHT] = {
    D_OFF, D_OFF, D_OFF, D_OFF, D_OFF,
    D_OFF, D_OFF, D_OFF, D_OFF, D_OFF,
    D_ON, D_ON, D_ON, D_ON, D_ON,
    D_OFF, D_ON, D_ON, D_ON, D_OFF,
    D_OFF, D_OFF, D_ON, D_OFF, D_OFF
};



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
int LEDMatEngine_init(engineptr_t eng) {
    joy_init();
    disp_init();
    dcoord.x = 0;
    dcoord.y = 0;
    return 1;
}

int LEDMatEngine_destroy(engineptr_t eng){
    return 1;
}

int LEDMatEngine_gameloop(engineptr_t eng) {
    input_t system_input = LEDMatEngine_input(eng);
    
    process_game_state(eng, system_input);

    //printf("%d %d %d %d %d\n", eng->state, eng->playstate, eng->pausestate, eng->menustate, eng->deathstate);
    if(eng->state == GAME_STA_EXIT) {
        return 1;
    }

    //LEDMatEngine_input(eng);
    LEDMatEngine_render(eng);
    
    //printf("%d\n", eng->score);
    usleep(1000000/48);
    
    return 0;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void LEDMatEngine_render(engineptr_t eng) {
    
    disp_clear_buf();
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
        case GAME_STA_EXIT:
        default:
            break;
    }
    disp_update();
}


static input_t LEDMatEngine_input(engineptr_t eng) {
    static input_t last_input = INPUT_NULL;
    static jswitch_t last_switch = J_NOPRESS;

    joy_update();
    jcoord_t coord = joy_get_coord();
    jswitch_t sw = joy_get_switch();
    input_t actual_input = INPUT_NULL;
    
    if(sw == J_PRESS && last_switch == J_NOPRESS) {
        process_game_state(eng, INPUT_ENTER);
    }
    last_switch = sw;
    /* NOT WORKING!
    if(actual_input != INPUT_NULL && actual_input != INPUT_ENTER) {
        //sound_play(SFX_HOP, eng->volume, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    */
    if(coord.y > DEADZONE)
        actual_input = INPUT_UP;
    else if(coord.y < -DEADZONE)
        actual_input = INPUT_DOWN;
    else if(coord.x > DEADZONE)
        actual_input = INPUT_RIGHT;
    else if(coord.x < -DEADZONE)
        actual_input = INPUT_LEFT;

    if(sw != last_switch) {
        last_switch = sw;
        actual_input = sw == J_PRESS ? INPUT_ENTER : INPUT_NULL;
    }

    if(actual_input != last_input) {
        last_input = actual_input;
        return actual_input;
    } 
    return INPUT_NULL;
}


static void render_pause(engineptr_t eng) {
    dcoord.x = 1;
    dcoord.y = 6;

    if(eng->pausestate == PAUSE_STA_OP_1) {
        write_word("RES", dcoord, 3);
    } else if(eng->pausestate == PAUSE_STA_OP_2) {
        write_word("RST", dcoord, 3);
    } else if(eng->pausestate == PAUSE_STA_OP_3) {
        write_word("EXT", dcoord, 3);
    }

    draw_menu_arrows();
    show_menu_lives(eng->level->frog->lives);
    show_menu_score(eng->score);
}


static void render_menu(engineptr_t eng) {
    dcoord.x = 1;
    dcoord.y = 6;
    if(eng->menustate == MENU_STA_OP_1) {
        write_word("PLY", dcoord, 3);
    } else if(eng->menustate == MENU_STA_OP_2) {
        write_word("QIT", dcoord, 3);
    }

    draw_menu_arrows();
    show_menu_score(get_highscore());
    
    //disp_write_sanitized(dcoord, D_ON);
    //write_letter('E', dcoord);
}


static void render_death(engineptr_t eng) {
    dcoord.x = 1;
    dcoord.y = 6;
    if(eng->deathstate == DEATH_STA_MENU_OP_1) {
        write_word("TRY", dcoord, 3);
    } else if(eng->deathstate == DEATH_STA_MENU_OP_2) {
        write_word("EXT", dcoord, 3);
    }

    draw_menu_arrows();
    show_menu_score(eng->score);
}


static void render_map(levelptr_t level) {
    static uint8_t flicker = 0;

    int16_t i=0, p=0;
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
            for(p = -2; p < (LEVEL_WIDTH / lane->delta) + 2; p++) {
                
                if(lane->type == MOB_CAR || lane->type == MOB_LOG) {
                    dlevel_t val = lane->type == MOB_CAR ? D_ON : D_OFF;
                    int16_t pretended_x = scale_width(Lane_get_elem_x(lane, p), BLOCK_WIDTH);                    
                    int16_t limit_x = scale_width(Lane_get_elem_x_end(lane, p), BLOCK_WIDTH);
                    //printf("%d %d %d %d %d\n", i, p, Lane_get_elem_x(lane, p), pretended_x, limit_x);
                    while(pretended_x < limit_x){
                        //printf("%d %d %d %d %d\n", i, p, Lane_get_elem_x(lane, p), pretended_x, limit_x);
                        dcoord.x = pretended_x;
                        disp_write_sanitized(dcoord, val);
                        pretended_x += BLOCK_WIDTH;
                    }
                } else if(lane->type == MOB_FINISH) {
                    dcoord.x = scale_width(Lane_get_elem_x(lane, p), BLOCK_WIDTH);
                    disp_write_sanitized(dcoord, D_ON);
                }
            }
        }
    }
    
    dcoord.x = scale_width(level->frog->x, BLOCK_WIDTH);
    dcoord.y = level->frog->lane;
    disp_write_sanitized(dcoord, flicker == 1 ? D_ON : D_OFF);
    if(flicker == 2) {
        flicker = 0;
    }
    flicker++;
    
    laneptr_t finisherlane = level->lanes[0];
    for(i = 0; i < LVL_FINISHSPOTS; i++) {
        dcoord.x = scale_width(Lane_get_elem_x(finisherlane, level->finishers[i]), BLOCK_WIDTH),
        dcoord.y = 0;
        disp_write_sanitized(dcoord, D_OFF);
    }
}


static void show_menu_score(uint32_t score) {
    dcoord.y = DISP_MAX_Y+1;
    dcoord.x = DISP_MAX_X;
    while(dcoord.y > 0) {
        dcoord.y--;
        disp_write_sanitized(dcoord, (score >> dcoord.y) & 1 ? D_ON : D_OFF);
    }
}


static void draw_menu_arrows() {
    dcoord.y = 0;
    dcoord.x = 5;
    write_bmp(arrow_up, dcoord);
    dcoord.y = DISP_MAX_Y-4;
    dcoord.x = 5;
    write_bmp(arrow_down, dcoord);
}


static void show_menu_lives(uint8_t lives) {
    dcoord.y = 0;
    dcoord.x = 0;
    while(dcoord.y < lives) {
        disp_write_sanitized(dcoord, D_ON);
        dcoord.y++;
    }
}