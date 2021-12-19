/***************************************************************************//**
  @file     LEDlib.h
  @brief    Header de la librería para manipulación de LED matrix
  @author   Grupo 7
 ******************************************************************************/
#ifndef __LEDLIB_H
#define __LEDLIB_H


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "joydisp/disdrv.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define LETTER_WIDTH (4)
#define LETTER_HEIGHT (5)
#define BMP_WIDTH (5)
#define BMP_HEIGHT (5)


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
/**
 * @brief Prevents writes outside of display box (WARNING! This should not be used randomly when you don't want errors)
 * @param coord Pretended write coord
 * @param val Value to write
*/
void disp_write_sanitized(dcoord_t coord, dlevel_t val);

/**
 * @brief Clears display buffer
*/
void disp_clear_buf();

/**
 * @brief Writes letter from ROM to LED matrix display
 * @param c Pretended letter
 * @param initial Top-right corner coordinates
*/
void write_letter(char c, dcoord_t initial);

/**
 * @brief Writes string to LED matrix display
 * @param s Pretended string
 * @param initial Top-right corner coordinates
 * @param len Length of string
*/
void write_word(const char* s, dcoord_t initial, uint8_t len);

/**
 * @brief Writes bitmap to LED matrix display
 * @param bmp Pointer to bmp (array of values)
 * @param initial Top-right corner coordinates
*/
void write_bmp(const dlevel_t* bmp, dcoord_t initial);

#endif