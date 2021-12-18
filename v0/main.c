#include <stdio.h>
#include "config.h"
#include <stdlib.h>
#include "objects/Lane.h"
#include "objects/Level.h"
#include "objects/GameEngine.h"
#include <time.h>


#ifdef LEDMAT
    #include "objects/LEDMatrixEngine.h"
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


uint8_t is_in_array(int16_t *arr, int16_t elem, int16_t len);

int main()
{
    engineptr_t engine = malloc(sizeof(engine_t));
    engine->gameloop = GAME_FUN;
    engine->init = INIT_FUN;
    engine->destroy = DESTROY_FUN;

    srand((unsigned int) time(NULL));

    engine_init_wrapper(engine);
    levelptr_t level = engine->level;
    
    while(!engine->gameloop(engine)) {}

    engine_destroy_wrapper(engine);
    
    return 0;
}