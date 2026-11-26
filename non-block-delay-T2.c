
#include "non-block-delay-T2.h"

/*
 * delay = (PR2 + 1)*prescaler*postscaler*4/Fosc;
 * Fosc = _XTAL_FREQ (= 20 MHz optimal);
 * PR2 (= 249 optimal);
 * prescaler (= 1 optimal);
 * postscaler (= 8 optimal)
 * 
 * at optimal values, delay_per_cycle = 400 us;
 */

#define CONV_US 1000000UL

static uint8_t pr2_val;
static uint8_t prescaler_val;
static uint8_t postscaler_val;
static volatile uint32_t systick_TMR2;
static uint32_t durationTick_rapid;
static volatile uint32_t startTick_rapid;
static volatile uint8_t delay_begin_flag_rapid;

static inline uint32_t prescalerCalculate(uint8_t x) {
    return ((uint32_t)(x == 0U) ? 1U : ((x == 1U) ? 4U : 16U));
}

static inline uint32_t postscalerCalculate(uint8_t x) {
    return ((uint32_t)x + 1U);
}

void nonBlockDelayT2_init(uint8_t load_pr2, uint8_t activate_scaler, uint8_t prescaler, uint8_t postscaler) {
    T2CONbits.TMR2ON = 0U;
    T2CONbits.T2CKPS = (activate_scaler & T2_PRESCALER_ON) != UINT8_C(0x00) ? prescaler : 0U;
    T2CONbits.TOUTPS = (activate_scaler & T2_POSTSCALER_ON) != UINT8_C(0x00) ? postscaler : 0U;
    PR2 = load_pr2;
    pr2_val = load_pr2;
    prescaler_val = (activate_scaler & T2_PRESCALER_ON) != UINT8_C(0x00) ? prescaler : 0U;
    postscaler_val = (activate_scaler & T2_POSTSCALER_ON) != UINT8_C(0x00) ? postscaler : 0U;
    TMR2 = 0U;
    systick_TMR2 = 0U;
    durationTick_rapid = 0U;
    startTick_rapid = 0U;
    delay_begin_flag_rapid = UINT8_C(0);

    PIR1bits.TMR2IF = 0U;
    PIE1bits.TMR2IE = 1U;
    INTCONbits.PEIE = 1U;
    INTCONbits.GIE = 1U;
    T2CONbits.TMR2ON = 1U;

    return;
}

void nonBlockDelayT2_init_rapid(uint8_t load_pr2, uint8_t activate_scaler, uint8_t prescaler, uint8_t postscaler, uint32_t delay_time_us, uint32_t Fosc) {
    T2CONbits.TMR2ON = 0U;
    T2CONbits.T2CKPS = (activate_scaler & T2_PRESCALER_ON) != UINT8_C(0x00) ? prescaler : 0U;
    T2CONbits.TOUTPS = (activate_scaler & T2_POSTSCALER_ON) != UINT8_C(0x00) ? postscaler : 0U;
    PR2 = load_pr2;
    pr2_val = load_pr2;
    prescaler_val = (activate_scaler & T2_PRESCALER_ON) != UINT8_C(0x00) ? prescaler : 0U;
    postscaler_val = (activate_scaler & T2_POSTSCALER_ON) != UINT8_C(0x00) ? postscaler : 0U;
    TMR2 = 0U;
    systick_TMR2 = 0U;
    startTick_rapid = 0U;
    delay_begin_flag_rapid = UINT8_C(0);

    PIR1bits.TMR2IF = 0U;
    PIE1bits.TMR2IE = 1U;
    INTCONbits.PEIE = 1U;
    INTCONbits.GIE = 1U;
    T2CONbits.TMR2ON = 1U;

    durationTick_rapid = getDurationTick(delay_time_us, Fosc);
    durationTick_rapid = (durationTick_rapid == 0U) ? 1U : durationTick_rapid;
    return;
}

void startDelay_rapid_TM2(void) {
    uint8_t tmr2ie_state = PIE1bits.TMR2IE;
    PIE1bits.TMR2IE = 0U;

    startTick_rapid = systick_TMR2;
    delay_begin_flag_rapid = UINT8_C(1);

    PIE1bits.TMR2IE = tmr2ie_state;

    return;
}

