/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

 #include "board.h"

 void eepromPageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite);
 void eepromWaitEepromStandbyState(void);

 void i2cInit()
 {
   I2C_DeInit(I2C);

   GPIO_InitTypeDef gpio_init;

   I2C_InitTypeDef i2c_init;
   //rise time 100ns fall 10ns 100khz standard mode
   i2c_init.I2C_Timing = 0x10805E89;
   i2c_init.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
   i2c_init.I2C_DigitalFilter = 0;
   i2c_init.I2C_OwnAddress1 = 0x00;
   i2c_init.I2C_Mode = I2C_Mode_I2C;
   i2c_init.I2C_Ack = I2C_Ack_Enable;
   i2c_init.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

   I2C_Init(I2C, &i2c_init);
   I2C_Cmd(I2C, ENABLE);

   GPIO_PinAFConfig(I2C_GPIO, I2C_SCL_GPIO_PinSource, I2C_GPIO_AF);
   GPIO_PinAFConfig(I2C_GPIO, I2C_SDA_GPIO_PinSource, I2C_GPIO_AF);

   gpio_init.GPIO_Pin = I2C_SCL_GPIO_PIN | I2C_SDA_GPIO_PIN;
   gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
   //gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
   gpio_init.GPIO_Mode = GPIO_Mode_AF;
   gpio_init.GPIO_OType = GPIO_OType_OD;
   gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(I2C_GPIO, &gpio_init);

 }


 /**
   * @brief  Reads a block of data from the EEPROM.
   * @param  pBuffer : pointer to the buffer that receives the data read
   *   from the EEPROM.
   * @param  ReadAddr : EEPROM's internal address to read from.
   * @param  NumByteToRead : number of bytes to read from the EEPROM.
   * @retval None
   */
 bool I2C_EE_ReadBlock(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
 {
     return true;
 }

 void eepromReadBlock(uint8_t * buffer, size_t address, size_t size)
 {
   while (!I2C_EE_ReadBlock(buffer, address, size)) {
     i2cInit();
   }
 }

 /**
   * @brief  Writes buffer of data to the I2C EEPROM.
   * @param  buffer : pointer to the buffer containing the data to be
   *   written to the EEPROM.
   * @param  address : EEPROM's internal address to write to.
   * @param  size : number of bytes to write to the EEPROM.
   * @retval None
   */
 void eepromWriteBlock(uint8_t * buffer, size_t address, size_t size)
 {
   uint8_t offset = address % EEPROM_PAGE_SIZE;
   uint8_t count = EEPROM_PAGE_SIZE - offset;
   if (size < count) {
     count = size;
   }
   while (count > 0) {
     eepromPageWrite(buffer, address, count);
     eepromWaitEepromStandbyState();
     address += count;
     buffer += count;
     size -= count;
     count = EEPROM_PAGE_SIZE;
     if (size < EEPROM_PAGE_SIZE) {
       count = size;
     }
   }
 }

 uint8_t eepromIsTransferComplete()
 {
   return 1;
 }

 /**
   * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
   * @note   The number of byte can't exceed the EEPROM page size.
   * @param  pBuffer : pointer to the buffer containing the data to be
   *   written to the EEPROM.
   * @param  WriteAddr : EEPROM's internal address to write to.
   * @param  NumByteToWrite : number of bytes to write to the EEPROM.
   * @retval None
   */
 bool I2C_EE_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
 {
     return true;
 }

 void eepromPageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
 {
   while (!I2C_EE_PageWrite(pBuffer, WriteAddr, NumByteToWrite)) {
     i2cInit();
   }
 }

 /**
   * @brief  Wait for EEPROM Standby state
   * @param  None
   * @retval None
   */
 bool I2C_EE_WaitEepromStandbyState(void)
 {
     /*
   do {
     I2C_GenerateSTART(I2C, ENABLE);
     if (!I2C_WaitEvent(I2C_EVENT_MASTER_MODE_SELECT))
       return false;

     I2C_Send7bitAddress(I2C, I2C_ADDRESS_EEPROM, I2C_Direction_Transmitter);
   } while (!I2C_WaitEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

   I2C_GenerateSTOP(I2C, ENABLE);
   */
   return true;
 }

 void eepromWaitEepromStandbyState(void)
 {
   while (!I2C_EE_WaitEepromStandbyState()) {
     i2cInit();
   }
 }
