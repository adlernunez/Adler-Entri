#include <stdint.h>
#include <stdio.h>
#include "stm32f401re_usart_driver.h"
#include "dht11_driver.h"

// --- trigger temperature ---
#define TEMP_THRESHOLD 34

int main(void)
{
    char string[60];

    // 1. Initialize USART2 (PA2 TX, PA3 RX)
    usart2_GPIO_PinSetUp();
    USART_Handle_t usart2 = {0};
    usart2.pUSARTx = USART2;
    usart2.USART_Config.USART_Baud = USART_STD_BAUD_9600;
    usart2.USART_Config.USART_Mode = USART_MODE_RXTX;
    usart2.USART_Config.USART_Parity = USART_NO_PARITY;
    usart2.USART_Config.USART_StopBit = USART_STOP_BIT_1;
    usart2.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
    usart2.USART_Config.USART_OverSampling = USART_OVERSAMPLING_16;
    usart2.USART_Config.USART_HWflowControl = USART_NO_CTS_RTS;
    USART_Init(&usart2);
    USART_PeripheralControl(USART2, ENABLE);

    // 2. Initialize the DHT11 Sensor (On PA1)
    DHT11_Init();
    DHT11_Data_t mySensorData = {0};

    // 3. Initialize the Onboard Green LED (PA5)
    GPIO_handle_t led_green = {0};
    led_green.pGPIOx = GPIOA;
    led_green.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    led_green.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    led_green.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    led_green.GPIO_PinConfig.GPIO_PinOPType = GPIO_PUSHPULL;
    led_green.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_Init(&led_green);

    while(1)
    {
        // ~2 second delay for sensor stabilization
        for(uint32_t ms = 0; ms < 2000; ms++) {
            for(uint32_t us = 0; us < 1000; us++) {
                while(!(SYSTICK->CTRL & (1<<16)));
            }
        }

        // Read the sensor
        DHT11_Read(&mySensorData);

        // --- LED LOGIC ---
        // If temp is 35 or above, turn onboard LED ON (PA5)
        if(mySensorData.temp_int >= TEMP_THRESHOLD) {
            GPIO_WriteToOutputPin(GPIOA, 5, GPIO_PIN_SET);
        } else {
            GPIO_WriteToOutputPin(GPIOA, 5, GPIO_PIN_RESET);
        }

        // Format the output string
        sprintf(string, "Temp: %d.%d C \r\n",
                mySensorData.temp_int, mySensorData.temp_dec);

        // Send results to Serial Monitor
        USART_Transmit_String(USART2, string);
    }
}
