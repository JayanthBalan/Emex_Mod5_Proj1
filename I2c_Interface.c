
#include "I2C_Interface.h"

#define I2C_TIMEOUT_COUNT 5000UL

static uint8_t i2c_wait_idle(void)
{
    uint32_t timeout;

    for(timeout = I2C_TIMEOUT_COUNT; timeout > 0UL; timeout--) {
        if((SSPCON2 & 0x1FU) == 0U && SSPSTATbits.R_W == 0U) {
            return I2C_SUCCESS;
        }
    }

    return I2C_TIMEOUT;
}

void i2c_init(uint32_t Fosc, uint32_t SCL_Freq)
{
    uint32_t divider;

    if(Fosc == 0UL || SCL_Freq == 0UL) {
        return;
    }

    divider = Fosc / (4UL * SCL_Freq);

    if(divider == 0UL || divider > 256UL) {
        return;
    }

    SSPCONbits.SSPEN = 0U;

    TRISCbits.TRISC3 = 1U;
    TRISCbits.TRISC4 = 1U;

    PIE1bits.SSPIE = 0U;

    SSPCON = 0x00U;
    SSPCON2 = 0x00U;
    SSPSTAT = 0x80U;

    SSPADD = (uint8_t)(divider - 1UL);

    SSPCONbits.SSPM = 0x08U;
    SSPCONbits.SSPEN = 1U;

    PIR1bits.SSPIF = 0U;

    return;
}

uint8_t i2c_start(void)
{
    if(i2c_wait_idle() != I2C_SUCCESS) {
        return I2C_TIMEOUT;
    }

    PIR1bits.SSPIF = 0U;
    SSPCON2bits.SEN = 1U;

    return i2c_wait_idle();
}

uint8_t i2c_restart(void)
{
    if(i2c_wait_idle() != I2C_SUCCESS) {
        return I2C_TIMEOUT;
    }

    PIR1bits.SSPIF = 0U;
    SSPCON2bits.RSEN = 1U;

    return i2c_wait_idle();
}

uint8_t i2c_stop(void)
{
    if(i2c_wait_idle() != I2C_SUCCESS) {
        return I2C_TIMEOUT;
    }

    PIR1bits.SSPIF = 0U;
    SSPCON2bits.PEN = 1U;

    return i2c_wait_idle();
}

uint8_t i2c_write(uint8_t data)
{
    if(i2c_wait_idle() != I2C_SUCCESS) {
        return I2C_TIMEOUT;
    }

    PIR1bits.SSPIF = 0U;
    SSPBUF = data;

    if(i2c_wait_idle() != I2C_SUCCESS) {
        return I2C_TIMEOUT;
    }

    if(SSPCON2bits.ACKSTAT != 0U) {
        return I2C_ERROR;
    }

    return I2C_SUCCESS;
}

uint8_t i2c_read(uint8_t *data, uint8_t ack)
{
    uint32_t timeout;

    if(data == 0 || (ack != I2C_ACK && ack != I2C_NACK)) {
        return I2C_ERROR;
    }

    if(i2c_wait_idle() != I2C_SUCCESS) {
        return I2C_TIMEOUT;
    }

    PIR1bits.SSPIF = 0U;
    SSPCON2bits.RCEN = 1U;

    for(timeout = I2C_TIMEOUT_COUNT; timeout > 0UL; timeout--) {
        if(SSPSTATbits.BF != 0U) {
            break;
        }
    }

    if(SSPSTATbits.BF == 0U) {
        return I2C_TIMEOUT;
    }

    *data = SSPBUF;

    if(i2c_wait_idle() != I2C_SUCCESS) {
        return I2C_TIMEOUT;
    }

    PIR1bits.SSPIF = 0U;
    SSPCON2bits.ACKDT = (ack == I2C_ACK) ? 0U : 1U;
    SSPCON2bits.ACKEN = 1U;

    return i2c_wait_idle();
}

uint8_t i2c_writeBytes(uint8_t slave_addr, const uint8_t *data, uint8_t size)
{
    uint8_t status;

    if(data == 0 || size == 0U || slave_addr > 0x7FU) {
        return I2C_ERROR;
    }

    status = i2c_start();

    if(status != I2C_SUCCESS) {
        return status;
    }

    status = i2c_write((uint8_t)(slave_addr << 1U));

    if(status != I2C_SUCCESS) {
        i2c_stop();
        return status;
    }

    for(uint8_t iter = 0U; iter < size; iter++) {
        status = i2c_write(data[iter]);

        if(status != I2C_SUCCESS) {
            i2c_stop();
            return status;
        }
    }

    return i2c_stop();
}

uint8_t i2c_readBytes(uint8_t slave_addr, uint8_t *data, uint8_t size)
{
    uint8_t status;

    if(data == 0 || size == 0U || slave_addr > 0x7FU) {
        return I2C_ERROR;
    }

    status = i2c_start();

    if(status != I2C_SUCCESS) {
        return status;
    }

    status = i2c_write((uint8_t)((uint8_t)(slave_addr << 1U) | 1U));

    if(status != I2C_SUCCESS) {
        i2c_stop();
        return status;
    }

    for(uint8_t iter = 0U; iter < size; iter++) {
        uint8_t ack = (iter < (size - 1U)) ? I2C_ACK : I2C_NACK;

        status = i2c_read(&data[iter], ack);

        if(status != I2C_SUCCESS) {
            i2c_stop();
            return status;
        }
    }

    return i2c_stop();
}

void i2c_deinit(void)
{
    SSPCONbits.SSPEN = 0U;

    SSPCON = 0x00U;
    SSPCON2 = 0x00U;

    PIR1bits.SSPIF = 0U;

    TRISCbits.TRISC3 = 1U;
    TRISCbits.TRISC4 = 1U;

    return;
}
