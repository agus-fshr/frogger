/***************************************************************************//**
  @file     GameEngine.c
  @brief    Implementación de la interfaz específica de Allegro
  @author   Grupo 7
 ******************************************************************************/
#include "AllegroEngine.h"

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void render_map(levelptr_t level);
static void render_pause(engineptr_t eng);
static void render_menu(engineptr_t eng);
static void render_death(engineptr_t eng);
static void render_street_lane(levelptr_t level, uint8_t lanenum);
static void render_water_lane(levelptr_t level, uint8_t lanenum);
static void render_floor_lane(levelptr_t level, uint8_t lanenum);
static void draw_score(uint32_t score);
static input_t AllegroEngine_input(engineptr_t eng, int key);
static void AllegroEngine_render(engineptr_t eng);
static void draw_vehicle(laneptr_t lane, uint8_t lanenum, uint8_t p);
static void draw_log(laneptr_t lane, uint8_t lanenum, uint8_t p);


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static ALLEGRO_DISPLAY* disp;
static ALLEGRO_BITMAP* bitmap;
static ALLEGRO_TIMER* timer;
static ALLEGRO_EVENT_QUEUE* queue;
static ALLEGRO_SAMPLE_ID background_music;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
int AllegroEngine_init(engineptr_t eng) {
    al_init();
    al_install_keyboard();
    timer = al_create_timer(TIMEBASE);
    queue = al_create_event_queue();
    
    sound_init();

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_VIDEO_BITMAP);
    
    al_init_ttf_addon();
    al_init_font_addon();
    al_init_primitives_addon();
    al_init_image_addon();

    al_set_new_display_flags(ALLEGRO_FRAMELESS);
    disp = al_create_display(DISP_WIDTH, DISP_HEIGHT);
    al_register_event_source(queue, al_get_display_event_source(disp));
    Frog_move(eng->level->frog, SPAWN_X, SPAWN_Y);
    Frog_reset_lives(eng->level->frog);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    
    al_start_timer(timer);

    sound_play(SFX_JINGLE, eng->volume, ALLEGRO_PLAYMODE_LOOP, &background_music);
    return 1;
}


int AllegroEngine_destroy(engineptr_t eng){
    //al_destroy_bitmap(bitmap);
    al_destroy_display(disp);
    sound_destroy();
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    return 0;
}


