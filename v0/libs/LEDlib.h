#ifndef __LEDLIB_H
#define __LEDLIB_H

#include "joydisp/disdrv.h"

#define LETTER_WIDTH (4)
#define LETTER_HEIGHT (5)

#define BMP_WIDTH (5)
#define BMP_HEIGHT (5)


void disp_write_sanitized(dcoord_t coord, dlevel_t val);
void disp_clear_buf();
const dlevel_t* get_letter(char c);
void write_letter(char c, dcoord_t initial);
void write_word(const char* s, dcoord_t initial, uint8_t len);
void write_bmp(const dlevel_t* bmp, dcoord_t initial);

#endif