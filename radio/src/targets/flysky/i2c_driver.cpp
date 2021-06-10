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

#include "opentx.h"

#define I2C_TIMEOUT_MAX 1000

/**
  * @brief reads a certain byte of data from a certain starting address of a device on the I2C2 bus into the array
     * @param driver_Addr: I2C device address
     * @param start_Addr: start byte address
     * @param number_Bytes: the number of bytes to be read (less than one page)
     * @param read_Buffer: Array pointer to store read data
     * @retval read successfully
  */
uint8_t I2C2_Read_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *read_Buffer)
{
  uint8_t read_Num;
  uint8_t I2C_Timeout;

  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) == SET){
    TRACE("Wait busy set");
  }

  I2C_TransferHandling(I2C2, driver_Addr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write); //I2C_No_StartStop I2C_Generate_Start_Write
  I2C_SendData(I2C2, start_Addr);
  I2C_TransferHandling(I2C2, driver_Addr, number_Bytes, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

  for (read_Num = 0; read_Num < number_Bytes; read_Num++)
  {
    read_Buffer[read_Num] = I2C_ReceiveData(I2C2);
  }
  while (I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF) == RESET)
    ;
  return 0;
}

uint8_t I2C2_Write_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *write_Buffer)
{
  uint8_t write_Num;
  uint8_t I2C_Timeout;
  
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) == SET){
    TRACE("Wait busy set");
  }
  I2C_TransferHandling(I2C2, driver_Addr, number_Bytes + 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);
  
  I2C_Timeout = 100000;
  while (I2C_GetFlagStatus(I2C2, I2C_FLAG_TXIS) == RESET)
  {
    if ((I2C_Timeout--) == 0)
    {
      TRACE("Timeout before write");
      return 1;
    }
  }

  I2C_SendData(I2C2, start_Addr);
  //while(I2C_GetFlagStatus(I2C2, I2C_FLAG_TXIS) == RESET);

  //I2C_TransferHandling(I2C2, driver_Addr, number_Bytes, I2C_AutoEnd_Mode, I2C_No_StartStop);
  for (write_Num = 0; write_Num < number_Bytes; write_Num++)
  {
    I2C_SendData(I2C2, write_Buffer[write_Num]);
  }
  I2C_Timeout = 100000;
  while (I2C_GetFlagStatus(I2C2, I2C_FLAG_TXIS) == RESET)
  {
    if ((I2C_Timeout--) == 0)
    {
      TRACE("Timeout after write");
      return 1;
    }
  }
  return 0;
}

void eepromReadBlock(uint8_t *buffer, size_t address, size_t size)
{
  I2C2_Read_NBytes(I2C_ADDRESS_EEPROM, address, size, buffer);
}

