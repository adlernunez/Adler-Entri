/*
 * pwm_motor.c
 *
 * Created: 06-01-2026 15:40:52
 * Author : adler
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{  // MOTOR CONTROL PINS
   DDRD|=(1<<DDD5);
   DDRD|=(1<<DDD6);
   
   //PHASE CORRECT PWM
   TCCR0A|=(1<<WGM00);
   TCCR0A&=~(1<<WGM01);
   
   //NON INVERTING
   TCCR0A|=(1<<COM0A1);
   TCCR0A&=~(1<<COM0A0);
   
   //PRESCALR 8
   /*
   TCCR0B&=~(1<<CS00)|(1<<CS02);
   TCCR0B|=(1<<CS01);*/
   
   //PRESCALR 64
   /*
   TCCR0B|=(1<<CS00)|(1<<CS01);
   TCCR0B&=~(1<<CS02);
   */
   
    //PRESCALAR 256
	 /*
    TCCR0B&=~(1<<CS00)|(1<<CS01);
    TCCR0B|=(1<<CS02);
     */
	 
	 //PRESCALAR 1024
	  TCCR0B|=(1<<CS00)|(1<<CS02);
	  TCCR0B&=~(1<<CS01);
    while (1) 
    {
		PORTD&=~(1<<PD5);
		PORTD|=(1<<PD6);
		
		for(int i=0;i<=255;i++)
	    {
		    OCR0A=i;
		    _delay_ms(5);
	    }
	    
	    PORTD&=~(1<<PD5);
	    PORTD&=~(1<<PD6);
		_delay_ms(2000);
		
		PORTD&=~(1<<PD5);
		PORTD|=(1<<PD6);
		
		for(int i=255;i>0;i--)
		{
			OCR0A=i;
			_delay_ms(5);
		}

	    
    }
}


