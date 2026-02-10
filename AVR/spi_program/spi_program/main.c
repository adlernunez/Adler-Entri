/*
 * spi_program.c
 *
 * Created: 06-01-2026 20:43:15
 * Author : adler
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	
	UART_Init();
	uint8_t chip_id;
//SETTING PB3, PB5, PB2 AS OUTPUT AND PB4 AS INPUT
	DDRB |=(1<<DDB3)|(1<<DDB5)|(1<<DDB2); //mosi, scl, csb are output
	DDRB &=~(1<<DDB4);// miso as input
	
//ENABLE SPI AND CONFIG AS MASTER.
	SPCR|=(1<<SPE)|(1<<MSTR)|(1<<SPR0);
	SPCR&=~((1<<SPR1)|(1<<CPHA)|(1<<CPOL));
    /* Replace with your application code */
    while (1) 
    {
		PORTB&=~(1<<PB2); //select slave low
		SPDR= (0XD0|0X80);//ensuring 1 on msb (reading)
		while (!(SPSR & (1<<SPIF)));//WAITING UNTIL FLAG IS SET
		(void)SPDR;//READING SPDR(FOR DUMMY GARBAGE VALUE
	
		SPDR=0X00;// SENDING DUMMY TO READ THE DATA FROM BMP280
		while (!(SPSR & (1<<SPIF)));//WAITINBG UNTIL FLAG IS SET
		chip_id = SPDR; // READING CHIP ID
		PORTB |= (1<<PB2);    // CS HIGH
		UART_TxHex(chip_id);
		_delay_ms(1000);	
	
	}
	
	
}

