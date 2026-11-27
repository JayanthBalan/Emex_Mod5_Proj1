/* 
 * File:   rtc_interface.h
 * Author: jayan
 *
 * Created on November 27, 2026, 12:09 PM
 */

#ifndef RTC_INTERFACE_H
#define	RTC_INTERFACE_H

#include <stdint.h>
#include "I2C_Interface.h"

#define RTC_ADDR 0x68U

typedef struct _rtc_time {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
} rtc_time_t;

#ifdef	__cplusplus
extern "C" {
#endif
    
    uint8_t rtc_init(uint32_t Fosc, const char *time);
    rtc_time_t rtc_gettime(void);
    void rtc_deinit(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* RTC_INTERFACE_H */

