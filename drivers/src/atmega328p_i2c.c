/*
 * @file              atmega328p_i2c.c
 *
 * @brief             XXXXX driver implementation for ATmega328P microcontroller.
 *
 * @details           This file provides the implementation of functions to initialize, 
 *                    de-initialize, etc, etc, XXXX for the ATmega328P.
 *                    It defines all the APIs to manage XXXX peripherals including
 *                    initialization, data handling, and XXXXX operations.
 *
 * @author            JESUS HUMBERTO ONTIVEROS MAYORQUIN
 * @date              19/11/2024 14:22:47
 *
 * @note              This driver is specifically designed for the ATmega328P microcontroller.
 *                    Ensure proper configurations before using these functions.
 */

#include "atmega328p_i2c.h"
#include "atmega328p_gpio.h"
#include <stdbool.h>

/*********************************************************************
 * @fn            - I2C_startCond
 *
 * @brief         - Generates the I2C START condition.
 *
 * @param[in]     - pI2Cx: Pointer to the I2C peripheral registers structure.
 *
 * @return        - true: If the START condition was successfully generated.
 *                - false: If the START condition failed (status mismatch).
 *
 * @Note          - This function sets the START condition on the I2C bus 
 *                  by configuring the TWCR register and waits for the TWINT
 *                  flag. Ensure the I2C peripheral is properly initialized
 *                  before calling this function.
 *********************************************************************/
static bool I2C_startCond(I2C_Regs_t *pI2Cx){

    pI2Cx->TWCR = ((1<<I2C_TWCR_TWINT) | (1<<I2C_TWCR_TWSTA) | (1<<I2C_TWCR_TWEN));

    while(!(pI2Cx->TWCR & (1<<I2C_TWCR_TWINT)));

    if ((pI2Cx->TWSR & 0xF8) == I2C_FLG_START)
	    return true;

	return false;
 }

/*********************************************************************
 * @fn            - I2C_sendAddr
 *
 * @brief         - Sends the 7-bit slave address along with the read/write 
 *                  action to the I2C bus.
 *
 * @param[in]     - pI2Cx: Pointer to the I2C peripheral registers structure.
 * @param[in]     - addr: 7-bit address of the I2C slave device.
 * @param[in]     - action: Specifies whether to write to or read from the slave.
 *                  Use the macros I2C_ACTION_WRITE or I2C_ACTION_READ.
 *
 * @return        - true: If the address and action were acknowledged.
 *                - false: If the address or action was not acknowledged.
 *
 * @Note          - Ensure that a START condition has been generated before
 *                  calling this function. The `action` parameter determines 
 *                  the operation (read/write) for the slave address.
 *********************************************************************/
static bool I2C_sendAdrr(I2C_Regs_t *pI2Cx, uint8_t adrr, uint8_t action){
	
	uint8_t cmp = 0;
	adrr = (adrr << 1 );

	if (action == I2C_ACTION_WRITE){
		adrr &=~ 1;
		cmp = I2C_FLG_SLA_W_ACK;
	}
	else{
		adrr |= 1;
		cmp = I2C_FLG_SLA_R_ACK;
	}

	pI2Cx->TWDR = adrr;
	pI2Cx->TWCR = ((1<<I2C_TWCR_TWINT) | (1<<I2C_TWCR_TWEN));

	while(!(pI2Cx->TWCR & (1<<I2C_TWCR_TWINT)));

	if ((pI2Cx->TWSR & 0xF8) == cmp)
		return true;
	 
	return false;
 }

/*********************************************************************
 * @fn            - I2C_write
 *
 * @brief         - Writes a single byte of data to the I2C data register.
 *
 * @param[in]     - pI2Cx: Pointer to the I2C peripheral registers structure.
 * @param[in]     - data2write: Byte of data to be transmitted.
 *
 * @return        - true: If the data was acknowledged by the slave.
 *                - false: If the data was not acknowledged (NACK received).
 *
 * @Note          - This function sends a byte over the I2C bus and waits
 *                  for the acknowledgment status from the slave device.
 *********************************************************************/
static bool I2C_write(I2C_Regs_t *pI2Cx, uint8_t data2write){
	
    bool ret = false;
	
    pI2Cx->TWDR = data2write;
    pI2Cx->TWCR = ((1<<I2C_TWCR_TWINT) | (1<<I2C_TWCR_TWEN));
    while(!(pI2Cx->TWCR & (1<<I2C_TWCR_TWINT)));
	
    if ((pI2Cx->TWSR & 0xF8) == I2C_FLG_DATA_ACK)
    	ret = true;
	
    return ret;
}

