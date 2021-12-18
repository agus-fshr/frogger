/*
 *   ________  ___       ___       _______   ________  ________  ________
 *  |\   __  \|\  \     |\  \     |\  ___ \ |\   ____\|\   __  \|\   __  \
 *  \ \  \|\  \ \  \    \ \  \    \ \   __/|\ \  \___|\ \  \|\  \ \  \|\  \
 *   \ \   __  \ \  \    \ \  \    \ \  \_|/_\ \  \  __\ \   _  _\ \  \\\  \
 *    \ \  \ \  \ \  \____\ \  \____\ \  \_|\ \ \  \|\  \ \  \\  \\ \  \\\  \
 *     \ \__\ \__\ \_______\ \_______\ \_______\ \_______\ \__\\ _\\ \_______\
 *      \|__|\|__|\|_______|\|_______|\|_______|\|_______|\|__|\|__|\|_______|
 *
 */

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/color.h>
#include <allegro5/allegro_font.h>


int main(void) {

	int high = 1000;
	int width = 1000;
	const float fps = 60;

	/* INICIALIZATION OF THE DISPLAY */

	al_init();
	al_init_font_addon();

	ALLEGRO_DISPLAY* window = al_create_display(1000, 1000);
	ALLEGRO_FONT* font;
	font = al_load_font("pixel.otf", 70, 0);
	al_set_new_window_title(window, "frogger");
	al_set_window_position(window, width / 2, 0);

	/* TIMER */

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / fps); // cantidad de ticks por segundo (en este caso 60 tick por segundo)
	al_start_timer(timer);
	al_stop_timer(timer);
	al_set_timer_count(timer, 0);

	while (1) {
		al_resume_timer(timer);
		//al_draw_text(font, al_map_rgb(1, 1, 1), 200, 200, NULL, " time: al_get_time() + '0' ");  //esto era para poner un texto pero no me salio
		//printf("timer count %f\n", al_get_time()); 

		if (al_get_time() == 60.0) {				
			al_clear_to_color(al_map_rgb(0, 0, 0));
			break;
		}
		else {
			al_clear_to_color(al_map_rgb(100, 0, 100 + ((int)al_get_time()) * 10));
		}

		al_flip_display();
	}

	al_destroy_font(font);
	return 0;
}
/*
* al_get_time te dice cuantos segundos pasaron desde que se inicializo en timer (deveulve el float)
* al_get_time_counter lo mismo pero deveulve int
*/


/*
* Para usar dos timers:
* 
* ALLEGRO_EVENT evento;
* ALLEGRO_EVENT_QUEUE* queue = al_created_event_queue();
* al_register_event_source(queue, al_get_timer_event_source(timer));
* 
* al_wait_for_event(queue, &event){
*	if(evento.type == ALLEGRO_EVENT_TIMER){
*		if(evento.timer.source == timer){  // importante si se usa mas de un timer
*			contador ++;
*		}
*	}
* }
* 
*/