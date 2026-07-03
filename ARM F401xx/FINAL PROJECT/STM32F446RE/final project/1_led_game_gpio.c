/**
 ******************************************************************************
 * @author         : Adler Nunez
 ******************************************************************************
 */

#include "stm32f446xx_gpio_driver.h"
#define BTN_PRESSED 0 													// active-low state for the button input.

void delay(void)
{
    for(uint32_t i = 0 ; i < 500000/2 ; i ++); 							// 200ms delay.
}

int main(void)
{
    GPIO_Handle_t GpioLedRed, GpioLedGreen, GPIOBtn; 					// Declares handle structures for two LEDs and one button.
    GpioLedRed.pGPIOx = GPIOA; 											// Sets the GPIO port to GPIOA for the Red LED.
    GpioLedRed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;			// Selects pin 0 of GPIOA.
    GpioLedRed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;			 	// Configures the pin as a digital output.
    GpioLedRed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; 			// Sets the output speed to fast.
    GpioLedRed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; 		// Sets the output type to push-pull.
    GpioLedRed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD; 		// Disables internal pull-up and pull-down resistors.

     							// Enables the peripheral clock for GPIOA.
	GPIO_Init(&GpioLedRed);											    // Writes the configuration to the GPIOA peripheral registers.

    GpioLedGreen.pGPIOx = GPIOA; 										// Sets the GPIO port to GPIOA for the Green LED.
    GpioLedGreen.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_1; 		// Selects pin 1 of GPIOA.
    GpioLedGreen.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;			// Configures the pin as a digital output.
    GpioLedGreen.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;		// Sets the output speed to fast.
    GpioLedGreen.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP; 		// Sets the output type to push-pull.
    GpioLedGreen.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD; 	// Disables internal pull-up and pull-down resistors.

    GPIO_Init(&GpioLedGreen); 											// Writes the configuration to the GPIOA peripheral registers (clock already enabled).

    GPIOBtn.pGPIOx = GPIOC; 											// Sets the GPIO port to GPIOC for the button.
    GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;				// Selects pin 13 of GPIOC.
    GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN; 				// Configures the pin as a digital input.
    GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;				// Sets the pin speed to fast.
    GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU; 			// Enables the internal pull-up resistor.

    GPIO_PeriClockControl(GPIOC, ENABLE); 								// Enables the peripheral clock for GPIOC.
    GPIO_Init(&GPIOBtn); 												// Writes the configuration to the GPIOC peripheral registers.

    uint8_t current_led = 0;											// Initializes a state variable (0 represents Red, 1 represents Green).

    while(1) 															// Initiates an infinite loop for the main program execution.
    {
        if(current_led == 0)											// Checks if the current state is the Red LED.
        {
            GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_0, 1); 			//  PA0 high (Red LED on).
            GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_1, 0); 			//  PA1 low (Green LED off).
        }
        else 															// Executes if the current state is the Green LED.
        {
            GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_0, 0); 			//  PA0 low (Red LED off).
            GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_1, 1); 			//  PA1 high (Green LED on).
        }

        uint8_t button_was_pressed = 0; 										// track button input.

        for(uint32_t i = 0; i < 350000; i++) 									// non-blocking delay to check if user pressed the button each second.
        {
            if(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == BTN_PRESSED) 	// Polls the input state of PC13.
            {
                button_was_pressed = 1; 										// Sets the flag indicating a button press was detected.
                for(uint32_t d = 0; d < 62500; d++); 							// debouncing delay for 50ms.
                break;
            }
        }

        if(button_was_pressed) 													// Checks if the button press flag was set during the polling loop.
        {
            if(current_led == 1) 												// Checks if button was pressed during green led state
            {
                for(int j = 0; j < 3; j++)										// Initiates a loop to execute a sequence 3 times.
                {
                    GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_0, 1); 			// Drives PA0 high.
                    GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_1, 1); 			// Drives PA1 high.
                    delay();
                    GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_0, 0); 			// Drives PA0 low.
                    GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_1, 0); 			// Drives PA1 low.
                    delay();
                }
            }
            else 																// Checks if button was pressed during red led state
            {
                GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_0, 0); 				// Drives PA0 low.
                GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_1, 0); 				// Drives PA1 low.

                for(uint32_t p = 0; p < 2500000; p++); 							// delay to show the game is lost for 2 sec.
            }

            while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == BTN_PRESSED); // Blocks execution until PC13 returns to logical high.
        }

        if(current_led == 0) 													// Checks if the current state is 0 (Red).
        {
            current_led = 1;													// Updates the state to 1 (Green) for the next iteration.
        }
        else 																	// Checks if the current state is 1 (Green).
        {
            current_led = 0;													// Updates the state to 0 (Red) for the next iteration.
        }
    }
    return 0;
}
