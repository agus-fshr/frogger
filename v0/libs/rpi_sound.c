/***************************************************************************//**
  @file     rpi_sound.h
  @brief    Implementación de interfaz de audio para Allegro
  @author   Grupo 7
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "rpi_sound.h"


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
    initAudio();
}


void sound_play(soundfx_t sfx, float volume, uint8_t mode) {
    /*ALLEGRO_SAMPLE *sample = al_load_sample(sounds[sfx]);
    if(sample) {
        al_play_sample(sample, volume, 0.0, 1.0, mode, 0); // no extra gain nor speeding up.
    }
    */
   playMusic(sounds[SFX_JINGLE], SDL_MIX_MAXVOLUME);
    // al_destroy_sample(sample);
    // Leak!
}


void sound_destroy() {
    endAudio();
}