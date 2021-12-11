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

//DMAFifo<32> heartbeatFifo __DMA (HEARTBEAT_DMA_Stream);

void trainerSendNextFrame();

void init_trainer_ppm() {
}

void stop_trainer_ppm() {
}

void init_trainer_capture() {
  GPIO_PinAFConfig(TRAINER_GPIO, TRAINER_IN_GPIO_PinSource, TRAINER_GPIO_AF);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = TRAINER_IN_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(TRAINER_GPIO, &GPIO_InitStructure);

  EXTMODULE_TIMER->CR1 &= ~TIM_CR1_CEN;
  EXTMODULE_TIMER->PSC = EXTMODULE_TIMER_FREQ / 2000000 - 1;  // 0.5uS
  EXTMODULE_TIMER->ARR = 65535;
  EXTMODULE_TIMER->CCR2 = GET_PPM_DELAY(EXTERNAL_MODULE) * 2;
  EXTMODULE_TIMER->CCMR1 = TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0;
  EXTMODULE_TIMER->BDTR |= TIM_BDTR_MOE;
  EXTMODULE_TIMER->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1;
  EXTMODULE_TIMER->EGR = 1;  // Restart

  WRITE_REG(EXTMODULE_TIMER->SR, ~(TIM_SR_CC1IF));  // Clear capture interrupt flag (PPMIN)
  EXTMODULE_TIMER->DIER |= TIM_DIER_CC1IE;          // Enable capture interrupt     (PPMIN)

  // WRITE_REG(EXTMODULE_TIMER->SR, ~(TIM_SR_CC2IF));  // Clear compare interrupt flag (PPMOUT)
  // EXTMODULE_TIMER->DIER |= TIM_DIER_CC2IE;          // Enable compare interrupt     (PPMOUT)

  NVIC_EnableIRQ(EXTMODULE_TIMER_IRQn);
  NVIC_SetPriority(EXTMODULE_TIMER_IRQn, 2);

//   EnablePPMTim();
}

void stop_trainer_capture() {
}

void trainerSendNextFrame() {
}

extern "C" void TRAINER_DMA_IRQHandler() {
}

extern "C" void TRAINER_TIMER_IRQHandler() {
}

void init_cppm_on_heartbeat_capture(void) {
}

void init_sbus_on_heartbeat_capture() {
}

void stop_sbus_on_heartbeat_capture() {
}

int sbusGetByte(uint8_t* byte) {
  return 0;
}
