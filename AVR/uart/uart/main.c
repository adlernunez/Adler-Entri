/*
 * uart.c
 *
 8n1 mode, 8 bit data, no parity, 1 stop bit. baud rate 9600, send letter a with 1 second delay. 
 
 * Created: 12-01-2026 20:14:15
 * Author : adler
 */ 

#include <avr/io.h>
#include<util/delay.h>

int main(void)
{
	char a='A';
	UBRR0H=0;
	UBRR0L=103;// set BAUD RATE 9600, CALCULATION, lower 8 bit
	
	UCSR0B|=(1<<TXEN0);//ENABLE TX
	
	UCSR0C|=(1<<UCSZ00)|(1<<UCSZ01);// 8 BIT SELECT
	UCSR0C&=~((1<<UPM00)|(1<<UPM01)|(1<<USBS0));// NO PARITY MODE AND 1 STOP BIT
	
	
    /* Replace with your application code */
    while (1) 
    {
		UDR0=a;
		while(!((UCSR0A)&(1<<UDRE0)));//wait until tx is complete
		_delay_ms(1000);
    }
}

