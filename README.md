# FlySky i6X port of OpenTX

This is a port of **OpenTX** for the venerable Flysky i6X RC radio transmitter. You can find instructions and videos contributed by fellow early adopters in the [RCGroups thread](https://www.rcgroups.com/forums/showthread.php?3916435-FlySky-I6X-port-of-OpenTX).

![OpenI6X](https://circleci.com/gh/OpenI6X/opentx.svg?style=shield)
[![Release](https://img.shields.io/github/v/release/OpenI6X/opentx?include_prereleases)](https://github.com/OpenI6X/opentx/releases)
[![GitHub all releases](https://img.shields.io/github/downloads/OpenI6X/opentx/total)](https://github.com/OpenI6X/opentx/releases)
[![xx](https://img.shields.io/badge/telegram-group-blue)](https://t.me/otx_flysky_i6x)

## Installation

Go to the wiki for detailed steps: https://github.com/OpenI6X/opentx/wiki

## Implemented features 

* AFHDS2A protocol
* PPM in/out
* CRSF with ExpressLRS and Crossfire modules, CRSFshot enabled
  * MEGA Bauds up to 1.8M
  * ExpressLRS V2 configuration (ELRSV2.lua port)
* Telemetry
* Audio tones, alarms and vario custom implementation
* Bootloader (DFU bootloader) - Start by pushing trims to the center, like regular OpenTX one
* USB (To enable on standard cable: General Settings > USB Detect set to "Once")
  * Joystick mode
  * Mass Storage to backup/restore eeprom

## Navigation:

| Key | Function |
| --- | --- |
| UP     | Hold for stats |                              
| DOWN   | Hold for telemetry |                                  
| OK     | Hold for Model menu |
| CANCEL | EXIT. Hold for graceful shutdown (Needed to save timers) |                      
| BIND   | Go RIGHT. Go to next PAGE. Hold for general menu. |
| RIGHT POT | Scroll right or left |

## Contributors

* ExpressLRS V2 config, USB support, sound support, bootloader, backporting, bugfixing - Janek ([ajjjjjjjj](https://github.com/ajjjjjjjj)) ongoing collaboration.
* All the RF code was taken from the great KotelloRC's [erfly6: Er9X for i6 and i6x](https://bitbucket.org/KotelloRC/erfly6/src/master/).
* ADC code taken from [OpenGround](https://github.com/fishpepper/OpenGround).
* This work is based on Jakub's ([qba667](https://github.com/qba667)) work and is forked from his repo.
* All the contributors of [OpenTX](https://github.com/opentx/opentx/). 
