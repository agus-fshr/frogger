/***************************************************************************//**
  @file     LEDlib.h
  @brief    Implementación de la librería para manipulación de LED matrix
  @author   Grupo 7
 ******************************************************************************/
#include "LEDlib.h"


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static const dlevel_t* get_letter(char c);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
const dlevel_t LETTER_A[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_OFF, D_ON, D_ON, D_OFF,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_ON, D_ON, D_ON,
    D_ON, D_OFF, D_OFF, D_ON
};
const dlevel_t LETTER_B[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_ON, D_ON, D_OFF,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_ON, D_OFF,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_ON, D_ON, D_ON
};
const dlevel_t LETTER_C[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_OFF, D_ON, D_ON, D_ON,
    D_ON, D_OFF, D_OFF, D_OFF,
    D_ON, D_OFF, D_OFF, D_OFF,
    D_ON, D_OFF, D_OFF, D_OFF,
    D_OFF, D_ON, D_ON, D_ON
};
const dlevel_t LETTER_D[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_ON, D_ON, D_OFF,
    D_OFF, D_OFF, D_OFF, D_ON,
    D_OFF, D_OFF, D_OFF, D_ON,
    D_OFF, D_OFF, D_OFF, D_ON,
    D_ON, D_ON, D_ON, D_OFF
};
const dlevel_t LETTER_E[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_ON, D_ON, D_ON,
    D_ON, D_OFF, D_OFF, D_OFF,
    D_ON, D_ON, D_OFF, D_OFF,
    D_ON, D_OFF, D_OFF, D_OFF,
    D_ON, D_ON, D_ON, D_ON
};
const dlevel_t LETTER_F[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_ON, D_ON, D_ON,
    D_ON, D_OFF, D_OFF, D_OFF,
    D_ON, D_ON, D_ON, D_OFF,
    D_ON, D_OFF, D_OFF, D_OFF,
    D_ON, D_OFF, D_OFF, D_OFF
};
const dlevel_t LETTER_G[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_ON, D_ON, D_ON,
    D_ON, D_OFF, D_OFF, D_OFF,
    D_ON, D_OFF, D_ON, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_OFF, D_ON, D_ON, D_OFF
};
const dlevel_t LETTER_H[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_ON, D_ON, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON
};
const dlevel_t LETTER_I[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_ON, D_ON, D_OFF,
    D_OFF, D_ON, D_OFF, D_OFF,
    D_OFF, D_ON, D_OFF, D_OFF,
    D_OFF, D_ON, D_OFF, D_OFF,
    D_ON, D_ON, D_ON, D_OFF
};
const dlevel_t LETTER_J[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_OFF, D_OFF, D_OFF, D_ON,
    D_OFF, D_OFF, D_OFF, D_ON,
    D_OFF, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_OFF, D_ON, D_ON, D_OFF
};
const dlevel_t LETTER_K[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_ON, D_OFF,
    D_ON, D_ON, D_OFF, D_OFF,
    D_ON, D_OFF, D_ON, D_OFF,
    D_ON, D_OFF, D_OFF, D_ON
};
const dlevel_t LETTER_L[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_OFF, D_OFF, D_OFF,
    D_ON, D_OFF, D_OFF, D_OFF,
    D_ON, D_OFF, D_OFF, D_OFF,
    D_ON, D_OFF, D_OFF, D_OFF,
    D_ON, D_ON, D_ON, D_ON
};
const dlevel_t LETTER_M[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_ON, D_ON, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON
};
const dlevel_t LETTER_N[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_ON, D_OFF, D_ON,
    D_ON, D_ON, D_OFF, D_ON,
    D_ON, D_OFF, D_ON, D_ON,
    D_ON, D_OFF, D_ON, D_ON
};
const dlevel_t LETTER_O[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_OFF, D_ON, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_ON, D_ON, D_ON
};
const dlevel_t LETTER_P[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_ON, D_ON, D_OFF,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_ON, D_ON, D_OFF,
    D_ON, D_OFF, D_OFF, D_OFF
};
const dlevel_t LETTER_Q[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_ON, D_ON, D_OFF,
    D_ON, D_OFF, D_ON, D_OFF,
    D_ON, D_OFF, D_ON, D_OFF,
    D_ON, D_ON, D_ON, D_OFF,
    D_OFF, D_OFF, D_OFF, D_ON
};
const dlevel_t LETTER_R[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_ON, D_ON, D_OFF,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_ON, D_ON, D_OFF,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON
};
const dlevel_t LETTER_S[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_OFF, D_ON, D_ON, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_OFF, D_ON, D_ON, D_OFF,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_ON, D_ON, D_OFF
};
const dlevel_t LETTER_T[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_ON, D_ON, D_OFF,
    D_OFF, D_ON, D_OFF, D_OFF,
    D_OFF, D_ON, D_OFF, D_OFF,
    D_OFF, D_ON, D_OFF, D_OFF,
    D_OFF, D_ON, D_OFF, D_OFF
};
const dlevel_t LETTER_U[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_OFF, D_ON, D_ON, D_OFF
};
const dlevel_t LETTER_V[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON,
    D_OFF, D_ON, D_ON, D_OFF,
    D_OFF, D_ON, D_ON, D_OFF
};
const dlevel_t LETTER_X[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_OFF, D_OFF, D_ON,
    D_OFF, D_ON, D_ON, D_OFF,
    D_OFF, D_ON, D_ON, D_OFF,
    D_ON, D_OFF, D_OFF, D_ON,
    D_ON, D_OFF, D_OFF, D_ON
};
const dlevel_t LETTER_Y[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_OFF, D_ON, D_OFF,
    D_ON, D_OFF, D_ON, D_OFF,
    D_OFF, D_ON, D_OFF, D_OFF,
    D_OFF, D_ON, D_OFF, D_OFF,
    D_OFF, D_ON, D_OFF, D_OFF
};
const dlevel_t LETTER_Z[LETTER_HEIGHT*LETTER_WIDTH] = {
    D_ON, D_ON, D_ON, D_ON,
    D_OFF, D_OFF, D_OFF, D_ON,
    D_OFF, D_OFF, D_ON, D_OFF,
    D_OFF, D_ON, D_OFF, D_OFF,
    D_ON, D_ON, D_ON, D_ON
};



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void disp_write_sanitized(dcoord_t coord, dlevel_t val) {
    if(coord.x <= DISP_MAX_X && coord.y <= DISP_MAX_Y) {
        disp_write(coord, val);
    }
}


