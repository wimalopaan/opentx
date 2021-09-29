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

uint16_t GetPPMTimCapture(void) {
  return EXTMODULE_TIMER->CCR1;
}
uint32_t GetPPMOutState(void) {
  return PPM_OUT_GPIO_PORT->IDR & PPM_OUT_PIN_MASK;
}
void SetPPMTimCompare(uint16_t val) {
  EXTMODULE_TIMER->CCR2 = val;
}
uint16_t GetPPMTimCompare(void) {
  return EXTMODULE_TIMER->CCR2;
}
uint32_t GetPPMTimCompareInterruptFlag(void) {
  return EXTMODULE_TIMER->SR & TIM_SR_CC2IF;
}
void ClearPPMTimCompareInterruptFlag(void) {
  WRITE_REG(EXTMODULE_TIMER->SR, ~(TIM_SR_CC2IF));
}
void EnablePPMTim(void) {
  SET_BIT(EXTMODULE_TIMER->CR1, TIM_CR1_CEN);
}
void DisablePPMTim(void) {
  CLEAR_BIT(EXTMODULE_TIMER->CR1, TIM_CR1_CEN);
}
void EnablePPMOut(void) {
  SET_BIT(EXTMODULE_TIMER->CCER, TIM_CCER_CC2E);
}
void DisablePPMOut(void) {
  CLEAR_BIT(EXTMODULE_TIMER->CCER, TIM_CCER_CC2E);
}
void sei(void) {
  __enable_irq();
}
void cli(void) {
  __disable_irq();
}

void extmoduleStop() {
  TRACE("extmoduleStop");
  DisablePPMOut();
  DisablePPMTim();
}

void extmodulePpmStart() {
  TRACE("extmodulePpmStart");
  /**EXTMODULE_TIMER GPIO Configuration
  PF9   ------> TIM15_CH1
  PF10   ------> TIM15_CH2
  */
  // //PF9
  // PPM_IN_GPIO_PORT->MODER |= GPIO_MODER_MODER9_1;       // Select alternate function mode
  // PPM_IN_GPIO_PORT->AFR[1] |= (0x0000000U << (1 * 4));  // Select alternate function 0
  // PPM_IN_GPIO_PORT->PUPDR |= GPIO_PUPDR_PUPDR9_0;       // PullUp
  // //PF10
  // PPM_OUT_GPIO_PORT->MODER |= GPIO_MODER_MODER10_1;      // Select alternate function mode
  // PPM_OUT_GPIO_PORT->AFR[1] |= (0x0000000U << (2 * 4));  // Select alternate function 0

  GPIO_PinAFConfig(EXTMODULE_TX_GPIO, EXTMODULE_TX_GPIO_PinSource, EXTMODULE_TX_GPIO_AF);

  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = EXTMODULE_TX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(EXTMODULE_TX_GPIO, &GPIO_InitStructure);

  EXTMODULE_TIMER->CR1 &= ~TIM_CR1_CEN;
  EXTMODULE_TIMER->PSC = EXTMODULE_TIMER_FREQ / 2000000 - 1;  // 0.5uS
  EXTMODULE_TIMER->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0;
  EXTMODULE_TIMER->BDTR |= TIM_BDTR_MOE;
  EXTMODULE_TIMER->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1;

  WRITE_REG(EXTMODULE_TIMER->SR, ~(TIM_SR_CC1IF));  // Clear capture interrupt flag (PPMIN)
  EXTMODULE_TIMER->DIER |= TIM_DIER_CC1IE;          // Enable capture interrupt     (PPMIN)

  WRITE_REG(EXTMODULE_TIMER->SR, ~(TIM_SR_CC2IF));  // Clear compare interrupt flag (PPMOUT)
  EXTMODULE_TIMER->DIER |= TIM_DIER_CC2IE;          // Enable compare interrupt     (PPMOUT)

  NVIC_EnableIRQ(EXTMODULE_TIMER_IRQn);
  NVIC_SetPriority(EXTMODULE_TIMER_IRQn, 2);

  EnablePPMTim();
  EnablePPMOut();
}

void extmodulePxxStart() {
  TRACE("extmodulePxxStart");
}

