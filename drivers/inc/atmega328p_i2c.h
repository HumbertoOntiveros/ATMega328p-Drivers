/*
 * atmega328p_i2c.c
 *
 * Created: 19/11/2024 14:22:47
 * Author : JESUS HUMBERTO ONTIVEROS MAYORQUIN
 *
 * Description:
 * Briefly describe the driver's functionality, key configurations,
 * and expected outcomes for user clarity.
 *
 */

#ifndef __ATMEGA328P_I2C_H__
#define __ATMEGA328P_I2C_H__

#include "atmega328p.h"

/******************************************************************************************
 *                                  Driver's Specific Details                                  *
 ******************************************************************************************/
/*
 * Configuration structure for I2C peripheral
 */
typedef struct
{
    uint32_t SCLSpeed;
    uint8_t DeviceAddress;
    uint8_t Mode;
}I2C_Config_t;

/*
 * Handle structure for a I2C peripheral
 */
typedef struct
{
    I2C_Regs_t    *pReg;
    I2C_Config_t  Config;
    uint8_t       *pTxBuffer;/* !< To store the app. Tx buffer address > */
    uint8_t       *pRxBuffer;/* !< To store the app. Rx buffer address > */
    uint32_t      TxLen;/* !< To store Tx len > */
    uint32_t      RxLen;/* !< To store Tx len > */
    uint8_t       TxRxState;/* !< To store Communication state > */
    uint8_t       DevAddr;/* !< To store slave/device address > */
    uint32_t      RxSize;/* !< To store Rx size  > */
    uint8_t       Sr;/* !< To store repeated start value  > */
}I2C_t;

/*
 * I2C Modes
 */
#define I2C_MODE_MASTER     0
#define I2C_MODE_SLAVE      1

/*
 * I2C application states
 */
#define I2C_READY 					0
#define I2C_BUSY_IN_RX 				1
#define I2C_BUSY_IN_TX 				2

/*
 * I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_50k   50000UL 	
#define I2C_SCL_SPEED_80k   80000UL	
#define I2C_SCL_SPEED_100k  100000UL	
#define I2C_SCL_SPEED_200k  200000UL
#define I2C_SCL_SPEED_300k  300000UL  
#define I2C_SCL_SPEED_400k  400000UL  

/*
 * I2C action definitions
 * Used to specify the operation type (read/write) in I2C communication.
 */
#define I2C_ACTION_WRITE 0 /* Write operation */
#define I2C_ACTION_READ  1 /* Read operation */

/*
 * I2C related status flags definitions
 */

// Master Transmitter
#define I2C_FLG_START         0x08  // START sent
#define I2C_FLG_RSTART        0x10  // Repeated START sent
#define I2C_FLG_SLA_W_ACK     0x18  // SLA+W, ACK
#define I2C_FLG_SLA_W_NACK    0x20  // SLA+W, NACK
#define I2C_FLG_DATA_ACK      0x28  // Data sent, ACK
#define I2C_FLG_DATA_NACK     0x30  // Data sent, NACK
#define I2C_FLG_ARB_LOST      0x38  // Arbitration lost

// Master Receiver
#define I2C_FLG_SLA_R_ACK     0x40  // SLA+R, ACK
#define I2C_FLG_SLA_R_NACK    0x48  // SLA+R, NACK
#define I2C_FLG_DATA_R_ACK    0x50  // Data recv, ACK
#define I2C_FLG_DATA_R_NACK   0x58  // Data recv, NACK

// Slave Receiver
#define I2C_FLG_SLA_W_RCV     0x60  // SLA+W recv, ACK
#define I2C_FLG_ARB_LOST_W    0x68  // Arb lost, SLA+W
#define I2C_FLG_GEN_CALL      0x70  // Gen call recv, ACK
#define I2C_FLG_ARB_LOST_GC   0x78  // Arb lost, Gen call
#define I2C_FLG_DATA_W_ACK    0x80  // Data recv, ACK (SLA+W)
#define I2C_FLG_DATA_W_NACK   0x88  // Data recv, NACK (SLA+W)
#define I2C_FLG_DATA_GC_ACK   0x90  // Data recv, ACK (GC)
#define I2C_FLG_DATA_GC_NACK  0x98  // Data recv, NACK (GC)
#define I2C_FLG_STOP_RSTART   0xA0  // STOP or RSTART recv

