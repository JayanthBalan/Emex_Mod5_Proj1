
#include "non-block-delay-T2.h"

void startDelay_rapid(void) {
    startDelay_rapid_TM2();
    
    return;
}

uint32_t getSysTick(void) {
    return getSysTick_TM2();
}

uint32_t getDurationTick(uint32_t delay_time_us, uint32_t Fosc) {
    return getDurationTick_TM2(delay_time_us, Fosc);
}
