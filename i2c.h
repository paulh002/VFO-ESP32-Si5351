
#ifndef _I2C_H_
#define _I2C_H_

void i2c_init(uint8_t sda = 21,uint8_t scl = 22);
bool i2c_send_byte( uint8_t address,uint8_t data );
uint8_t i2c_read_byte( bool ack, bool stop );
uint8_t i2c_receive_byte( uint8_t address );

uint8_t i2c_receive_byte_data( uint8_t address, uint8_t reg );
bool i2c_send_byte_data( uint8_t address, uint8_t reg, uint8_t data );
bool i2c_send_byte_data_bulk( uint8_t address, uint8_t reg, uint8_t bytes, uint8_t *data );














#endif
