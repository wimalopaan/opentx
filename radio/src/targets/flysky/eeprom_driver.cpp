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


uint8_t eepromIsTransferComplete()
{
  return 1;
}

uint32_t eepromTransmitData(uint8_t *command, uint8_t *tx, uint8_t *rx, uint32_t comlen, uint32_t count)
{

  return 0;
}

uint8_t eepromTransmitByte(uint8_t out, bool skipFirst)
{
  return 0;
}

enum EepromCommand {
  COMMAND_WRITE_STATUS_REGISTER = 0x01,
  COMMAND_BYTE_PROGRAM = 0x02,
  COMMAND_READ_ARRAY = 0x03,
  COMMAND_READ_STATUS = 0x05,
  COMMAND_WRITE_ENABLE = 0x06,
  COMMAND_BLOCK_ERASE = 0x20,
};

void eepromPrepareCommand(EepromCommand command, uint32_t address)
{
}

uint8_t eepromReadStatus()
{
  return 0;
}

void eepromWriteStatusRegister()
{
 
}

void eepromWriteEnable()
{

}

void eepromStartWrite(uint8_t * buffer, size_t address, size_t size)
{
  
}

void eepromStartRead(uint8_t * buffer, size_t address, size_t size)
{
  
}

void eepromBlockErase(uint32_t address)
{
  
}

// SPI i/f to EEPROM (4Mb)
// Peripheral ID 21 (0x00200000)
// Connections:
// SS   PA11 (peripheral A)
// MISO PA12 (peripheral A)
// MOSI PA13 (peripheral A)
// SCK  PA14 (peripheral A)
// Set clock to 3 MHz, AT25 device is rated to 70MHz, 18MHz would be better
void eepromInit()
{
  
}

extern "C" void SPI_IRQHandler()
{
  
}
