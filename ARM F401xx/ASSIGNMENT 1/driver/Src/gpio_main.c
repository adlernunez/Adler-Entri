#include "stm32f401xx.h"
#include "STM32F401XX_gpio_driver.h"
#include "altledfunc.h"

int main(void)
{
    while(1)
    {
        ALT_LEDblink_Button_LED();
    }
    return 0;
}