void eepromPageWrite(uint8_t *pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
{
  I2C2_Write_NBytes(I2C_ADDRESS_EEPROM, WriteAddr, NumByteToWrite, pBuffer);
}

uint8_t eepromIsTransferComplete()
{
  return 1;
}

/**
  * @brief  Wait for EEPROM Standby state
  * @param  None
  * @retval None
  */
bool I2C_EE_WaitEepromStandbyState(void)
{
  return true;
}

void eepromWaitEepromStandbyState(void)
{
  while (!I2C_EE_WaitEepromStandbyState())
  {
    eepromInit();
  }
}
uint8_t eepromReadStatus()
{
  return 1;
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  buffer : pointer to the buffer containing the data to be
  *   written to the EEPROM.
  * @param  address : EEPROM's internal address to write to.
  * @param  size : number of bytes to write to the EEPROM.
  * @retval None
  */
void eepromWriteBlock(uint8_t *buffer, size_t address, size_t size)
{
  uint8_t offset = address % EEPROM_PAGE_SIZE;
  uint8_t count = EEPROM_PAGE_SIZE - offset;
  if (size < count)
  {
    count = size;
  }
  while (count > 0)
  {
    eepromPageWrite(buffer, address, count);
    eepromWaitEepromStandbyState();
    address += count;
    buffer += count;
    size -= count;
    count = EEPROM_PAGE_SIZE;
    if (size < EEPROM_PAGE_SIZE)
    {
      count = size;
    }
  }
}

void eepromStartRead(uint8_t *buffer, size_t address, size_t size)
{
  eepromReadBlock(buffer, address, size);
}

void eepromStartWrite(uint8_t *buffer, size_t address, size_t size)
{
  eepromWriteBlock(buffer, address, size);
}

void eepromBlockErase(uint32_t address)
{
}

// Resolve clash with libopencm3 defines

#undef FLASH_BASE
#undef PERIPH_BASE
#undef I2C2_BASE
#undef I2C2_BASE
#undef I2C2
#undef I2C2
#undef I2C_CR1_PECEN
#undef I2C_CR1_ALERTEN
#undef I2C_CR1_SMBDEN
#undef I2C_CR1_SMBHEN
#undef I2C_CR1_GCEN
#undef I2C_CR1_WUPEN
#undef I2C_CR1_NOSTRETCH
#undef I2C_CR1_SBC
#undef I2C_CR1_RXDMAEN
#undef I2C_CR1_TXDMAEN
#undef I2C_CR1_ANFOFF
#undef I2C_CR1_ERRIE
#undef I2C_CR1_TCIE
#undef I2C_CR1_STOPIE
#undef I2C_CR1_NACKIE
#undef I2C_CR1_ADDRIE
#undef I2C_CR1_RXIE
#undef I2C_CR1_TXIE
#undef I2C_CR1_PE
#undef I2C_CR2_PECBYTE
#undef I2C_CR2_AUTOEND
#undef I2C_CR2_RELOAD
#undef I2C_CR2_NACK
#undef I2C_CR2_STOP
#undef I2C_CR2_START
#undef I2C_CR2_HEAD10R
#undef I2C_CR2_ADD10
#undef I2C_CR2_RD_WRN
#undef I2C_ISR_BUSY
#undef I2C_ISR_ALERT
#undef I2C_ISR_TIMEOUT
#undef I2C_ISR_PECERR
#undef I2C_ISR_OVR
#undef I2C_ISR_ARLO
#undef I2C_ISR_BERR
#undef I2C_ISR_TCR
#undef I2C_ISR_TC
#undef I2C_ISR_STOPF
#undef I2C_ISR_NACKF
#undef I2C_ISR_ADDR
#undef I2C_ISR_RXNE
#undef I2C_ISR_TXIS
#undef I2C_ISR_TXE
#undef I2C_ICR_ALERTCF
#undef I2C_ICR_TIMOUTCF
#undef I2C_ICR_PECCF
#undef I2C_ICR_OVRCF
#undef I2C_ICR_ARLOCF
#undef I2C_ICR_BERRCF
#undef I2C_ICR_STOPCF
#undef I2C_ICR_NACKCF
#undef I2C_ICR_ADDRCF
#undef I2C1_BASE
#undef I2C1

#include "i2c_common_v2.h"

#include <stdio.h>
#include <string.h>

void eepromInit()
{

  uint32_t i2c;

  i2c = I2C2;

  TRACE("eepromInit");
  i2c_reset(i2c);

  /* Disable the I2C before changing any configuration. */
  i2c_peripheral_disable(i2c);

  /* setup from libopencm3-examples */
  i2c_enable_analog_filter(i2c);
  i2c_set_digital_filter(i2c, 0);
  i2c_set_speed(i2c, i2c_speed_sm_100k, 8); // i2c_speed_fm_400k  i2c_speed_sm_100k
  i2c_enable_stretching(i2c);
  i2c_set_7bit_addr_mode(i2c);

  /* If everything is configured -> enable the peripheral. */
  i2c_peripheral_enable(i2c);
  TRACE("done");
}