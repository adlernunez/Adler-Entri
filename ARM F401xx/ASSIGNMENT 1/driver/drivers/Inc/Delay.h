#ifndef DELAY_H
#define DELAY_H

#include "stm32f401xx.h"
#include "STM32F401XX_gpio_driver.h"


void delay_ms(uint32_t ms)
{
    for(uint32_t i = 0; i < (ms * 8400); i++);
}


#endif
