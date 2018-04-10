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

#ifndef _HAL_H_
#define _HAL_H_


  #define KEYS_GPIO_REG_PAGE            GPIOD->IDR
  #define KEYS_GPIO_PIN_PAGE            GPIO_Pin_3  // PD.03
  #define KEYS_GPIO_REG_MENU            GPIOD->IDR
  #define KEYS_GPIO_PIN_MENU            GPIO_Pin_7  // PD.07
  #define KEYS_GPIO_REG_EXIT            GPIOD->IDR
  #define KEYS_GPIO_PIN_EXIT            GPIO_Pin_2  // PD.02
  #define KEYS_GPIO_REG_ENTER           GPIOE->IDR
  #define KEYS_GPIO_PIN_ENTER           GPIO_Pin_10 // PE.10

  #define KEYS_GPIO_REG_PLUS            GPIOD->IDR
  #define KEYS_GPIO_PIN_PLUS            GPIO_Pin_3
  #define KEYS_GPIO_REG_MINUS           GPIOD->IDR
  #define KEYS_GPIO_PIN_MINUS           GPIO_Pin_4


// Trims

  #define TRIMS_GPIO_REG_LHL            GPIOD->IDR
  #define TRIMS_GPIO_PIN_LHL            GPIO_Pin_15 // PD.15
  #define TRIMS_GPIO_REG_LHR            GPIOC->IDR
  #define TRIMS_GPIO_PIN_LHR            GPIO_Pin_1  // PC.01
  #define TRIMS_GPIO_REG_LVD            GPIOE->IDR
  #define TRIMS_GPIO_PIN_LVD            GPIO_Pin_6  // PE.06
  #define TRIMS_GPIO_REG_LVU            GPIOE->IDR
  #define TRIMS_GPIO_PIN_LVU            GPIO_Pin_5  // PE.05
  #define TRIMS_GPIO_REG_RVD            GPIOC->IDR
  #define TRIMS_GPIO_PIN_RVD            GPIO_Pin_3  // PC.03
  #define TRIMS_GPIO_REG_RHL            GPIOE->IDR
  #define TRIMS_GPIO_PIN_RHL            GPIO_Pin_3  // PE.03
  #define TRIMS_GPIO_REG_RVU            GPIOC->IDR
  #define TRIMS_GPIO_PIN_RVU            GPIO_Pin_2  // PC.02
  #define TRIMS_GPIO_REG_RHR            GPIOE->IDR
  #define TRIMS_GPIO_PIN_RHR            GPIO_Pin_4  // PE.04

// Switches

  #define SWITCHES_GPIO_REG_A_L         GPIOE->IDR
  #define SWITCHES_GPIO_PIN_A_L         GPIO_Pin_7  // PE.07
  #define SWITCHES_GPIO_REG_A_H         GPIOE->IDR
  #define SWITCHES_GPIO_PIN_A_H         GPIO_Pin_13 // PE.13

  #define SWITCHES_GPIO_REG_B_L         GPIOE->IDR
  #define SWITCHES_GPIO_PIN_B_L         GPIO_Pin_15 // PE.15
  #define SWITCHES_GPIO_REG_B_H         GPIOA->IDR
  #define SWITCHES_GPIO_PIN_B_H         GPIO_Pin_5  // PA.05

  #define SWITCHES_GPIO_REG_C_L         GPIOD->IDR
  #define SWITCHES_GPIO_PIN_C_L         GPIO_Pin_11 // PD.11
  #define SWITCHES_GPIO_REG_C_H         GPIOE->IDR
  #define SWITCHES_GPIO_PIN_C_H         GPIO_Pin_0  // PE.00

  #define SWITCHES_GPIO_REG_D_L         GPIOE->IDR
  #define SWITCHES_GPIO_PIN_D_L         GPIO_Pin_1  // PE.01
  #define SWITCHES_GPIO_REG_D_H         GPIOE->IDR
  #define SWITCHES_GPIO_PIN_D_H         GPIO_Pin_2  // PE.02


  #define KEYS_RCC_AHB1Periph           (RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOD|RCC_AHBPeriph_GPIOE)
  #define KEYS_GPIOA_PINS               SWITCHES_GPIO_PIN_B_H
  #define KEYS_GPIOC_PINS               (TRIMS_GPIO_PIN_LHR | TRIMS_GPIO_PIN_RVD | TRIMS_GPIO_PIN_RVU)
  #define KEYS_GPIOD_PINS               (TRIMS_GPIO_PIN_LHL | KEYS_GPIO_PIN_MENU | KEYS_GPIO_PIN_EXIT | KEYS_GPIO_PIN_PAGE | SWITCHES_GPIO_PIN_C_L | SWITCHES_GPIO_PIN_H)
  #define KEYS_GPIOE_PINS               (ENC_GPIO_PIN_A | ENC_GPIO_PIN_B | KEYS_GPIO_PIN_ENTER | TRIMS_GPIO_PIN_RHR | TRIMS_GPIO_PIN_RHL | TRIMS_GPIO_PIN_LVD | TRIMS_GPIO_PIN_LVU | SWITCHES_GPIO_PIN_C_H | SWITCHES_GPIO_PIN_D_L | SWITCHES_GPIO_PIN_D_H | SWITCHES_GPIO_PIN_B_L | SWITCHES_GPIO_PIN_A_L | SWITCHES_GPIO_PIN_A_H | SWITCHES_GPIO_PIN_F)


