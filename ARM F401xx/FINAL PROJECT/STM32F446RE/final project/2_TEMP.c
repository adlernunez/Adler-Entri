
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f446xx_usart_driver.h"
#include "dht11_driver.h"
#include "stm32f446xx_i2c_driver.h"
#include "stm32f446xx_spi_driver.h"
#include "ssd1306.h"

// global handles for our peripherals, setting them to 0
USART_Handle_t usart2 = {0};
I2C_Handle_t i2c1_handle = {0};
SPI_Handle_t spi1_handle = {0}; // spi handle variable

// function to setup all the pins
void GPIO_Manual_Init(void) {
    // turn on clocks for port A and B
    GPIOA_CLK_EN();
    GPIOB_CLK_EN();

    // setup usart pins
    usart2_GPIO_PinSetUp();

    // --- I2C PINS (PB8, PB9) ---
    GPIO_handle_t i2c_pins = {0};
    i2c_pins.pGPIOx = GPIOB;
    i2c_pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN; // alt function mode
    i2c_pins.GPIO_PinConfig.GPIO_PinAltFunction = 4; // use 4 for i2c
    i2c_pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OPENDRAIN; // open drain 
    i2c_pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PULLUP; // pull up 
    i2c_pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; // make it fast

    // init pin 8
    i2c_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
    GPIO_Init(&i2c_pins);
    
    // init pin 9
    i2c_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
    GPIO_Init(&i2c_pins);

    // --- SPI1 PINS (PA4, PA5, PA6, PA7) ---
    GPIO_handle_t spi_pins = {0};
    spi_pins.pGPIOx = GPIOA;
    spi_pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN; // alt function mode
    spi_pins.GPIO_PinConfig.GPIO_PinAltFunction = 5; // use 5 for spi1
    spi_pins.GPIO_PinConfig.GPIO_PinOPType = 0; // push pull (default)
    spi_pins.GPIO_PinConfig.GPIO_PinPuPdControl = 0; // no pull up or down
    spi_pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; // make it fast

    // init pin 5 for clock
    spi_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    GPIO_Init(&spi_pins);
    
    // init pin 6 for miso
    spi_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    GPIO_Init(&spi_pins);
    
    // init pin 7 for mosi
    spi_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
    GPIO_Init(&spi_pins);

    // change mode to 1 for the chip select pin
    spi_pins.GPIO_PinConfig.GPIO_PinMode = 1; 
    spi_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_4;
    GPIO_Init(&spi_pins);

    // Set Chip Select HIGH initially (Idle state)
}

// function to setup spi
void SPI1_Setup(void) {
    spi1_handle.pSPIx = SPI1;
    spi1_handle.SPI_Config.SPI_Device_Mode = SPI_DEVICE_MODE_MASTER; // set as master
    spi1_handle.SPI_Config.SPI_Bus_Config = SPI_BUS_CONFIG_FD;      // full duplex
    spi
