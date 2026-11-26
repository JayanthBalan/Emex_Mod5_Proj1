
#include "CLCD_Interface.h"

#define CLCD_PORT PORTD
#define CLCD_PORT_TRIS TRISD

#define ENABLE_TRIS TRISE1
#define REGISTER_SELECT_TRIS TRISE2

#define ENABLE PORTEbits.RE1
#define REGISTER_SELECT PORTEbits.RE2

#define DEFAULT_DELAY_US 100U
#define EN_DELAY_US 1U
#define MAX_DELAY_US 4500U

void writeCLCD(uint8_t info, uint8_t mode) {
    REGISTER_SELECT = mode;
    CLCD_PORT = info;
    ENABLE = 1U;
    __delay_us(EN_DELAY_US);
    ENABLE = 0U;
    __delay_us(DEFAULT_DELAY_US);

    return;
}

void cursorReturnCLCD(void) {
    writeCLCD(0x02U, CLCD_INST_MODE);
    __delay_us(MAX_DELAY_US);

    return;
}

void clearCLCD(void) {
    writeCLCD(0x01U, CLCD_INST_MODE);
    __delay_us(MAX_DELAY_US);

    return;
}

void writeCharCLCD(unsigned char data, uint8_t addr) {
    writeCLCD(addr, CLCD_INST_MODE);
    writeCLCD(data, CLCD_DAT_MODE);

    return;
}

void writeStringCLCD(const unsigned char *string, uint8_t start_Addr) {
    writeCLCD(start_Addr, CLCD_INST_MODE);

    uint8_t iter = 0;
    for(const unsigned char *sel = string; *sel != '\0' && iter < CLCD_LINE_LEN; sel++, iter++) {
        writeCLCD(*sel, CLCD_DAT_MODE);
    }

    return;
}

void CLCD_deinit(void) {
    writeCLCD(0x01U, CLCD_INST_MODE);
    __delay_us(MAX_DELAY_US);
    writeCLCD(0x08U, CLCD_INST_MODE);

    return;
}

void displayOptionsCLCD(uint8_t disp, uint8_t cursor, uint8_t blink) {
    writeCLCD((uint8_t)(0x08U | blink | (cursor << 1U) | (disp << 2U)), CLCD_INST_MODE);

    return;
}

void CLCD_init(uint8_t cursorS, uint8_t cursorID, uint8_t disp, uint8_t cursor, uint8_t blink) {
    ADCON1 = 0x06U;
    CLCD_PORT_TRIS = 0x00U;
    CLCD_PORT = 0x00U;
    ENABLE_TRIS = 0U;
    REGISTER_SELECT_TRIS = 0U;
    ENABLE = 0U;
    REGISTER_SELECT = 0U;

    __delay_ms(30);

    writeCLCD(0x30U, CLCD_INST_MODE);
    __delay_ms(5);

    writeCLCD(0x30U, CLCD_INST_MODE);
    __delay_us(150);

    writeCLCD(0x30U, CLCD_INST_MODE);
    __delay_us(150);

    writeCLCD(0x38U, CLCD_INST_MODE);
    __delay_us(100);

    writeCLCD(0x01U, CLCD_INST_MODE);
    __delay_ms(5);

    writeCLCD(0x06U, CLCD_INST_MODE);
    __delay_us(100);

    displayOptionsCLCD(disp, cursor, blink);

    return;
}
