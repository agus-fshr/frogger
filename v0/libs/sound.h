#ifndef __SOUND_H
#define __SOUND_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

typedef enum {SFX_JINGLE = 0, SFX_HOP,SFX_SQUASH, SFX_PLUNK} soundfx_t;

static const char *sounds[] = {
    "sfx/jingle.wav",
    "sfx/hop.wav",
    "sfx/squash.wav",
    "sfx/plunk.wav"
};

void sound_init(void);
void sound_play(soundfx_t sfx, float volume, ALLEGRO_PLAYMODE mode, ALLEGRO_SAMPLE_ID* ret_id);
void sound_destroy(void);


#endif