#include "GameEngine.h"

void manage_score(engineptr_t eng);


static FILE* highscorefile;

void initialize_game_status(engineptr_t eng) {
    eng->state = GAME_STA_MENU;
    eng->deathstate = DEATH_STA_MENU_OP_1;
    eng->menustate = MENU_STA_OP_1;
    eng->pausestate = PAUSE_STA_OP_1;
    eng->playstate = PLAY_STA_INIT;
    eng->exitstate = EXIT_STA_OP_1;
    eng->volume = .1f;
}

void process_game_state(engineptr_t eng, input_t input) {

    switch(eng->state) {
        case GAME_STA_MENU:
            process_menu_state(eng, input);
            break;
        
        case GAME_STA_PAUSE:
            process_pause_state(eng, input);
            break;

        case GAME_STA_PLAY:
            process_play_state(eng, input);
            break;

        case GAME_STA_DEATH:
            process_death_state(eng, input);
            break;

        default:
            break;
    }
}

void process_menu_state(engineptr_t eng, input_t input) {
    switch(input) {
        case INPUT_UP:
        case INPUT_DOWN:
            if(eng->menustate == MENU_STA_OP_2) eng->menustate = MENU_STA_OP_1;
            else if(eng->menustate == MENU_STA_OP_1) eng->menustate = MENU_STA_OP_2;
            break;

        case INPUT_ENTER:
            if(eng->menustate == MENU_STA_OP_1) {
                eng->state = GAME_STA_PLAY;
                eng->playstate = PLAY_STA_INIT;
            }
            else eng->state = GAME_STA_EXIT;
            break;
            
        default:
            break;
    }
}

void process_pause_state(engineptr_t eng, input_t input) {
    switch(input) {
        case INPUT_UP:
            if(eng->pausestate == PAUSE_STA_OP_2) eng->pausestate = PAUSE_STA_OP_1;
            else if(eng->pausestate == PAUSE_STA_OP_3) eng->pausestate = PAUSE_STA_OP_2;
            else if(eng->pausestate == PAUSE_STA_OP_1) eng->pausestate = PAUSE_STA_OP_3;
            break;
        
        case INPUT_DOWN:
            if(eng->pausestate == PAUSE_STA_OP_1) eng->pausestate = PAUSE_STA_OP_2;
            else if(eng->pausestate == PAUSE_STA_OP_2) eng->pausestate = PAUSE_STA_OP_3;
            else if(eng->pausestate == PAUSE_STA_OP_3) eng->pausestate = PAUSE_STA_OP_1;
            break;
        
        case INPUT_LEFT:
            eng->volume -= 0.1f;
            if(eng->volume < 0) eng->volume = 0.0f;
            break;


        case INPUT_RIGHT:
            eng->volume += 0.1f;
            if(eng->volume > 0) eng->volume = 1.0f;
            break;

        case INPUT_ENTER:
            if(eng->pausestate == PAUSE_STA_OP_1) eng->state = GAME_STA_PLAY;
            else if(eng->pausestate == PAUSE_STA_OP_2) {
                eng->state = GAME_STA_PLAY;
                eng->playstate = PLAY_STA_INIT;
            } else if(eng->pausestate == PAUSE_STA_OP_3) {
                eng->state = GAME_STA_MENU;
                eng->menustate = MENU_STA_OP_1;
            }
            break;
            
        default:
            if(eng->playstate == PLAY_STA_INIT) {
                Level_reset(eng->level);
                eng->score = 0;
            }
            break;
    }
}

void process_death_state(engineptr_t eng, input_t input) {
    
    switch(input) {
        case INPUT_UP:
        case INPUT_DOWN:
            if(eng->deathstate == DEATH_STA_MENU_OP_2) eng->deathstate = DEATH_STA_MENU_OP_1;
            else if(eng->deathstate == DEATH_STA_MENU_OP_1) eng->deathstate = DEATH_STA_MENU_OP_2;
            break;

        case INPUT_ENTER:
            if(eng->deathstate == MENU_STA_OP_1) {
                eng->state = GAME_STA_PLAY;
                eng->playstate = PLAY_STA_INIT;
            }
            else eng->state = GAME_STA_EXIT;
            break;
            
        default:
            break;
    }
}

void process_play_state(engineptr_t eng, input_t input) {
    uint8_t i = 0;

    if(eng->playstate == PLAY_STA_INIT) {
        eng->playstate = PLAY_STA_1;
        Level_reset(eng->level);
    }
    if(eng->level->frog->lives == 0) {
        eng->state = GAME_STA_DEATH;
        eng->deathstate = DEATH_STA_MENU_OP_1;
        return;
    }

    for(i = 0; i < LEVEL_HEIGHT; i++) {
        Lane_tick(eng->level->lanes[i]);
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
    
    eng->score += Level_process_collisions(eng->level, eng->volume);
    manage_score(eng);

    switch(input) {
        case INPUT_UP:
            if(eng->level->frog->lane >= 1)
                eng->level->frog->lane -= 1;
            eng->level->frog->movement = MOVE_UP;
            //if(eng->pausestate == PAUSE_STA_OP_2) eng->pausestate = PAUSE_STA_OP_1;
            //else if(eng->pausestate == PAUSE_STA_OP_3) eng->pausestate = PAUSE_STA_OP_2;
            break;
        
        case INPUT_DOWN:
            if(eng->level->frog->lane < LEVEL_HEIGHT-1)
                eng->level->frog->lane += 1;
            eng->level->frog->movement = MOVE_DOWN;
            break;
        
        case INPUT_LEFT:
            if(eng->level->frog->x >= REFERENCE_WIDTH)
                eng->level->frog->x -= REFERENCE_WIDTH;
            eng->level->frog->movement = MOVE_LEFT;
            //eng->volume -= 0.1f;
            //if(eng->volume < 0) eng->volume = 0.0f;
            break;


        case INPUT_RIGHT:
            if(eng->level->frog->x < (LEVEL_WIDTH-1)*REFERENCE_WIDTH)
                eng->level->frog->x += REFERENCE_WIDTH;
            eng->level->frog->movement = MOVE_RIGHT;
            //eng->volume += 0.1f;
            //if(eng->volume > 0) eng->volume = 1.0f;
            break;

        case INPUT_ENTER:
            eng->state = GAME_STA_PAUSE;
            eng->pausestate = PAUSE_STA_OP_1;
            break;
            
        default:
            break;
    }
}

void engine_destroy_wrapper(engineptr_t eng) {
    eng->destroy(eng);
    Level_delete(eng->level);
    fclose(highscorefile);
    free(eng);
}

void engine_init_wrapper(engineptr_t eng) {
    highscorefile = fopen("highscore.txt", "w+");
    initialize_game_status(eng);
    eng->level = malloc(sizeof(level_t));
    Level_init(eng->level);
    Level_reset(eng->level);
    eng->init(eng);
}

float scale_width(int16_t width, int16_t block_width) {
    return width*block_width/REFERENCE_WIDTH;
}

void manage_score(engineptr_t eng) {
    fseek(highscorefile, 0, SEEK_SET);
    uint32_t highscore;
    fread(&highscore, sizeof(uint32_t), 1, highscorefile);
    if(eng->score > highscore) {
        fwrite(&eng->score, sizeof(uint32_t), 1, highscorefile);
        fflush(highscorefile);
    }
}