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

// Resolve clash with libopencm3 defines

#undef FLASH_BASE
#undef PERIPH_BASE
#undef I2C1_BASE
#undef I2C2_BASE
#undef I2C1
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

#include "i2c_common_v2.h"

#include <stdio.h>
#include <string.h>

void eepromInit()
{

  uint32_t i2c;

  i2c = I2C2;
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
}

void eepromPageRead(uint8_t *buffer, size_t address, size_t size)
{
  uint8_t wb[2];
  wb[0] = (uint8_t)(address >> 8);
  wb[1] = (uint8_t)(address & 0xFF);
  // uint8_t start_page = address / 64;
  // uint8_t end_page = (address + size -1) / 64;
  // TRACE("eepromPageRead addr %d size %d [from %d to %d]", address, size, start_page, end_page);
  i2c_transfer7(I2C2, I2C_ADDRESS_EEPROM, wb, 2, buffer, size);
  //delay_ms(1);
  RTOS_WAIT_MS(1);
  //DUMP(buffer, size);
}

void eepromPageWrite(uint8_t *buffer, uint16_t address, uint8_t size)
{
  static uint8_t temp[2 + EEPROM_PAGE_SIZE];
  temp[0] = (uint8_t)(address >> 8);
  temp[1] = (uint8_t)(address & 0xFF);
  // uint8_t start_page = address / 64;
  // uint8_t end_page = (address + size - 1) / 64;
  // TRACE("eepromPageWrite addr %d size %d [start page %d end page %d]", address, size, start_page, end_page);
  memcpy(temp + 2, buffer, size);
  //DUMP(temp, size + 2);
  i2c_transfer7(I2C2, I2C_ADDRESS_EEPROM, temp, size + 2, NULL, 0);
  //delay_ms(5);
  RTOS_WAIT_MS(5);

#if defined(EEPROM_VERIFY_WRITES)
  eepromPageRead(temp, address, size);
  for (int i = 0; i < size; i++)
  {
    if (temp[i] != buffer[i])
    {
      TRACE("--------- eeprom verify failed  ----------");
      while (1)
        ;
    }
  }

#endif
}

void eepromReadBlock(uint8_t *buffer, size_t address, size_t size)
{
  //TRACE("eepromStartRead addr %d %d bytes", address, size);
  // first segment, until page limit
  uint8_t offset = address % EEPROM_PAGE_SIZE;
  uint8_t count = EEPROM_PAGE_SIZE - offset;
  if (size < count)
  {
    count = size;
  }
  eepromPageRead(buffer, address, count);
  if (size > count)
  {
    // second segment, entire pages in between
    uint16_t remaining = (size - count) % EEPROM_PAGE_SIZE;
    uint8_t full_pages = (size - count) / EEPROM_PAGE_SIZE;
    uint8_t i;
    for (i = 0; i < full_pages; i++)
    {
      eepromPageRead(
        buffer + count + (i * EEPROM_PAGE_SIZE), 
        address + count + (i * EEPROM_PAGE_SIZE), 
        EEPROM_PAGE_SIZE);
    }
    if (remaining)
    {
      eepromPageRead(
        buffer + count + (full_pages * EEPROM_PAGE_SIZE), 
        address + count + (full_pages * EEPROM_PAGE_SIZE), 
        remaining);
    }
  }

  //DUMP(buffer, size);
}

void eepromWriteBlock(uint8_t * buffer, size_t address, size_t size)
{
  //TRACE("eepromStartWrite addr %d %d bytes", address, size);
  // first segment, until page limit
  uint8_t offset = address % EEPROM_PAGE_SIZE;
  uint8_t count = EEPROM_PAGE_SIZE - offset;
  if (size < count)
  {
    count = size;
  }
  eepromPageWrite(buffer, address, count);
  if (size > count)
  {
    // second segment, entire pages in between
    uint16_t remaining = (size - count) % EEPROM_PAGE_SIZE;
    uint8_t full_pages = (size - count) / EEPROM_PAGE_SIZE;
    uint8_t i;
    for (i = 0; i < full_pages; i++)
    {
      eepromPageWrite(
        buffer + count + (i * EEPROM_PAGE_SIZE),
        address + count + (i * EEPROM_PAGE_SIZE),
        EEPROM_PAGE_SIZE);
    }

    // third segment, remainder
    if (remaining)
    {
      eepromPageWrite(
        buffer + count + (full_pages * EEPROM_PAGE_SIZE), 
        address + count + (full_pages * EEPROM_PAGE_SIZE), 
        remaining);
    }
  }
}

void i2c_test()
{
  static uint8_t temp[128];
  uint8_t i;
  for (i = 0; i < 128; i++)
  {
    temp[i] = i;
  }
  DUMP(temp, 128);
  eepromWriteBlock(temp, 10, 128);
  memset(temp, 0, 128);
  eepromReadBlock(temp, 10, 128);
}

void eepromBlockErase(uint32_t address)
{
  // TRACE("eepromBlockErase");
  // static uint8_t erasedBlock[EEPROM_BLOCK_SIZE]; // can't be on the stack!
  // memset(erasedBlock, 0xFF, sizeof(erasedBlock));
  // eepromStartWrite(erasedBlock, address, EEPROM_BLOCK_SIZE);
  // TRACE("done");
}
uint8_t eepromReadStatus()
{
  return 1;
}
uint8_t eepromIsTransferComplete()
{
  return 1;
}
