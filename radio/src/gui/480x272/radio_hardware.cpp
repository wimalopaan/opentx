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

enum MenuRadioHardwareItems {
  ITEM_RADIO_HARDWARE_CALIBRATION,
  ITEM_RADIO_HARDWARE_LABEL_STICKS,
  ITEM_RADIO_HARDWARE_STICK1,
  ITEM_RADIO_HARDWARE_STICK2,
  ITEM_RADIO_HARDWARE_STICK3,
  ITEM_RADIO_HARDWARE_STICK4,
  ITEM_RADIO_HARDWARE_LABEL_POTS,
  ITEM_RADIO_HARDWARE_POT1,
  ITEM_RADIO_HARDWARE_POT2,
  ITEM_RADIO_HARDWARE_POT3,
  ITEM_RADIO_HARDWARE_LS,
  ITEM_RADIO_HARDWARE_RS,
#if defined(PCBX12S)
  ITEM_RADIO_HARDWARE_LS2,
  ITEM_RADIO_HARDWARE_RS2,
#endif
  ITEM_RADIO_HARDWARE_LABEL_SWITCHES,
  ITEM_RADIO_HARDWARE_SA,
  ITEM_RADIO_HARDWARE_SB,
  ITEM_RADIO_HARDWARE_SC,
  ITEM_RADIO_HARDWARE_SD,
  ITEM_RADIO_HARDWARE_SE,
  ITEM_RADIO_HARDWARE_SF,
  ITEM_RADIO_HARDWARE_SG,
  ITEM_RADIO_HARDWARE_SH,
  ITEM_RADIO_HARDWARE_SERIAL_BAUDRATE,
  ITEM_RADIO_HARDWARE_BLUETOOTH_MODE,
  ITEM_RADIO_HARDWARE_BLUETOOTH_PAIRING_CODE,
  ITEM_RADIO_HARDWARE_BLUETOOTH_NAME,
#if defined(AUX_SERIAL)
  ITEM_RADIO_HARDWARE_AUX_SERIAL_MODE,
#endif
  ITEM_RADIO_HARDWARE_JITTER_FILTER,
  ITEM_RADIO_HARDWARE_BAT_CAL,
  ITEM_RADIO_HARDWARE_MAX
};

#define HW_SETTINGS_COLUMN             150
#if defined(PCBX10)
#define POTS_ROWS                      NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1
#else
#define POTS_ROWS                      NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1
#endif
#define SWITCHES_ROWS                  NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1, NAVIGATION_LINE_BY_LINE|1
#define BLUETOOTH_ROWS                 0, uint8_t(g_eeGeneral.bluetoothMode != BLUETOOTH_TELEMETRY ? HIDDEN_ROW : -1), uint8_t(g_eeGeneral.bluetoothMode == BLUETOOTH_OFF ? -1 : 0)
#define SWITCH_TYPE_MAX(sw)            ((MIXSRC_SF-MIXSRC_FIRST_SWITCH == sw || MIXSRC_SH-MIXSRC_FIRST_SWITCH == sw) ? SWITCH_2POS : SWITCH_3POS)

