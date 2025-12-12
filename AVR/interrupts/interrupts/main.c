/*
 * interrupts.c
 *
 * Created: 04-12-2025 20:44:38
 * Author : adler
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
ISR(TIMER0_OVF_vect)
int main(void)
{
    sei();
	TCNT0=0;
	TCNT0A&=~((1<<WGM00)|(1<<WGM01));//normal mode
	TCCR0B |= (1 << CS01) | (1 << CS00);//prescalar 64
	TCCR0B &= ~(1 << CS02);
    while (1) 
    {
    }
}

