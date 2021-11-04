#include <stdio.h>

#include "opentx.h"

uint16_t UartGoodPkts;
uint16_t UartBadPkts;

uint8_t SX127x_RATES_VALUES[] = {0x06, 0x05, 0x04, 0x02};
uint8_t SX128x_RATES_VALUES[] = {0x05, 0x03, 0x01, 0x00};
// uint8_t TLM_INTERVAL_VALUES[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
// uint8_t MAX_POWER_VALUES[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
// uint8_t RF_FREQUENCY_VALUES[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};

char commitSha[] = "??????";

static uint8_t reqWaitMs = 100;
static tmr10ms_t lastReqTime = 0;

int findIndex(uint8_t* array, uint8_t max_idx, uint8_t value) {
  for (int i = 0; i <= max_idx; i++) {
    if (array[i] == value) {
      return i;
    }
  }
  return -1;
}

void elrsProcessResponse(uint8_t len, uint8_t* data) {
  if (data[0] == 0xEA && data[1] == 0xEE) {
    // Type 0xff - "sendLuaParams"
    if (data[2] == 0xFF) {
      if (len == 12) {
        g_model.moduleData[EXTERNAL_MODULE].elrs.tlm_interval = data[5]; //findIndex(TLM_INTERVAL_VALUES, ELRS_MAX_TLM_INTERVAL, data[5]);
        g_model.moduleData[EXTERNAL_MODULE].elrs.max_power = data[6]; //findIndex(MAX_POWER_VALUES, ELRS_MAX_MAX_POWER, data[6]);
        g_model.moduleData[EXTERNAL_MODULE].elrs.rf_frequency = data[7] - 1; //findIndex(RF_FREQUENCY_VALUES, ELRS_MAX_RF_FREQ, data[7]);
        if (data[7] == 6) {
          g_model.moduleData[EXTERNAL_MODULE].elrs.rf_type = ELRS_RF_SX128x;
          g_model.moduleData[EXTERNAL_MODULE].elrs.pkt_rate = findIndex(SX128x_RATES_VALUES, ELRS_MAX_RATE_SX128, data[4]);
        } else {
          g_model.moduleData[EXTERNAL_MODULE].elrs.rf_type = ELRS_RF_SX127x;
          g_model.moduleData[EXTERNAL_MODULE].elrs.pkt_rate = findIndex(SX127x_RATES_VALUES, ELRS_MAX_RATE_SX127, data[4]);
        }
        UartBadPkts = data[8];
        UartGoodPkts = data[9] * 256 + data[10];
      }
    }
    // Type 0xfe - "luaCommitPacket"
    if (data[2] == 0xFE && len == 9) {
      //DUMP(telemetryRxBuffer, telemetryRxBufferCount);
      sprintf(commitSha, "v%d.%d.%d", data[4], data[6], data[8]);
    }
  }
}

void elrsSendRequest(uint8_t request, uint8_t value) {
  static uint8_t data[4];
  data[0] = 0xEE;
  data[1] = 0xEA;
  data[2] = request;
  TRACE("Sending request %d value %d", request, value);
  switch (request) {
    case ELRS_AIR_RATE:
      if (g_model.moduleData[EXTERNAL_MODULE].elrs.rf_type == ELRS_RF_NONE) {
        // We have no response from module
        return;
      } else if (g_model.moduleData[EXTERNAL_MODULE].elrs.rf_type == ELRS_RF_SX127x) {
        data[3] = SX127x_RATES_VALUES[value];
      } else {
        data[3] = SX128x_RATES_VALUES[value];
      }
      break;
    case ELRS_TLM_INTERVAL:
      // data[3] = value; //TLM_INTERVAL_VALUES[value];
      // break;
    case ELRS_MAX_POWER:
      data[3] = value; //MAX_POWER_VALUES[value];
      break;
      /* not yet implemented
    case ELRS_BIND:      
      break;
    case ELRS_WEBSERVER:      
      break;
      */
    default:
      data[3] = value;
  }
  crossfireTelemetryPush(0x2D, data, 4);
  if (request != ELRS_PING) {
    elrsPing(true);
  }
}

void elrsPing(bool now) {
  if (now || get_tmr10ms() > lastReqTime + reqWaitMs) {
    elrsSendRequest(ELRS_PING, 0);
    lastReqTime = get_tmr10ms();
  }
}