
#include "non-block-delay-T0.h"

uint32_t getSysTick(void) {
    return getSysTick_TM0();
}

void startDelay_rapid(void) {
    startDelay_rapid_TM0();
    
    return;
}

uint32_t getDurationTick(uint32_t delay_time_us, uint32_t Fosc) {
    return getDurationTick_TM0(delay_time_us, Fosc);
}
