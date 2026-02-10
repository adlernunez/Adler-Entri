/*
 * trafficlightwithmotor.c
 *
 * Created: 25-11-2025 11:34:13
 * Author : adler
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void traffic_light();
ISR(INT0_vect);

int main(void)
{
	
	sei(); //enable global interrupt
	EIFR|=(1<<INTF0); // 
	EIMSK|=(1<<INT0);//	External interrupt mask reg int0 enabled, D2 PIN IS EXTERNAL INT0
	EICRA|=(1<<ISC00)|(1<<ISC01);// CONFIGURING INTERRUPT FOR RISING EDGE
	DDRD|=(1<<DDD0);//D0, D1 AS OUTPUT
	DDRD|=(1<<DDD1);
	DDRD|=(1<<DDD2);
	DDRB|=(1<<DDB0);//B0,B1,B2,AS GREEN YELLOW RED
	DDRB|=(1<<DDB1);
	DDRB|=(1<<DDB2);
	
    while (1) 
    {
		
	traffic_light(); // default traffic lights
		
		
    }
}

void traffic_light()
{
	
	PORTB|=(1<<PB1);// YELLOW ON
	_delay_ms(3000);
	PORTB&=~(1<<PB1);// YELLOW OFF
	_delay_ms(1000);
	
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

ISR(INT0_vect)
{
	PORTB&=~(1<<PB0);//GREEN OFF
	PORTB&=~(1<<PB1);// YELLOW OFF
	PORTB|=(1<<PB2);//RED ON
	
	PORTD|=(1<<PD0);// CW
	PORTD&=~(1<<PD1);
	_delay_ms(5000);
	
	PORTD&=~(1<<PD0);// MOTOR STOP
	_delay_ms(5000);
	
	PORTD|=(1<<PD1);// CCW
	PORTD&=~(1<<PD0);
	_delay_ms(5000);
	
	PORTD&=~(1<<PD0);// MOTOR STOP
	PORTD&=~(1<<PD1);
	
	PORTB&=~(1<<PB2);//RED OFF
	_delay_ms(1000);
	
	PORTB|=(1<<PB1);// YELLOW ON
	_delay_ms(3000);
	PORTB&=~(1<<PB1);// YELLOW OFF
	_delay_ms(1000);
}