/*
 * trafficlightwithmotor.c
 *
 * Created: 25-11-2025 11:34:13
 * Author : adler
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void traffic_light();
void motor();

int main(void)
{
    DDRD|=(1<<DDD0);//D0, D1 AS OUTPUT
	DDRD|=(1<<DDD1); 
	DDRD&=~(1<<DDD2);		//D2 AS INPUT
	PORTD|=(1<<PD2);		// HIGH I/P TO D2

	DDRB|=(1<<DDB0);//B0,B1,B2,AS GREEN YELLOW RED
	DDRB|=(1<<DDB1);
	DDRB|=(1<<DDB2);
    while (1) 
    {
		//traffic_light();		// default traffic lights
		
		if(!(PIND&(1<<PD2)))	//sensing s/w i/p
		{
			motor();			//motor traffic lights
		}
		
    }
}

void traffic_light()
{
	
	PORTB|=(1<<PB0);// GREEN ON
	_delay_ms(5000);
	PORTB&=~(1<<PB0);//GREEN OFF
	_delay_ms(1000);
	
	PORTB|=(1<<PB1);// YELLOW ON
	_delay_ms(3000);
	PORTB&=~(1<<PB1);// YELLOW OFF
	_delay_ms(1000);
	
	PORTB|=(1<<PB2);//RED ON
	_delay_ms(5000);
	PORTB&=~(1<<PB2);//RED OFF
	_delay_ms(1000);
	
	PORTB|=(1<<PB1);// YELLOW ON
	_delay_ms(3000);
	PORTB&=~(1<<PB1);// YELLOW OFF
	_delay_ms(1000);
}

void motor()
{

	PORTB|=(1<<PB2);//RED ON
	
	PORTD|=(1<<PD0);// CW
	PORTD&=~(1<<PD1);
	_delay_ms(10000);
	
	PORTD&=~(1<<PD0);// MOTOR STOP
	_delay_ms(20000);
	
	PORTD|=(1<<PD1);// CCW
	PORTD&=~(1<<PD0);
	_delay_ms(10000);
	
	PORTB&=(1<<PB2);//RED OFF
	_delay_ms(1000);
	
	PORTB|=(1<<PB1);// YELLOW ON
	_delay_ms(3000);
	PORTB&=~(1<<PB1);// YELLOW OFF
	_delay_ms(1000);
}