
#include <stdint.h>
#include <stdio.h>
#include "stm32f446xx.h"
#include "stm32f446xx_usart_driver.h"
#include "stm32f446xx_i2c_driver.h"
#include "dht11_driver.h"
#include "ssd1306.h"

// global variables for usart and i2c, set to 0
USART_Handle_t usart2 = {0};
I2C_Handle_t i2c1_handle = {0};

// function to setup the pins
void GPIO_Manual_Init(void) {
    // turn on clocks for port A and B
    GPIOA_PCLK_EN();
    GPIOB_PCLK_EN();

    // 2. setup usart pins
    usart2_GPIO_PinSetUp();

    // 3. setup i2c pins (pin 8 and 9)
    GPIO_Handle_t i2c_pins = {0};
    i2c_pins.pGPIOx = GPIOB;
    i2c_pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN; // alt mode
    i2c_pins.GPIO_PinConfig.GPIO_PinAltFunMode = 4; // use 4
    i2c_pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD; // open drain
    i2c_pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU; // pull up
    i2c_pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST; // fast speed

    // init pin 8
    i2c_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
    GPIO_Init(&i2c_pins);

    // init pin 9
    i2c_pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
    GPIO_Init(&i2c_pins);
}

// main function
int main(void)
{
    // call the pin setup function
    GPIO_Manual_Init();

    // 1. do usart setup
    usart2.pUSARTx = USART2;
    usart2.USART_Config.USART_Baud = USART_STD_BAUD_9600; // 9600 baud
    usart2.USART_Config.USART_Mode = USART_MODE_TX_ONLY; // tx only
    USART_Init(&usart2);
    USART_PeripheralControl(USART2, ENABLE); // turn it on

    // print hello message to the terminal
    USART_Transmit_String(&usart2, "\r\n=== F446 SENSOR TO OLED ===\r\n");

    // 2. do i2c setup
    i2c1_handle.pI2Cx = I2C1;
    i2c1_handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
    i2c1_handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE; // turn on ack
    i2c1_handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
    I2C_Init(&i2c1_handle);

    // 3. setup the oled screen
    SSD1306_Init(&i2c1_handle);
    SSD1306_Fill(&i2c1_handle, 0x00); // clear the screen to black

    // 4. setup the dht11 sensor
    DHT11_Init();
    DHT11_Data_t sensorData = {0}; // make a struct for data

    // make a buffer for the text
    char display_buffer[32];

    // loop forever
    while(1)
    {
        // wait for a bit (around 2 seconds)
        for(uint32_t i = 0; i < 2000000; i++);

        // if sensor reads fine
        if(DHT11_Read(&sensorData) == DHT11_OK)
        {
            // format the temp string
            sprintf(display_buffer, "Temp: %d.%d C", sensorData.temp_int, sensorData.temp_dec);

            // print to serial monitor
            USART_Transmit_String(&usart2, display_buffer);
            USART_Transmit_String(&usart2, "\r\n");

            // --- DRAW TO OLED ---
            // move cursor and print spaces to clear the old text
            SSD1306_SetCursor(&i2c1_handle, 0, 2);
            SSD1306_WriteString(&i2c1_handle, "                  ");

            // move cursor back and print the new text
            SSD1306_SetCursor(&i2c1_handle, 0, 2);
            SSD1306_WriteString(&i2c1_handle, display_buffer);

        }
        else
        {
            // if sensor fails, print error
            USART_Transmit_String(&usart2, "Sensor Error!\r\n");
            SSD1306_SetCursor(&i2c1_handle, 0, 2);
            SSD1306_WriteString(&i2c1_handle, "Sensor Error!     ");
        }
    }
}
