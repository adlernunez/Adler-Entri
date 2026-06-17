# GPIO Driver 
## Overview
This project implements a bare-metal General Purpose Input/Output (GPIO) driver to control physical pins on the STM32. It serves as the foundation for the entire system, managing bidirectional 1-wire communication for a DHT11 sensor and controlling an onboard LED for physical thermal alerts.

## Objectives
* Develop a custom GPIO driver to abstract AHB1 bus memory mapping and register manipulation.
* Configure pins dynamically for INPUT, OUTPUT, and Alternate Functions (AF).
* Implement SysTick timers for precise microsecond-level pulse measurement.
* Provide visual feedback using LED indicators based on thermal thresholds.

## Components Used
* STM32 Nucleo Board (STM32F401RE / F446RE)
* DHT11 Temperature & Humidity Sensor
* Internal LED (PA5)
* STM32CubeIDE

## Working Principle
The GPIO driver directly writes to the `MODER`, `OTYPER`, `OSPEEDR`, and `PUPDR` registers. For the DHT11, the protocol requires switching a single data pin between output (to send an 18ms wake-up pulse) and input (to measure the 40-bit response). Each data bit is identified by comparing the microsecond duration of HIGH vs. LOW logic states.

## System Architecture
* **GPIO Driver:** Hides bitwise shifting complexity through a `GPIO_Handle_t` struct.
* **1-Wire Protocol Logic:** Uses SysTick polling to capture strict timing edges.
* **LED Feedback Logic:** Temp >= 34°C triggers PA5 HIGH; Temp < 34°C triggers PA5 LOW.

## Implementation Flow
1. Initialize the GPIO clocks via RCC registers.
2. Configure PA5 as a Push-Pull output for the LED.
3. Send the 18ms Start Signal to the DHT11, then instantly swap the pin mode to Input.
4. Calculate temperature bits based on high/low pulse width ratios.
5. Toggle the LED based on the decoded integer temperature value.

## Key Observations
* **Timing Sensitivity:** 1-wire protocol requires exact microsecond execution. Compiler optimizations were managed by using the `volatile` keyword for time-critical polling loops.
* **Pin Abstraction:** The custom driver successfully hid the complex bit-masking required to swap pin directions on the fly.

## Results
Successfully achieved stable, repeatable environmental readings with an immediate physical LED alert when the temperature threshold is breached.

## Key Learnings
* Mastering GPIO memory offsets and register masking without high-level HAL libraries.
* Understanding the critical difference between Push-Pull and Open-Drain outputs when dealing with external sensors.

💻 **Source Code:** https://github.com/adlernunez/Adler-Entri/tree/master/ARM%20F401xx/FINAL%20PROJECT/STM32F446RE

## Application Layer (main.c snippet)
```c
#include "stm32f401re_gpio_driver.h"
#include "dht11_driver.h"

#define TEMP_THRESHOLD 34

int main(void) {
    // Initialize the Onboard Green LED (PA5)
    GPIO_handle_t led_green = {0};
    led_green.pGPIOx = GPIOA;
    led_green.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    led_green.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    led_green.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    led_green.GPIO_PinConfig.GPIO_PinOPType = GPIO_PUSHPULL;
    led_green.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GPIO_Init(&led_green);

    DHT11_Init();
    DHT11_Data_t mySensorData = {0};

    while(1) {
        delay_ms(2000); // 2-second sensor recovery delay
        if(DHT11_Read(&mySensorData) == DHT11_OK) {
            if(mySensorData.temp_int >= TEMP_THRESHOLD) {
                GPIO_WriteToOutputPin(GPIOA, 5, GPIO_PIN_SET);
            } else {
                GPIO_WriteToOutputPin(GPIOA, 5, GPIO_PIN_RESET);
            }
        }
    }
}

DEMO:https://drive.google.com/drive/folders/1eXq7sDkj5dFtnOcIj5tX9Nme-Wcqtehi?usp=sharing