uint32_t getSysTick_TM2(void) {
    uint32_t tick;
    uint8_t tmr2ie_state = PIE1bits.TMR2IE;
    PIE1bits.TMR2IE = 0U;
    tick = systick_TMR2;
    PIE1bits.TMR2IE = tmr2ie_state;
    return tick;
}

uint8_t block_TMR2(void) {
    uint8_t tmr2ie_state = PIE1bits.TMR2IE;
    PIE1bits.TMR2IE = 0U;
    return tmr2ie_state;
}

void unblock_TMR2(uint8_t tmr2ie_state) {
    PIE1bits.TMR2IE = tmr2ie_state;

    return;
}

uint8_t pause_TMR2(void) {
    uint8_t tmr2on_state = T2CONbits.TMR2ON;
    T2CONbits.TMR2ON = 0U;
    return tmr2on_state;
}

void play_TMR2(uint8_t tmr2on_state) {
    T2CONbits.TMR2ON = tmr2on_state;

    return;
}

uint32_t getDurationTick_TM2(uint32_t delay_time_us, uint32_t Fosc) {
    uint32_t delay_per_cycle_us = ((uint32_t)pr2_val + 1U)*prescalerCalculate(prescaler_val)*postscalerCalculate(postscaler_val)*UINT32_C(4)/(Fosc/CONV_US);
    return delay_time_us/delay_per_cycle_us;
}

void TMR2_ISR(void) {
    if(PIR1bits.TMR2IF == 1U) {
        PIR1bits.TMR2IF = 0U;
        systick_TMR2++;
    }
    return;
}

uint8_t TMR2_ISR_rapid_1shot(void) {
    uint8_t flag = 0U;
    if(PIR1bits.TMR2IF == 1U) {
        PIR1bits.TMR2IF = 0U;
        systick_TMR2++;
    }
    if((delay_begin_flag_rapid == UINT8_C(1)) && ((systick_TMR2 - startTick_rapid) >= durationTick_rapid)) {
        flag = UINT8_C(1);
        delay_begin_flag_rapid = UINT8_C(0);
    }
    return flag;
}

uint8_t TMR2_ISR_rapid_periodic(void) {
    uint8_t flag = 0U;
    if(PIR1bits.TMR2IF == 1U) {
        PIR1bits.TMR2IF = 0U;
        systick_TMR2++;
    }
    if((delay_begin_flag_rapid == UINT8_C(1)) && ((systick_TMR2 - startTick_rapid) >= durationTick_rapid)) {
        startTick_rapid += durationTick_rapid;
        flag = UINT8_C(1);
    }
    return flag;
}

uint8_t timerT2_expired(uint32_t durationTick, uint32_t startTick) {
    uint8_t status = 0U;
    if((getSysTick() - startTick) >= durationTick) {
        status = UINT8_C(1);
    }
    return status;
}

void nonBlockDelayT2_deinit(void) {
    T2CONbits.TMR2ON = 0U;

    PIR1bits.TMR2IF = 0U;
    PIE1bits.TMR2IE = 0U;

    TMR2 = 0U;
    PR2 = 0U;

    systick_TMR2 = 0U;
    pr2_val = 0U;
    prescaler_val = 0U;
    postscaler_val = 0U;
    durationTick_rapid = 0U;
    startTick_rapid = 0U;
    delay_begin_flag_rapid = UINT8_C(0);

    return;
}

void nonBlockDelayT2_deinit_rapid(void) {
    T2CONbits.TMR2ON = 0U;

    PIR1bits.TMR2IF = 0U;
    PIE1bits.TMR2IE = 0U;

    TMR2 = 0U;
    PR2 = 0U;

    systick_TMR2 = 0U;
    pr2_val = 0U;
    prescaler_val = 0U;
    postscaler_val = 0U;
    durationTick_rapid = 0U;
    startTick_rapid = 0U;
    delay_begin_flag_rapid = UINT8_C(0);

    return;
}
