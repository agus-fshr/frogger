#include "AllegroEngine.h"
#include <stdio.h>

static void render_map(levelptr_t level);
static void render_pause(engineptr_t eng);
static void render_menu(engineptr_t eng);
static void render_death(engineptr_t eng);
static void render_street_lane(levelptr_t level, uint8_t lanenum);
static void render_water_lane(levelptr_t level, uint8_t lanenum);
static void draw_score(uint32_t score);

static ALLEGRO_DISPLAY* disp;
static ALLEGRO_BITMAP* bitmap;

int AllegroEngine_init(engineptr_t eng, void* param) {
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_VIDEO_BITMAP);
    al_init_ttf_addon();
    al_set_new_display_flags(ALLEGRO_FRAMELESS);
    disp = al_create_display(DISP_WIDTH, DISP_HEIGHT);
    al_register_event_source((ALLEGRO_EVENT_QUEUE*) param, al_get_display_event_source(disp));
    Frog_move(eng->level->frog, SPAWN_X, SPAWN_Y);
    Frog_reset_lives(eng->level->frog);
    return al_init_image_addon();
}

int AllegroEngine_destroy(engineptr_t eng, void* param){
    //al_destroy_bitmap(bitmap);
    al_destroy_display(disp);
    return 0;
}

int AllegroEngine_render(engineptr_t eng, void* param) {
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
    }
    al_flip_display();
    return 0;
}

int AllegroEngine_input(engineptr_t eng, void* keycode) {
    int key = *((int*) keycode);
    levelptr_t level = eng->level;

    switch(key) {
        case ALLEGRO_KEY_UP:
            process_game_state(eng, INPUT_UP);
            break;

        case ALLEGRO_KEY_DOWN:
            process_game_state(eng, INPUT_DOWN);
            break;

        case ALLEGRO_KEY_LEFT:
            process_game_state(eng, INPUT_LEFT);
            break;

        case ALLEGRO_KEY_RIGHT:
            process_game_state(eng, INPUT_RIGHT);
            break;

        case ALLEGRO_KEY_P:
        case ALLEGRO_KEY_ENTER:
            process_game_state(eng, INPUT_ENTER);
            break;
        
        case ALLEGRO_KEY_ESCAPE:
            return 1;
            break;
        
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
    
    return 0;
}

static void render_pause(engineptr_t eng) {
    al_clear_to_color(al_map_rgb(50, 50, 255));
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_draw_textf(font,al_map_rgb(0,0,0), 
        DISP_WIDTH/2,
        DISP_HEIGHT/2,
        ALLEGRO_ALIGN_CENTRE,
        "PAUSE MENU\t%c RESUME\t%c RESET\t%c QUIT", 
        eng->pausestate==PAUSE_STA_OP_1 ? '>' : ' ',
        eng->pausestate==PAUSE_STA_OP_2 ? '>' : ' ',
        eng->pausestate==PAUSE_STA_OP_3 ? '>' : ' '
    );

    al_draw_textf(font,al_map_rgb(0,0,0), 
        DISP_WIDTH/2,
        DISP_HEIGHT/2 + BLOCK_HEIGHT,
        ALLEGRO_ALIGN_CENTRE,
        "SCORE: %d", eng->score*100
    );
    al_destroy_font(font);
}


static void render_menu(engineptr_t eng) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    ALLEGRO_FONT* font = al_create_builtin_font();
    al_draw_text(font,al_map_rgb(255,255,255), 
        DISP_WIDTH/2,
        DISP_HEIGHT/2 - BLOCK_HEIGHT,
        ALLEGRO_ALIGN_CENTRE,"THIS IS a MENU");

    al_draw_textf(font,al_map_rgb(255,255,255), 
        DISP_WIDTH/2,
        DISP_HEIGHT/2,
        ALLEGRO_ALIGN_CENTRE,
        "MAIN MENU\t%c PLAY\t%c QUIT", 
        eng->menustate==MENU_STA_OP_1 ? '>' : ' ',
        eng->menustate==MENU_STA_OP_2 ? '>' : ' '
    );

    al_draw_text(font,al_map_rgb(255,255,255), 
        DISP_WIDTH/2,
        DISP_HEIGHT/2 + BLOCK_HEIGHT,
        ALLEGRO_ALIGN_CENTRE,
        "HIGH SCORE: IDK lol"
    );
    al_destroy_font(font);
}

