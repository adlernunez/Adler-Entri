#include "stm32f401xx.h"
#include "STM32F401XX_gpio_driver.h"
#include "Delay.h"

uint8_t current_Buttonstate=0;									//setting current state to 0
uint8_t previous_Buttonstate=1;									//setting current state to 1 as the button is active low


//previous button state is saved when button released
void Led_buttonfunction_ON_OFF()
{

	current_Buttonstate = GPIO_READFromInputPin(GPIOC,GPIO_PIN_NO_13);	// button state is saved to current_Buttonstate

	/*check for previous state and current_Buttonstate, if current_Buttonstate
	 goes low then, it toggles, once pressed 5ms delay to take next input. Then updates the previous state.
	*/
	if((previous_Buttonstate ==1)&&(current_Buttonstate==0)){
			GPIO_ToggleOutputPin(GPIOA,GPIO_PIN_NO_5);
			while(GPIO_READFromInputPin(GPIOC, GPIO_PIN_NO_13) == 0);
			delay_ms(10);
		}
		previous_Buttonstate = current_Buttonstate;

}
