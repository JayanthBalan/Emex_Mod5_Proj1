/*
 * File:   Main.c
 * Author: jayan
 *
 * Created on November 25, 2026, 2:25 PM
 */

#include <xc.h>

#pragma config WDTE=OFF
#define _XTAL_FREQ 20000000U

#include "ADC_Interface.h"
#include "CLCD_Interface.h"
#include "I2C_Interface.h"
#include "Keypad_Scanner.h"
#include "non-block-delay-T0.h"
#include "non-block-delay-T2.h"
#include "rtc_interface.h"

#define LINE1_MONITOR_TITLE "TIME - EV - SP\0\0"

static void timeToString(rtc_time_t *time, char *str_begloc);

void main(void) {
    CLCD_init(CLCD_OPTION_OFF, CLCD_OPTION_ON, CLCD_OPTION_ON, CLCD_OPTION_OFF, CLCD_OPTION_OFF);
    while(rtc_init(_XTAL_FREQ, __TIME__) != I2C_SUCCESS);
    
    rtc_time_t curr;
    char line1[CLCD_LINE_LEN] = LINE1_MONITOR_TITLE;
    char line2[CLCD_LINE_LEN] = {0};
    
    writeStringCLCD((const unsigned char*)line1, CLCD_GET_ADDR(0,0));
    while(1) {
        curr = rtc_gettime();
        timeToString(&curr, line2);
        writeStringCLCD((const unsigned char*)line2, CLCD_GET_ADDR(1,0));
        __delay_ms(100);
    }
    
    return;
}


static void timeToString(rtc_time_t *time, char *str_begloc) {
    str_begloc[0] = (time->hours / 10U) + '0';
    str_begloc[1] = (time->hours % 10U) + '0';
    str_begloc[2] = ':';
    str_begloc[3] = (time->minutes / 10U) + '0';
    str_begloc[4] = (time->minutes % 10U) + '0';
    str_begloc[5] = ':';
    str_begloc[6] = (time->seconds / 10U) + '0';
    str_begloc[7] = (time->seconds % 10U) + '0';
    str_begloc[8] = '\0';

    return;
}