static void render_death(engineptr_t eng) {
    al_clear_to_color(al_map_rgb(100, 0, 0));

    ALLEGRO_FONT* font = al_create_builtin_font();
    al_draw_text(font,al_map_rgb(255,255,255), 
        DISP_WIDTH/2,
        DISP_HEIGHT/2 - BLOCK_HEIGHT,
        ALLEGRO_ALIGN_CENTRE,"U DED");

    al_draw_textf(font,al_map_rgb(255,255,255), 
        DISP_WIDTH/2,
        DISP_HEIGHT/2,
        ALLEGRO_ALIGN_CENTRE,
        "DEAD MENU\t%c TRY AGAIN\t%c QUIT", 
        eng->deathstate==DEATH_STA_MENU_OP_1 ? '>' : ' ',
        eng->deathstate==DEATH_STA_MENU_OP_2 ? '>' : ' '
    );

    al_draw_text(font,al_map_rgb(255,255,255), 
        DISP_WIDTH/2,
        DISP_HEIGHT/2 + BLOCK_HEIGHT,
        ALLEGRO_ALIGN_CENTRE,
        "HIGH SCORE: IDK lol"
    );
    al_destroy_font(font);

}


static void render_map(levelptr_t level) {
    al_clear_to_color(al_map_rgb(80, 80, 255));
    int16_t i=0, p=0, x=0;
    for(i = 0; i < LEVEL_HEIGHT; i++) {
        laneptr_t lane = level->lanes[i];
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
        if(lane->type == MOB_CAR) {
            render_street_lane(level, i);
        } else if(lane->type == MOB_LOG) {
            render_water_lane(level, i);
            red = 150;
            green = 100;
            blue = 100;
        } else if(lane->type == MOB_FINISH) {
            red = 0;
            green = 150;
            blue = 0;
        } else if(lane->type == MOB_FLOOR) {
            al_draw_filled_rectangle(
                0, i*BLOCK_HEIGHT, DISP_WIDTH,
                (1 + i)*BLOCK_HEIGHT, al_map_rgb(150, 150, 150)); 
        }

        
        if(lane->delta != 0) {
            for(p = -1; p < (LEVEL_WIDTH / lane->delta) + 2; p++) {
                
                if(lane->type == MOB_CAR){
                    if(lane->mob_length == 1) {
                        bitmap = al_load_bitmap("sprites/mobinit.png");
                        al_draw_bitmap(
                            bitmap, 
                            Lane_get_elem_x(lane, p), 
                            i*BLOCK_HEIGHT, 
                            lane->step < 0 ? 0 : ALLEGRO_FLIP_HORIZONTAL);
                        al_destroy_bitmap(bitmap);
                    } else {
                        if(lane->step > 0) {
                            bitmap = al_load_bitmap("sprites/mobinit.png");
                            al_draw_bitmap(
                                bitmap, 
                                Lane_get_elem_x(lane, p), 
                                i*BLOCK_HEIGHT, 
                                lane->step < 0 ? 0 : ALLEGRO_FLIP_HORIZONTAL);
                            al_destroy_bitmap(bitmap);

                            for(x = 1; x < lane->mob_length; x++) {
                                bitmap = al_load_bitmap("sprites/mobcont.png");

                                al_draw_bitmap(
                                    bitmap, 
                                    Lane_get_elem_x(lane, p)+x*BLOCK_WIDTH, 
                                    i*BLOCK_HEIGHT, 
                                    lane->step < 0 ? 0 : ALLEGRO_FLIP_HORIZONTAL);
                            }
                            al_destroy_bitmap(bitmap);
                        } else {
                            bitmap = al_load_bitmap("sprites/mobinit.png");
                            al_draw_bitmap(
                                bitmap, 
                                (lane->mob_length-1)*BLOCK_WIDTH+Lane_get_elem_x(lane, p), 
                                i*BLOCK_HEIGHT, 
                                lane->step < 0 ? 0 : ALLEGRO_FLIP_HORIZONTAL);
                            al_destroy_bitmap(bitmap);

                            for(x = 0; x < lane->mob_length-1; x++) {
                                bitmap = al_load_bitmap("sprites/mobcont.png");

                                al_draw_bitmap(
                                    bitmap, 
                                    Lane_get_elem_x(lane, p)+x*BLOCK_WIDTH, 
                                    i*BLOCK_HEIGHT, 
                                    lane->step < 0 ? 0 : ALLEGRO_FLIP_HORIZONTAL);
                            }
                            al_destroy_bitmap(bitmap);
                        }


                    }
                    /*
                    switch(lane->mob_length) {
                        case 1:
                            bitmap = al_load_bitmap("sprites/mob1_64.png");
                            break;
                        case 2:
                            bitmap = al_load_bitmap("sprites/mob2_64.png");
                            //al_draw_bitmap(bitmap, Lane_get_elem_x(lane, p), i*BLOCK_HEIGHT, 0);
                            break;
                        case 3:
                            bitmap = al_load_bitmap("sprites/mob3_64.png");
                        default:
                            break;
                    }
                    if(lane->mob_length > 3) {

                        al_draw_filled_rectangle(
                            Lane_get_elem_x(lane, p) < 0 ? 0 : Lane_get_elem_x(lane, p),
                            i*BLOCK_HEIGHT,
                            Lane_get_elem_x_end(lane, p) < 0 ? 0 : Lane_get_elem_x_end(lane, p),
                            (1 + i)*BLOCK_HEIGHT,
                            al_map_rgb(red, green, blue));
                    
                    } else {
                        al_draw_bitmap(
                            bitmap, 
                            Lane_get_elem_x(lane, p), 
                            i*BLOCK_HEIGHT, 
                            lane->step > 0 ? 0 : ALLEGRO_FLIP_HORIZONTAL);
                    }
                    al_destroy_bitmap(bitmap);
                    */
                } else if(lane->type == MOB_LOG) {
                    if(lane->mob_length == 2) {
                        bitmap = al_load_bitmap("sprites/log1_64.png");
                        al_draw_bitmap(
                            bitmap, 
                            Lane_get_elem_x(lane, p), 
                            i*BLOCK_HEIGHT, 0);
                        al_destroy_bitmap(bitmap);

                        bitmap = al_load_bitmap("sprites/log3_64.png");
                        al_draw_bitmap(
                            bitmap, 
                            Lane_get_elem_x(lane, p) + BLOCK_WIDTH, 
                            i*BLOCK_HEIGHT, 0);
                        al_destroy_bitmap(bitmap);
                    } else {
                        bitmap = al_load_bitmap("sprites/log1_64.png");
                        al_draw_bitmap(
                            bitmap, 
                            Lane_get_elem_x(lane, p), 
                            i*BLOCK_HEIGHT, 0);
                        al_destroy_bitmap(bitmap);

                        bitmap = al_load_bitmap("sprites/log2_64.png");
                        uint8_t j = 0;
                        for(j = 1; j < lane->mob_length - 1; j++) {
                            al_draw_bitmap(
                                bitmap, 
                                Lane_get_elem_x(lane, p) + j*BLOCK_WIDTH, 
                                i*BLOCK_HEIGHT, 0);
                        }
                        al_destroy_bitmap(bitmap);
                        
                        bitmap = al_load_bitmap("sprites/log3_64.png");
                        al_draw_bitmap(
                            bitmap, 
                            Lane_get_elem_x(lane, p) + (lane->mob_length-1)*BLOCK_WIDTH, 
                            i*BLOCK_HEIGHT, 0);
                        al_destroy_bitmap(bitmap);
                    }
                } else if(lane->type == MOB_FINISH) {
                    bitmap = al_load_bitmap("sprites/lily_64.png");
                    al_draw_bitmap(
                        bitmap, 
                        Lane_get_elem_x(lane, p), 
                        0, 0);
                    al_destroy_bitmap(bitmap);
                } else {
                    al_draw_filled_rectangle(
                        Lane_get_elem_x(lane, p) < 0 ? 0 : Lane_get_elem_x(lane, p),
                        i*BLOCK_HEIGHT,
                        Lane_get_elem_x_end(lane, p) < 0 ? 0 : Lane_get_elem_x_end(lane, p),
                        (1 + i)*BLOCK_HEIGHT,
                        al_map_rgb(red, green, blue));
                }
            }
        }
        
    }
    
    uint16_t frogx = level->frog->x;
    uint16_t frogy = level->frog->lane;
    bitmap = al_load_bitmap("sprites/frog_64.png");

    /*al_draw_filled_rectangle(frogx, frogy*BLOCK_WIDTH, frogx + BLOCK_WIDTH, (frogy + 1)*BLOCK_HEIGHT, 
                            al_map_rgb(0, 255, 0));
                            */
    al_draw_bitmap(bitmap, frogx, frogy*BLOCK_HEIGHT, 0);
    al_destroy_bitmap(bitmap);

    laneptr_t finisherlane = level->lanes[0];
    for(i = 0; i < LVL_FINISHSPOTS; i++) {
        al_draw_filled_rectangle(
            Lane_get_elem_x(finisherlane, level->finishers[i]),
            0,
            Lane_get_elem_x_end(finisherlane, level->finishers[i]),
            BLOCK_HEIGHT,
            al_map_rgb(255, 255, 0)); 
    }
    
    bitmap = al_load_bitmap("sprites/heart.png");
    for(i = 0; i < level->frog->lives; i++) {
        al_draw_bitmap(bitmap, (LEVEL_WIDTH-1-i)*BLOCK_WIDTH, (LEVEL_HEIGHT-1)*BLOCK_HEIGHT, 0);
    }
    al_destroy_bitmap(bitmap);
}

