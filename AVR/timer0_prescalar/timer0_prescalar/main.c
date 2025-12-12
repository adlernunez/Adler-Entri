/*
 * timer0_prescalar.c
 *
 * Created: 09-12-2025 10:33:03
 * Author : adler
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
void delay1_s(void);

int main(void)
{
    TCNT0=0; //TIMER SELECT 0
	TCCR0A&=~((1<<WGM00)|(1<<WGM01));// NORMAL
	TCCR0B|=((1<<CS02)|(1<<CS00));//SETTING PRESCALAR TO 1024
	TCCR0B&=~(1<<CS01);
	DDRB|=(1<<DDB5);// LED ON PORTB5
    while (1) 
    { PORTB^=(1<<PB5); // TOGGLE LED
		delay1_s();
    }
}

void delay1_s(void)
{
	for(int i=0;i<61;i++)  // Pre scalar 1024, makes timer run 61 times to get 1 sec delay
	{
		while (!(TIFR0 & (1 << TOV0)));   // Wait here until Timer0 overflows.
		// The loop keeps checking the TOV0 bit, which becomes 1 when the timer completes a full count cycle.

		TIFR0 |= (1 << TOV0);             // Clear the overflow flag by writing a 1 to TOV0.
		// This resets the flag so the timer can detect the next overflow event.

	}
}