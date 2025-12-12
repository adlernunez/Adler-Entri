/*
 * timer0.c
 *
 * Created: 25-11-2025 21:13:07
 * Author : adler
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>


void delay_1s(void)
{
	TCNT0=0; //LOAD COUNT VALUE
	TCCR0A&=~(1<<WGM00 | 1<<WGM01); //SET NORMAL
	
	//NO PRESCALING
	TCCR0B&=~(1<<CS02);//0
	TCCR0B&=~(1<<CS01);//0
	TCCR0B|=(1<<CS00);//1
	
	for(long int i=0;i<62500;i++)
	{
		while(!(TIFR0&(1<<TOV0)));//CHECKING TOV0 FLAG
		TIFR0|=(1<<TOV0);//WRITING 1 TO MAKE TOV0=0
		
	}
}


int main(void)
{
    DDRB|=(1<<DDB5);
    while (1) 
    {
		PORTB^=(1<<PB5);
		delay_1s();
    }
}

