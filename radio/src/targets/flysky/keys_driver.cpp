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


static const uint8_t buttonmap[] = {
    TRM_RH_UP, TRM_RH_DWN, TRM_RV_UP,  TRM_RV_DWN,
    TRM_LV_UP, TRM_LV_DWN, TRM_LH_DWN, TRM_LH_UP,
    KEY_DOWN,  KEY_UP,     KEY_ENTER,  KEY_EXIT
};

uint32_t scanMatrix(uint32_t columnStart, uint32_t columnEnd)
{

    uint32_t result = 0;
    /*
    uint32_t idx = columnStart * KEY_MATRIX_LINES;
    for(uint8_t column = columnStart; column <= columnEnd; column++) {
        //activate column
        KEYS_MATRIX_COLUMNS_GPIO->BSRR = columns[column];
        //read lines
        uint16_t lineStates = GPIO_ReadOutputData(KEYS_MATRIX_LINES_GPIO);
        for(uint32_t line = 0; line < KEY_MATRIX_LINES; line++) {
            if((lineStates & lines[line]) != 0){
                //result |= 1 << buttonmap[idx];
            }
            idx++;
        }
        //deactivate
        KEYS_MATRIX_COLUMNS_GPIO->BRR = columns[column];
    }
    */
    return result;
}


uint32_t readKeys()
{
  uint32_t result = scanMatrix(2,2);
  /*
  //bind active low
  if((KEYS_BIND_GPIO->ODR & KEYS_BIND_PIN) == 0) {
      result |= 1 << KEY_BIND;
  }*/
  return result;
}

uint32_t readTrims()
{
  uint32_t result = scanMatrix(0,1);
  // TRACE("readTrims(): result=0x%02x", result);
  return result;
}

uint8_t trimDown(uint8_t idx)
{
  return readTrims() & (1 << idx);
}

uint8_t keyDown()
{
  return readKeys();
}

/* TODO common to ARM */
void readKeysAndTrims()
{
  uint8_t index = 0;
  uint32_t in = readKeys();
  for (uint8_t i = 1; i != uint8_t(1 << TRM_BASE); i <<= 1) {
    keys[index++].input(in & i);
  }

  in = readTrims();
  for (uint8_t i = 1; i != uint8_t(1 << 8); i <<= 1) {
    keys[index++].input(in & i);
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

  // TRACE("switch %d => %d", index, xxx);
  return xxx;
}
#endif

void keysInit()
{
    GPIO_InitTypeDef gpio_init;
    //default state is low
    gpio_init.GPIO_Mode  = GPIO_Mode_IN;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_DOWN;
    gpio_init.GPIO_Pin   = KEYS_LINES_PINS;
    GPIO_Init(KEYS_MATRIX_LINES_GPIO, &gpio_init);
    gpio_init.GPIO_PuPd  = GPIO_PuPd_UP;
    gpio_init.GPIO_Pin   = KEYS_BIND_PIN;
    GPIO_Init(KEYS_BIND_GPIO, &gpio_init);
    gpio_init.GPIO_Mode  = GPIO_Mode_OUT;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_PuPd = GPIO_PuPd_NOPULL;
    gpio_init.GPIO_Pin   = KEYS_COLUMNS_PINS;
    GPIO_Init(KEYS_MATRIX_COLUMNS_GPIO, &gpio_init);


    //reset all columns -> all lines are 0
    KEYS_MATRIX_COLUMNS_GPIO->BRR = KEYS_COLUMNS_PINS;
}