// ADC
#define ADC_MAIN                        ADC1
#define ADC_DMA                         DMA2
#define ADC_DMA_SxCR_CHSEL              0
#define ADC_DMA_Stream                  DMA2_Stream4
#define ADC_SET_DMA_FLAGS()             ADC_DMA->HIFCR = (DMA_HIFCR_CTCIF4 | DMA_HIFCR_CHTIF4 | DMA_HIFCR_CTEIF4 | DMA_HIFCR_CDMEIF4 | DMA_HIFCR_CFEIF4)
#define ADC_TRANSFER_COMPLETE()         (ADC_DMA->HISR & DMA_HISR_TCIF4)
#define ADC_SAMPTIME                    2   // sample time = 28 cycles

  #define ADC_RCC_AHB1Periph            (RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_DMA1)
  #define ADC_RCC_APB1Periph            0
  #define ADC_RCC_APB2Periph            RCC_APB2Periph_ADC1
  #define ADC_GPIO_PIN_STICK_RV         GPIO_Pin_0  // PA.00
  #define ADC_GPIO_PIN_STICK_RH         GPIO_Pin_1  // PA.01
  #define ADC_GPIO_PIN_STICK_LV         GPIO_Pin_2  // PA.02
  #define ADC_GPIO_PIN_STICK_LH         GPIO_Pin_3  // PA.03
  #define ADC_CHANNEL_STICK_RV          ADC_Channel_0  // ADC1_IN0
  #define ADC_CHANNEL_STICK_RH          ADC_Channel_1  // ADC1_IN1
  #define ADC_CHANNEL_STICK_LV          ADC_Channel_2  // ADC1_IN2
  #define ADC_CHANNEL_STICK_LH          ADC_Channel_3  // ADC1_IN3
  #define ADC_GPIO_PIN_POT1             GPIO_Pin_6  // PA.06
  #define ADC_GPIO_PIN_POT2             GPIO_Pin_0  // PB.00
  #define ADC_GPIO_PIN_BATT             GPIO_Pin_0  // PC.00
  #define ADC_GPIOA_PINS                (ADC_GPIO_PIN_STICK_RV | ADC_GPIO_PIN_STICK_RH | ADC_GPIO_PIN_STICK_LH | ADC_GPIO_PIN_STICK_LV | ADC_GPIO_PIN_POT1)
  #define ADC_GPIOB_PINS                ADC_GPIO_PIN_POT2
  #define ADC_GPIOC_PINS                ADC_GPIO_PIN_BATT
  #define ADC_CHANNEL_POT1              ADC_Channel_6
  #define ADC_CHANNEL_POT2              ADC_Channel_8
  #define ADC_CHANNEL_BATT              ADC_Channel_10

// PWR and LED driver
#define PWR_RCC_AHB1Periph              (RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOE | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE)
#if defined(PCBX9E) || defined(PCBX7) || defined(PCBXLITE)
#define PWR_PRESS_BUTTON
#endif

  #define PWR_SWITCH_GPIO               GPIOD
  #define PWR_SWITCH_GPIO_PIN           GPIO_Pin_1  // PD.01
  #define PWR_ON_GPIO                   GPIOD
  #define PWR_ON_GPIO_PIN               GPIO_Pin_0  // PD.00

#define PWR_ON_GPIO_MODER               GPIO_MODER_MODER0
#define PWR_ON_GPIO_MODER_OUT           GPIO_MODER_MODER0_0


