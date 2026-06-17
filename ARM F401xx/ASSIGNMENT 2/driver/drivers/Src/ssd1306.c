#include "ssd1306.h"
#include "fonts.h"

// --- LOW LEVEL I2C WRITERS ---

static void SSD1306_WriteCommand(I2C_Handle_t *hi2c, uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    I2C_MasterSendData(hi2c, buf, 2, SSD1306_I2C_ADDR);
}

static void SSD1306_WriteData(I2C_Handle_t *hi2c, uint8_t data) {
    uint8_t buf[2] = {0x40, data};
    I2C_MasterSendData(hi2c, buf, 2, SSD1306_I2C_ADDR);
}

// --- INITIALIZATION AND GRAPHICS ---

void SSD1306_Init(I2C_Handle_t *hi2c) {
    SSD1306_WriteCommand(hi2c, 0xAE); // Display OFF
    SSD1306_WriteCommand(hi2c, 0xD5); // Set Display Clock Divide Ratio
    SSD1306_WriteCommand(hi2c, 0x80);
    SSD1306_WriteCommand(hi2c, 0xA8); // Set Multiplex Ratio
    SSD1306_WriteCommand(hi2c, 0x3F); // 64 lines
    SSD1306_WriteCommand(hi2c, 0xD3); // Set Display Offset
    SSD1306_WriteCommand(hi2c, 0x00);
    SSD1306_WriteCommand(hi2c, 0x40); // Set Start Line
    SSD1306_WriteCommand(hi2c, 0x8D); // Enable Charge Pump (CRITICAL)
    SSD1306_WriteCommand(hi2c, 0x14);
    SSD1306_WriteCommand(hi2c, 0x20); // Memory Addressing Mode
    SSD1306_WriteCommand(hi2c, 0x00); // Horizontal Addressing
    SSD1306_WriteCommand(hi2c, 0xA1); // Segment Remap
    SSD1306_WriteCommand(hi2c, 0xC8); // COM Output Scan Direction
    SSD1306_WriteCommand(hi2c, 0xDA); // COM Hardware Configuration
    SSD1306_WriteCommand(hi2c, 0x12);
    SSD1306_WriteCommand(hi2c, 0x81); // Set Contrast
    SSD1306_WriteCommand(hi2c, 0xCF);
    SSD1306_WriteCommand(hi2c, 0xD9); // Set Pre-charge Period
    SSD1306_WriteCommand(hi2c, 0xF1);
    SSD1306_WriteCommand(hi2c, 0xDB); // Set VCOMH Deselect Level
    SSD1306_WriteCommand(hi2c, 0x40);
    SSD1306_WriteCommand(hi2c, 0xA4); // Entire Display ON resume
    SSD1306_WriteCommand(hi2c, 0xA6); // Normal Display

    SSD1306_Fill(hi2c, 0x00);         // Clear screen memory to Black
    SSD1306_WriteCommand(hi2c, 0xAF); // Display ON
}

void SSD1306_Fill(I2C_Handle_t *hi2c, uint8_t color) {
    for(uint8_t page = 0; page < 8; page++) {
        SSD1306_WriteCommand(hi2c, 0xB0 + page); // Set Page Address
        SSD1306_WriteCommand(hi2c, 0x00);        // Set Lower Column
        SSD1306_WriteCommand(hi2c, 0x10);        // Set Higher Column
        for(uint8_t col = 0; col < 128; col++) {
            SSD1306_WriteData(hi2c, color);
        }
    }
}

// --- TEXT FUNCTIONS ---

void SSD1306_SetCursor(I2C_Handle_t *hi2c, uint8_t column, uint8_t page) {
    SSD1306_WriteCommand(hi2c, 0xB0 + page);                     // Set Page (row)
    SSD1306_WriteCommand(hi2c, 0x00 | (column & 0x0F));          // Set lower column address
    SSD1306_WriteCommand(hi2c, 0x10 | ((column >> 4) & 0x0F));   // Set higher column address
}

void SSD1306_WriteChar(I2C_Handle_t *hi2c, char ch) {
    if(ch < 32 || ch > 126) return;

    for(uint8_t i = 0; i < 5; i++) {
        SSD1306_WriteData(hi2c, Font5x7[ch - 32][i]);
    }
    SSD1306_WriteData(hi2c, 0x00); // 1px spacing
}

void SSD1306_WriteString(I2C_Handle_t *hi2c, char *str) {
    while(*str) {
        SSD1306_WriteChar(hi2c, *str++);
    }
}
