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

}

#define I2C_TIMEOUT_MAX 1000
bool I2C_WaitEvent(uint32_t event)
{

  return true;
}

bool I2C_WaitEventCleared(uint32_t event)
{

  return true;
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

}
//defined in eeprom_driver
// uint8_t eepromIsTransferComplete()
// {
  // return 1;
// }

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

}

#if !defined(BOOT) && !defined(SOFTWARE_VOLUME)
const uint8_t volumeScale[VOLUME_LEVEL_MAX+1] = {
  0,  1,  2,  3,  5,  9,  13,  17,  22,  27,  33,  40,
  64, 82, 96, 105, 112, 117, 120, 122, 124, 125, 126, 127
};

void setScaledVolume(uint8_t volume)
{

}

void setVolume(uint8_t volume)
{

}

int32_t getVolume()
{
  return -1; // TODO
}
#endif
