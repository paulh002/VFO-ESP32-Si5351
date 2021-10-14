
#ifndef _I2C_H_
#define _I2C_H_

#include "Arduino.h"

class I2c_direct
{
public:
	
I2c_direct();
void i2c_init(uint8_t sda,uint8_t scl);
bool i2c_send_byte( uint8_t address,uint8_t data );
uint8_t i2c_read_byte( bool ack, bool stop );
uint8_t i2c_receive_byte( uint8_t address );
uint8_t i2c_receive_byte_data( uint8_t address, uint8_t reg );
bool i2c_send_byte_data( uint8_t address, uint8_t reg, uint8_t data );
bool i2c_send_byte_data_bulk( uint8_t address, uint8_t reg, uint8_t bytes, uint8_t *data );

private:
uint8_t si_sda;
uint8_t si_scl;

void i2c_stop_condition();
void i2c_start_condition();
void i2c_write_bit(uint8_t bit);
uint8_t i2c_read_bit( void );
bool i2c_write_byte( uint8_t B, bool start, bool stop );
};


#endif
