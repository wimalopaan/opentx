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
#include "board.h"

volatile uint8_t buzzerCount;

void setVolume(uint8_t volume)
{
  switch (volume) {
    case 0: PWM_TIMER->CCR1 = PWM_TIMER->ARR / 32; break;
    case 1: PWM_TIMER->CCR1 = PWM_TIMER->ARR / 16; break;
    case 2: PWM_TIMER->CCR1 = PWM_TIMER->ARR / 8; break;
    case 3: PWM_TIMER->CCR1 = PWM_TIMER->ARR / 4; break;
    case 4: PWM_TIMER->CCR1 = PWM_TIMER->ARR / 2; break;
  }
}

inline void buzzerOn()
{
  TRACE("buzzerOn beepVol %d", g_eeGeneral.beepVolume);

  setVolume(g_eeGeneral.beepVolume + 2);

  PWM_TIMER->CR1 = TIM_CR1_CEN;
}

inline void buzzerOff()
{
  PWM_TIMER->CR1 &= ~TIM_CR1_CEN;
  PWM_TIMER->CNT = 0;                     //
  PWM_TIMER->SR = (U16)~TIM_FLAG_Update;  // solves random quiet buzz issue when timer stopped
}

void buzzerSound(uint8_t duration)
{
  buzzerOn();
  buzzerCount = duration;
}

void buzzerHeartbeat()
{
  if (buzzerCount) {
    if (--buzzerCount == 0)
      buzzerOff();
  }
}
