
#include "Keypad_Scanner.h"
#include <xc.h>

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 20000000UL
#endif

#define KEY_COUNT UINT8_C(12)
#define RANK_COUNT UINT8_C(4)
#define FILE_COUNT UINT8_C(3)
#define BLOCK_DELAY_MS 1U

#define BASE_MASK_8 UINT8_C(0x01)
#define BASE_MASK_16 UINT16_C(0x0001)
#define FILE_MASK UINT8_C(0x07)
#define RANK_MASK UINT8_C(0x0F)

static uint16_t scanKeys_raw(void);
static uint16_t debounceKeys(uint32_t currTick, uint32_t totTick);
static uint16_t scanKeys_edge(uint32_t curr, uint32_t tot);
static uint16_t scanKeys_level(uint32_t curr, uint32_t tot);

static uint16_t enabled_keys;
static uint16_t keys_pressed;
static uint16_t candidate_keys;
static uint16_t stable_keys;

static uint8_t delay_ms;

void keypad_keys_init(uint16_t sel_keys) {
    enabled_keys = sel_keys & UINT16_C(0x0FFF);
    keys_pressed = UINT16_C(0);
    candidate_keys = UINT16_C(0);
    stable_keys = UINT16_C(0);
    
    return;
}

void keypad_keys_deinit(void) {
    enabled_keys = UINT16_C(0);
    keys_pressed = UINT16_C(0);
    candidate_keys = UINT16_C(0);
    stable_keys = UINT16_C(0);
    
    return;
}

static uint16_t scanKeys_raw(void) {
    uint16_t keypad_keys = UINT16_C(0);
    uint8_t portb_status = PORTB;
    uint8_t trisb_status = TRISB;
    uint8_t trisd_status = TRISD;

    PORTB |= FILE_MASK;
    TRISB |= FILE_MASK;

    PORTD |= RANK_MASK;
    TRISD &= (uint8_t)(~RANK_MASK);

    for (uint8_t rank = 0U; rank < RANK_COUNT; rank++) {
        uint8_t row_mask = (uint8_t)(BASE_MASK_8 << (RANK_COUNT - rank - 1U));

        PORTD |= RANK_MASK;
        PORTD &= (uint8_t)(~row_mask);

        __delay_us(25);

        for (uint8_t file = 0U; file < FILE_COUNT; file++) {
            if ((PORTB & (BASE_MASK_8 << file)) == UINT8_C(0)) {
                keypad_keys |= (uint16_t)(BASE_MASK_16 << (rank * FILE_COUNT + file));
            }
        }
    }

    PORTD |= RANK_MASK;
    TRISD = (uint8_t)((TRISD & (uint8_t)(~RANK_MASK)) | (trisd_status & RANK_MASK));
    PORTB = portb_status;
    TRISB = trisb_status;

    return keypad_keys;
}

/*
                 TRUTH TABLE
  |-----Input----|          |---Output---|
press1         press2       debounced_val
  0              0               0
  0              1               0
  1              0               0
  1              1               1

debounced_val = press1 & press2
*/


static uint16_t debounceKeys(uint32_t currTick, uint32_t totTick)
{
    static uint8_t debounce_active;
    static uint32_t startTick;

    uint16_t raw_keys = scanKeys_raw();

    if ((currTick == UINT32_C(0)) && (totTick == UINT32_C(0))) {
        if (raw_keys != stable_keys) {
            for(uint8_t iter = 0; iter < delay_ms; iter++) {
                CLRWDT();
                __delay_ms(BLOCK_DELAY_MS);
            }

            raw_keys &= scanKeys_raw();

            candidate_keys = raw_keys;
            stable_keys = raw_keys;
        }

        debounce_active = UINT8_C(0);
        startTick = UINT32_C(0);

        return stable_keys;
    }

    if (raw_keys != candidate_keys) {
        candidate_keys = raw_keys;
        startTick = currTick;
        debounce_active = UINT8_C(1);

        return stable_keys;
    }

    if (debounce_active != UINT8_C(0)) {
        if ((currTick - startTick) >= totTick) {
            stable_keys = candidate_keys;
            debounce_active = UINT8_C(0);
        }
    }

    return stable_keys;
}


/*
                      TRUTH TABLE
  |----Input-----|                 |----Output------|
keys         keys_pressed       out_val       keys_pressed
 0               0                 0                0
 0               1                 0                0
 1               0                 1                1
 1               1                 0                1

keys_pressed = keys
out_val = keys & (~keys_pressed)
*/

static uint16_t scanKeys_edge(uint32_t curr, uint32_t tot) {
    uint16_t keys = debounceKeys(curr, tot) & enabled_keys;
    uint16_t out_val = keys & (uint16_t)(~keys_pressed);

    keys_pressed = keys;

    return out_val;
}

static uint16_t scanKeys_level(uint32_t curr, uint32_t tot) {
    return debounceKeys(curr, tot) & enabled_keys;
}

uint16_t scanKeys_keypad_block(uint8_t scanType, uint8_t block_delay_ms) {
    uint16_t keypad_keys;
    delay_ms = block_delay_ms;

    if (scanType == KEYPAD_EDGE_TRIGGER) {
        keypad_keys = scanKeys_edge(0x00000000, 0x00000000);
    }
    else if (scanType == KEYPAD_LEVEL_TRIGGER) {
        keypad_keys = scanKeys_level(0x00000000, 0x00000000);
    }
    else {
        keypad_keys = UINT16_C(0);
    }

    return keypad_keys;
}

uint16_t scanKeys_keypad_nonblock(uint8_t scanType, uint32_t currTick, uint32_t totTicks) {
    uint16_t keypad_keys;

    if (scanType == KEYPAD_EDGE_TRIGGER) {
        keypad_keys = scanKeys_edge(currTick, totTicks);
    }
    else if (scanType == KEYPAD_LEVEL_TRIGGER) {
        keypad_keys = scanKeys_level(currTick, totTicks);
    }
    else {
        keypad_keys = UINT16_C(0);
    }

    return keypad_keys;
}
