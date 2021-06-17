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

#define KEY_MATRIX_LINES 4
#define KEY_MATRIX_COLUMNS 3
static const uint16_t columns[] = {KEYS_MATRIX_R1_PIN, KEYS_MATRIX_R2_PIN, KEYS_MATRIX_R3_PIN};
static const uint16_t lines[] = {KEYS_MATRIX_L1_PIN, KEYS_MATRIX_L2_PIN, KEYS_MATRIX_L3_PIN, KEYS_MATRIX_L4_PIN};
/*
  KEY_RIGHT,
  KEY_LEFT,
*/

/*
      R1	    R2	        R3
  L1	Roll R	Throttle U	Down
  L2	Roll L	Throttle D	Up
  L3	Pitch U	Yaw R	    OK
  L4	Pitch D	Yaw L	    Cancel*/

const uint8_t keysMap[] = {
    TRM_RH_UP, TRM_RH_DWN, TRM_RV_UP, TRM_RV_DWN,
    TRM_LV_UP, TRM_LV_DWN, TRM_LH_UP, TRM_LH_DWN,
    KEY_DOWN, KEY_UP, KEY_ENTER, KEY_EXIT};

uint32_t scanMatrix(uint32_t columnStart, uint32_t columnEnd)
{
  uint32_t result = 0;
  uint8_t column = 0;
  uint8_t line = 0;
  uint16_t index = columnStart * KEY_MATRIX_LINES;
  for (column = columnStart; column <= columnEnd; column++)
  {
    //set to low
    KEYS_MATRIX_COLUMNS_GPIO->BRR = columns[column];
    //read lines
    for (line = 0; line < KEY_MATRIX_LINES; line++)
    {
      if ((KEYS_MATRIX_LINES_GPIO->IDR & lines[line]) == 0)
      {
        result |= (1 << ((uint32_t)keysMap[index]));
      }
      index++;
    }
    //set to hight
    KEYS_MATRIX_COLUMNS_GPIO->BSRR = columns[column];
  }
  return result;
}

uint32_t readKeys()
{
  uint32_t result = scanMatrix(2, 2);
  //bind active low
  if ((KEYS_BIND_GPIO->IDR & KEYS_BIND_PIN) == 0)
  {
    if (!result)
    {
      result |= 1 << KEY_BIND;
    }
    else
    {
      //bind as shift
      if (result & (1 << KEY_DOWN))
      {
        result &= ~(1 << KEY_DOWN);
        result |= 1 << KEY_LEFT;
      }
      if (result & (1 << KEY_UP))
      {
        result &= ~(1 << KEY_UP);
        result |= 1 << KEY_RIGHT;
      }
    }
  }
  return result;
}

uint32_t readTrims()
{
  return scanMatrix(0, 1);
}

uint8_t trimDown(uint8_t idx)
{
  return readTrims() & (1 << idx);
}

bool keyDown()
{
  return readKeys() || readTrims();
}

/* TODO common to ARM */
void readKeysAndTrims()
{
  uint8_t index = 0;
  uint32_t keys_in = readKeys();
  for (uint8_t i = 1; i != uint8_t(1 << TRM_BASE); i <<= 1)
  {
    keys[index++].input(keys_in & i);
  }

  uint32_t trims_in = readTrims();
  for (uint8_t i = 1; i != uint8_t(1 << 8); i <<= 1)
  {
    keys[index++].input(trims_in & i);
  }
if ((keys_in || trims_in))
  {
    if (trims_in & (1 << TRM_RH_UP))
    {
      TRACE("TRM_RH_UP");
    }
    if (trims_in & (1 << TRM_RH_DWN))
    {
      TRACE("TRM_RH_DWN");
    }
    if (trims_in & (1 << TRM_RV_UP))
    {
      TRACE("TRM_RV_UP");
    }
    if (trims_in & (1 << TRM_RV_DWN))
    {
      TRACE("TRM_RV_DWN");
    }
    if (trims_in & (1 << TRM_LV_UP))
    {
      TRACE("TRM_LV_UP");
    }
    if (trims_in & (1 << TRM_LV_DWN))
    {
      TRACE("TRM_LV_DWN");
    }
    if (trims_in & (1 << TRM_LH_UP))
    {
      TRACE("TRM_LH_UP");
    }
    if (trims_in & (1 << TRM_LH_DWN))
    {
      TRACE("TRM_LH_DWN");
    }
    if (trims_in & (1 << KEY_DOWN))
    {
      TRACE("KEY_DOWN");
    }
    if ((keys_in) & (1 << KEY_UP))
    {
      TRACE("KEY_UP");
    }
    if ((keys_in) & (1 << KEY_ENTER))
    {
      TRACE("KEY_ENTER");
    }
    if ((keys_in) & (1 << KEY_EXIT))
    {
      TRACE("KEY_EXIT");
    }
    if ((keys_in) & (1 << KEY_LEFT))
    {
      TRACE("KEY_LEFT");
    }
    if ((keys_in) & (1 << KEY_RIGHT))
    {
      TRACE("KEY_RIGHT");
    }
  }
  if ((keys_in || trims_in) && (g_eeGeneral.backlightMode & e_backlight_mode_keys))
  {
    // on keypress turn the light on
    backlightOn();
  }
}

uint8_t keyState(uint8_t index)
{
  return keys[index].state();
}

#if !defined(BOOT)
uint32_t switchState(uint8_t index)
{
  uint32_t xxx = 0;
  uint8_t pos = index % 3;    // 0, 1, 2
  uint8_t sw_num = index / 3; // 0, 1, 2, 3
  uint8_t adc_num = sw_num + 4;
  if (sw_num > 1)
  {
    adc_num += 2; // skip the 2 pots
  }
  uint16_t value = adcValues[adc_num];
  // min is 0, max from adc is 4095
  if ((value < 1365) && (pos == 0))
  {
    xxx = 1 << index;
  }
  else if ((value > 1365) && (value < 2730) && (pos == 1))
  {
    xxx = 1 << index;
  }
  else if ((value > 2730) && (pos == 2))
  {
    xxx = 1 << index;
  }
  //TRACE("switch idx %d sw_num %d value %d pos %d xxx %d", index, sw_num, value, pos, xxx);
  return xxx;
}
#endif

void keysInit()
{
  RCC_AHBPeriphClockCmd(KEYS_RCC_AHB1Periph, ENABLE);
  GPIO_InitTypeDef gpio_init;
  //default state is low
  gpio_init.GPIO_Mode = GPIO_Mode_IN;
  gpio_init.GPIO_OType = GPIO_OType_PP;
  gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
  gpio_init.GPIO_PuPd = GPIO_PuPd_UP;
  gpio_init.GPIO_Pin = KEYS_LINES_PINS;
  GPIO_Init(KEYS_MATRIX_LINES_GPIO, &gpio_init);
  gpio_init.GPIO_Mode = GPIO_Mode_OUT;
  gpio_init.GPIO_Pin = KEYS_COLUMNS_PINS;
  GPIO_Init(KEYS_MATRIX_COLUMNS_GPIO, &gpio_init);
  //set to height
  KEYS_MATRIX_COLUMNS_GPIO->BSRR = KEYS_COLUMNS_PINS;
}
