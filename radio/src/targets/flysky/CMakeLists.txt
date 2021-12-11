option(SHUTDOWN_CONFIRMATION "Shutdown confirmation" OFF)
option(LCD_DUAL_BUFFER "Dual LCD Buffer" OFF)
option(TARANIS_INTERNAL_PPM "Taranis internal module hack to output PPM" OFF)

if(PCB STREQUAL I6X)
  set(PWR_BUTTON "PRESS")
  set(SPLASH "OFF")
  set(CPU_TYPE STM32F0)
  set(CPU_TYPE_FULL STM32F072)  # for size report
  set(LINKER_SCRIPT targets/flysky/STM32F072VB_FLASH.ld)
  set(HAPTIC NO)
  set(LUA_EXPORT lua_export_x7)
  set(FLAVOUR i6x)
  set(NANO YES)
  set(GVARS NO)
  set(FRSKY_HUB NO)
  set(LUA NO)
  set(GUI_DIR 128x64)
  set(NAVIGATION_TYPE 9x)
  set(BITMAPS_TARGET 9x_bitmaps)
  set(FONTS_TARGET 9x_fonts_1bit)
  set(LCD_DRIVER lcd_driver.cpp)
  set(DEFAULT_MODE 2)
  add_definitions(-DTELEMETRY_FRSKY)
  add_definitions(-DCROSSFIRE)
  add_definitions(-DNAVIGATION_POT1)
  add_definitions(-DNAVIGATION_POT2)
  add_definitions(-DSTM32F072)
  add_definitions(-DPWR_BUTTON_${PWR_BUTTON})
  add_definitions(-DEEPROM_VARIANT=0)
  add_definitions(-DSOFTWARE_VOLUME)
  set(GUI_DIR 128x64)
  set(BITMAPS_TARGET 9x_bitmaps)
  set(FONTS_TARGET 9x_fonts_1bit)
  set(GVAR_SCREEN model_gvars.cpp)
  set(SERIAL2_DRIVER serial2_driver.cpp)
endif()


set(TARGET_SRC
  ${TARGET_SRC}
  A7105_SPI.cpp
  AFHDS2A_a7105.cpp
  intmodule_pulses_driver.cpp
  trainer_driver.cpp
  )

set(HSE_VALUE 8000000)
set(SDCARD NO)
set(EEPROM EEPROM_RLC)
set(TARGET_DIR flysky)
set(PPM_LIMITS_SYMETRICAL YES)
add_definitions(-DPCBI6 -DBUZZER)
add_definitions(-DPCBI6_USB_MSD)
# add_definitions(-DMENU_DIAGKEYS)
# add_definitions(-DMENU_DIAGANAS)
# add_definitions(-DPCBI6_ELRSV2) # ~6kB FLASH
set(GUI_SRC ${GUI_SRC}
  model_input_edit.cpp
  model_mix_edit.cpp
  model_display.cpp
  radio_diagkeys.cpp
  radio_diaganas.cpp
  radio_hardware.cpp
  view_channels.cpp
  view_telemetry.cpp
  view_about.cpp
  )

set(SRC ${SRC} buzzer.cpp)

set(TARGET_SRC
  ${TARGET_SRC}
  board.cpp
  buzzer_driver.cpp
  backlight_driver.cpp
  extmodule_driver.cpp
  adc_driver.cpp
  i2c_common_v2.cpp
  i2c_driver.cpp
  crc_driver.cpp
  tools/elrsV2.cpp
  ../common/arm/stm32/mixer_scheduler_driver.cpp
  )

set(FIRMWARE_TARGET_SRC
  ${FIRMWARE_TARGET_SRC}
  lcd_driver.cpp
  pwr_driver.cpp
  )
set(FIRMWARE_DEPENDENCIES ${FIRMWARE_DEPENDENCIES} ${BITMAPS_TARGET})
