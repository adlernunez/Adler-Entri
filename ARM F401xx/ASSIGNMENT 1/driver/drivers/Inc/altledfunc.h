#ifndef ALTLEDFUNC_H_
#define ALTLEDFUNC_H_

#include "stm32f401xx.h"
#include "STM32F401XX_gpio_driver.h"

/* ─────────────────────────────────────────────
 *  delay_ms  (was Delay.h)
 * ───────────────────────────────────────────── */
void delay_ms(uint32_t ms)
{
    for(uint32_t i = 0; i < (ms * 8400); i++);
}

/* ─────────────────────────────────────────────
 *  Button  (was button.h)
 * ───────────────────────────────────────────── */
uint8_t current_Buttonstate  = 0;   // setting current state to 0
uint8_t previous_Buttonstate = 1;   // setting previous state to 1 as the button is active low

// previous button state is saved when button released
void Led_buttonfunction_ON_OFF()
{
    current_Buttonstate = GPIO_READFromInputPin(GPIOC, GPIO_PIN_NO_13); // button state is saved to current_Buttonstate

    /* check for previous state and current_Buttonstate, if current_Buttonstate
     * goes low then, it toggles, once pressed 5ms delay to take next input. Then updates the previous state.
     */
    if((previous_Buttonstate == 1) && (current_Buttonstate == 0))
    {
        GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
        while(GPIO_READFromInputPin(GPIOC, GPIO_PIN_NO_13) == 0);
        delay_ms(10);
    }
    previous_Buttonstate = current_Buttonstate;
}

/* ─────────────────────────────────────────────
 *  ALT_LEDblink_Button_LED
 * ───────────────────────────────────────────── */
void ALT_LEDblink_Button_LED()
{
    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_11); // toggle led1 at PA11
    // 10ms x 25 times is 250ms
    // between every 10ms we can check the input from the button to work simultaneously.
    // another option is to create a timer function.
    for(int i = 0; i < 25; i++)
    {
        delay_ms(10);
        Led_buttonfunction_ON_OFF();             // Checking button __ip__
    }
    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_11); // toggle led1 at PA11

    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_12); // toggle led2 at PA12
    // 10ms x 25 times is 250ms
    // between every 10ms we can check the input from the button to work simultaneously.
    // another option is to create a timer function.
    for(int i = 0; i < 25; i++)
    {
        delay_ms(10);
        Led_buttonfunction_ON_OFF();             // Checking button __ip__
    }
    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_12);
}

#endif
