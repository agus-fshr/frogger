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
}//a a
