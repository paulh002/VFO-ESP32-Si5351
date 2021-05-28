#include "i2c.h"

I2c_direct::I2c_direct()
{
}

void I2c_direct::i2c_init(uint8_t sda,uint8_t scl)
{
  si_sda = sda ;
  si_scl = scl ;  
  
  pinMode ( si_sda, OUTPUT );           // Data line
  pinMode ( si_scl, OUTPUT );           // Clock line

  digitalWrite ( si_sda, HIGH ); 
  digitalWrite ( si_scl, HIGH );
  delayMicroseconds ( 100 );
}


void I2c_direct::i2c_start_condition()
{
  digitalWrite ( si_scl, HIGH );        
  digitalWrite ( si_sda, HIGH );  
  delayMicroseconds ( 1 );
  digitalWrite ( si_sda, LOW );       // Startcondition with the data pin LOW
  delayMicroseconds ( 1 );
  digitalWrite ( si_scl, LOW );       // and the clock pin LOW
  delayMicroseconds ( 1 );  
}

void I2c_direct::i2c_stop_condition()
{
  digitalWrite ( si_sda, LOW );       // Startcondition with the data pin LOW
  delayMicroseconds ( 1 );
  digitalWrite ( si_scl, HIGH );        // One final pulse on the clock
  delayMicroseconds ( 1 );    
  digitalWrite ( si_sda, HIGH );    
  delayMicroseconds ( 1 );
}

void I2c_direct::i2c_write_bit(uint8_t bit)
{
  if ( bit > 0 )              // Is the current bit a '1'?
    digitalWrite ( si_sda, HIGH );    // Yes, then send a '1'
  else
    digitalWrite ( si_sda, LOW );   // Otherwise send a '0'  

    delayMicroseconds ( 1 );        // Timeout

    digitalWrite ( si_scl, HIGH );      // Clock the bit into the Si5351
    delayMicroseconds ( 1 );

    digitalWrite ( si_scl, LOW );     // Pulsing the clock pin
}

uint8_t I2c_direct::i2c_read_bit( void )
{
    uint8_t b;
    
    digitalWrite ( si_sda, HIGH ); 
    delayMicroseconds ( 1 );
    pinMode ( si_sda, INPUT );
    digitalWrite ( si_scl, HIGH );
    delayMicroseconds ( 1 );
    
    if( digitalRead ( si_sda) ) b = 1;
    else b = 0;
    pinMode ( si_sda, OUTPUT );

    digitalWrite ( si_scl, LOW );     // Pulsing the clock pin
    
    return b;
}

bool I2c_direct::i2c_write_byte( uint8_t B,
                     bool start,
                     bool stop )
{
    uint8_t ack = 0;

    if( start ) 
      i2c_start_condition();

    uint8_t i;
    for( i = 0; i < 8; i++ )
    {
        i2c_write_bit( B & 0x80 );   // write the most-significant bit
        B <<= 1;
    }
    
    ack = i2c_read_bit();
    if (ack ==0) ack = 1; else ack = 0;

    if( stop ) 
      i2c_stop_condition();
    
    return ack;
}

uint8_t I2c_direct::i2c_read_byte( bool ack,
                       bool stop )
{
    uint8_t B = 0;

    uint8_t i;
    for( i = 0; i < 8; i++ )
    {
        B <<= 1;
        B |= i2c_read_bit();
    }

    if( ack ) i2c_write_bit(0);
      else i2c_write_bit(1);

    if( stop ) i2c_stop_condition();

    return B;
}

bool I2c_direct::i2c_send_byte( uint8_t address,
                    uint8_t data )
{
    if( i2c_write_byte( address << 1, true, false ) )   // start, send address, write
    {
        // send data, stop
        if( i2c_write_byte( data, false, true ) )  
          return true;
    }
    
    i2c_stop_condition();   // make sure to impose a stop if NAK'd
    return false;
}

// Receiving a byte with a I2C:
uint8_t I2c_direct::i2c_receive_byte( uint8_t address )
{
    if( i2c_write_byte( ( address << 1 ) | 0x01, true, false ) )   // start, send address, read
    {
        return i2c_read_byte( false, true );
    }

    return 0;   // return zero if NAK'd
}

bool I2c_direct::i2c_send_byte_data( uint8_t address,
                         uint8_t reg,
                         uint8_t data )
{
    if( i2c_write_byte( address << 1, true, false ) )   // start, send address, write
    {
        if( i2c_write_byte( reg, false, false ) )   // send desired register
        {
            if( i2c_write_byte( data, false, true ) ) return true;   // send data, stop
        }
    }

    i2c_stop_condition();
    return false;
}

bool I2c_direct::i2c_send_byte_data_bulk( uint8_t address, uint8_t reg, uint8_t bytes, uint8_t *data )
{
 
	if( i2c_write_byte( address << 1, true, false ) )   // start, send address, write
    {
        if( i2c_write_byte( reg, false, false ) )   // send desired register
        {
            for (int i = 0; i < bytes; i++)
			{
				i2c_write_byte( data[i], false, false );
			}
		i2c_stop_condition();
		return true;
        }
    }

    i2c_stop_condition();
    return false;
}


uint8_t I2c_direct::i2c_receive_byte_data( uint8_t address,
                               uint8_t reg )
{
    if( i2c_write_byte( address << 1, true, false ) )   // start, send address, write
    {
        if( i2c_write_byte( reg, false, true ) )   // send desired register
        {
            if( i2c_write_byte( ( address << 1) | 0x01, true, false ) )   // start again, send address, read
            {
                return i2c_read_byte( false, true );   // read data
            }
        }
    }


    i2c_stop_condition();
    return 0;   // return zero if NACKed
}
