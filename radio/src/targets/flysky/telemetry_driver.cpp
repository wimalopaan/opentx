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

void telemetryPortInit(uint32_t baudrate, uint8_t mode)
{
 
}

void telemetryPortSetDirectionOutput()
{
 
}

void telemetryPortSetDirectionInput()
{
 
}

void sportSendBuffer(uint8_t * buffer, uint32_t count)
{

}

extern "C" void TELEMETRY_DMA_TX_IRQHandler(void)
{
 
}

#define USART_FLAG_ERRORS (USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE)
extern "C" void TELEMETRY_USART_IRQHandler(void)
{
  
}

// TODO we should have telemetry in an higher layer, functions above should move to a sport_driver.cpp
uint8_t telemetryGetByte(uint8_t * byte)
{
	return 0;
}
