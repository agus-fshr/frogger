#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <time.h>

#define RANDOM_VERT_SPAWN (rand()%8)*60

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main()
{
	srand(time(NULL));    //seed para rand

	typedef struct cars_t {
		float x;
		float y;
	} cars_t;

    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    float x, y;                                                  //posiciones iniciales
    x = 0;
    y = 480;

	cars_t abc = {
		x = 0,
		y = RANDOM_VERT_SPAWN,
	};

	cars_t def = {
		x = 0,
		y = RANDOM_VERT_SPAWN,
	};

	#define CAR_LANES 2

    cars_t cars[CAR_LANES] = {abc, def};


    #define KEY_SEEN     1
    #define KEY_RELEASED 2

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                // once again, no game logic. fishy? maybe.
                redraw = true;
                break;
        
            case ALLEGRO_EVENT_KEY_DOWN:
                if(event.keyboard.keycode == ALLEGRO_KEY_UP)
                    y-=60;
                if(event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                    y+=60;
                if(event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                    x-=60;
                if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                    x+=60;
        
                if(event.keyboard.keycode != ALLEGRO_KEY_ESCAPE)
                    break;
        
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "X: %.1f Y: %.1f", x, y);
            al_draw_filled_rectangle(x, y, x + 60, y + 60, al_map_rgb(255, 0, 0));          //bichito bueno

            for (int i=0; i<CAR_LANES; i++){
            	al_draw_filled_rectangle(cars[i].x, cars[i].y, cars[i].x+60, cars[i].y+60, al_map_rgb(0, 0, 255));              //bichito malo
				(cars[i].x)++;
			}                                                                            //update pos en x

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