// Slave Transmitter
#define I2C_FLG_SLA_R_RCV     0xA8  // SLA+R recv, ACK
#define I2C_FLG_ARB_LOST_R    0xB0  // Arb lost, SLA+R
#define I2C_FLG_DATA_T_ACK    0xB8  // Data sent, ACK
#define I2C_FLG_DATA_T_NACK   0xC0  // Data sent, NACK
#define I2C_FLG_LAST_ACK      0xC8  // Last data sent, ACK

// Miscellaneous
#define I2C_FLG_NO_INFO       0xF8  // No relevant state
#define I2C_FLG_BUS_ERR       0x00  // Illegal START/STOP


#define I2C_DISABLE_SR  	RESET
#define I2C_ENABLE_SR   	SET


/*
 * I2C application events macros
 */
#define I2C_EV_TX_CMPLT  	 	0
#define I2C_EV_RX_CMPLT  	 	1
#define I2C_EV_STOP       		2
#define I2C_ERROR_BERR 	 		3
#define I2C_ERROR_ARLO  		4
#define I2C_ERROR_AF    		5
#define I2C_ERROR_OVR   		6
#define I2C_ERROR_TIMEOUT 		7
#define I2C_EV_DATA_REQ         8
#define I2C_EV_DATA_RCV         9

/******************************************************************************************
 *                            APIs supported by this driver                               *
 *             For more information about the APIs check the function definitions         *
 ******************************************************************************************/
/*
 * Init and De-init
 */
void I2C_Init(I2C_t *pI2CInst);
void I2C_DeInit(I2C_t *pI2CInst);


/*
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_t *pI2CInst, uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
void I2C_MasterReceiveData(I2C_t *pI2CInst, uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterSendDataIT(I2C_t *pI2CInst,uint8_t *pTxbuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr);
uint8_t I2C_MasterReceiveDataIT(I2C_t *pI2CInst, uint8_t *pRxBuffer, uint8_t Len, uint8_t SlaveAddr,uint8_t Sr);

void I2C_CloseReceiveData(I2C_t *pI2CInst);
void I2C_CloseSendData(I2C_t *pI2CInst);


void I2C_SlaveSendData(I2C_Regs_t *pI2CRegs,uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_Regs_t *pI2CRegs);

/*
 * IRQ Configuration and ISR handling
 */
void I2C_IRQInterruptConfig(I2C_t *pI2CInst, uint8_t EnorDi);
void I2C_EV_IRQHandling(I2C_t *pI2CInst);
void I2C_ER_IRQHandling(I2C_t *pI2CInst);

/*
 * Other Peripheral Control APIs
 */
void I2C_PeripheralControl(I2C_Regs_t *pI2CRegs, uint8_t EnOrDi);
void I2C_GenerateStopCondition(I2C_Regs_t *pI2CRegs);
void I2C_SlaveEnableDisableCallbackEvents(I2C_Regs_t *pI2CRegs, uint8_t EnorDi);

/*
 * Calculate the value for the TWBR register using the I2C_SET_CLOCK macro.
 * The macro computes the correct value for TWBR based on the desired SCL
 * clock speed (ClockSpeed) and the system clock frequency (F_CPU).
 * Assumes a prescaler value of 1 (TWPS = 0).
 */
#define I2C_SET_CLOCK(SCL_FREQUENCY, F_CPU) \
    (((F_CPU) / (16 + (2 * (SCL_FREQUENCY)))) - 8) / 2

/*
 * Application callback
 */
void I2C_ApplicationEventCallback(I2C_t *pI2CInst,uint8_t AppEv);

/*
 * I2C Error Handler
 */
void I2C_ErrHandler(I2C_t *pI2CInst, uint8_t Err);

#endif // __ATMEGA328P_I2C_H__

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