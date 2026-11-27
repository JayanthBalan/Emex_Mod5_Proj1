/* 
 * File:   I2C_Interface.h
 * Author: jayan
 *
 * Created on November 23, 2026, 3:41 PM
 */

#ifndef I2C_INTERFACE_H
#define	I2C_INTERFACE_H

#include <xc.h>
#include <stdint.h>

#define I2C_ACK 0U
#define I2C_NACK 1U

#define I2C_SUCCESS 0U
#define I2C_ERROR 1U
#define I2C_TIMEOUT 2U

#define I2C_STANDARD_MODE 100000UL
#define I2C_FAST_MODE 400000UL

#ifdef	__cplusplus
extern "C" {
#endif
    
    void i2c_init(uint32_t Fosc, uint32_t SCL_Freq);
    
    uint8_t i2c_start(void);
    uint8_t i2c_restart(void);
    uint8_t i2c_stop(void);

    uint8_t i2c_write(uint8_t data);
    uint8_t i2c_read(uint8_t *data, uint8_t ack);

    uint8_t i2c_writeBytes(uint8_t slave_addr, const uint8_t *data, uint8_t size);
    uint8_t i2c_readBytes(uint8_t slave_addr, uint8_t *data, uint8_t size);

    void i2c_deinit(void);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_INTERFACE_H */