// Internal Module
  #define INTMODULE_PULSES
  #define INTMODULE_RCC_AHB1Periph      (RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_DMA2)
  #define INTMODULE_RCC_APB1Periph      0
  #define INTMODULE_RCC_APB2Periph      RCC_APB2Periph_TIM1
  #define INTMODULE_PWR_GPIO            GPIOC
  #define INTMODULE_PWR_GPIO_PIN        GPIO_Pin_6  // PC.06
  #define INTMODULE_TX_GPIO             GPIOA
  #define INTMODULE_TX_GPIO_PIN         GPIO_Pin_10 // PA.10
  #define INTMODULE_TX_GPIO_PinSource   GPIO_PinSource10
  #define INTMODULE_TIMER               TIM1
  #define INTMODULE_TIMER_CC_IRQn       TIM1_CC_IRQn
  #define INTMODULE_TIMER_CC_IRQHandler TIM1_CC_IRQHandler
  #define INTMODULE_TX_GPIO_AF          GPIO_AF_TIM1
  #define INTMODULE_DMA_CHANNEL         DMA_Channel_6
  #define INTMODULE_DMA_STREAM          DMA2_Stream5
  #define INTMODULE_DMA_STREAM_IRQn     DMA2_Stream5_IRQn
  #define INTMODULE_DMA_STREAM_IRQHandler DMA2_Stream5_IRQHandler
  #define INTMODULE_DMA_FLAG_TC         DMA_IT_TCIF5
  #define INTMODULE_TIMER_FREQ          (PERI2_FREQUENCY * TIMER_MULT_APB2)

// Trainer Port
#if defined(PCBXLITE)
  #define TRAINER_RCC_AHBPeriph        0
  #define TRAINER_RCC_APB1Periph        0
#else
  #define TRAINER_RCC_AHBPeriph        (RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC)
  #define TRAINER_RCC_APB1Periph        RCC_APB1Periph_TIM3
  #define TRAINER_GPIO                  GPIOC
  #define TRAINER_IN_GPIO_PIN           GPIO_Pin_8  // PC.08
  #define TRAINER_IN_GPIO_PinSource     GPIO_PinSource8
  #define TRAINER_OUT_GPIO_PIN          GPIO_Pin_9  // PC.09
  #define TRAINER_OUT_GPIO_PinSource    GPIO_PinSource9
  #define TRAINER_DETECT_GPIO           GPIOA
  #define TRAINER_DETECT_GPIO_PIN       GPIO_Pin_8  // PA.08
  #define TRAINER_TIMER                 TIM3
  #define TRAINER_TIMER_IRQn            TIM3_IRQn
  #define TRAINER_GPIO_AF               GPIO_AF_TIM3
  #define TRAINER_DMA                   DMA1
  #define TRAINER_DMA_CHANNEL           DMA_Channel_5
  #define TRAINER_DMA_STREAM            DMA1_Stream2
  #define TRAINER_DMA_IRQn              DMA1_Stream2_IRQn
  #define TRAINER_DMA_IRQHandler        DMA1_Stream2_IRQHandler
  #define TRAINER_DMA_FLAG_TC           DMA_IT_TCIF2
  #define TRAINER_TIMER_IRQn            TIM3_IRQn
  #define TRAINER_TIMER_IRQHandler      TIM3_IRQHandler
  #define TRAINER_TIMER_FREQ            (PERI1_FREQUENCY * TIMER_MULT_APB1)
#endif

 #define SD_RCC_AHB1Periph 0
 #define HAPTIC_RCC_AHB1Periph 0
 #define EXTMODULE_RCC_AHB1Periph 0
 #define  TELEMETRY_RCC_AHB1Periph 0
 #define SPORT_UPDATE_RCC_AHB1Periph 0
#define BT_RCC_AHB1Periph 0
#define TRAINER_RCC_AHB1Periph 0
#define HAPTIC_RCC_APB1Periph 0
#define SD_RCC_APB1Periph 0
#define BT_RCC_APB1Periph 0
#define TELEMETRY_RCC_APB1Periph 0
#define HAPTIC_RCC_APB2Periph 0
#define EXTMODULE_RCC_APB2Periph 0
#define BT_RCC_APB2Periph 0
#define SD_GPIO_PRESENT_GPIO 0
// Serial Port

  #define TRAINER_BATTERY_COMPARTMENT
  #define SERIAL_RCC_AHB1Periph         (RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_DMA1)
  #define SERIAL_RCC_APB1Periph         RCC_APB1Periph_USART3
  #define SERIAL_GPIO                   GPIOB
  #define SERIAL_GPIO_PIN_TX            GPIO_Pin_10 // PB.10
  #define SERIAL_GPIO_PIN_RX            GPIO_Pin_11 // PB.11
  #define SERIAL_GPIO_PinSource_TX      GPIO_PinSource10
  #define SERIAL_GPIO_PinSource_RX      GPIO_PinSource11
  #define SERIAL_GPIO_AF                GPIO_AF_USART3
  #define SERIAL_USART                  USART3
  #define SERIAL_USART_IRQHandler       USART3_IRQHandler
  #define SERIAL_USART_IRQn             USART3_IRQn
  #define SERIAL_DMA_Stream_RX          DMA1_Stream1
  #define SERIAL_DMA_Channel_RX         DMA_Channel_4

