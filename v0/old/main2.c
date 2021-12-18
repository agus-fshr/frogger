//#include"libs/joydisp/termlib.h"
#include<stdio.h>
#include"objects/Lane.h"
#include"objects/Level.h"
#include"libs/render.h"
#include<allegro5/allegro5.h>
#include<allegro5/allegro_font.h>

#define WATER   (' ')
#define MOB     ('X')

int main(void) {
    printf("Hola\n");
    laneptr_t lane = malloc(sizeof(lane_t));
    lane->type = MOB_CAR;
    lane->speed_ticks = 0;
    lane->delta = 4;
    lane->last_position = 2;

    uint32_t ticks = 0;
    uint8_t i = 0;
    char c;
    while((c = getchar()) != 'q') {
        if(lane->speed_ticks != 0){
            if(!(ticks % lane->speed_ticks))
                lane->last_position++;
            if(!(ticks % (lane->speed_ticks*lane->delta)))
                lane->last_position = 0;
        }
        printf("Ticks: %d --", ticks);
        for(i = 0; i < 16; i++) {
            if(i < lane->last_position)
                putchar(WATER);
            else if(i == lane->last_position)
                putchar(MOB);
            else if(!((i - lane->last_position) % (lane->delta)))
                putchar(MOB);
            else
                putchar(WATER);
        }
        printf("--\n");
        ticks++;
    }
    
    /*
    levelptr_t currlvl = malloc(sizeof(levelptr_t));
    frogptr_t player = malloc(sizeof(frogptr_t));
    Level_init(currlvl);
    
    currlvl->frog = player;

    player->position.x = 5;
    player->position.y = 10;

    currlvl->mob_lanes[10][1]->x = 5;
    currlvl->mob_lanes[10][1]->type = MOB_CAR;

    printf("Colision: %d", Level_check_collisions(currlvl));
    */
    /*
    if(!al_init()) {
        printf("couldn't initialize allegro\n");
        return 1;
    }
    if(!al_install_keyboard()) {
        printf("couldn't initialize keyboard\n");
        return 1;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    if(!timer) {
        printf("couldn't initialize timer\n");
        return 1;
    }

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if(!queue) {
        printf("couldn't initialize queue\n");
        return 1;
    }

    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    if(!disp) {
        printf("couldn't initialize display\n");
        return 1;
    }

    ALLEGRO_FONT* font = al_create_builtin_font();
    if(!font) {
        printf("couldn't initialize font\n");
        return 1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while(1) {
        al_wait_for_event(queue, &event);

        switch(event.type) {
            case ALLEGRO_EVENT_TIMER:
                // game logic goes here.
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    */

    /*
    levelptr_t level;
    Level_init(level);
    level->vehicle_lanes[2]->position.x = 2;
    level->vehicle_lanes[2]->position.y = 3;

    level->frog->position.x = 4;
    level->frog->position.y = 4;

    while(1) {
        render_level(level);
    }
    */
    return 0;
}