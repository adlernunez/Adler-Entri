/*
 * pwm.c
 *
 * Created: 30-12-2025 13:27:43
 * Author : adler
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)

{	
	DDRD|=(1<<DDD6); // PWM OC0A, LED HERE	

	
	TCCR0A|=(1<<WGM00);//CONFIG FOR phaSE CORRECT PWM
	TCCR0A&=~(1<<WGM01);
	
	TCCR0A|=(1<<COM0A1); //CONFIG FOR NON INVERTING
	TCCR0B&=~(1<<COM0A0);
	   
	TCCR0B&=~(1<<CS00)|(1<<CS02);   // PRESCALR 8
	TCCR0B|=(1<<CS01);
	
	
    while (1) 
    {	
		for(int i=0;i<=255;i++)
		{
			OCR0A=i;
			_delay_ms(5);
		}
		
		
		for(int i=255;i>0;i--)
		{
			OCR0A=i;
			_delay_ms(5);
		}
    }
}