/*
// Telemetry
#define TELEMETRY_RCC_AHB1Periph        (RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1)
#define TELEMETRY_RCC_APB1Periph        RCC_APB1Periph_USART2
#define TELEMETRY_DIR_GPIO              GPIOD
#define TELEMETRY_DIR_GPIO_PIN          GPIO_Pin_4  // PD.04
#if defined(PCBXLITE)
#define TELEMETRY_DIR_OUTPUT()          TELEMETRY_DIR_GPIO->BSRRH = TELEMETRY_DIR_GPIO_PIN
#define TELEMETRY_DIR_INPUT()           TELEMETRY_DIR_GPIO->BSRRL = TELEMETRY_DIR_GPIO_PIN
#else
#define TELEMETRY_DIR_OUTPUT()          TELEMETRY_DIR_GPIO->BSRRL = TELEMETRY_DIR_GPIO_PIN
#define TELEMETRY_DIR_INPUT()           TELEMETRY_DIR_GPIO->BSRRH = TELEMETRY_DIR_GPIO_PIN
#endif
#define TELEMETRY_GPIO                  GPIOD
#define TELEMETRY_TX_GPIO_PIN           GPIO_Pin_5  // PD.05
#define TELEMETRY_RX_GPIO_PIN           GPIO_Pin_6  // PD.06
#define TELEMETRY_GPIO_PinSource_TX     GPIO_PinSource5
#define TELEMETRY_GPIO_PinSource_RX     GPIO_PinSource6
#define TELEMETRY_GPIO_AF               GPIO_AF_USART2
#define TELEMETRY_USART                 USART2
#define TELEMETRY_DMA_Stream_TX         DMA1_Stream6
#define TELEMETRY_DMA_Channel_TX        DMA_Channel_4
#define TELEMETRY_DMA_TX_Stream_IRQ     DMA1_Stream6_IRQn
#define TELEMETRY_DMA_TX_IRQHandler     DMA1_Stream6_IRQHandler
#define TELEMETRY_DMA_TX_FLAG_TC        DMA_IT_TCIF6
#define TELEMETRY_USART_IRQHandler      USART2_IRQHandler
#define TELEMETRY_USART_IRQn            USART2_IRQn
*/

// PCBREV

  #define PCBREV_RCC_AHB1Periph         RCC_AHBPeriph_GPIOA
  #define PCBREV_GPIO                   GPIOA
  #define PCBREV_GPIO_PIN               GPIO_Pin_14  // PA.14




// Heartbeat

  #define TRAINER_MODULE_HEARTBEAT
  #define HEARTBEAT_RCC_AHB1Periph      RCC_AHBPeriph_GPIOC
  #define HEARTBEAT_RCC_APB2Periph      RCC_APB2Periph_USART6
  #define HEARTBEAT_GPIO                GPIOC
  #define HEARTBEAT_GPIO_PIN            GPIO_Pin_7  // PC.07
  #define HEARTBEAT_GPIO_PinSource      GPIO_PinSource7
  #define HEARTBEAT_GPIO_AF_SBUS        GPIO_AF_USART6
  #define HEARTBEAT_GPIO_AF_CAPTURE     GPIO_AF_TIM3
  #define HEARTBEAT_USART               USART6
  #define HEARTBEAT_USART_IRQHandler    USART6_IRQHandler
  #define HEARTBEAT_USART_IRQn          USART6_IRQn
  #define HEARTBEAT_DMA_Stream          DMA2_Stream1
  #define HEARTBEAT_DMA_Channel         DMA_Channel_5

  #define BACKLIGHT_RCC_AHB1Periph      RCC_AHBPeriph_GPIOD
  #define BACKLIGHT_RCC_APB1Periph      0
  #define BACKLIGHT_RCC_APB2Periph      RCC_APB2Periph_TIM15
  #define BACKLIGHT_TIMER_FREQ          (PERI1_FREQUENCY * TIMER_MULT_APB1)
  #define BACKLIGHT_TIMER               TIM5
  #define BACKLIGHT_GPIO                GPIOD
  #define BACKLIGHT_GPIO_PIN            GPIO_Pin_13 // PD.13
  #define BACKLIGHT_GPIO_PinSource      GPIO_PinSource13
  #define BACKLIGHT_GPIO_AF             GPIO_AF_TIM5
  #define BACKLIGHT_CCMR1               TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 // Channel2, PWM
  #define BACKLIGHT_CCER                TIM_CCER_CC2E
  #define BACKLIGHT_COUNTER_REGISTER    BACKLIGHT_TIMER->CCR2



