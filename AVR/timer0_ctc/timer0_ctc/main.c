/*
 * timer0_ctc.c
 *
 * Created: 09-12-2025 12:15:49
 * Author : adler
 */ 
#define F_CPU 16000000ul
#include <avr/io.h>

void delay_1s(void);

int main(void)
{   TCNT0=0;
	OCR0A=0X9C;
	
   TCCR0A&=~(1<<WGM00); // ctc mode
   TCCR0A|=(1<<WGM01);
   
   TCCR0B|=((1<<CS02)|(1<<CS01));// 1024 prescaling
   TCCR0B&=~(1<<CS01);
   
   DDRB|=(1<<DDB5);// portb 5  pin on.
	
   
    while (1) 
    { PORTB^=(1<<PB5);
      delay_1s();
	}
}

void delay_1s(void)
{
	for(int i=0;i<100;i++)       // repeat 100 times to get 1 sec delay with 1024 prescaling, and compare value of 156
	{while(!(TIFR0&(1<<OCR0A))); //wait till ocf0 flag is set
		TIFR0|=(1<<OCF0A);		// reset the flag
	}
}