#include "stm32f401xx.h"
#include "STM32F401XX_gpio_driver.h"
#include "button.h"
#include "alt_blink.h"
#include "GPIO_setup.h"



int main (void){

	GPIO_SETUP();


	while(1)
	{


		ALT_LEDblink_Button_LED();

	}
}