// LCD driver

  #define LCD_RCC_AHB1Periph            (RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD )
  #define LCD_RCC_APB1Periph            0
  #define LCD_SPI_GPIO                  GPIOC
  #define LCD_MOSI_GPIO_PIN             GPIO_Pin_12 // PC.12
  #define LCD_MOSI_GPIO_PinSource       GPIO_PinSource12
  #define LCD_CLK_GPIO_PIN              GPIO_Pin_10 // PC.10
  #define LCD_CLK_GPIO_PinSource        GPIO_PinSource10
  #define LCD_A0_GPIO_PIN               GPIO_Pin_11 // PC.11
  #define LCD_NCS_GPIO                  GPIOA
  #define LCD_NCS_GPIO_PIN              GPIO_Pin_15 // PA.15
  #define LCD_RST_GPIO                  GPIOD
  #define LCD_RST_GPIO_PIN              GPIO_Pin_12 // PD.12
  #define LCD_DMA                       DMA1
  #define LCD_DMA_Stream                DMA1_Stream7
  #define LCD_DMA_Stream_IRQn           DMA1_Stream7_IRQn
  #define LCD_DMA_Stream_IRQHandler     DMA1_Stream7_IRQHandler
  #define LCD_DMA_FLAGS                 (DMA_HIFCR_CTCIF7 | DMA_HIFCR_CHTIF7 | DMA_HIFCR_CTEIF7 | DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CFEIF7)
  #define LCD_DMA_FLAG_INT              DMA_HIFCR_CTCIF7
  #define LCD_SPI                       SPI3
  #define LCD_GPIO_AF                   GPIO_AF_SPI3

#define LCD_RCC_APB2Periph              0

// I2C Bus: EEPROM and CAT5137 digital pot for volume control
#define I2C_RCC_APB1Periph              RCC_APB1Periph_I2C1
#define I2C                             I2C1
#define I2C_GPIO_AF                     GPIO_AF_I2C1

  #define I2C_RCC_AHB1Periph            RCC_AHBPeriph_GPIOB
  #define I2C_SPI_GPIO                  GPIOB
  #define I2C_SCL_GPIO_PIN              GPIO_Pin_6  // PB.06
  #define I2C_SDA_GPIO_PIN              GPIO_Pin_7  // PB.07
  #define I2C_WP_GPIO                   GPIOB
  #define I2C_WP_GPIO_PIN               GPIO_Pin_9  // PB.09
  #define I2C_SCL_GPIO_PinSource        GPIO_PinSource6
  #define I2C_SDA_GPIO_PinSource        GPIO_PinSource7

  #define I2C_SPEED                     400000
#define I2C_ADDRESS_EEPROM              0xA2
#define I2C_ADDRESS_VOLUME              0x5C
#define I2C_FLASH_PAGESIZE              64

// Audio
#define AUDIO_RCC_AHB1Periph            (RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_DMA1)
#define AUDIO_RCC_APB1Periph            (RCC_APB1Periph_TIM6)
#define AUDIO_OUTPUT_GPIO               GPIOA
#define AUDIO_OUTPUT_GPIO_PIN           GPIO_Pin_4  // PA.04
#define AUDIO_DMA_Stream                DMA1_Stream5
#define AUDIO_DMA_Stream_IRQn           DMA1_Stream5_IRQn
#define AUDIO_TIM_IRQn                  TIM6_DAC_IRQn
#define AUDIO_TIM_IRQHandler            TIM6_DAC_IRQHandler
#define AUDIO_DMA_Stream_IRQHandler     DMA1_Stream5_IRQHandler
#define AUDIO_TIMER                     TIM6
#define AUDIO_DMA                       DMA1

// Xms Interrupt
#define INTERRUPT_xMS_RCC_APB1Periph    RCC_APB1Periph_TIM14
#define INTERRUPT_xMS_TIMER             TIM14
#define INTERRUPT_xMS_IRQn              TIM8_TRG_COM_TIM14_IRQn
#define INTERRUPT_xMS_IRQHandler        TIM8_TRG_COM_TIM14_IRQHandler

// 2MHz Timer
#define TIMER_2MHz_RCC_APB1Periph       RCC_APB1Periph_TIM7
#define TIMER_2MHz_TIMER                TIM7

#endif // _HAL_H_
