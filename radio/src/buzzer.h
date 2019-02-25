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

#ifndef _BUZZER_H_
#define _BUZZER_H_


extern void pushPrompt(uint16_t prompt);
extern void pushUnit(uint8_t unit, uint8_t idx, uint8_t id);
extern bool isPlaying();
extern void audioEvent(unsigned int index);

extern uint8_t g_beepCnt;
extern uint8_t beepAgain;
extern uint8_t beepAgainOrig;
extern uint8_t beepOn;
extern bool warble;
extern bool warbleC;

#if defined(HAPTIC)
extern uint8_t hapticTick;
#endif /* HAPTIC */


#if defined(SIMU)
inline void _beep(uint8_t b)
{
	g_beepCnt = b;
}
#else
inline void _beep(uint8_t b)
{
  buzzerSound(b);
}
#endif
void beep(uint8_t val);


#define AUDIO_HELLO()          PUSH_SYSTEM_PROMPT(AUDIO_HELLO)
#define AUDIO_BYE()
#define AUDIO_TX_BATTERY_LOW() PUSH_SYSTEM_PROMPT(AU_TX_BATTERY_LOW)
#define AUDIO_INACTIVITY()     PUSH_SYSTEM_PROMPT(AU_INACTIVITY)
#define AUDIO_ERROR_MESSAGE(e) PUSH_SYSTEM_PROMPT((e))
#define AUDIO_TIMER_MINUTE(t)  playDuration(t, 0, 0)
  // TODO
#define AUDIO_TIMER_30()       PUSH_SYSTEM_PROMPT(AU_TIMER_30)
#define AUDIO_TIMER_20()       PUSH_SYSTEM_PROMPT(AU_TIMER_20)

#define AUDIO_KEY_PRESS()        beep(0)
#define AUDIO_KEY_ERROR()        beep(2)
#define AUDIO_WARNING2()         beep(2)
#define AUDIO_WARNING1()         beep(3)
#define AUDIO_ERROR()            beep(4)
#define AUDIO_MIX_WARNING(x)     beep(1)
#define AUDIO_POT_MIDDLE(x)      beep(2)
#define AUDIO_TIMER_COUNTDOWN(idx, val)  beep(2)
#define AUDIO_TIMER_ELAPSED(idx) beep(3)
#define AUDIO_VARIO_UP()         _beep(1)
#define AUDIO_VARIO_DOWN()       _beep(1)
#define AUDIO_TRIM_PRESS(f)      { if (!IS_KEY_FIRST(event)) warble = true; beep(1); }
#define AUDIO_TRIM_MIDDLE()      beep(2)
#define AUDIO_TRIM_MIN()         beep(2)
#define AUDIO_TRIM_MAX()         beep(2)
#define AUDIO_PLAY(p)            beep(3)

#define IS_AUDIO_BUSY() (g_beepCnt || beepAgain || beepOn)

#define AUDIO_RESET()
#define AUDIO_FLUSH()

#define PLAY_PHASE_OFF(phase)
#define PLAY_PHASE_ON(phase)
#define PLAY_SWITCH_MOVED(sw)
#define PLAY_LOGICAL_SWITCH_OFF(sw)
#define PLAY_LOGICAL_SWITCH_ON(sw)
#define PLAY_MODEL_NAME()
#define START_SILENCE_PERIOD()

#define PROMPT_CUSTOM_BASE      0
#define PROMPT_I18N_BASE        256
#define PROMPT_SYSTEM_BASE      480

#define I18N_PLAY_FUNCTION(lng, x, ...) void lng ## _ ## x(__VA_ARGS__, uint8_t id)
#define PLAY_FUNCTION(x, ...)           void x(__VA_ARGS__, uint8_t id)

#define PUSH_CUSTOM_PROMPT(p, id)       pushPrompt(PROMPT_CUSTOM_BASE+(p))
#define PUSH_NUMBER_PROMPT(p)           pushPrompt(PROMPT_I18N_BASE+(p))
#define PUSH_SYSTEM_PROMPT(p)           pushPrompt(PROMPT_SYSTEM_BASE+(p))
#define PLAY_NUMBER(n, u, a)            playNumber((n), (u), (a), id)
#define PUSH_UNIT_PROMPT(p, i)   		pushUnit((p), (i), id)
#define PLAY_DURATION(d, att)    		{}
#define PLAY_TIME
#define IS_PLAY_TIME()                  (0)
#define IS_PLAYING(id)                  isPlaying()
#define PLAY_VALUE(v, id)        		playValue((v), (id))

#define AUDIO_RSSI_ORANGE()      audioEvent(AU_RSSI_ORANGE)
#define AUDIO_RSSI_RED()         audioEvent(AU_RSSI_RED)
#define AUDIO_RAS_RED()          audioEvent(AU_RAS_RED)
#define AUDIO_TELEMETRY_LOST()   audioEvent(AU_TELEMETRY_LOST)
#define AUDIO_TELEMETRY_BACK()   audioEvent(AU_TELEMETRY_BACK)
#define AUDIO_TRAINER_LOST()     audioEvent(AU_TRAINER_LOST)
#define AUDIO_TRAINER_BACK()     audioEvent(AU_TRAINER_BACK)

#define AUDIO_VARIO(fq, t, p, f) {}

#define setScaledVolume(v)


#endif // _BUZZER_H_