bool menuRadioHardware(event_t event)
{
  MENU(STR_HARDWARE, RADIO_ICONS, menuTabGeneral, MENU_RADIO_HARDWARE, ITEM_RADIO_HARDWARE_MAX, { 0, LABEL(Sticks), 0, 0, 0, 0, LABEL(Pots), POTS_ROWS, LABEL(Switches), SWITCHES_ROWS, 0, BLUETOOTH_ROWS, 0, 0, 0 });

  uint8_t sub = menuVerticalPosition;

  for (int i=0; i<NUM_BODY_LINES; ++i) {
    coord_t y = MENU_CONTENT_TOP + i*FH;
    int k = i + menuVerticalOffset;
    for (int j=0; j<=k; j++) {
      if (mstate_tab[j] == HIDDEN_ROW)
        k++;
    }
    LcdFlags attr = (sub == k ? ((s_editMode>0) ? BLINK|INVERS : INVERS) : 0);
    switch (k) {
      case ITEM_RADIO_HARDWARE_CALIBRATION:
        lcdDrawText(MENUS_MARGIN_LEFT, y, "Calibration", attr);
        if (attr && s_editMode>0) {
          pushMenu(menuRadioCalibration);
        }
        break;
      case ITEM_RADIO_HARDWARE_LABEL_STICKS:
        lcdDrawText(MENUS_MARGIN_LEFT, y, STR_STICKS);
        break;

      case ITEM_RADIO_HARDWARE_STICK1:
      case ITEM_RADIO_HARDWARE_STICK2:
      case ITEM_RADIO_HARDWARE_STICK3:
      case ITEM_RADIO_HARDWARE_STICK4:
        editStickHardwareSettings(HW_SETTINGS_COLUMN, y, k - ITEM_RADIO_HARDWARE_STICK1, event, attr);
        break;

      case ITEM_RADIO_HARDWARE_LS:
      case ITEM_RADIO_HARDWARE_RS:
#if defined(PCBX12S)
      case ITEM_RADIO_HARDWARE_LS2:
      case ITEM_RADIO_HARDWARE_RS2:
#endif
      {
        int idx = k - ITEM_RADIO_HARDWARE_LS;
        uint8_t mask = (0x01 << idx);
        lcdDrawTextAtIndex(INDENT_WIDTH, y, STR_VSRCRAW, NUM_STICKS+NUM_POTS+idx+1, menuHorizontalPosition < 0 ? attr : 0);
        if (ZEXIST(g_eeGeneral.anaNames[NUM_STICKS+NUM_POTS+idx]) || (attr && menuHorizontalPosition == 0))
          editName(HW_SETTINGS_COLUMN, y, g_eeGeneral.anaNames[NUM_STICKS+NUM_POTS+idx], LEN_ANA_NAME, event, attr && menuHorizontalPosition == 0);
        else
          lcdDrawMMM(HW_SETTINGS_COLUMN, y, 0);
        uint8_t potType = (g_eeGeneral.slidersConfig & mask) >> idx;
        potType = editChoice(HW_SETTINGS_COLUMN+50, y, STR_SLIDERTYPES, potType, SLIDER_NONE, SLIDER_WITH_DETENT, menuHorizontalPosition == 1 ? attr : 0, event);
        g_eeGeneral.slidersConfig &= ~mask;
        g_eeGeneral.slidersConfig |= (potType << idx);
        break;
      }

      case ITEM_RADIO_HARDWARE_LABEL_POTS:
        lcdDrawText(MENUS_MARGIN_LEFT, y, STR_POTS);
        break;

      case ITEM_RADIO_HARDWARE_POT1:
      case ITEM_RADIO_HARDWARE_POT2:
      case ITEM_RADIO_HARDWARE_POT3:
      {
        int idx = k - ITEM_RADIO_HARDWARE_POT1;
        uint8_t shift = (2*idx);
        uint8_t mask = (0x03 << shift);
        lcdDrawTextAtIndex(INDENT_WIDTH, y, STR_VSRCRAW, NUM_STICKS+idx+1, menuHorizontalPosition < 0 ? attr : 0);
        if (ZEXIST(g_eeGeneral.anaNames[NUM_STICKS+idx]) || (attr && menuHorizontalPosition == 0))
          editName(HW_SETTINGS_COLUMN, y, g_eeGeneral.anaNames[NUM_STICKS+idx], LEN_ANA_NAME, event, attr && menuHorizontalPosition == 0);
        else
          lcdDrawMMM(HW_SETTINGS_COLUMN, y, 0);
        uint8_t potType = (g_eeGeneral.potsConfig & mask) >> shift;
        potType = editChoice(HW_SETTINGS_COLUMN+50, y, STR_POTTYPES, potType, POT_NONE, POT_WITHOUT_DETENT, menuHorizontalPosition == 1 ? attr : 0, event);
        g_eeGeneral.potsConfig &= ~mask;
        g_eeGeneral.potsConfig |= (potType << shift);
        break;
      }
      case ITEM_RADIO_HARDWARE_LABEL_SWITCHES:
        lcdDrawText(MENUS_MARGIN_LEFT, y, STR_SWITCHES);
        break;
      case ITEM_RADIO_HARDWARE_SA:
      case ITEM_RADIO_HARDWARE_SB:
      case ITEM_RADIO_HARDWARE_SC:
      case ITEM_RADIO_HARDWARE_SD:
      case ITEM_RADIO_HARDWARE_SE:
      case ITEM_RADIO_HARDWARE_SF:
      case ITEM_RADIO_HARDWARE_SG:
      case ITEM_RADIO_HARDWARE_SH:
      {
        int index = k-ITEM_RADIO_HARDWARE_SA;
        int config = SWITCH_CONFIG(index);
        lcdDrawTextAtIndex(INDENT_WIDTH, y, STR_VSRCRAW, MIXSRC_FIRST_SWITCH-MIXSRC_Rud+index+1, menuHorizontalPosition < 0 ? attr : 0);
        if (ZEXIST(g_eeGeneral.switchNames[index]) || (attr && menuHorizontalPosition == 0))
          editName(HW_SETTINGS_COLUMN, y, g_eeGeneral.switchNames[index], LEN_SWITCH_NAME, event, menuHorizontalPosition == 0 ? attr : 0);
        else
          lcdDrawMMM(HW_SETTINGS_COLUMN, y, 0);
        config = editChoice(HW_SETTINGS_COLUMN+50, y, STR_SWTYPES, config, SWITCH_NONE, SWITCH_TYPE_MAX(index), menuHorizontalPosition == 1 ? attr : 0, event);
        if (attr && checkIncDec_Ret) {
          swconfig_t mask = (swconfig_t)0x03 << (2*index);
          g_eeGeneral.switchConfig = (g_eeGeneral.switchConfig & ~mask) | ((swconfig_t(config) & 0x03) << (2*index));
        }
        break;
      }

      case ITEM_RADIO_HARDWARE_SERIAL_BAUDRATE:
        lcdDrawText(MENUS_MARGIN_LEFT, y, STR_MAXBAUDRATE);
        lcdDrawNumber(HW_SETTINGS_COLUMN+50, y, CROSSFIRE_BAUDRATES[g_eeGeneral.telemetryBaudrate], attr|LEFT);
        if (attr) {
          g_eeGeneral.telemetryBaudrate = DIM(CROSSFIRE_BAUDRATES) - 1 - checkIncDecModel(event, DIM(CROSSFIRE_BAUDRATES) - 1 - g_eeGeneral.telemetryBaudrate, 0, DIM(CROSSFIRE_BAUDRATES) - 1);
          if (checkIncDec_Ret && IS_EXTERNAL_MODULE_ON()) {
            pauseMixerCalculations();
            pausePulses();
            EXTERNAL_MODULE_OFF();
            RTOS_WAIT_MS(20); // 20ms so that the pulses interrupt will reinit the frame rate
            telemetryProtocol = 255; // force telemetry port + module reinitialization
            EXTERNAL_MODULE_ON();
            resumePulses();
            resumeMixerCalculations();
          }
        }
        break;

      case ITEM_RADIO_HARDWARE_BLUETOOTH_MODE:
        lcdDrawText(MENUS_MARGIN_LEFT, y, STR_BLUETOOTH);
        g_eeGeneral.bluetoothMode = editChoice(HW_SETTINGS_COLUMN+50, y, STR_BLUETOOTH_MODES, g_eeGeneral.bluetoothMode, BLUETOOTH_OFF, BLUETOOTH_TRAINER, attr, event);
        break;

      case ITEM_RADIO_HARDWARE_BLUETOOTH_PAIRING_CODE:
        lcdDrawText(INDENT_WIDTH, y, STR_BLUETOOTH_PIN_CODE);
        lcdDrawText(HW_SETTINGS_COLUMN+50, y, "0000", 0);
        break;

      case ITEM_RADIO_HARDWARE_BLUETOOTH_NAME:
        lcdDrawText(INDENT_WIDTH, y, STR_NAME);
        editName(HW_SETTINGS_COLUMN+50, y, g_eeGeneral.bluetoothName, LEN_BLUETOOTH_NAME, event, attr);
        break;

#if defined(AUX_SERIAL)
      case ITEM_RADIO_HARDWARE_AUX_SERIAL_MODE:
        lcdDrawText(MENUS_MARGIN_LEFT, y, STR_AUX_SERIAL_MODE);
        g_eeGeneral.auxSerialMode = editChoice(HW_SETTINGS_COLUMN+50, y, STR_AUX_SERIAL_MODES, g_eeGeneral.auxSerialMode, 0, UART_MODE_MAX, attr, event);
        if (attr && checkIncDec_Ret) {
          auxSerialInit(g_eeGeneral.auxSerialMode, modelTelemetryProtocol());
        }
        break;
#endif

      case ITEM_RADIO_HARDWARE_JITTER_FILTER:
      {
        lcdDrawText(MENUS_MARGIN_LEFT, y, STR_JITTER_FILTER);
        uint8_t b = 1-g_eeGeneral.jitterFilter;
        g_eeGeneral.jitterFilter = 1 - editCheckBox(b, HW_SETTINGS_COLUMN+50, y, attr, event);
        break;
      }

      case ITEM_RADIO_HARDWARE_BAT_CAL:
        lcdDrawText(MENUS_MARGIN_LEFT, y, STR_BATT_CALIB);
        lcdDrawNumber(HW_SETTINGS_COLUMN+50, y, getBatteryVoltage(), attr|LEFT|PREC2, 0, NULL, "V");
        if (attr) CHECK_INCDEC_GENVAR(event, g_eeGeneral.txVoltageCalibration, -127, 127);
        break;
    }
  }

  return true;
}
