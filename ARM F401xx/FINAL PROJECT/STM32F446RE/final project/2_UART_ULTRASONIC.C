/**
 * @file           : main.c
 * @brief          : HC-SR04 Ultrasonic Telemetry & Boundary Warning System
 * @processor      : STM32F401RE
 * @architecture   : Bare-Metal C (No HAL)
 */

#include <stdint.h>
#include <stdio.h>
#include "STM32F401XX_gpio_driver.h"
#include "stm32f401re_usart_driver.h"
#include "HCSR04_DRIVER.h"

#define DISTANCE_BOUNDARY_CM 30 // Set your near/far limit here

/* ==================================================================== */
/* HARDWARE INITIALIZATION                         */
/* ==================================================================== */

// Configures PA2 (TX) and PA3 (RX) for USART2 Communication
void USART2_GPIO_Setup(void)
{
    GPIO_PeriClockControl(GPIOA, ENABLE);

    GPIO_handle_t usart_pins = {0};
    usart_pins.pGPIOx = GPIOA;
    usart_pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    usart_pins.GPIO_PinConfig.GPIO_PinAltFunction = 7; 
    usart_pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    usart_pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PULLUP;
    usart_pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_PUSHPULL;

    usart_pins.GPIO_PinConfig.GPIO_PinNumber = 2; // PA2
    GPIO_Init(&usart_pins);

    usart_pins.GPIO_PinConfig.GPIO_PinNumber = 3; // PA3
    GPIO_Init(&usart_pins);
}

// Configures PA0 (Red) and PA5 (Green) for Output
void LED_GPIO_Setup(void)
{
    GPIO_PeriClockControl(GPIOA, ENABLE); 

    GPIO_handle_t GpioLeds = {0}; 
    GpioLeds.pGPIOx = GPIOA;
    GpioLeds.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLeds.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLeds.GPIO_PinConfig.GPIO_PinOPType = GPIO_PUSHPULL; 
    GpioLeds.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD; 

    GpioLeds.GPIO_PinConfig.GPIO_PinNumber = 0; // PA0
    GPIO_Init(&GpioLeds);

    GpioLeds.GPIO_PinConfig.GPIO_PinNumber = 5; // PA5
    GPIO_Init(&GpioLeds);
}

/* ==================================================================== */
/* MAIN PROGRAM                              */
/* ==================================================================== */

int main(void)
{
    // 1. Initialize GPIO Peripherals
    USART2_GPIO_Setup();
    LED_GPIO_Setup();

    // 2. Initialize USART2 (9600 Baud, 8-bit, No Parity)
    USART_Handle_t usart2 = {0};
    usart2.pUSARTx = USART2;
    usart2.USART_Config.USART_Baud = USART_STD_BAUD_9600;
    usart2.USART_Config.USART_Mode = USART_MODE_RXTX;
    usart2.USART_Config.USART_Parity = USART_NO_PARITY;
    usart2.USART_Config.USART_StopBit = USART_STOP_BIT_1;
    usart2.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;

    USART_Init(&usart2);
    USART_PeripheralControl(USART2, ENABLE);

    // 3. Initialize Sensor and Hardware Timers
    SysTick_Delay_init();
    GPIO_setup();  /* Initializes PA1 (Trig) and PA4 (Echo) */
    Timer2_Init();

    // 4. Working Variables
    uint32_t distance = 0;
    char buffer[50];

    // Boot Trace Verification
    USART_Transmit_String(USART2, "\r\n--- STM32 Radar System Online ---\r\n");

    while(1)
    {
        // Poll the hardware sensor
        distance = HCSR04_GetDistance();            

        // Boundary Logic & LED Indicators
        if (distance > DISTANCE_BOUNDARY_CM) 
        {
            // Object is FAR: Red ON, Green OFF
            GPIO_WriteToOutputPin(GPIOA, 0, 1);
            GPIO_WriteToOutputPin(GPIOA, 5, 0);
        }
        else if (distance > 0) 
        {
            // Object is NEAR: Green ON, Red OFF
            GPIO_WriteToOutputPin(GPIOA, 0, 0);
            GPIO_WriteToOutputPin(GPIOA, 5, 1);
        }
        else 
        {
            // Sensor Error / Timeout: Both OFF
            GPIO_WriteToOutputPin(GPIOA, 0, 0);
            GPIO_WriteToOutputPin(GPIOA, 5, 0);
        }

        // Format telemetry and transmit to PC
        sprintf(buffer, "Target Distance: %02lu cm\r\n", distance);
        USART_Transmit_String(USART2, buffer);

        // 500ms Non-Blocking Delay equivalent
        Delay_us(500000);
    }
    
    return 0;
}