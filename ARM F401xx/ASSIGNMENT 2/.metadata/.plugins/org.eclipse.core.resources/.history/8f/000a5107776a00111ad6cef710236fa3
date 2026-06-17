#include "stm32f401re_i2c_driver.h"

void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi) {
    if(EnorDi == ENABLE) {
        if(pI2Cx == I2C1) I2C1_CLK_EN();
        else if(pI2Cx == I2C2) I2C2_CLK_EN();
        else if(pI2Cx == I2C3) I2C3_CLK_EN();
    } else {
        if(pI2Cx == I2C1) I2C1_CLK_DI();
        else if(pI2Cx == I2C2) I2C2_CLK_DI();
        else if(pI2Cx == I2C3) I2C3_CLK_DI();
    }
}

void I2C_Init(I2C_Handle_t *pI2CHandle) {
    I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

    // 1. Configure ACK
    if(pI2CHandle->I2C_Config.I2C_AckControl == I2C_ACK_ENABLE) {
        pI2CHandle->pI2Cx->I2C_CR1 |= (1 << 10);
    }

    // 2. Set peripheral clock frequency (Assuming 16MHz APB1)
    pI2CHandle->pI2Cx->I2C_CR2 |= 16;

    // 3. CCR (100kHz standard mode)
    uint32_t ccr = 16000000 / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
    pI2CHandle->pI2Cx->I2C_CCR = (ccr & 0xFFF);

    // 4. Enable Peripheral
    pI2CHandle->pI2Cx->I2C_CR1 |= (1 << 0);
}

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr) {
    // Generate START
    pI2CHandle->pI2Cx->I2C_CR1 |= (1 << 8);
    while(!(pI2CHandle->pI2Cx->I2C_SR1 & (1 << 0))); // Wait SB

    // Send Address
    pI2CHandle->pI2Cx->I2C_DR = (SlaveAddr << 1);
    while(!(pI2CHandle->pI2Cx->I2C_SR1 & (1 << 1))); // Wait ADDR
    (void)pI2CHandle->pI2Cx->I2C_SR2; // Clear ADDR

    // Send Data
    for(uint32_t i = 0; i < Len; i++) {
        while(!(pI2CHandle->pI2Cx->I2C_SR1 & (1 << 7))); // TXE
        pI2CHandle->pI2Cx->I2C_DR = pTxbuffer[i];
    }

    // Wait for BTF and Generate STOP
    while(!(pI2CHandle->pI2Cx->I2C_SR1 & (1 << 2))); // BTF
    pI2CHandle->pI2Cx->I2C_CR1 |= (1 << 9); // STOP
}
