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
  return TIM15->CCR1;
}
uint32_t GetPPMOutState(void) {
  return PPM_OUT_GPIO_PORT->IDR & PPM_OUT_PIN_MASK;
}
void SetPPMTimCompare(uint16_t val) {
  TIM15->CCR2 = val;
}
uint16_t GetPPMTimCompare(void) {
  return TIM15->CCR2;
}
uint32_t GetPPMTimCompareInterruptFlag(void) {
  return TIM15->SR & TIM_SR_CC2IF;
}
void ClearPPMTimCompareInterruptFlag(void) {
  WRITE_REG(TIM15->SR, ~(TIM_SR_CC2IF));
}
void EnablePPMTim(void) {
  SET_BIT(TIM15->CR1, TIM_CR1_CEN);
}
void DisablePPMTim(void) {
  CLEAR_BIT(TIM15->CR1, TIM_CR1_CEN);
}
void EnablePPMOut(void) {
  SET_BIT(TIM15->CCER, TIM_CCER_CC2E);
}
void DisablePPMOut(void) {
  CLEAR_BIT(TIM15->CCER, TIM_CCER_CC2E);
}

void extmoduleStop() {
  TRACE("extmoduleStop");
  DisablePPMOut();
  DisablePPMTim();
}

void extmodulePpmStart() {
  TRACE("extmodulePpmStart");
  /*------------PPM_TIMER_Init(TIM15 clock 3mHz)------------------------------*/
  /**TIM15 GPIO Configuration
  PF9   ------> TIM15_CH1
  PF10   ------> TIM15_CH2
  */
  //PF9
  PPM_IN_GPIO_PORT->MODER  |= GPIO_MODER_MODER9_1;      // Select alternate function mode
  PPM_IN_GPIO_PORT->AFR[1] |= (0x0000000U << (1 * 4));  // Select alternate function 0
  PPM_IN_GPIO_PORT->PUPDR  |= GPIO_PUPDR_PUPDR9_0;      // PullUp
  //PF10
  PPM_OUT_GPIO_PORT->MODER  |= GPIO_MODER_MODER10_1;    // Select alternate function mode
  PPM_OUT_GPIO_PORT->AFR[1] |= (0x0000000U << (2 * 4)); // Select alternate function 0

  /* TIM15 clock enable */
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM15EN);  
  /* Delay after an RCC peripheral clock enabling */
  __IO uint32_t tmpreg;
  tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM15EN);

  TIM15->PSC   = 15;                                    // Prescaler
  TIM15->CCMR1 = (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0); /*OCyREF toggles on compare match*/
  TIM15->BDTR |= TIM_BDTR_MOE;

  TIM15->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1;
  TIM15->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;

  WRITE_REG(TIM15->SR, ~(TIM_SR_CC1IF));    // Clear capture interrupt flag
  TIM15->DIER |= TIM_DIER_CC1IE;            // Enable capture interrupt
  WRITE_REG(TIM15->SR, ~(TIM_SR_CC2IF));    // Clear compare interrupt flag
  TIM15->DIER = TIM_DIER_CC2IE;             // Enable compare interrupt

  NVIC_SetPriority(TIM15_IRQn, 2);
  NVIC_EnableIRQ(TIM15_IRQn);
  
  (void)tmpreg;

  EnablePPMTim();
  EnablePPMOut();
}

void extmodulePxxStart() {
  TRACE("extmodulePxxStart");
}

void extmoduleCrossfireStart() {
  TRACE("extmoduleCrossfireStart");
}

void extmoduleSendNextFrame() {
  TRACE("extmoduleSendNextFrame");
}

/*--------------handler for PPM Timer ----------------------------------------*/
void TIM15_IRQHandler(void) {
  // TODO decide to bring implementation of pulses.cpp from erfly6 or not

  // if (TIM15->SR & TIM_SR_CC2IF) {  // Compare PPM-OUT
  //   WRITE_REG(TIM15->SR, ~(TIM_SR_CC2IF));
  //   ISR_TIMER1_COMPA_vect();
  // }
  // if (TIM15->SR & TIM_SR_CC1IF) {  // Capture PPM-IN
  //   WRITE_REG(TIM15->SR, ~(TIM_SR_CC1IF));
  //   ISR_TIMER3_CAPT_vect();
  // }
}