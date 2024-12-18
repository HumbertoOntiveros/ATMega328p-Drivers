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
 */
static bool I2C_startCond(I2C_Regs_t *pI2Cx)
{
    pI2Cx->TWCR = ((1<<I2C_TWCR_TWINT) | (1<<I2C_TWCR_TWSTA) | (1<<I2C_TWCR_TWEN));

    while(!(pI2Cx->TWCR & (1<<I2C_TWCR_TWINT)));

    if (((pI2Cx->TWSR & 0xF8) == I2C_FLG_START) || ((pI2Cx->TWSR & 0xF8) == I2C_FLG_RSTART))
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
 */
static bool I2C_sendAdrr(I2C_Regs_t *pI2Cx, uint8_t adrr, uint8_t action)
{
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
 */
static bool I2C_write(I2C_Regs_t *pI2Cx, uint8_t data2write)
{
    bool ret = false;
	
    pI2Cx->TWDR = data2write;
    pI2Cx->TWCR = ((1<<I2C_TWCR_TWINT) | (1<<I2C_TWCR_TWEN));
    while(!(pI2Cx->TWCR & (1<<I2C_TWCR_TWINT)));
	
    if ((pI2Cx->TWSR & 0xF8) == I2C_FLG_DATA_ACK)
    	ret = true;
	
    return ret;
}

/*********************************************************************
 * @fn            - I2C_read
 *
 * @brief         - Reads a byte of data from the I2C bus.
 *
 * @param[in]     - pI2Cx: Pointer to the I2C register structure.
 * @param[in]     - ACK_NACK: Indicates whether to send an ACK (1) or NACK (0)
 *                    after receiving the data byte.
 *
 * @return        - The received data byte from the TWDR register.
 *
 * @Note          - This function waits until the TWINT flag is set, 
 *                  indicating that the reception is complete.
 */
static uint8_t I2C_read(I2C_Regs_t *pI2Cx, uint8_t ACK_NACK)
{
	
    pI2Cx->TWCR = ((1 << I2C_TWCR_TWINT) | (1 << I2C_TWCR_TWEN) | (ACK_NACK << I2C_TWCR_TWEA));

    while(!(pI2Cx->TWCR & (1<<I2C_TWCR_TWINT)));
    return pI2Cx->TWDR;
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
 */
static void I2C_stopCond(I2C_Regs_t *pI2Cx)
{

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
 */
void I2C_Init(I2C_t *pI2CInst)
{
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
    pI2CInst->pReg->TWCR = (1 << I2C_TWCR_TWEN);

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
 */
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
 */
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
 */
void I2C_MasterReceiveData(I2C_t *pI2CInst, uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
    // 1. Generate the START condition
    if (!I2C_startCond(pI2CInst->pReg))
        I2C_ErrHandler(pI2CInst, I2C_FLG_BUS_ERR);

    // 2. Send the address of the slave with r/w bit set to w(0) (total 8 bits )
    if (!I2C_sendAdrr(pI2CInst->pReg, SlaveAddr, I2C_ACTION_READ))
        I2C_ErrHandler(pI2CInst, I2C_FLG_BUS_ERR);

    // 3. Receive data
    for (uint8_t i = 0; i < Len; i++) {
        // Set ACK for all bytes except the last one
        uint8_t ack = (i < (Len - 1)) ? 1 : 0;
        
        // Read the data byte and send ACK/NACK
        pRxBuffer[i] = I2C_read(pI2CInst->pReg, ack);
    }

    //4. Generate STOP condition
    if(Sr == I2C_DISABLE_SR )
		I2C_stopCond(pI2CInst->pReg);
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
 * @return        - None
 *
 * @Note          - This function does not block and uses interrupts for data transmission.
 */
void I2C_MasterSendDataIT(I2C_t *pI2CInst, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
    if ((pI2CInst->TxRxState == I2C_READY) && (Len > 0))
    {
        pI2CInst->pTxBuffer = pTxBuffer;
        pI2CInst->TxLen = Len;
        pI2CInst->DevAddr = SlaveAddr;
        pI2CInst->Sr = Sr;

        // Generate START Condition and enable I2C interruptions.
        pI2CInst->pReg->TWCR |= ((1<<I2C_TWCR_TWINT) | (1<<I2C_TWCR_TWSTA) | (1<<I2C_TWCR_TWEN) | (1<<I2C_TWCR_TWIE));

        // Set the I2C state to BUSY in TX mode
        pI2CInst->TxRxState = I2C_BUSY_IN_TX;
    }
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
 * @return        - None
 *
 * @Note          - This function does not block and uses interrupts for data reception.
 */
void I2C_MasterReceiveDataIT(I2C_t *pI2CInst, uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
    if ((pI2CInst->TxRxState == I2C_READY) && (Len > 0))
    {
        pI2CInst->pRxBuffer = pRxBuffer;
        pI2CInst->RxLen = Len;
        pI2CInst->DevAddr = SlaveAddr;
        pI2CInst->Sr = Sr;

        // Implement code to Generate START Condition and enable I2C interruptions.
        pI2CInst->pReg->TWCR |= ((1<<I2C_TWCR_TWINT) | (1<<I2C_TWCR_TWSTA) | (1<<I2C_TWCR_TWEN) | (1<<I2C_TWCR_TWIE));

        // Set the I2C state to BUSY in RX mode
        pI2CInst->TxRxState = I2C_BUSY_IN_RX;
    }
}

/*********************************************************************
 * @fn          - I2C_CloseReceiveData
 *
 * @brief       - Closes the I2C receive operation and resets internal state.
 *
 * @param[in]   - pI2CInst: Pointer to the I2C instance structure.
 *
 * @return      - None
 *
 * @note        - This function disables I2C interrupts, clears the receive buffer,
 *                and resets the acknowledgment bit.
 */
void I2C_CloseReceiveData(I2C_t *pI2CInst)
{
    // Disable I2C interrupts and keeps TWINT set
    pI2CInst->pReg->TWCR &= ~(1 << I2C_TWCR_TWIE) & ~(1 << I2C_TWCR_TWINT);

    // Clean holders for Rx
    pI2CInst->TxRxState = I2C_READY;
    pI2CInst->pRxBuffer = NULL;
    pI2CInst->RxLen = 0;
}

/*********************************************************************
 * @fn          - I2C_CloseSendData
 *
 * @brief       - Closes the I2C send operation and resets internal state.
 *
 * @param[in]   - pI2CInst: Pointer to the I2C instance structure.
 *
 * @return      - None
 *
 * @note        - This function disables I2C interrupts and clears the transmit buffer.
 */
void I2C_CloseSendData(I2C_t *pI2CInst)
{
    // Disable I2C interrupts and keeps TWINT set
    pI2CInst->pReg->TWCR &= ~(1 << I2C_TWCR_TWIE) & ~(1 << I2C_TWCR_TWINT);

    // Clean holders for Tx
    pI2CInst->TxRxState = I2C_READY;
    pI2CInst->pTxBuffer = NULL;
    pI2CInst->TxLen = 0;
}

/*********************************************************************
 * @fn          - I2C_EV_IRQHandling
 *
 * @brief       - Handles I2C event interrupts for various transaction states.
 *
 * @param[in]   - pI2CInst: Pointer to the I2C instance structure.
 *
 * @return      - None
 *
 * @note        - Processes events like Start, SLA+W/R ACK, data sent/received,
 *                and manages state transitions for Tx/Rx operations.
 */
void I2C_IRQHandling(I2C_t *pI2CInst)
{
    uint8_t status = pI2CInst->pReg->TWSR;

    // 1. Handle For interrupt generated by Start OR Respeated Start event
    if ((status == I2C_FLG_START) || (status == I2C_FLG_RSTART))
    {
        // Send SLA + W/R
        uint8_t adrr = (pI2CInst->DevAddr << 1);

        adrr = (pI2CInst->TxRxState == I2C_BUSY_IN_TX) ? (adrr & ~1) : (adrr | 1);

        pI2CInst->pReg->TWDR = adrr;
        pI2CInst->pReg->TWCR &= ~(1<<I2C_TWCR_TWSTA);
        pI2CInst->pReg->TWCR |= ((1<<I2C_TWCR_TWINT) | (1<<I2C_TWCR_TWEN));
    }

    // 2. Handle For interrupt generated by SLA + W event
    else if (status == I2C_FLG_SLA_W_ACK)
    {
        // Send first data in pI2CInst->pTxBuffer
        pI2CInst->pReg->TWDR = *pI2CInst->pTxBuffer;
        pI2CInst->pReg->TWCR |= ((1<<I2C_TWCR_TWINT) | (1<<I2C_TWCR_TWEN));
        pI2CInst->pTxBuffer++;
        pI2CInst->TxLen--;

    }
    
    // 3. Handle For interrup generate by SLA+W, NACK received event
    else if (status == I2C_FLG_SLA_W_NACK)
    {
        I2C_stopCond(pI2CInst->pReg);
        I2C_CloseSendData(pI2CInst);
        I2C_ApplicationEventCallback(pI2CInst, I2C_EV_TX_CMPLT);
    }

    // 4. Handle For interrupt generated by SLA + R event
    else if (status == I2C_FLG_SLA_R_ACK)
    {
        // Configure TWCR: Send ACK or NACK based on RxLen
        pI2CInst->pReg->TWCR = (1 << I2C_TWCR_TWINT) | (1 << I2C_TWCR_TWEN) | (1<<I2C_TWCR_TWIE) |
                           ((pI2CInst->RxLen > 1) ? (1 << I2C_TWCR_TWEA) : 0);
    }

    // 5. Handle For interrup generated by SLA+R, NACK event
    else if (status == I2C_FLG_SLA_R_NACK)
    {
        I2C_stopCond(pI2CInst->pReg);
        I2C_CloseReceiveData(pI2CInst);
        I2C_ApplicationEventCallback(pI2CInst, I2C_EV_RX_CMPLT);
    }

    // 6. Handle For interrupt generated by Data sent, ACK event
    else if (status == I2C_FLG_DATA_ACK)
    {
        if(pI2CInst->TxLen > 0)
        {
            // Send next byte of data in pI2CInst->pTxBuffer
            pI2CInst->pReg->TWDR = *pI2CInst->pTxBuffer;
            pI2CInst->pReg->TWCR |= ((1<<I2C_TWCR_TWINT) | (1<<I2C_TWCR_TWEN));
            pI2CInst->pTxBuffer++;
            pI2CInst->TxLen--;
            return;
        }

        // Transmission complete, send Stop condition if Sr disabled.
        if (!pI2CInst->Sr) I2C_stopCond(pI2CInst->pReg);
        
        // Reset all the member elements of the handle structure.
        I2C_CloseSendData(pI2CInst);

        // Notify the application about transmission complete
        I2C_ApplicationEventCallback(pI2CInst, I2C_EV_TX_CMPLT);
    }

    // 7. Handle For interrupt generated by Data sent, NACK event
    else if (status == I2C_FLG_DATA_NACK)
    {
        if (!pI2CInst->Sr) I2C_stopCond(pI2CInst->pReg);
        I2C_CloseSendData(pI2CInst);
        I2C_ApplicationEventCallback(pI2CInst, I2C_EV_TX_CMPLT);
    }

    // 8. Handle For interrupt generated by Data recv, ACK event
    else if (status == I2C_FLG_DATA_R_ACK)
    {
        // Read date received
        *pI2CInst->pRxBuffer = pI2CInst->pReg->TWDR;
        pI2CInst->pRxBuffer++;
        pI2CInst->RxLen--;

        // Set ACK for all bytes except the last one
        uint8_t ack = (pI2CInst->RxLen > 1) ? 1 : 0;

        // Send ACK for next byte if there is more data
        pI2CInst->pReg->TWCR = ack 
            ? (pI2CInst->pReg->TWCR | (1 << I2C_TWCR_TWEA))  // Set ACK for next byte
            : (pI2CInst->pReg->TWCR & ~(1 << I2C_TWCR_TWEA)); // Clear ACK for last byte
            
        // Clear TWINT flag
        pI2CInst->pReg->TWCR |= ((1<<I2C_TWCR_TWINT) | (1<<I2C_TWCR_TWEN));
    }

    // 9. Handle For interrupt generated by Data recv, NACK event
    else if (status == I2C_FLG_DATA_R_NACK)
    {
        *pI2CInst->pRxBuffer = pI2CInst->pReg->TWDR;

        // Transmission complete, send Stop condition if Sr disabled.
        if (!pI2CInst->Sr) I2C_stopCond(pI2CInst->pReg);
            
        // Reset all the member elements of the handle structure.
        I2C_CloseReceiveData(pI2CInst);

        // Notify the application about reception complete
        I2C_ApplicationEventCallback(pI2CInst, I2C_EV_RX_CMPLT);
    }
    
    // 10. Handle For interrupt generated by Illegal START/STOP event
    else if(status == I2C_FLG_BUS_ERR)
    {
        I2C_stopCond(pI2CInst->pReg);
        I2C_ApplicationEventCallback(pI2CInst, I2C_ERROR_BERR);
    }
}

/*********************************************************************
 * @fn            - I2C_PeripheralControl
 *
 * @brief         - Enables or disables the I2C peripheral.
 *
 * @param[in]     - pI2CRegs: Pointer to the I2C register structure.
 * @param[in]     - EnOrDi: Enable or Disable control:
 *                           - Pass 1 to enable the I2C peripheral.
 *                           - Pass 0 to disable the I2C peripheral.
 *
 * @return        - None
 *
 * @Note          - Modifies the TWEN (Two-wire Enable) bit in the TWCR (Two-wire Control Register).
 */
void I2C_PeripheralControl(I2C_Regs_t *pI2CRegs, uint8_t EnOrDi)
{
    if(EnOrDi)
    {
        pI2CRegs->TWCR |= (1<<I2C_TWCR_TWEN); 
    }else
    {
        pI2CRegs->TWCR &= ~(1<<I2C_TWCR_TWEN); 
    }
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
 */
__attribute__((weak)) void I2C_ApplicationEventCallback(I2C_t *pI2CInst, uint8_t AppEv) 
{

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
 */
__attribute__((weak)) void I2C_ErrHandler(I2C_t *pI2CInst, uint8_t Err) 
{
    // User should override this function to handle I2C errors.
    while (1);    
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