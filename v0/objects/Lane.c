#include "Lane.h"
#include <stdio.h>


#ifdef LEDMAT
    #include "LEDMatrixEngine.h"
#else
    #include "AllegroEngine.h"
#endif

void Lane_tick(laneptr_t lane) {
    lane->ticks++;
    lane->x0 += lane->step;
    if((lane->x0 >= BLOCK_WIDTH*lane->delta) || (lane->x0 <= -BLOCK_WIDTH*lane->delta)) {
        lane->x0 = 0;
    }
}

int16_t Lane_get_elem_x(laneptr_t lane, int8_t elem) {
    return lane->x0 + elem*lane->delta*BLOCK_WIDTH;
}

int16_t Lane_get_elem_x_end(laneptr_t lane, int8_t elem) {
    return lane->x0 + (elem * lane->delta + lane->mob_length)*BLOCK_WIDTH;
}
