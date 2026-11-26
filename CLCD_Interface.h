/* 
 * File:   CLCD_Interface.h
 * Author: jayan
 *
 * Created on October 19, 2026, 1:16 PM
 */

#ifndef CLCD_INTERFACE_H
#define	CLCD_INTERFACE_H

#include <stdint.h>
#include <xc.h>

#define _XTAL_FREQ 20000000UL

#define CLCD_LINE_LEN 16U
#define CLCD_GET_ADDR_LINE0(y) (0x80 + ((y)%CLCD_LINE_LEN))
#define CLCD_GET_ADDR_LINE1(y) (0xC0 + ((y)%CLCD_LINE_LEN))
#define CLCD_GET_ADDR_LINE2(y) (0x94 + ((y)%CLCD_LINE_LEN))
#define CLCD_GET_ADDR_LINE3(y) (0xD4 + ((y)%CLCD_LINE_LEN))
#define CLCD_GET_ADDR(x, y) \
    (((x) == 0U) ? CLCD_GET_ADDR_LINE0(y) : \
    (((x) == 1U) ? CLCD_GET_ADDR_LINE1(y) : \
    (((x) == 2U) ? CLCD_GET_ADDR_LINE2(y) : \
    (((x) == 3U) ? CLCD_GET_ADDR_LINE3(y) : 0x01U))))
        

#define CLCD_INST_MODE UINT8_C(0)
#define CLCD_DAT_MODE UINT8_C(1)

#define CLCD_OPTION_ON UINT8_C(1)
#define CLCD_OPTION_OFF UINT8_C(0)

#ifdef	__cplusplus
extern "C" {
#endif

    void CLCD_init(uint8_t cursorS, uint8_t cursorID, uint8_t disp, uint8_t cursor, uint8_t blink);
    void writeCLCD(uint8_t info, uint8_t mode);
    void writeStringCLCD(const unsigned char *string, uint8_t start_Addr);
    void writeCharCLCD(unsigned char data, uint8_t addr);
    void displayOptionsCLCD(uint8_t disp, uint8_t cursor, uint8_t blink);
    void cursorReturnCLCD(void);
    void clearCLCD(void);
    void CLCD_deinit(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* CLCD_INTERFACE_H */

