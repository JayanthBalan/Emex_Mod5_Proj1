/* 
 * File:   ADC_Interface.h
 * Author: jayan
 *
 * Created on November 18, 2026, 12:31 PM
 */

#ifndef ADC_INTERFACE_H
#define	ADC_INTERFACE_H

#include <stdint.h>

#define ADC_MAX 1023U
#define ADC_MIN 0U

#define ADC_CHANNEL0 0U
#define ADC_CHANNEL1 1U
#define ADC_CHANNEL2 2U
#define ADC_CHANNEL3 3U
#define ADC_CHANNEL4 4U
#define ADC_CHANNEL5 5U
#define ADC_CHANNEL6 6U
#define ADC_CHANNEL7 7U

#define ADC_PIN_CHANNEL0 PORTAbits.RA0
#define ADC_PIN_CHANNEL1 PORTAbits.RA1
#define ADC_PIN_CHANNEL2 PORTAbits.RA2
#define ADC_PIN_CHANNEL3 PORTAbits.RA3
#define ADC_PIN_CHANNEL4 PORTAbits.RA5
#define ADC_PIN_CHANNEL5 PORTEbits.RE0
#define ADC_PIN_CHANNEL6 PORTEbits.RE1
#define ADC_PIN_CHANNEL7 PORTEbits.RE2

#define ADC_TRIS_CHANNEL0 TRISAbits.TRISA0
#define ADC_TRIS_CHANNEL1 TRISAbits.TRISA1
#define ADC_TRIS_CHANNEL2 TRISAbits.TRISA2
#define ADC_TRIS_CHANNEL3 TRISAbits.TRISA3
#define ADC_TRIS_CHANNEL4 TRISAbits.TRISA5
#define ADC_TRIS_CHANNEL5 TRISEbits.TRISE0
#define ADC_TRIS_CHANNEL6 TRISEbits.TRISE1
#define ADC_TRIS_CHANNEL7 TRISEbits.TRISE2

#ifdef	__cplusplus
extern "C" {
#endif
    
    void adc_init(void);
    uint16_t adc_read(uint8_t channel);
    void adc_deinit(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* ADC_INTERFACE_H */

