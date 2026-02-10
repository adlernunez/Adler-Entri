#include "stm32f401xx.h"
#include "STM32F401XX_gpio_driver.h"
#include "Delay.h"
#include "button.h"

void ALT_LEDblink_Button_LED()
{
    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_11); // toggle led1 at PA11
        //10ms x 250 times is 250ms
        //between every10ms we can check the input from the button to work simultaneously.
        //another option is to create a timer function.
    for(int i = 0; i < 25; i++)
    {
        delay_ms(10);
        Led_buttonfunction_ON_OFF();  			// Checking button ip
    }
    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_11); // toggle led1 at PA11


    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_12); // toggle led2 at PA12
    //10ms x 250 times is 250ms
    //between every10ms we can check the input from the button to work simultaneously.
    //another option is to create a timer function.
    for(int i = 0; i < 25; i++)
    {
        delay_ms(10);
        Led_buttonfunction_ON_OFF();  			// Checking button ip
    }
    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_12);
}
