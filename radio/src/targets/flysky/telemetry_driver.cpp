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

Fifo<uint8_t, TELEMETRY_FIFO_SIZE> telemetryFifo;
uint32_t telemetryErrors = 0;

void telemetryPortInit(uint32_t baudrate, uint8_t mode) {
  TRACE("telemetryPortInit %d", baudrate);

  if (baudrate == 0) {
    USART_DeInit(TELEMETRY_USART);
    return;
  }

  RCC_AHBPeriphClockCmd(TELEMETRY_RCC_AHB1Periph, ENABLE);
  RCC_APB1PeriphClockCmd(TELEMETRY_RCC_APB1Periph, ENABLE);

  /*
  Bits 13:12 PL[1:0]: Channel priority level
  These bits are set and cleared by software.
  00: Low
  01: Medium
  10: High
  11: Very high
  */

  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = TELEMETRY_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /*
  From Cleanflight:
          ioConfig_t ioCfg = IO_CONFIG(GPIO_Mode_AF, GPIO_Speed_50MHz,
              ((options & SERIAL_INVERTED) || (options & SERIAL_BIDIR_PP)) ? GPIO_OType_PP : GPIO_OType_OD,
              ((options & SERIAL_INVERTED) || (options & SERIAL_BIDIR_PP)) ? GPIO_PuPd_DOWN : GPIO_PuPd_UP
          );
  */
  GPIO_InitStructure.GPIO_Pin = TELEMETRY_TX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;  // was GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(TELEMETRY_GPIO, &GPIO_InitStructure);

  //GPIO_PinAFConfig(TELEMETRY_GPIO, TELEMETRY_GPIO_PinSource_RX, TELEMETRY_GPIO_AF);
  GPIO_PinAFConfig(TELEMETRY_GPIO, TELEMETRY_GPIO_PinSource_TX, TELEMETRY_GPIO_AF);

  USART_InitStructure.USART_BaudRate = baudrate;
  if (mode & TELEMETRY_SERIAL_8E2) {
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_2;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
  } else {
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
  }
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_Init(TELEMETRY_USART, &USART_InitStructure);

  // Half duplex
  USART_HalfDuplexCmd(TELEMETRY_USART, ENABLE);
  /*
   ===============================================================================
                           Half-duplex mode function
   ===============================================================================

    This subsection provides a set of functions allowing to manage the USART
    Half-duplex communication.

    The USART can be configured to follow a single-wire half-duplex protocol where
    the TX and RX lines are internally connected.

    USART Half duplex communication is possible through the following procedure:
       1. Program the Baud rate, Word length, Stop bits, Parity, Mode transmitter
          or Mode receiver and hardware flow control values using the USART_Init()
          function.
       2. Configures the USART address using the USART_SetAddress() function.
       3. Enable the USART using the USART_Cmd() function.
       4. Enable the half duplex mode using USART_HalfDuplexCmd() function.

  @note The RX pin is no longer used
  @note In Half-duplex mode the following bits must be kept cleared:
          - LINEN and CLKEN bits in the USART_CR2 register.
          - SCEN and IREN bits in the USART_CR3 register.
    */
  // Level inversion
  USART_InvPinCmd(TELEMETRY_USART, USART_InvPin_Tx | USART_InvPin_Rx, ENABLE);

  USART_Cmd(TELEMETRY_USART, ENABLE);
  USART_ITConfig(TELEMETRY_USART, USART_IT_RXNE, ENABLE);
  NVIC_SetPriority(TELEMETRY_USART_IRQn, 6);
  NVIC_EnableIRQ(TELEMETRY_USART_IRQn);
}

void telemetryPortSetDirectionOutput() {
  TELEMETRY_USART->CR1 &= ~USART_CR1_RE;  // disable receive
  TELEMETRY_USART->CR1 |= USART_CR1_TE;   // enable transmit
}

void telemetryPortSetDirectionInput() {
  TELEMETRY_USART->CR1 &= ~USART_CR1_TE;  // disable trasmit
  TELEMETRY_USART->CR1 |= USART_CR1_RE;   // enable receive
}