int AllegroEngine_gameloop(engineptr_t eng) {
    static ALLEGRO_EVENT event;
    gamestate_t prevstate = eng->state;

    al_wait_for_event(queue, &event);
    switch(event.type) {
        case ALLEGRO_EVENT_TIMER:
            prevstate = process_game_state(eng, INPUT_NULL);
            if(eng->state == GAME_STA_DEATH && prevstate != GAME_STA_DEATH)
                sound_play(SFX_RINGTONE, eng->volume, ALLEGRO_PLAYMODE_ONCE, NULL);
            
            if(eng->state == GAME_STA_EXIT) {
                return 1;
            }
            ALLEGRO_SAMPLE_INSTANCE* bgmusic_ins = al_lock_sample_id(&background_music);
            al_set_sample_instance_gain(bgmusic_ins, eng->volume);
            al_unlock_sample_id(&background_music);
            
            break;
    
        case ALLEGRO_EVENT_KEY_DOWN:
            process_game_state(eng, AllegroEngine_input(eng, event.keyboard.keycode));
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            return 1;
    }

    if(al_is_event_queue_empty(queue))
    {
        AllegroEngine_render(eng);
    }
    return 0;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void AllegroEngine_render(engineptr_t eng) {
    switch(eng->state) {
        case GAME_STA_MENU:
            render_menu(eng);
            break;

        case GAME_STA_PLAY:
            render_map(eng->level);
            draw_score(eng->score * 100);
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
    al_flip_display();
}


static input_t AllegroEngine_input(engineptr_t eng, int key) {
    #ifdef CHEAT
    levelptr_t level = eng->level;
    #endif
    switch(key) {
        #ifdef CHEAT
            case ALLEGRO_KEY_B:
                level->number -= 2;
                Level_next(level);
                break;

            case ALLEGRO_KEY_N:
                Level_next(level);
                break;
        #endif

        default:
            break;
    }

    if(key == ALLEGRO_KEY_UP ||
            key == ALLEGRO_KEY_DOWN ||
            key == ALLEGRO_KEY_LEFT || 
            key == ALLEGRO_KEY_RIGHT) {
        sound_play(SFX_HOP, eng->volume, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    if(key == ALLEGRO_KEY_UP) return INPUT_UP;
    if(key == ALLEGRO_KEY_DOWN) return INPUT_DOWN;
    if(key == ALLEGRO_KEY_LEFT) return INPUT_LEFT;
    if(key == ALLEGRO_KEY_RIGHT) return INPUT_RIGHT;
    if(key == ALLEGRO_KEY_ENTER) return INPUT_ENTER;
    return INPUT_NULL;
}


static void render_pause(engineptr_t eng) {
    // A lot of magic numbers ahead, these manage the position
    // and centering of both text and other drawings
    ALLEGRO_FONT* font = al_create_builtin_font();

    bitmap = al_load_bitmap(GET_BMP(BMP_PAUSE_BG));
    al_draw_scaled_bitmap(bitmap, 0, 0, 300, 300,   // image scaling
                DISP_WIDTH/3, DISP_HEIGHT/3, 
                DISP_WIDTH/3, DISP_HEIGHT/3, 0);
    al_destroy_bitmap(bitmap);

    
    font = al_load_ttf_font(SELECTED_FONT, 32, 0);
    al_draw_multiline_textf(font,al_map_rgb(0,0,0), 
        DISP_WIDTH/2,
        DISP_HEIGHT/3 + 1.5*BLOCK_HEIGHT,
        DISP_WIDTH/3,
        0,
        ALLEGRO_ALIGN_CENTRE,
        "%c    RESUME     \n \n %c   RESET     \n \n %c   QUIT     \n \n \n HIGH SCORE: %d", 
        eng->pausestate==PAUSE_STA_OP_1 ? OPTION_SELECTED : OPTION_UNSELECTED,
        eng->pausestate==PAUSE_STA_OP_2 ? OPTION_SELECTED : OPTION_UNSELECTED,
        eng->pausestate==PAUSE_STA_OP_3 ? OPTION_SELECTED : OPTION_UNSELECTED,
        get_highscore()*HIGHSCORE_MULT
    );
    al_destroy_font(font);
    
}


static void render_menu(engineptr_t eng) {
    // Bienvenido a la escuela de Magia y Hechicería de Hogwarts
    // A lot of magic numbers ahead, these manage the position
    // and centering of both text and other drawings

    bitmap = al_load_bitmap(GET_BMP(BMP_MAIN_BG));
    al_draw_scaled_bitmap(bitmap, 100, 0, 1900, 1600, 0, 0, DISP_WIDTH, DISP_HEIGHT, 0); // image scaling
    al_destroy_bitmap(bitmap);

    ALLEGRO_FONT* font = al_load_ttf_font(SELECTED_FONT, 148, 0);  //fontsize:148
    al_draw_text(font,al_map_rgb(0,180,0),  //green text
        DISP_WIDTH/2,
        DISP_HEIGHT/3 - 3*BLOCK_HEIGHT,
        ALLEGRO_ALIGN_CENTRE,"FROGGER");
    al_destroy_font(font);

    font = al_load_ttf_font(SELECTED_FONT, 28, 0);  //fontsize: 28
    al_draw_text(font,al_map_rgb(200,0,0),  // Red text
        DISP_WIDTH/2,
        DISP_HEIGHT/3 - (1.5)*BLOCK_HEIGHT,
        ALLEGRO_ALIGN_CENTRE,"Christmas Edition!");
    al_destroy_font(font);

    font = al_load_ttf_font(SELECTED_FONT, 36, 0);  // Fontsize: 36
    al_draw_multiline_textf(font,al_map_rgb(255,255,255), // White
        DISP_WIDTH/2,
        DISP_HEIGHT/2 + BLOCK_HEIGHT,
        DISP_WIDTH,
        0,
        ALLEGRO_ALIGN_CENTRE,
        "\n \n MAIN MENU \n \n \n %c   PLAY     \n \n %c   QUIT     \n \n \n \n HIGH SCORE: %d", 
        eng->menustate==MENU_STA_OP_1 ? OPTION_SELECTED : OPTION_UNSELECTED,
        eng->menustate==MENU_STA_OP_2 ? OPTION_SELECTED : OPTION_UNSELECTED,
        get_highscore()*HIGHSCORE_MULT
    );
    al_destroy_font(font);
}


static void render_death(engineptr_t eng) {
    // A lot of magic numbers ahead, these manage the position
    // and centering of both text and other drawings
    ALLEGRO_FONT* font = al_create_builtin_font();

    bitmap = al_load_bitmap(GET_BMP(BMP_DEATH_BG));
    al_draw_scaled_bitmap(bitmap, 0, 0, 261, 314,       // image size scaling
                DISP_WIDTH/3-BLOCK_WIDTH, DISP_HEIGHT/5, 
                DISP_WIDTH/3+2*BLOCK_WIDTH, DISP_HEIGHT/2+BLOCK_HEIGHT, 0);
    al_destroy_bitmap(bitmap);

    
    font = al_load_ttf_font(SELECTED_FONT, 32, 0);
    al_draw_multiline_textf(font,al_map_rgb(0,0,0), //black
        DISP_WIDTH/2,
        DISP_HEIGHT/3 + 2*BLOCK_HEIGHT,
        DISP_WIDTH/3,
        0,
        ALLEGRO_ALIGN_CENTRE,
        "\n      GAME OVER      \n \n  %c  TRY AGAIN      \n     %c  QUIT        \n \n \n    HIGH SCORE: %d    ", 
        eng->deathstate==DEATH_STA_MENU_OP_1 ? OPTION_SELECTED : OPTION_UNSELECTED,
        eng->deathstate==DEATH_STA_MENU_OP_2 ? OPTION_SELECTED : OPTION_UNSELECTED,
        get_highscore()*HIGHSCORE_MULT
    );
    al_destroy_font(font);
}


static void render_map(levelptr_t level) {
    al_clear_to_color(al_map_rgb(80, 80, 255));
    int16_t i=0, p=0;
    for(i = 0; i < LEVEL_HEIGHT; i++) {
        laneptr_t lane = level->lanes[i];
        if(lane->type == MOB_CAR) {
            render_street_lane(level, i);
        } else if(lane->type == MOB_LOG) {
            render_water_lane(level, i);
        } else if(lane->type == MOB_FINISH) {
            render_water_lane(level, i);
        } else if(lane->type == MOB_FLOOR) {
            render_floor_lane(level, i);
        }

        if(lane->delta != 0) {
            for(p = -1; p < (LEVEL_WIDTH / lane->delta) + 2; p++) {
                if(lane->type == MOB_CAR){
                    draw_vehicle(lane, i, p);
                } else if(lane->type == MOB_LOG) {
                    draw_log(lane, i, p);
                } else if(lane->type == MOB_FINISH) {
                    if(!is_in_array(level->finishers, p, LVL_FINISHSPOTS)) {
                        bitmap = al_load_bitmap(GET_BMP(BMP_LILYPAD));
                        al_draw_bitmap(
                            bitmap, 
                            Lane_get_elem_x(lane, p), 
                            0, 0);
                        al_destroy_bitmap(bitmap);
                    }
                }
            }
        }
    }
    
    uint16_t frogx = level->frog->x;
    uint16_t frogy = level->frog->lane;
    bitmap = al_load_bitmap(GET_BMP(BMP_FROG));
    al_draw_bitmap(bitmap, frogx, frogy*BLOCK_HEIGHT, 0);
    al_destroy_bitmap(bitmap);

    bitmap = al_load_bitmap(GET_BMP(BMP_HEART));
    for(i = 0; i < level->frog->lives; i++) {
        // Draws hearts representing remaining lives
        al_draw_bitmap(bitmap, (LEVEL_WIDTH-1-i)*BLOCK_WIDTH, (LEVEL_HEIGHT-1)*BLOCK_HEIGHT, 0);
    }
    al_destroy_bitmap(bitmap);
}

// shows score in bottom-left corner of the screen
static void draw_score(uint32_t score) {
    ALLEGRO_FONT* font = al_create_builtin_font();

    font = al_load_ttf_font(SELECTED_FONT, 28, 0);  //fontisze:28
    al_draw_textf(font,al_map_rgb(0,0,0), // black
        BLOCK_WIDTH/2,
        (LEVEL_HEIGHT-0.5)*BLOCK_HEIGHT,
        0,"SCORE: %d", score);
    al_destroy_font(font);
}


static void render_street_lane(levelptr_t level, uint8_t lanenum) {
    uint16_t x = 0;
    static uint8_t orientation = 0;
    bitmap = al_load_bitmap(GET_BMP(BMP_ROAD));
    for(x=0; x < DISP_WIDTH; x+=BLOCK_WIDTH) {
        al_draw_bitmap(bitmap, x, lanenum*BLOCK_HEIGHT,
            orientation % 2 ? ALLEGRO_FLIP_VERTICAL : 0);
    }
    al_destroy_bitmap(bitmap);
    if(++orientation == 2) orientation = 0;
}


static void render_water_lane(levelptr_t level, uint8_t lanenum) {
    uint16_t x = 0;

    bitmap = al_load_bitmap(GET_BMP(BMP_WATER));
    for(x=0; x < DISP_WIDTH; x+=BLOCK_WIDTH) {
        al_draw_bitmap(bitmap, x, lanenum*BLOCK_HEIGHT, 0);
    }
    al_destroy_bitmap(bitmap);
}


static void render_floor_lane(levelptr_t level, uint8_t lanenum) {
    uint16_t x = 0;

    bitmap = al_load_bitmap(GET_BMP(BMP_FLOOR));
    for(x=0; x < DISP_WIDTH; x+=BLOCK_WIDTH) {
        al_draw_bitmap(bitmap, x, lanenum*BLOCK_HEIGHT, 0);
    }
    al_destroy_bitmap(bitmap);
}


static void draw_log(laneptr_t lane, uint8_t lanenum, uint8_t p) {
    if(lane->mob_length == 2) {
        // draws simple log
        bitmap = al_load_bitmap(GET_BMP(BMP_LOG_HEAD));
        al_draw_bitmap(
            bitmap, 
            Lane_get_elem_x(lane, p), 
            lanenum*BLOCK_HEIGHT, 0);
        al_destroy_bitmap(bitmap);

        bitmap = al_load_bitmap(GET_BMP(BMP_LOG_END));
        al_draw_bitmap(
            bitmap, 
            Lane_get_elem_x(lane, p) + BLOCK_WIDTH, 
            lanenum*BLOCK_HEIGHT, 0);
        al_destroy_bitmap(bitmap);
    } else {
        // draws head, then (mob_length - 2) bodies, then foot
        bitmap = al_load_bitmap(GET_BMP(BMP_LOG_HEAD));
        al_draw_bitmap(
            bitmap, 
            Lane_get_elem_x(lane, p), 
            lanenum*BLOCK_HEIGHT, 0);
        al_destroy_bitmap(bitmap);

        bitmap = al_load_bitmap(GET_BMP(BMP_LOG_BODY));
        uint8_t j = 0;
        for(j = 1; j < lane->mob_length - 1; j++) {
            al_draw_bitmap(
                bitmap, 
                Lane_get_elem_x(lane, p) + j*BLOCK_WIDTH, 
                lanenum*BLOCK_HEIGHT, 0);
        }
        al_destroy_bitmap(bitmap);
        
        bitmap = al_load_bitmap(GET_BMP(BMP_LOG_END));
        al_draw_bitmap(
            bitmap, 
            Lane_get_elem_x(lane, p) + (lane->mob_length-1)*BLOCK_WIDTH, 
            lanenum*BLOCK_HEIGHT, 0);
        al_destroy_bitmap(bitmap);
    }
}


static void draw_vehicle(laneptr_t lane, uint8_t lanenum, uint8_t p) {
    uint8_t x = 0;
    if(lane->mob_length == 1) {
        // single carriage
        bitmap = al_load_bitmap(GET_BMP(BMP_CARRIAGE));
        al_draw_bitmap(
            bitmap, 
            Lane_get_elem_x(lane, p), 
            lanenum*BLOCK_HEIGHT, 
            lane->step < 0 ? 0 : ALLEGRO_FLIP_HORIZONTAL);
        al_destroy_bitmap(bitmap);
    } else {
        // draws initial mob sprite, then (mob_length - 1) secondary mob sprites
        // and orients based on movement direction
        if(lane->step > 0) {
            bitmap = al_load_bitmap(GET_BMP(BMP_CARRIAGE));
            al_draw_bitmap(
                bitmap, 
                Lane_get_elem_x(lane, p), 
                lanenum*BLOCK_HEIGHT, 
                lane->step < 0 ? 0 : ALLEGRO_FLIP_HORIZONTAL);
            al_destroy_bitmap(bitmap);

            for(x = 1; x < lane->mob_length; x++) {
                bitmap = al_load_bitmap(GET_BMP(BMP_REINDEER));

                al_draw_bitmap(
                    bitmap, 
                    Lane_get_elem_x(lane, p)+x*BLOCK_WIDTH, 
                    lanenum*BLOCK_HEIGHT, 
                    lane->step < 0 ? 0 : ALLEGRO_FLIP_HORIZONTAL);
            }
            al_destroy_bitmap(bitmap);
        } else {
            bitmap = al_load_bitmap(GET_BMP(BMP_CARRIAGE));
            al_draw_bitmap(
                bitmap, 
                (lane->mob_length-1)*BLOCK_WIDTH+Lane_get_elem_x(lane, p), 
                lanenum*BLOCK_HEIGHT, 
                lane->step < 0 ? 0 : ALLEGRO_FLIP_HORIZONTAL);
            al_destroy_bitmap(bitmap);

            for(x = 0; x < lane->mob_length-1; x++) {
                bitmap = al_load_bitmap(GET_BMP(BMP_REINDEER));

                al_draw_bitmap(
                    bitmap, 
                    Lane_get_elem_x(lane, p)+x*BLOCK_WIDTH, 
                    lanenum*BLOCK_HEIGHT, 
                    lane->step < 0 ? 0 : ALLEGRO_FLIP_HORIZONTAL);
            }
            al_destroy_bitmap(bitmap);
        }
    }
}