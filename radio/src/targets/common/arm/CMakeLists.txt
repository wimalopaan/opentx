set(TIMERS 3 CACHE STRING "Timers count (2 or 3)")
set_property(CACHE TIMERS PROPERTY STRINGS 2 3)
option(CLI "Command Line Interface" OFF)
option(DEBUG "Debug mode" OFF)
option(LOG_TELEMETRY "Telemetry Logs on SD card" OFF)
option(TRACE_SD_CARD "Traces SD enabled" OFF)
option(TRACE_FATFS "Traces FatFS enabled" OFF)
option(TRACE_AUDIO "Traces audio enabled" OFF)
option(DEBUG_TRACE_BUFFER "Debug Trace Screen" OFF)
option(CROSSFIRE "Crossfire TX Module" ON)
option(MULTIMODULE "DIY Multiprotocol TX Module (https://github.com/pascallanger/DIY-Multiprotocol-TX-Module)" OFF)
option(SUPPORT_D16_EU_ONLY "XJT module only supports D16-EU and LR12-EU" OFF) # TODO rename to XJT_EU_ONLY
option(DEBUG_INTERRUPTS "Count interrupts" OFF)
option(DEBUG_USB_INTERRUPTS "Count individual USB interrupts" OFF)
option(DEBUG_TASKS "Task switching statistics" OFF)
option(DEBUG_TIMERS "Time critical parts of the code" OFF)

if(TIMERS EQUAL 3)
  add_definitions(-DTIMERS=3)
else()
  add_definitions(-DTIMERS=2)
endif()

if(LOG_TELEMETRY)
  add_definitions(-DLOG_TELEMETRY)
endif()

if(TRACE_SD_CARD)
  add_definitions(-DTRACE_SD_CARD)
  set(DEBUG ON)
  set(DEBUG_TRACE_BUFFER ON)
endif()

if(TRACE_FATFS)
  add_definitions(-DTRACE_FATFS)
  set(DEBUG ON)
  set(DEBUG_TRACE_BUFFER ON)
endif()

if(TRACE_AUDIO)
  add_definitions(-DTRACE_AUDIO)
  set(DEBUG ON)
  set(DEBUG_TRACE_BUFFER ON)
endif()

if(DEBUG_TRACE_BUFFER)
  add_definitions(-DDEBUG_TRACE_BUFFER)
endif()

if(DEBUG_INTERRUPTS)
  add_definitions(-DDEBUG_INTERRUPTS)
  set(DEBUG ON)
  if(DEBUG_USB_INTERRUPTS)
    add_definitions(-DDEBUG_USB_INTERRUPTS)
  endif()
endif()

if(DEBUG_TASKS)
  add_definitions(-DDEBUG_TASKS)
  set(DEBUG ON)
endif()

if(DEBUG_TIMERS)
  add_definitions(-DDEBUG_TIMERS)
  set(DEBUG ON)
endif()

if(CLI)
  add_definitions(-DCLI)
  set(FIRMWARE_SRC ${FIRMWARE_SRC} cli.cpp)
  set(SRC ${SRC} dump.cpp)
elseif(DEBUG)
  set(SRC ${SRC} dump.cpp)
endif()

if(DEBUG)
  add_definitions(-DDEBUG)
endif()

if(SUPPORT_D16_EU_ONLY)
  add_definitions(-DMODULE_D16_EU_ONLY_SUPPORT)
endif()

if(HAPTIC)
  add_definitions(-DHAPTIC)
  set(SRC ${SRC} haptic.cpp)
  set(TARGET_SRC ${TARGET_SRC} haptic_driver.cpp)
endif()

if(MULTIMODULE)
  add_definitions(-DMULTIMODULE)
  set(SRC ${SRC} pulses/multi.cpp telemetry/spektrum.cpp telemetry/flysky_ibus.cpp telemetry/multi.cpp)
endif()

if(CROSSFIRE)
  add_definitions(-DCROSSFIRE)
  set(PULSES_SRC
    ${PULSES_SRC}
    crossfire.cpp
    )
  set(SRC
    ${SRC}
    telemetry/crossfire.cpp
    )
endif()


# Protocols supported

# if(PPM)
  add_definitions(-DPPM)
# endif()


if(PCB STREQUAL I6X)
  set(SRC ${SRC} telemetry/flysky_ibus.cpp)
  add_definitions(-DTELEMETRY_FRSKY)
endif()

if(NOT PCB STREQUAL I6X)
add_definitions(-DTELEMETRY_FRSKY -DTELEMETRY_FRSKY_SPORT -DGPS -DDSM2 -DPXX)
endif()
add_definitions(-DBOLD_FONT -DBATTGRAPH -DTHRTRACE)
add_definitions(-DREQUIRED_SDCARD_VERSION="${SDCARD_VERSION}")
include_directories(${COOS_DIR} ${COOS_DIR}/kernel ${COOS_DIR}/portable)
foreach(LANGUAGE ${TTS_LANGUAGES})
  string(TOLOWER ${LANGUAGE} lang_lower)
  set(SRC ${SRC} translations/tts_${lang_lower}.cpp)
endforeach()
set(SRC
  ${SRC}
  main.cpp
  tasks.cpp
  telemetry/telemetry.cpp
  telemetry/telemetry_holders.cpp
  telemetry/telemetry_sensors.cpp
  crc.cpp
  vario.cpp
  )
  
if(PCB STREQUAL I6X)
else()
set(SRC
  ${SRC}
  audio.cpp
  io/frsky_sport.cpp
  telemetry/frsky.cpp
  telemetry/frsky_d_arm.cpp
  telemetry/frsky_sport.cpp
  )
endif()
if(NOT CPU_TYPE STREQUAL STM32F0)
set(FIRMWARE_TARGET_SRC
  ${FIRMWARE_TARGET_SRC}
  diskio.cpp
)
endif()
set(TARGET_SRC
  ${TARGET_SRC}
  keys_driver.cpp
  telemetry_driver.cpp
  )
set(FIRMWARE_SRC
  ${FIRMWARE_SRC}
  ${COOS_DIR}/kernel/core.c
  ${COOS_DIR}/kernel/hook.c
  ${COOS_DIR}/kernel/task.c
  ${COOS_DIR}/kernel/event.c
  ${COOS_DIR}/kernel/time.c
  ${COOS_DIR}/kernel/timer.c
  ${COOS_DIR}/kernel/flag.c
  ${COOS_DIR}/kernel/mutex.c
  ${COOS_DIR}/kernel/serviceReq.c
  ${COOS_DIR}/portable/GCC/port.c
  ${COOS_DIR}/portable/arch.c
  )

set(PULSES_SRC
  ${PULSES_SRC}
  pulses.cpp
  ppm.cpp
 ) 
if(NOT CPU_TYPE STREQUAL STM32F0)
set(PULSES_SRC
  ${PULSES_SRC}
  pxx.cpp
  pxx1.cpp
  pxx2.cpp
  dsm2.cpp
  sbus.cpp
  )
endif()
