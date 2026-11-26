
/* 
 * File:   non-block-delay-T2.h
 * Author: jayan
 *
 * Created on October 12, 2026, 9:09 PM
 */

#ifndef NON_BLOCK_DELAY_T2_H
#define	NON_BLOCK_DELAY_T2_H

#include <xc.h>
#include <stdint.h>

#define T2_PRESCALER_OFF UINT8_C(0x00)
#define T2_PRESCALER_ON UINT8_C(0x0F)
#define T2_POSTSCALER_OFF UINT8_C(0x00)
#define T2_POSTSCALER_ON UINT8_C(0xF0)

#ifdef	__cplusplus
extern "C" {
#endif
    
    uint32_t getDurationTick(uint32_t delay_time_us, uint32_t Fosc);
    void nonBlockDelayT2_init(uint8_t load_pr2, uint8_t activate_scaler, uint8_t prescaler, uint8_t postscaler);
    uint8_t timerT2_expired(uint32_t durationTick, uint32_t startTick);
    void TMR2_ISR(void);
    uint32_t getSysTick(void);
    void nonBlockDelayT2_deinit(void);
    
    void nonBlockDelayT2_init_rapid(uint8_t load_pr2, uint8_t activate_scaler, uint8_t prescaler, uint8_t postscaler, uint32_t delay_time_us, uint32_t Fosc);
    void startDelay_rapid(void);
    uint8_t TMR2_ISR_rapid_1shot(void);
    uint8_t TMR2_ISR_rapid_periodic(void);
    void nonBlockDelayT2_deinit_rapid(void);
    
    void startDelay_rapid_TM2(void);
    uint32_t getSysTick_TM2(void);
    uint32_t getDurationTick_TM2(uint32_t delay_time_us, uint32_t Fosc);
    
    uint8_t block_TMR2(void);
    void unblock_TMR2(uint8_t tmr2ie_state);
    uint8_t pause_TMR2(void);
    void play_TMR2(uint8_t tmr2on_state);
    
#ifdef	__cplusplus
}
#endif

#endif	/* NON_BLOCK_DELAY_T2_H */
