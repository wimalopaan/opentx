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



void backlightInit()
{
    GPIO_InitTypeDef gpio_init;
    gpio_init.GPIO_Mode  = GPIO_Mode_OUT;
    gpio_init.GPIO_OType = GPIO_OType_PP;
    gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
    gpio_init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    gpio_init.GPIO_Pin   = BACKLIGHT_GPIO_PIN;
    GPIO_Init(BACKLIGHT_GPIO, &gpio_init);
}

void backlightEnable()
{
    GPIO_SetBits(BACKLIGHT_GPIO, BACKLIGHT_GPIO_PIN);
}

void backlightDisable()
{
    GPIO_ResetBits(BACKLIGHT_GPIO, BACKLIGHT_GPIO_PIN);
}

uint8_t isBacklightEnabled()
{
	return GPIO_ReadInputDataBit(BACKLIGHT_GPIO, BACKLIGHT_GPIO_PIN) != 0;
}
