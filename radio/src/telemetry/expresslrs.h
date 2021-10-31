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

#ifndef _EXPRESSLRS_H_
#define _EXPRESSLRS_H_

#include <inttypes.h>

#define ELRS_ID 0x2D // Is it exclusive to ELRS?

#define ELRS_MAX_RATE_SX127 3
const char STR_SX127x_RATES[] =
    "\005"
    "25Hz\0"
    "50Hz\0"
    "100Hz"
    "200Hz";

#define ELRS_MAX_RATE_SX128 3
const char STR_SX128x_RATES[] =
    "\005"
    "50Hz\0"
    "150Hz"
    "250Hz"
    "500Hz";

#define ELRS_MAX_TLM_INTERVAL 7
const char STR_TLM_INTERVALS[] =
    "\005"
    "Off\0 "
    "1:128"
    "1:64\0"
    "1:32\0"
    "1:16\0"
    "1:8\0 "
    "1:4\0 "
    "1:2\0 ";

#define ELRS_MAX_MAX_POWER 7
const char STR_MAX_POWERS[] =
    "\007"
    "10 mw\0 "
    "25 mw\0 "
    "50 mw\0 "
    "100 mw\0"
    "250 mw\0"
    "500 mw\0"
    "1000 mw"
    "2000 mw";

#define ELRS_MAX_RF_FREQ 5
const char STR_RF_FREQUENCIES[] =
    "\010"
    "915 AU\0 "
    "915 FCC\0"
    "868 EU\0 "
    "433 AU\0 "
    "433 EU\0 "
    "2.4G ISM";

enum ElrsParams {
  ELRS_PING,
  ELRS_AIR_RATE,
  ELRS_TLM_INTERVAL,
  ELRS_MAX_POWER,
  ELRS_RF_FREQ,
  ELRS_BIND,
  ELRS_WEBSERVER
};

enum ElrsRfTypes {
  ELRS_RF_NONE,
  ELRS_RF_SX127x,
  ELRS_RF_SX128x
};

extern uint16_t UartGoodPkts;
extern uint16_t UartBadPkts;

void elrsProcessResponse(uint8_t len, uint8_t* data);
void elrsSendRequest(uint8_t request, uint8_t value);
void elrsPing(bool now);

#endif  // _EXPRESSLRS_H_