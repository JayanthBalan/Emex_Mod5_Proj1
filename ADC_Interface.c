
#include "ADC_Interface.h"
#include <xc.h>

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 20000000UL
#endif

void adc_init(void) {
    ADCON1bits.ADFM = 1U;
    ADCON0bits.ADON = 1U;
    ADCON1bits.PCFG = 0U;
    
    ADCON1bits.ADCS2 = 0U;
    ADCON0bits.ADCS1 = 1U;
    ADCON0bits.ADCS0 = 0U;
    
    return;
}

uint16_t adc_read(uint8_t channel) {
    uint16_t out_value;
    
    ADCON0 &= 0xC7;
    ADCON0 |= (channel << 3);
    
    __delay_us(20);
    
    ADCON0bits.GO = 1U;
    
    while(ADCON0bits.GO_nDONE == 1U) {
        continue;
    }
    
    out_value = (uint16_t)((ADRESH << UINT8_C(8)) | ADRESL);
    out_value &= 0x03FF;
    
    return out_value;
}

void adc_deinit(void) {
    ADCON1bits.ADFM = 0U;
    ADCON0bits.ADON = 0U;
    ADCON1bits.PCFG = 7U;
    
    ADCON1bits.ADCS2 = 0U;
    ADCON0bits.ADCS1 = 0U;
    ADCON0bits.ADCS0 = 0U;
    
    return;
}