void disp_clear_buf() {
    uint8_t i = 0, j = 0;
    dcoord_t dcoord = {0, 0};
    for(i = 0; i <= DISP_MAX_X; i++) {
        dcoord.x = i;
        for(j = 0; j <= DISP_MAX_Y; j++) {
            dcoord.y = j;
            disp_write(dcoord, D_OFF);
        }
    }
}


void write_letter(char c, dcoord_t initial) {
    uint8_t i = 0, j = 0;
    dcoord_t dcoord = initial;
    const dlevel_t* letter = (const dlevel_t*) get_letter(c);
    for(i = 0; i < LETTER_WIDTH; i++) {
        dcoord.x = initial.x + i;
        for(j = 0; j < LETTER_HEIGHT; j++) {
            dcoord.y = initial.y + j;
            disp_write_sanitized(dcoord, letter[i + j*LETTER_WIDTH]);
            // to allow writing from outside display box
        }
    }
}


void write_word(const char* s, dcoord_t initial, uint8_t len) {
    uint8_t i = 0;
    dcoord_t dcoord;
    dcoord.y = initial.y;
    for(i = 0; i < len; i++) {
        dcoord.x = initial.x + i*LETTER_WIDTH;
        //printf("%c\n", s[i]);
        write_letter(s[i], dcoord);
        initial.x++;
    }
}


void write_bmp(const dlevel_t* bmp, dcoord_t initial) {
    uint8_t i = 0, j = 0;
    dcoord_t dcoord = initial;
    for(i = 0; i < BMP_WIDTH; i++) {
        dcoord.x = initial.x + i;
        for(j = 0; j < BMP_HEIGHT; j++) {
            dcoord.y = initial.y + j;
            disp_write_sanitized(dcoord, bmp[i + j*BMP_WIDTH]);
        }
    }
}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static const dlevel_t* get_letter(char c) {
    switch(c) {
        case 'A':
        case 'a':
            return LETTER_A;
        
        case 'B':
        case'b':
            return LETTER_B;

        case 'C':
        case'c':
            return LETTER_C;

        case 'D':
        case'd':
            return LETTER_D;

        case 'E':
        case'e':
            return LETTER_E;

        case 'F':
        case'f':
            return LETTER_F;

        case 'G':
        case'g':
            return LETTER_G;

        case 'H':
        case'h':
            return LETTER_H;

        case 'I':
        case'i':
            return LETTER_I;

        case 'J':
        case'j':
            return LETTER_J;
        
        case 'K':
        case'k':
            return LETTER_K;

        case 'L':
        case'l':
            return LETTER_L;
        
        case 'M':
        case'm':
            return LETTER_M;
        
        case 'N':
        case'n':
            return LETTER_N;
        

        case 'O':
        case'o':
            return LETTER_O;

        
        case 'P':
        case'p':
            return LETTER_P;
        

        case 'Q':
        case'q':
            return LETTER_Q;
        
        case 'R':
        case'r':
            return LETTER_R;
        
        case 'S':
        case's':
            return LETTER_S;
        

        case 'T':
        case't':
            return LETTER_T;
        

        case 'U':
        case'u':
            return LETTER_U;
        
        case 'V':
        case'v':
            return LETTER_V;

        case 'X':
        case'x':
            return LETTER_X;
        
        case 'Y':
        case'y':
            return LETTER_Y;
        
        case 'Z':
        case'z':
            return LETTER_Z;
        
        default:
            return ((void*) 0);
    }
}
