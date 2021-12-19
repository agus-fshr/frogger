/***************************************************************************//**
  @file     AllegroEngine.h
  @brief    Implementación de interfaz de audio para Allegro
  @author   Grupo 7
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "allegro_sound.h"


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
const char *sounds[] = {
    "sfx/jingle.wav",
    "sfx/hop.wav",
    "sfx/squash.wav",
    "sfx/plunk.wav"
};


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void sound_init(void) {
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(5);
}


void sound_play(soundfx_t sfx, float volume, ALLEGRO_PLAYMODE mode, ALLEGRO_SAMPLE_ID* ret_id) {
    ALLEGRO_SAMPLE *sample = al_load_sample(sounds[sfx]);
    if(sample) {
        al_play_sample(sample, volume, 0.0, 1.0, mode, 0);
    }
    // al_destroy_sample(sample);
    // Leak de memoria!
}


void sound_destroy() {
    al_uninstall_audio();
}