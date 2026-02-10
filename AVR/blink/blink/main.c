/*
 * blink.c
 *
 * Created: 15-11-2025 18:41:16
 * Author : adler
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB|=(1<<DDB5);
    /* Replace with your application code */
    while (1) 
    {
		PORTB^=(1<<PB5);
		_delay_ms(1000);
    }
}

