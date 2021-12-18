#include <stdio.h>
#include "config.h"
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "objects/Lane.h"
#include "objects/Level.h"
#include "libs/sound.h"
#include "objects/GameEngine.h"
#include <time.h>

#ifdef LEDMAT
    #include "objects/LEDMatrixEngine.h"
    #define INIT_FUN (LedMat_render)
    //#define RENDER_FUN (AllegroEngine_render)
    //#define DESTROY_FUN (AllegroEngine_destroy)
    //#define INPUT_FUN ()
#else
    #include "objects/AllegroEngine.h"
    #define INIT_FUN (&AllegroEngine_init)
    #define RENDER_FUN (&AllegroEngine_render)
    #define DESTROY_FUN (&AllegroEngine_destroy)
    #define INPUT_FUN (&AllegroEngine_input)
#endif

void must_init(bool test, const char *description);
uint8_t is_in_array(int16_t *arr, int16_t elem, int16_t len);

int main()
{
    uint8_t input_cooldown = 0, i = 0;

    engineptr_t engine = malloc(sizeof(engine_t));
    engine->render = RENDER_FUN;
    engine->init = INIT_FUN;
    engine->destroy = DESTROY_FUN;
    engine->process_input = INPUT_FUN;

    srand((unsigned int) time(NULL));

    must_init(al_init(), "allegro");



    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(TIMEBASE);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    //must_init(font, "font");

    sound_init();
    engine_init_wrapper(engine);
    engine->init(engine, queue);
    levelptr_t level = engine->level;

    must_init(al_init_primitives_addon(), "primitives");
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    
    al_start_timer(timer);

    ALLEGRO_SAMPLE_ID background_music;

    sound_play(SFX_JINGLE, engine->volume, ALLEGRO_PLAYMODE_LOOP, &background_music);
    while(1) {
        al_wait_for_event(queue, &event);
        uint8_t i;
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                if(engine->state == GAME_STA_PLAY) {
                    if(engine->playstate == PLAY_STA_INIT) {
                        Level_reset(engine->level);
                        engine->score = 0;
                    }

                    for(i = 0; i < LEVEL_HEIGHT; i++) {
                        Lane_tick(level->lanes[i]);
                    }
                    laneptr_t lane = level->lanes[level->frog->lane];
                    if(lane->type == MOB_LOG) {
                        int16_t newx = level->frog->x + lane->step;
                        if((newx > 0) && (newx+BLOCK_WIDTH < BLOCK_WIDTH*LEVEL_WIDTH)) {
                            level->frog->x = newx;
                        } else {
                            if(newx < 0) level->frog->x = 0;
                            else level->frog->x = (LEVEL_WIDTH - 1)*BLOCK_WIDTH;
                        }
                    }
                    // Si vas y venis en el lugar arriba de un tronco
                    // te deslizas por una desincronización entre
                    // el movimiento y el acompañamiento
                    // SOLUCIONADO: como? No se, pero solucionado
                } else if(engine->state == GAME_STA_EXIT) {
                    done = 1;
                }
                
                process_game_state((engineptr_t) engine, INPUT_NULL);
                /*
                if(&background_music) {
                    ALLEGRO_SAMPLE_INSTANCE* bgmusic_ins = al_lock_sample_id(&background_music);
                    al_set_sample_instance_gain(bgmusic_ins, engine->volume);
                    al_unlock_sample_id(&background_music);
                }
                */
                /*
                printf("GAME %d\tMENU %d\tPAUSE %d\tPLAY %d\tDEATH %d\tEXIT %d\n",
                    engine->state, engine->menustate, engine->pausestate,
                    engine->playstate, engine->deathstate,
                    engine->exitstate);
                */    
                redraw = 1;
                break;
        
            case ALLEGRO_EVENT_KEY_DOWN:
                done = engine->process_input(engine, &event.keyboard.keycode);
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = 1;
                break;
        }

        if(done)
            break;
    
        // BUG: si apretas rapido no analiza colisiones
        // SOLUCIONADO: no se como tampoco, pero bueno pero es ser de boca
        if(!done && redraw && al_is_event_queue_empty(queue))
        {
            engine->render(engine, level);
            //printf("%d\n", level->frog->lives);
            //printf("%d\n", level->number);
            /*done = */
            engine->score += Level_process_collisions(level, engine->volume);
            redraw = 0;
        }
    }

    //al_destroy_font(font);
    engine->destroy(engine, NULL);
    sound_destroy();
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    
    return 0;
}


void must_init(bool test, const char *description) {
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}
