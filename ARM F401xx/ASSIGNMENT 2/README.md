# STM32 Temperature Measurement System (DHT11)

## Overview
This project implements a real-time environmental monitoring system using the DHT11 digital sensor interfaced with an STM32 microcontroller. The project demonstrates the successful integration of 1-wire communication protocols through bare-metal driver development, ensuring precise timing for data acquisition.

## Objectives
- Implement 1-wire protocol communication via custom GPIO drivers.
- Configure SysTick timers for microsecond-level pulse measurement.
- Develop robust data parsing for DHT11 (Temperature & Humidity).
- Implement visual feedback using LED indicators (Red for High Temp, Green for Safe).
- Integrate USART for serial data logging.

## Components Used
- STM32 Nucleo Board (STM32F401)
- DHT11 Temperature & Humidity Sensor (3-pin module)
- Internal LED
- STM32CubeIDE

## Working Principle
The DHT11 uses a simplified 1-wire protocol where timing is critical:

1. **Start Signal**: The MCU pulls the data line LOW for 18ms to wake the sensor.
2. **Response**: The sensor pulls the line LOW then HIGH to signal readiness.
3. **Data Transmission**: The sensor sends 40 bits of data and uses 8 bits of temp data (Temp Integer/Decimal).
4. **Bit Encoding**: Each bit is identified by the duration of the HIGH pulse (approx. 28µs for '0' and 70µs for '1').
5. **Validation**: The checksum is verified to ensure data integrity.

## System Architecture

### GPIO Driver
- Configured for bidirectional I/O switching.
- Pin mode toggles between `OUTPUT` (to send start pulse) and `INPUT` (to read data).

### 1-Wire Protocol Logic
- Uses SysTick to poll the data line state.
- Employs a relative time-measurement approach (comparing HIGH vs. LOW pulse durations) to ensure compatibility regardless of CPU clock drift.

### LED Feedback Logic
- Threshold: Configured via `#define TEMP_THRESHOLD 35`.
- Logic:
  - `Temp >= 35°C`: Green LED (PA4) ON, 
  - `Temp < 35°C`: Green LED (PA4) OFF.

## Implementation Flow
1. Initialize USART, SysTick, and GPIO pins.
2. Send the 18ms Start Signal to the DHT11.
3. Switch GPIO to Input mode to capture the 40-bit response.
4. Calculate bits based on high/low pulse width ratios.
5. Update status LEDs based on the measured integer temperature value.
6. Transmit the formatted data string via USART.

## Key Observations
- **Timing Sensitivity**: 1-wire protocol requires precise microsecond execution. Compiler optimizations were managed by using `volatile` keyword and specific SysTick polling.
- **Pin Conflict Management**: On-board LED conflicts were resolved by remapping the Green LED to PA4.
- **Power Stability**: The 3-pin DHT11 module includes an internal pull-up resistor, simplifying wiring significantly compared to the 4-pin bare sensor.

## Results
- Successfully achieved stable, repeatable environmental readings.
- LED indicators provide immediate visual feedback for thermal threshold alerts.
- Serial output provides continuous data logging for long-term monitoring.

**Example Output:**
```
Temp: 32.5 C 
```

## Key Learnings
- Mastering 1-wire communication protocols without high-level HAL libraries.
- Managing peripheral pin conflicts between internal board resources and external drivers.
- Improving code modularity by separating driver logic from application-level `main.c` code.

## 💻 Source Code
https://github.com/adlernunez/Adler-Entri/tree/master/ARM%20F401xx/ASSIGNMENT%202/driver

### Application Layer (main.c snippet)
```c
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
        // ~2 second delay 
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
```

## 🎥 Demo Video
https://drive.google.com/file/d/1bNBRZwPsCYKZ_QxUWs4p40-rPpUYzbn_/view?usp=sharing
