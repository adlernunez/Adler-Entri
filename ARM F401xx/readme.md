==========================================================
STM32F401 GPIO PROJECT DOCUMENTATION - ADLER NUNEZ
=========================================================

PIN MAPPING
-----------

INTERNAL LED:
    LED 	     = GPIO PORT A, PIN 5

EXTERNAL LEDs:
    LED1         = GPIO PORT A, PIN 11
    LED2         = GPIO PORT A, PIN 12

INTERNAL USER BUTTON:
    BUTTON (B1)  = GPIO PORT C, PIN 13 (Active Low)

=======================
HEADER FILE STRUCTURE
=======================

stm32f401xx.h
    - Defines base addresses of all GPIO peripherals
    - Contains register definitions for GPIO, RCC, SPI, I2C, USART

STM32F401XX_gpio_driver.h
    - GPIO pin configuration definitions and macros
    - GPIO mode, speed, pull-up/pull-down, output type macros
    - Function prototypes for GPIO driver

STM32F401XX_gpio_driver.c
    - Implementation of GPIO driver functions
    - GPIO initialization, read, write, toggle functions
    - Peripheral clock control logic

gpio_main.c (or gpio_toggle.c)
    - Main program 
    - Calls GPIO functions in main loop
    - Integrates all functionality

GPIO_setup.h
    - GPIO initialization and configuration
    - Sets up all GPIO pins used in the project
    - Enables peripheral clocks

Delay.h
    - Software delay function (delay_ms)
    - Used for timing and debouncing

button.h
    - Button debouncing and state detection
    - Led_buttonfunction_ON_OFF() - toggles PA5 on button press

alt_blink.h
    - Alternating LED blink pattern (PA11 and PA12)
    - Blinks each LED for 250ms alternately
    - Integrated with button checking during delays.

==============
FUNCTIONALITY
==============

1. Internal LED (PA5) toggles ON/OFF when button (PC13) is pressed
2. External LEDs (PA11, PA12) blink alternately every 250ms
3. Button is checked during LED blink delays for responsive control

================================================================================
