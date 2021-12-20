/***************************************************************************//**
  @file     allegro_sound.h
  @brief    Header de interfaz de audio para Allegro
  @author   Grupo 7
 ******************************************************************************/
#ifndef __ALLEGRO_SOUND_H
#define __ALLEGRO_SOUND_H


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_SAMPLE_NUM 10

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum {SFX_JINGLE = 0, SFX_HOP,SFX_SQUASH, SFX_PLUNK, SFX_RINGTONE} soundfx_t;


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
void sound_play(soundfx_t sfx, float volume, float speed, ALLEGRO_PLAYMODE mode, ALLEGRO_SAMPLE_ID* ret_id);

/**
 * @brief Destroys sound module
*/
void sound_destroy(void);

#endif