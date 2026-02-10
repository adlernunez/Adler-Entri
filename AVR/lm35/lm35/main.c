/*
 * lm35.c
 *
 * Created: 04-01-2026 11:47:56
 * Author : adler
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include  <util/delay.h>


void ADC_Init(void)
{
	ADMUX|=(1<<REFS0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t ADC_Read(uint8_t channel)
{
	channel &=0x07; //limiting channel to 7
	ADMUX = (ADMUX&0XF0)|channel;// channel selection
	ADCSRA|=(1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	return(ADCL|(ADCH<<8));
}

int main(void)
{
    uint16_t adc_value;
	lcd_init();
	ADC_Init();
	
	
	while (1) 
	
    {
		adc_value=ADC_Read(0);
		float temp= adc_value*0.488;
		lcd_clear();
		lcd_set_cursor(0,0);
		lcd_print("temperature:");
		lcd_print_float(temp);
		lcd_data(0xdf);
		lcd_print("C");
		_delay_ms(500)
		
    }
}