/*********************************************************************
 * @fn            - I2C_stopCond
 *
 * @brief         - Generates the I2C STOP condition to release the bus.
 *
 * @param[in]     - pI2Cx: Pointer to the I2C peripheral registers structure.
 *
 * @return        - None
 *
 * @Note          - This function sets the STOP condition on the I2C bus by
 *                  configuring the TWCR register. Ensure that all communication
 *                  is completed before calling this function.
 *********************************************************************/
static void I2C_stopCond(I2C_Regs_t *pI2Cx){

	pI2Cx->TWCR |= ((1<<I2C_TWCR_TWINT) | (1<<I2C_TWCR_TWSTO) | (1<<I2C_TWCR_TWEN));
}

/*********************************************************************
 * @fn            - I2C_Init
 *
 * @brief         - Initializes the I2C peripheral with the specified configuration.
 *
 * @param[in]     - pI2CInst: Pointer to the I2C instance to initialize.
 *
 * @return        - None
 *
 * @Note          - Ensure that the I2C clock is enabled and the pin configuration
 *                  is properly set before calling this function.
 *********************************************************************/
void I2C_Init(I2C_t *pI2CInst)
{
    GPIO_Regs_t GPIO_i2c_Reg = GPIOC;

    if (pI2CInst == NULL)
        return; // Invalid pointer, exit the function.
    
    // Configure the clock frequency in master mode
    if (!pI2CInst->Config.Mode)
    {
        // Set the TWBR register with the calculated value
        pI2CInst->pReg->TWBR  = I2C_SET_CLOCK(pI2CInst->Config.SCLSpeed, F_CPU);

        // Prescaler is set to 1 (TWPS bits in TWSR = 0)
        pI2CInst->pReg->TWSR &= ~((1 << I2C_TWSR_TWPS1)|(1 << I2C_TWSR_TWPS0));
        
    }else
    {
        // Configure slave address
        pI2CInst->pReg->TWAR = (pI2CInst->Config.DeviceAddress << I2C_TWAR_TWA0);
    }
    
    // Enable I2C module
    pI2CInst->pReg->TWCR = (1 << I2C_TWCR_TWEA);

    // Enable pull-ups for SDA (PC4) and SCL (PC5)
    *GPIO_i2c_Reg.PORT |= (1 << PIN4) | (PIN5);
};

/*********************************************************************
 * @fn            - I2C_DeInit
 *
 * @brief         - Deinitializes the I2C peripheral and resets it to its default state.
 *
 * @param[in]     - pI2CInst: Pointer to the I2C instance to deinitialize.
 *
 * @return        - None
 *
 * @Note          - Use this function to reset the I2C peripheral for reconfiguration
 *                  or to save power by disabling it when not in use.
 *********************************************************************/
void I2C_DeInit(I2C_t *pI2CInst)
{
    if (pI2CInst == NULL)
        return; // Invalid pointer, exit the function.

    // Reset I2C registers to their default state
    pI2CInst->pReg->TWBR = 0x00;       // Reset TWBR register
    pI2CInst->pReg->TWSR = 0x00;       // Reset TWSR register (includes prescaler bits)
    pI2CInst->pReg->TWAR = 0x00;       // Reset TWAR register (slave address)
    pI2CInst->pReg->TWCR = 0x00;       // Disable I2C by clearing TWCR register
    pI2CInst->pReg->TWDR = 0xFF;       // Reset data register to default value (0xFF)

    // Disable pull-ups for SDA (PC4) and SCL (PC5)
    GPIO_Regs_t GPIO_i2c_Reg = GPIOC;
    *GPIO_i2c_Reg.PORT &= ~((1 << PIN4) | (1 << PIN5));
};

/*********************************************************************
 * @fn            - I2C_MasterSendData
 *
 * @brief         - Sends data from the master to a slave device using I2C.
 *
 * @param[in]     - pI2CInst: Pointer to the I2C instance.
 * @param[in]     - pTxbuffer: Pointer to the data buffer to transmit.
 * @param[in]     - Len: Length of the data to transmit.
 * @param[in]     - SlaveAddr: Address of the slave device.
 * @param[in]     - Sr: Repeated start condition flag.
 *
 * @return        - None
 *
 * @Note          - This is a blocking function and waits until transmission completes.
 *********************************************************************/
