/* 
 * File:   non-block-delay-T0.h
 * Author: jayan
 *
 * Created on October 1, 2026, 8:11 PM
 */

#ifndef NON_BLOCK_DELAY_T0_H
#define	NON_BLOCK_DELAY_T0_H

#include <xc.h>
#include <stdint.h>

#define T0_PRESCALER_OFF UINT8_C(0)
#define T0_PRESCALER_ON UINT8_C(1)

#ifdef	__cplusplus
extern "C" {
#endif
    
    void nonBlockDelayT0_init(uint8_t load_tmr0, uint8_t activate_prescaler, uint8_t prescaler);
    uint8_t timerT0_expired(uint32_t maxTick, uint32_t startTick);
    void TMR0_ISR(void);
    void nonBlockDelayT0_deinit(void);
    
    void nonBlockDelayT0_init_rapid(uint8_t load_tmr0, uint8_t activate_prescaler, uint8_t prescaler, uint32_t delay_time_us, uint32_t Fosc);
    uint8_t TMR0_ISR_rapid_1shot(void);
    uint8_t TMR0_ISR_rapid_periodic(void);
    void nonBlockDelayT0_deinit_rapid(void);
    
    uint32_t getSysTick(void);
    void startDelay_rapid(void);
    uint32_t getDurationTick(uint32_t delay_time_us, uint32_t Fosc);
    
    uint32_t getSysTick_TM0(void);
    void startDelay_rapid_TM0(void);
    uint32_t getDurationTick_TM0(uint32_t delay_time_us, uint32_t Fosc);
    
    uint8_t block_TMR0(void);
    void unblock_TMR0(uint8_t tmr0ie_state);
    
#ifdef	__cplusplus
}
#endif

#endif	/* NON_BLOCK_DELAY_T0_H */

