/***************************************************************************//**
  @file     AllegroEngine.h
  @brief    Header de interfaz de audio para Allegro
  @author   Grupo 7
 ******************************************************************************/
#ifndef __RPI_SOUND_H
#define __RPI_SOUND_H


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "audio/SDL2/audio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_SAMPLE_NUM 10

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum {SFX_JINGLE = 0, SFX_HOP,SFX_SQUASH, SFX_PLUNK} soundfx_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Initializes sound module
*/
void sound_init(void);

/**
 * @brief Plays selected sound
 * @param sfx Sound to play
 * @param volume Volume
 * @param mode Loop or once
 * @param ret_id To save reproduction id
*/
void sound_play(soundfx_t sfx, float volume, uint8_t mode);

/**
 * @brief Destroys sound module
*/
void sound_destroy(void);

#endif