void sportSendBuffer(const uint8_t* buffer, unsigned long count) {
  telemetryPortSetDirectionOutput();

  DMA_InitTypeDef DMA_InitStructure;
  DMA_StructInit(&DMA_InitStructure);
  DMA_DeInit(TELEMETRY_DMA_Channel_TX);

  DMA_InitStructure.DMA_PeripheralBaseAddr = CONVERT_PTR_UINT(&TELEMETRY_USART->TDR);
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_BufferSize = count;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_MemoryBaseAddr = CONVERT_PTR_UINT(buffer);

  DMA_Init(TELEMETRY_DMA_Channel_TX, &DMA_InitStructure);
  DMA_Cmd(TELEMETRY_DMA_Channel_TX, ENABLE);
  USART_DMACmd(TELEMETRY_USART, USART_DMAReq_Tx, ENABLE);
  DMA_ITConfig(TELEMETRY_DMA_Channel_TX, DMA_IT_TC, ENABLE);
  USART_ClearITPendingBit(TELEMETRY_USART, USART_IT_TC);

  // enable interrupt and set it's priority
  NVIC_EnableIRQ(TELEMETRY_DMA_TX_IRQn);
  NVIC_SetPriority(TELEMETRY_DMA_TX_IRQn, 7);
}

extern "C" void TELEMETRY_DMA_TX_IRQHandler(void) {
  DEBUG_INTERRUPT(INT_TELEM_DMA);
  if (DMA_GetITStatus(TELEMETRY_DMA_TX_FLAG_TC)) {
    DMA_ClearITPendingBit(TELEMETRY_DMA_TX_FLAG_TC);
    TELEMETRY_USART->CR1 |= USART_CR1_TCIE;
    if (telemetryProtocol == PROTOCOL_FRSKY_SPORT) {
      outputTelemetryBufferSize = 0;
      outputTelemetryBufferTrigger = 0x7E;
    }
  }
}

#define USART_FLAG_ERRORS (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE)
extern "C" void TELEMETRY_USART_IRQHandler(void) {
  DEBUG_INTERRUPT(INT_TELEM_USART);
  uint32_t status = TELEMETRY_USART->ISR;
  if ((status & USART_FLAG_TC) && (TELEMETRY_USART->CR1 & USART_CR1_TCIE)) {
    TELEMETRY_USART->CR1 &= ~USART_CR1_TCIE;
    telemetryPortSetDirectionInput();
    while (status & (USART_FLAG_RXNE)) {
      status = TELEMETRY_USART->RDR;
      status = TELEMETRY_USART->ISR;
    }
  }

  while (status & (USART_FLAG_RXNE | USART_FLAG_ERRORS)) {
    uint8_t data = TELEMETRY_USART->RDR;
    if (status & USART_FLAG_ERRORS) {
      telemetryErrors++;
    } else {
      telemetryFifo.push(data);
#if defined(LUA)
      if (telemetryProtocol == PROTOCOL_FRSKY_SPORT) {
        static uint8_t prevdata;
        if (prevdata == 0x7E && outputTelemetryBufferSize > 0 && data == outputTelemetryBufferTrigger) {
          sportSendBuffer(outputTelemetryBuffer, outputTelemetryBufferSize);
        }
        prevdata = data;
      }
#endif
    }
    status = TELEMETRY_USART->ISR;
  }
}

// TODO we should have telemetry in an higher layer, functions above should move to a sport_driver.cpp
uint8_t telemetryGetByte(uint8_t* byte) {
  // #if defined(SERIAL2)
  //   if (telemetryProtocol == PROTOCOL_FRSKY_D_SECONDARY) {
  //     if (serial2Mode == UART_MODE_TELEMETRY)
  //       return serial2RxFifo.pop(*byte);
  //     else
  //       return false;
  //   } else {
  //     return telemetryFifo.pop(*byte);
  //   }
  // #else
  return telemetryFifo.pop(*byte);
  // #endif
}