void I2C_MasterSendData(I2C_t *pI2CInst, uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
    // 1. Generate the START condition
    if (!I2C_startCond(pI2CInst->pReg))
        I2C_ErrHandler(pI2CInst, I2C_FLG_BUS_ERR);

    // 2. Send the address of the slave with r/w bit set to w(0) (total 8 bits )
    if (!I2C_sendAdrr(pI2CInst->pReg, SlaveAddr, I2C_ACTION_WRITE))
        I2C_ErrHandler(pI2CInst, I2C_FLG_BUS_ERR);

    //3. send the data until len becomes 0
    while (Len > 0)
    {
        I2C_write(pI2CInst->pReg, *pTxbuffer);
        pTxbuffer++;
        Len--;
    }
    
    //4. Generate STOP condition
    if(Sr == I2C_DISABLE_SR )
		I2C_stopCond(pI2CInst->pReg);
}

/*********************************************************************
 * @fn            - I2C_MasterReceiveData
 *
 * @brief         - Receives data at the master from a slave device using I2C.
 *
 * @param[in]     - pI2CInst: Pointer to the I2C instance.
 * @param[in]     - pRxBuffer: Pointer to the data buffer to store received data.
 * @param[in]     - Len: Length of the data to receive.
 * @param[in]     - SlaveAddr: Address of the slave device.
 * @param[in]     - Sr: Repeated start condition flag.
 *
 * @return        - None
 *
 * @Note          - This is a blocking function and waits until reception completes.
 *********************************************************************/
void I2C_MasterReceiveData(I2C_t *pI2CInst, uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr, uint8_t Sr)
{

}

/*********************************************************************
 * @fn            - I2C_MasterSendDataIT
 *
 * @brief         - Sends data from the master to a slave device using I2C in interrupt mode.
 *
 * @param[in]     - pI2CInst: Pointer to the I2C instance.
 * @param[in]     - pTxbuffer: Pointer to the data buffer to transmit.
 * @param[in]     - Len: Length of the data to transmit.
 * @param[in]     - SlaveAddr: Address of the slave device.
 * @param[in]     - Sr: Repeated start condition flag.
 *
 * @return        - Status of the operation: 0 for success, non-zero for failure.
 *
 * @Note          - This function does not block and uses interrupts for data transmission.
 *********************************************************************/
uint8_t I2C_MasterSendDataIT(I2C_t *pI2CInst, uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
    return 0;
}

/*********************************************************************
 * @fn            - I2C_MasterReceiveDataIT
 *
 * @brief         - Receives data at the master from a slave device using I2C in interrupt mode.
 *
 * @param[in]     - pI2CInst: Pointer to the I2C instance.
 * @param[in]     - pRxBuffer: Pointer to the data buffer to store received data.
 * @param[in]     - Len: Length of the data to receive.
 * @param[in]     - SlaveAddr: Address of the slave device.
 * @param[in]     - Sr: Repeated start condition flag.
 *
 * @return        - Status of the operation: 0 for success, non-zero for failure.
 *
 * @Note          - This function does not block and uses interrupts for data reception.
 *********************************************************************/
uint8_t I2C_MasterReceiveDataIT(I2C_t *pI2CInst, uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
    return 0;
}

/*********************************************************************
 * @fn            - I2C_ApplicationEventCallback
 *
 * @brief         - Application callback to handle specific I2C events.
 *
 * @param[in]     - pI2CInst: Pointer to the I2C instance structure.
 * @param[in]     - AppEv: Event code indicating the specific I2C event.
 *
 * @return        - None
 *
 * @Note          - This is a weak function meant to be overridden by
 *                  the application to handle I2C events such as data
 *                  transmission or reception completion.
 *********************************************************************/
__attribute__((weak)) void I2C_ApplicationEventCallback(I2C_t *pI2CInst, uint8_t AppEv) {
    // User should override this function to handle I2C application events.
}

/*********************************************************************
 * @fn            - I2C_ErrHandler
 *
 * @brief         - Handles I2C error conditions.
 *
 * @param[in]     - pI2CInst: Pointer to the I2C instance structure.
 * @param[in]     - Err: Error code indicating the specific I2C error.
 *
 * @return        - None
 *
 * @Note          - This is a weak function meant to be overridden by
 *                  the application to handle errors such as arbitration
 *                  loss, bus errors, or NACK reception.
 *********************************************************************/
__attribute__((weak)) void I2C_ErrHandler(I2C_t *pI2CInst, uint8_t Err) {
    // User should override this function to handle I2C errors.
}

/*
 * MIT License
 *
 * Copyright (c) 2024 HumbertoOntiveros
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Happy coding!
 */