void extmoduleSendNextFrame() {
  static bool delay = true;
  if (s_current_protocol[EXTERNAL_MODULE] == PROTO_PPM) {
    //TRACE("modulePulsesData[EXTERNAL_MODULE].ppm: %p",(void*)&modulePulsesData[EXTERNAL_MODULE].ppm);
    //DUMP((uint8_t*)(modulePulsesData[EXTERNAL_MODULE].ppm.pulses), 40);
    static uint16_t *pulsePtr = modulePulsesData[EXTERNAL_MODULE].ppm.ptr;

    if (*pulsePtr != 0) {
      if (delay) {
        SetPPMTimCompare(GetPPMTimCompare() + GET_PPM_DELAY(EXTERNAL_MODULE) * 2);
      } else {
        //TRACE("ptr %d val %d", (uint8_t)(pulsePtr - modulePulsesData[EXTERNAL_MODULE].ppm.pulses), *pulsePtr);
        uint16_t pulse_len = *pulsePtr;
        //if ((uint8_t)(pulsePtr - modulePulsesData[EXTERNAL_MODULE].ppm.pulses) == 8) {
          pulse_len -= GET_PPM_DELAY(EXTERNAL_MODULE) * 2;// * 9;
        //}
        SetPPMTimCompare(GetPPMTimCompare() + pulse_len);
        pulsePtr += 1;
      }
    } else {
      pulsePtr = modulePulsesData[EXTERNAL_MODULE].ppm.pulses;
      // polarity 1 +
      // polarity 0 -
      EXTMODULE_TIMER->CCER = TIM_CCER_CC2E | (GET_PPM_POLARITY(EXTERNAL_MODULE) ? 0 : TIM_CCER_CC2P);
      SetPPMTimCompare(GetPPMTimCompare() + GET_PPM_DELAY(EXTERNAL_MODULE) * 2);
      setupPulses(EXTERNAL_MODULE);
    }
    delay = !delay;
  }
}

void extmoduleTimerStart(uint32_t period, uint8_t state) {
  // if (state)
  //   EXTERNAL_MODULE_ON();
  // else if (!IS_TRAINER_EXTERNAL_MODULE())
  //   EXTERNAL_MODULE_OFF();

  // GPIO_PinAFConfig(EXTMODULE_TX_GPIO, EXTMODULE_TX_GPIO_PinSource, 0);

  // GPIO_InitTypeDef GPIO_InitStructure;
  // GPIO_InitStructure.GPIO_Pin = EXTMODULE_TX_GPIO_PIN;
  // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  // GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  // GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  // GPIO_Init(EXTMODULE_TX_GPIO, &GPIO_InitStructure);
  // GPIO_SetBits(EXTMODULE_TX_GPIO, EXTMODULE_TX_GPIO_PIN); // Set high

  // EXTMODULE_TIMER->CR1 &= ~TIM_CR1_CEN;
  // EXTMODULE_TIMER->PSC = EXTMODULE_TIMER_FREQ / 2000000 - 1; // 0.5uS from 30MHz
  // EXTMODULE_TIMER->ARR = (2000 * period);
  // EXTMODULE_TIMER->CCR2 = (2000 * period) - 1000;
  // EXTMODULE_TIMER->EGR = 1; // Restart
  // EXTMODULE_TIMER->SR &= ~TIM_SR_CC2IF;
  // EXTMODULE_TIMER->DIER |= TIM_DIER_CC2IE; // Enable this interrupt
  // EXTMODULE_TIMER->CR1 |= TIM_CR1_CEN;

  // NVIC_EnableIRQ(EXTMODULE_TIMER_CC_IRQn);
  // NVIC_SetPriority(EXTMODULE_TIMER_CC_IRQn, 7);
}
/*--------------handler for PPM Timer ----------------------------------------*/
//void TIM15_IRQHandler(void) {
// TODO decide to bring implementation of pulses.cpp from erfly6 or not
//}

extern "C" void EXTMODULE_TIMER_IRQHandler() {
  if (EXTMODULE_TIMER->SR & TIM_SR_CC2IF) {  // Compare PPM-OUT
    EXTMODULE_TIMER->SR &= ~TIM_SR_CC2IF;    // Clears interrupt on ch2
    extmoduleSendNextFrame();
  }
  if (EXTMODULE_TIMER->SR & TIM_SR_CC1IF) {  // Capture PPM-IN
    EXTMODULE_TIMER->SR &= ~TIM_SR_CC1IF;    // Clears interrupt on ch1
    //ISR_TIMER3_CAPT_vect();
  }
}
