/* 
 * File:   Keypad_Scanner.h
 * Author: jayan
 *
 * Created on October 8, 2026, 2:59 PM
 */

#ifndef KEYPAD_SCANNER_H
#define	KEYPAD_SCANNER_H

#include <stdint.h>

#define KEYPAD_EDGE_TRIGGER UINT8_C(0)
#define KEYPAD_LEVEL_TRIGGER UINT8_C(1)

#define DEBOUNCE_NONBLOCK_DELAY_US UINT32_C(20000)
#define DEBOUNCE_BLOCK_DELAY_MS 10U

#define KEY0 (UINT16_C(1) << 0)
#define KEY1 (UINT16_C(1) << 1)
#define KEY2 (UINT16_C(1) << 2)
#define KEY3 (UINT16_C(1) << 3)
#define KEY4 (UINT16_C(1) << 4)
#define KEY5 (UINT16_C(1) << 5)
#define KEY6 (UINT16_C(1) << 6)
#define KEY7 (UINT16_C(1) << 7)
#define KEY8 (UINT16_C(1) << 8)
#define KEY9 (UINT16_C(1) << 9)
#define KEY10 (UINT16_C(1) << 10)
#define KEY11 (UINT16_C(1) << 11)

#ifdef	__cplusplus
extern "C" {
#endif
    
    void keypad_keys_init(uint16_t sel_keys);
    uint16_t scanKeys_keypad_block(uint8_t scanType, uint8_t block_delay_ms);
    uint16_t scanKeys_keypad_nonblock(uint8_t scanType, uint32_t currTick, uint32_t totTicks);
    void keypad_keys_deinit(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* KEYPAD_SCANNER_H */

