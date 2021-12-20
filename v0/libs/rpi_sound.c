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
    playSound(sounds[sfx], SDL_MIX_MAXVOLUME*volume);
}

void music_play(soundfx_t sfx, float volume, uint8_t mode) {
    playSound(sounds[sfx], SDL_MIX_MAXVOLUME*volume);
}


void sound_destroy() {
    endAudio();
}