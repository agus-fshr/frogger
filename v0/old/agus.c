#include "libs/joydisp/joydrv.h"
#include <stdio.h>


int main() {
    joy_init();
    
    
    char c = ' ';
    while(1){
        joy_update();
        jcoord_t coords = joy_get_coord();
        jswitch_t sw = joy_get_switch();
        if(coords.x >= 100) c = 'R';
        if(coords.x <= -100) c = 'L';
        if(coords.y >= 100) c = 'U';
        if(coords.y <= -100) c = 'D';
        printf("%c - %c\n", c, sw);
    }
}
