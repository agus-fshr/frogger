#include <stdio.h>
#include "config.h"
#include "objects/GameEngine.h"


#ifdef LEDMAT
    #include "objects/LEDMatrixEngine.h"
    #include "libs/rpi_sound.h"
    #define INIT_FUN (&LEDMatEngine_init)
    #define GAME_FUN (&LEDMatEngine_gameloop)
    #define DESTROY_FUN (&LEDMatEngine_destroy)
#else
    #ifdef ALLEGRO
        #include "objects/AllegroEngine.h"
        #define INIT_FUN (&AllegroEngine_init)
        #define GAME_FUN (&AllegroEngine_gameloop)
        #define DESTROY_FUN (&AllegroEngine_destroy)
    #else
        #define INIT_FUN (NULL)
        #define GAME_FUN (NULL)
        #define DESTROY_FUN (NULL)
    #endif
#endif



int main()
{
    engineptr_t engine = malloc(sizeof(engine_t));
    engine->gameloop = GAME_FUN;
    engine->init = INIT_FUN;
    engine->destroy = DESTROY_FUN;
    
    engine_init_wrapper(engine);

    #ifdef LEDMAT
    sound_init();
    sound_play(SFX_JINGLE, 1.0, 0);
    #endif
    
    while(!engine->gameloop(engine)) {
        //game loop runs in while condition
    }

    engine_destroy_wrapper(engine);
    
    return 0;
}
