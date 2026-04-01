#include "stm32f401xx.h"
#include "STM32F401XX_gpio_driver.h"


void GPIO_SETUP(){

		GPIO_handle_t GpioLed;
			GpioLed.pGPIOx 								     = GPIOA;				//SETTING GPIO PORT
			GpioLed.GPIO_PinConfig.GPIO_PinNumber	 		 = GPIO_PIN_NO_5;		//SETTING GPIO PIN
			GpioLed.GPIO_PinConfig.GPIO_PinMode 			 = GPIO_MODE_OUT;		//SETTING GPIO MODE
			GpioLed.GPIO_PinConfig.GPIO_PinSpeed 			 = GPIO_SPEED_HIGH;		//SETTING GPIO SPEED
			GpioLed.GPIO_PinConfig.GPIO_PinOPType			 = GPIO_PUSHPULL;		//SETTING GPIO OPTYPER
			GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl		 = GPIO_NO_PUPD;			//SETTING GPIO PUPD CONTROL

		GPIO_handle_t Gpio_button;
			Gpio_button.pGPIOx 								 = GPIOC;				//SETTING GPIO PORT
			Gpio_button.GPIO_PinConfig.GPIO_PinNumber		 = GPIO_PIN_NO_13;		//SETTING GPIO PIN
			Gpio_button.GPIO_PinConfig.GPIO_PinMode   	  	 = GPIO_MODE_IN;			//SETTING GPIO MODE

		GPIO_handle_t Gpio_EXT_LED1;
			Gpio_EXT_LED1.pGPIOx 							 = GPIOA;				//SETTING GPIO PORT
			Gpio_EXT_LED1.GPIO_PinConfig.GPIO_PinNumber 	 = GPIO_PIN_NO_11;		//SETTING GPIO PIN
			Gpio_EXT_LED1.GPIO_PinConfig.GPIO_PinMode 		 = GPIO_MODE_OUT;		//SETTING GPIO MODE
			Gpio_EXT_LED1.GPIO_PinConfig.GPIO_PinSpeed	     = GPIO_SPEED_HIGH;		//SETTING GPIO SPEED
			Gpio_EXT_LED1.GPIO_PinConfig.GPIO_PinOPType	 	 = GPIO_PUSHPULL;		//SETTING GPIO OPTYPER
			Gpio_EXT_LED1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;			//SETTING GPIO PUPD CONTROL

		GPIO_handle_t Gpio_EXT_LED2;
			Gpio_EXT_LED2.pGPIOx 							 = GPIOA;				//SETTING GPIO PORT
			Gpio_EXT_LED2.GPIO_PinConfig.GPIO_PinNumber 	 = GPIO_PIN_NO_12;		//SETTING GPIO PIN
			Gpio_EXT_LED2.GPIO_PinConfig.GPIO_PinMode 		 = GPIO_MODE_OUT;		//SETTING GPIO MODE
			Gpio_EXT_LED2.GPIO_PinConfig.GPIO_PinSpeed		 = GPIO_SPEED_HIGH;		//SETTING GPIO SPEED
			Gpio_EXT_LED2.GPIO_PinConfig.GPIO_PinOPType 	 = GPIO_PUSHPULL;		//SETTING GPIO OPTYPER
			Gpio_EXT_LED2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;			//SETTING GPIO PUPD CONTROL


		GPIO_PeriClockControl(GPIOA, ENABLE);							//output PORT A ENABLED
		GPIO_PeriClockControl(GPIOC, ENABLE);							//input  PORT C ENABLED

		GPIO_Init(&GpioLed);											//initializing Gpioled 	  PA05
		GPIO_Init(&Gpio_button);										//initializing Gpiobutton PC13
		GPIO_Init(&Gpio_EXT_LED1);										//initializing Gpiobutton PA11
		GPIO_Init(&Gpio_EXT_LED2);										//initializing Gpiobutton PA12
}
