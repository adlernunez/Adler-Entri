/*
 * external_interrupt.c
 *
 * Created: 13-12-2025 10:09:21
 * Author : adler
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
ISR(INT0_vect);

int main(void)
{
	sei();  // ENABLING GLOBAL INTERRUPT
	EIFR|=(1<<INTF0);// clear interrupt flag initially
	DDRB|=(1<<DDB5); // PORTB5 ENABLED
	DDRB|=(1<<DDB4);
	EIMSK|=(1<<INT0); // EXT INT MASK REG INT0 ENABLED
	EICRA|=((1<<ISC00)|(1<<ISC01)); // CONFIGURING INTERRUPT FOR RISING EDGE
	


    while (1) 
    {PORTB&=~(1<<PB5);
	 PORTB^=(1<<PB4);
	 _delay_ms(1000);
	 
	 
    }
}

ISR(INT0_vect)  //INTERRUPT SERVICE ROUTINE FOR EXTERNAL INTERRUPT 0;
{
	PORTB&=~(1<<PB4);
	PORTB^=(1<<PB5); //TOGGLING LED
	_delay_ms(1000);
 
	
}