
#include "non-block-delay-T0.h"

/*
 * delay = (256 - TMR0)*prescaler*4/Fosc;
 * Fosc = _XTAL_FREQ (= 20 MHz optimal);
 * TMR0 (= 6 optimal);
 * prescaler (= 8 optimal);
 * 
 * at optimal values, delay_per_cycle = 400 us;
 */

#define CONV_US 1000000UL
#define TMR0_ISR_COMPENSATION UINT8_C(2)

static uint8_t tmr0_val;
static uint8_t prescaler_val;
static volatile uint32_t systick_TMR0;
static uint32_t durationTick_rapid;
static volatile uint32_t startTick_rapid;
static volatile uint8_t delay_begin_flag_rapid;

static inline uint32_t prescalerCalculate(uint8_t x) {
    return ((uint32_t)(2U)<<x);
}

void nonBlockDelayT0_init(uint8_t load_tmr0, uint8_t activate_prescaler, uint8_t prescaler) {    
    if(activate_prescaler != 0) {
        OPTION_REGbits.PSA = 0U;
        OPTION_REGbits.PS = prescaler; // pre-scaler = 128
    }
    else {
        OPTION_REGbits.PSA = 1U;
    }
    OPTION_REGbits.T0CS = 0U;
    prescaler_val = prescaler;
    
    tmr0_val = load_tmr0 + TMR0_ISR_COMPENSATION;
    systick_TMR0 = 0;
    TMR0 = load_tmr0;
    
    INTCONbits.TMR0IF = 0U;
    INTCONbits.TMR0IE = 1U;
    INTCONbits.PEIE = 1U;
    INTCONbits.GIE = 1U;
    
    return;
}

void nonBlockDelayT0_init_rapid(uint8_t load_tmr0, uint8_t activate_prescaler, uint8_t prescaler, uint32_t delay_time_us, uint32_t Fosc) {    
    if(activate_prescaler != 0) {
        OPTION_REGbits.PSA = 0U;
        OPTION_REGbits.PS = prescaler; // pre-scaler = 128
    }
    else {
        OPTION_REGbits.PSA = 1U;
    }
    OPTION_REGbits.T0CS = 0U;
    prescaler_val = prescaler;
    
    tmr0_val = load_tmr0 + TMR0_ISR_COMPENSATION;
    systick_TMR0 = 0;
    TMR0 = load_tmr0;
    
    INTCONbits.TMR0IF = 0U;
    INTCONbits.TMR0IE = 1U;
    INTCONbits.PEIE = 1U;
    INTCONbits.GIE = 1U;
    
    durationTick_rapid = getDurationTick(delay_time_us, Fosc);
    durationTick_rapid = (durationTick_rapid == 0) ? 1 : durationTick_rapid;
    delay_begin_flag_rapid = UINT8_C(0);
    return;
}

void startDelay_rapid_TM0(void) {
    uint8_t tmr0ie_state = INTCONbits.TMR0IE;
    INTCONbits.TMR0IE = 0U;

    startTick_rapid = systick_TMR0;
    delay_begin_flag_rapid = UINT8_C(1);

    INTCONbits.TMR0IE = tmr0ie_state;
    
    return;
}

uint32_t getSysTick_TM0(void) {
    uint32_t tick;
    uint8_t tmr0ie_state = INTCONbits.TMR0IE;
    INTCONbits.TMR0IE = 0U;
    tick = systick_TMR0;
    INTCONbits.TMR0IE = tmr0ie_state;
    return tick;
}

uint8_t block_TMR0(void) {
    uint8_t tmr0ie_state = INTCONbits.TMR0IE;
    INTCONbits.TMR0IE = 0U;
    return tmr0ie_state;
}

void unblock_TMR0(uint8_t tmr0ie_state) {
    INTCONbits.TMR0IE = tmr0ie_state;
    
    return;
}

uint32_t getDurationTick_TM0(uint32_t delay_time_us, uint32_t Fosc) {    
    uint32_t delay_per_cycle_us = (uint32_t)(256U - ((uint32_t)tmr0_val - TMR0_ISR_COMPENSATION))*prescalerCalculate(prescaler_val)*(UINT32_C(4))/(Fosc/CONV_US);
    return delay_time_us/delay_per_cycle_us;
}

void TMR0_ISR(void) {
    if(TMR0IF == 1) {
        TMR0 += tmr0_val;
        TMR0IF = 0;
        systick_TMR0++;
    }
    return;
}

uint8_t TMR0_ISR_rapid_1shot(void) {
    uint8_t flag = 0;
    if(TMR0IF == 1) {
        TMR0 += tmr0_val;
        TMR0IF = 0;
        systick_TMR0++;
    }
    if((delay_begin_flag_rapid == UINT8_C(1)) && ((systick_TMR0 - startTick_rapid) >= durationTick_rapid)) {
        flag = UINT8_C(1);
        delay_begin_flag_rapid = UINT8_C(0);
    }
    return flag;
}

uint8_t TMR0_ISR_rapid_periodic(void) {
    uint8_t flag = 0;
    if(TMR0IF == 1) {
        TMR0 += tmr0_val;
        TMR0IF = 0;
        systick_TMR0++;
    }
    if((delay_begin_flag_rapid == UINT8_C(1)) && ((systick_TMR0 - startTick_rapid) >= durationTick_rapid)) {
        startTick_rapid += durationTick_rapid;
        flag = UINT8_C(1);
    }
    return flag;
}


uint8_t timerT0_expired(uint32_t durationTick, uint32_t startTick) {
    uint8_t status = 0;
    if((getSysTick() - startTick) >= durationTick) {
        status = UINT8_C(1);
    }
    return status;
}

void nonBlockDelayT0_deinit(void) {
    OPTION_REGbits.PSA = 1U;
    
    INTCONbits.TMR0IF = 0U;
    INTCONbits.TMR0IE = 0U;
    
    TMR0 = 0;
    
    systick_TMR0 = 0U;
    tmr0_val = 0U;
    prescaler_val = 0U;
    
    return;
}

void nonBlockDelayT0_deinit_rapid(void) {
    OPTION_REGbits.PSA = 1U;
    
    INTCONbits.TMR0IF = 0U;
    INTCONbits.TMR0IE = 0U;
    
    TMR0 = 0;
    
    systick_TMR0 = 0U;
    tmr0_val = 0U;
    prescaler_val = 0U;
    durationTick_rapid = UINT8_C(0);
    startTick_rapid = UINT8_C(0);
    
    return;
}
