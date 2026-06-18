// include all the libraries and headers we need
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f446xx_usart_driver.h"
#include "dht11_driver.h"
#include "stm32f446xx_i2c_driver.h"
#include "stm32f446xx_spi_driver.h"
#include "ssd1306.h"

// global handles for the peripherals, setting to 0 just to be safe
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
    spi_pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    spi_pins.GPIO_PinConfig.GPIO_PinAltFunction = 5; // use 5 for spi1
    spi_pins.GPIO_PinConfig.GPIO_PinOPType = 0; // push pull
    spi_pins.GPIO_PinConfig.GPIO_PinPuPdControl = 0; // no pull up or down
    spi_pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

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

    // TODO: Set Chip Select HIGH initially (Idle state)
}

// function to setup spi
void SPI1_Setup(void) {
    spi1_handle.pSPIx = SPI1;
    spi1_handle.SPI_Config.SPI_Device_Mode = SPI_DEVICE_MODE_MASTER; // set as master
    spi1_handle.SPI_Config.SPI_Bus_Config = SPI_BUS_CONFIG_FD;      // full duplex
    spi1_handle.SPI_Config.SPI_DataFrame = SPI_DFF_8BIT;            // 8 bit data
    spi1_handle.SPI_Config.SPI_CPHA = SPI_CPHA_LOW;                 // cpha low
    spi1_handle.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;                 // cpol low
    spi1_handle.SPI_Config.SPI_SSM = SPI_SSM_EN;                    // ssm enabled
    spi1_handle.SPI_Config.SPI_CLK_Speed = SPI_PRESCALAR_128;       // make it slow so arduino works

    // call init
    SPI_Init(&spi1_handle);
}

// main function
int main(void)
{
    // setup pins
    GPIO_Manual_Init();

    // 1. do USART Init
    usart2.pUSARTx = USART2;
    usart2.USART_Config.USART_Baud = USART_STD_BAUD_9600; // 9600 baud rate
    usart2.USART_Config.USART_Mode = USART_MODE_TX_ONLY; // tx only
    USART_Init(&usart2);
    USART_PeripheralControl(USART2, ENABLE); // turn it on

    // print a string to the terminal
    USART_Transmit_String(USART2, "\r\n=== SENSOR TO OLED & ARDUINO ===\r\n");

    // 2. do I2C & SPI Init
    i2c1_handle.pI2Cx = I2C1;
    i2c1_handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
    i2c1_handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE; // enable ack
    I2C_Init(&i2c1_handle);

    // call spi setup
    SPI1_Setup();
    SPI_PeripheralControl(SPI1, ENABLE); // turn on spi 

    // 3. do OLED Init
    SSD1306_Init(&i2c1_handle);
    SSD1306_Fill(&i2c1_handle, 0x00); // make screen black

    // 4. do DHT11 Init
    DHT11_Init();
    DHT11_Data_t sensorData = {0}; // struct for sensor data

    // make a buffer for the text
    char display_buffer[32];

    // infinite loop
    while(1)
    {
        // wait for a bit
        for(uint32_t i = 0; i < 2000000; i++); 

        // check if sensor read is okay
        if(DHT11_Read(&sensorData) == DHT11_OK)
        {
            // format the temperature into the buffer
            sprintf(display_buffer, "Temp:%d.%dC", sensorData.temp_int, sensorData.temp_dec);

            // --- DRAW TO OLED ---
            // move cursor and print spaces to clear the line
            SSD1306_SetCursor(&i2c1_handle, 0, 2);
            SSD1306_WriteString(&i2c1_handle, "                  ");
            // move cursor back and print the real text
            SSD1306_SetCursor(&i2c1_handle, 0, 2);
            SSD1306_WriteString(&i2c1_handle, display_buffer);

            // 1. Pull Chip Select LOW to wake up the Arduino
            // write low to the pin

            // 2. Transmit the string + 1 extra byte for the null terminator '\0'
            // get string length and add 1
            uint8_t datalen = strlen(display_buffer) + 1;
            // send the buffer over spi
            SPI_Transmit_Buffer(SPI1, (uint8_t*)display_buffer, datalen);

            // 3. Pull Chip Select HIGH to end transmission
            // write high to the pin
        }
        else
        {
            // print error if sensor fails
            USART_Transmit_String(USART2, "Sensor Error!\r\n");
            SSD1306_SetCursor(&i2c1_handle, 0, 2);
            SSD1306_WriteString(&i2c1_handle, "Sensor Error!     ");
        }
    }
}
