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

#include "mixer_scheduler.h"
#include "opentx.h"

RTOS_TASK_HANDLE menusTaskId;
RTOS_DEFINE_STACK(menusStack, MENUS_STACK_SIZE);

RTOS_TASK_HANDLE mixerTaskId;
RTOS_DEFINE_STACK(mixerStack, MIXER_STACK_SIZE);

#if defined(VOICE)
RTOS_TASK_HANDLE audioTaskId;
RTOS_DEFINE_STACK(audioStack, AUDIO_STACK_SIZE);
#endif

RTOS_MUTEX_HANDLE audioMutex;
RTOS_MUTEX_HANDLE mixerMutex;

enum TaskIndex {
  MENU_TASK_INDEX,
  MIXER_TASK_INDEX,
  AUDIO_TASK_INDEX,
  CLI_TASK_INDEX,
  BLUETOOTH_TASK_INDEX,
  TASK_INDEX_COUNT,
  MAIN_TASK_INDEX = 255
};

void stackPaint() {
  menusStack.paint();
  mixerStack.paint();
#if defined(VOICE)
  audioStack.paint();
#endif
#if defined(CLI)
  cliStack.paint();
#endif
}

volatile uint16_t timeForcePowerOffPressed = 0;

bool isForcePowerOffRequested() {
  if (pwrOffPressed()) {
    if (timeForcePowerOffPressed == 0) {
      timeForcePowerOffPressed = get_tmr10ms();
    } else {
      uint16_t delay = (uint16_t)get_tmr10ms() - timeForcePowerOffPressed;
      if (delay > 1000 /*10s*/) {
        return true;
      }
    }
  } else {
    resetForcePowerOffRequest();
  }
  return false;
}

bool isModuleSynchronous(uint8_t moduleIdx) {
  switch (g_model.moduleData[moduleIdx].type) {
    case MODULE_TYPE_CROSSFIRE:
      //case MODULE_TYPE_NONE:
      return true;
  }
  return false;
}

void sendSynchronousPulses(uint8_t runMask) {
  // if ((runMask & (1 << INTERNAL_MODULE)) && isModuleSynchronous(INTERNAL_MODULE)) {
  //   TRACE("SYNC setupPulses internal module");
  //   if (setupPulses(INTERNAL_MODULE)) {
  //     TRACE("SYNC intmoduleSendNextFrame internal module");
  //     intmoduleSendNextFrame();
  //   }
  // }
  if ((runMask & (1 << EXTERNAL_MODULE)) && isModuleSynchronous(EXTERNAL_MODULE)) {
    TRACE("SYNC setupPulses external module");
    if (setupPulses(EXTERNAL_MODULE)) {
      TRACE("SYNC intmoduleSendNextFrame external module");
      extmoduleSendNextFrame();
    }
  }
}
uint32_t nextMixerTime[NUM_MODULES];

TASK_FUNCTION(mixerTask) {
  s_pulses_paused = true;
  static uint32_t t = 0;

  mixerSchedulerInit();
  mixerSchedulerStart();

  while (true) {
#if defined(SBUS)
    processSbusInput();
#endif
    // run mixer at least every 30ms
    bool timeout = mixerSchedulerWaitForTrigger(30);

    //re-enable trigger
    mixerSchedulerClearTrigger();
    mixerSchedulerEnableTrigger();
#if defined(SIMU)
    if (pwrCheck() == e_power_off)
      TASK_RETURN();
#else
    if (isForcePowerOffRequested()) {
      pwrOff();
    }
#endif

    if (!s_pulses_paused) {
      uint16_t t0 = getTmr2MHz();

      DEBUG_TIMER_START(debugTimerMixer);
      RTOS_LOCK_MUTEX(mixerMutex);

      doMixerCalculations();

      sendSynchronousPulses((1 << INTERNAL_MODULE) | (1 << EXTERNAL_MODULE));

      doMixerPeriodicUpdates();

      DEBUG_TIMER_START(debugTimerMixerCalcToUsage);
      DEBUG_TIMER_SAMPLE(debugTimerMixerIterval);
      RTOS_UNLOCK_MUTEX(mixerMutex);
      DEBUG_TIMER_STOP(debugTimerMixer);

#if defined(STM32) && !defined(SIMU)
      if (getSelectedUsbMode() == USB_JOYSTICK_MODE) {
        usbJoystickUpdate();
      }
#endif

#if defined(TELEMETRY_FRSKY)
      DEBUG_TIMER_START(debugTimerTelemetryWakeup);
      telemetryWakeup();
      DEBUG_TIMER_STOP(debugTimerTelemetryWakeup);
#endif

#if defined(BLUETOOTH)
      bluetoothWakeup();
#endif

      if (heartbeat == HEART_WDT_CHECK) {
        wdt_reset();
        heartbeat = 0;
      }

      t0 = getTmr2MHz() - t0;
      if (t0 > maxMixerDuration)
        maxMixerDuration = t0;
    }
  }
}

#define MENU_TASK_PERIOD_TICKS 25  // 50ms

#if defined(COLORLCD) && defined(CLI)
bool perMainEnabled = true;
#endif

TASK_FUNCTION(menusTask) {
  opentxInit();

#if defined(PWR_BUTTON_PRESS)
  while (true) {
    uint32_t pwr_check = pwrCheck();
    if (pwr_check == e_power_off) {
      break;
    } else if (pwr_check == e_power_press) {
      RTOS_WAIT_TICKS(MENU_TASK_PERIOD_TICKS);
      continue;
    }
#else
  while (pwrCheck() != e_power_off) {
#endif
    uint32_t start = (uint32_t)RTOS_GET_TIME();
    DEBUG_TIMER_START(debugTimerPerMain);
#if defined(COLORLCD) && defined(CLI)
    if (perMainEnabled) {
      perMain();
    }
#else
    perMain();
#endif
    DEBUG_TIMER_STOP(debugTimerPerMain);
    // TODO remove completely massstorage from sky9x firmware
    uint32_t runtime = ((uint32_t)RTOS_GET_TIME() - start);
    // deduct the thread run-time from the wait, if run-time was more than
    // desired period, then skip the wait all together
    if (runtime < MENU_TASK_PERIOD_TICKS) {
      RTOS_WAIT_TICKS(MENU_TASK_PERIOD_TICKS - runtime);
    }

    resetForcePowerOffRequest();
  }

#if defined(PCBX9E)
  toplcdOff();
#endif

#if defined(PCBHORUS)
  ledOff();
#endif

  drawSleepBitmap();
  opentxClose();
  boardOff();  // Only turn power off if necessary

  TASK_RETURN();
}

void tasksStart() {
  RTOS_INIT();

#if defined(CLI)
  cliStart();
#endif

  RTOS_CREATE_TASK(mixerTaskId, mixerTask, "Mixer", mixerStack, MIXER_STACK_SIZE, MIXER_TASK_PRIO);
  RTOS_CREATE_TASK(menusTaskId, menusTask, "Menus", menusStack, MENUS_STACK_SIZE, MENUS_TASK_PRIO);

#if !defined(SIMU) && defined(VOICE)
  RTOS_CREATE_TASK(audioTaskId, audioTask, "Audio", audioStack, AUDIO_STACK_SIZE, AUDIO_TASK_PRIO);
#endif

  RTOS_CREATE_MUTEX(audioMutex);
  RTOS_CREATE_MUTEX(mixerMutex);

  RTOS_START();
}