static void draw_score(uint32_t score) {
    ALLEGRO_FONT* font = al_create_builtin_font();
    al_draw_textf(font,al_map_rgb(0,0,0), 
        BLOCK_WIDTH/2,
        (LEVEL_HEIGHT-0.5)*BLOCK_HEIGHT,
        0,"SCORE: %d", score);
    al_destroy_font(font);
}

static void render_street_lane(levelptr_t level, uint8_t lanenum) {
    int16_t x = 0;
    static int8_t street_orientation = 5;
    for(x=0; x < DISP_WIDTH; x+=BLOCK_WIDTH) {
        uint8_t flipped = 0;
        switch(street_orientation) {
            case 0:
            case 4:
                bitmap = al_load_bitmap("sprites/road1_64.png");
                break;
            
            case 1:
                bitmap = al_load_bitmap("sprites/road2_64.png");
                break;

            case 2:
                bitmap = al_load_bitmap("sprites/road2_64.png");
                flipped = ALLEGRO_FLIP_VERTICAL;
                break;

            case 3:
            case 5:
                bitmap = al_load_bitmap("sprites/road1_64.png");
                flipped = ALLEGRO_FLIP_VERTICAL;
                break;
            
            default:
                break;
        }
        al_draw_bitmap(bitmap, x, lanenum*BLOCK_HEIGHT, flipped);
        al_destroy_bitmap(bitmap);
    }
    street_orientation--;
    if(street_orientation == -1) street_orientation = 5;
}



static void render_water_lane(levelptr_t level, uint8_t lanenum) {
    uint16_t x = 0;

    bitmap = al_load_bitmap("sprites/water1.png");
    for(x=0; x < DISP_WIDTH; x+=BLOCK_WIDTH) {
        al_draw_bitmap(bitmap, x, lanenum*BLOCK_HEIGHT, 0);
    }
    al_destroy_bitmap(bitmap);
}