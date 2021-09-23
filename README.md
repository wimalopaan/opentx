# FlySky i6x port of OpenTX

This is a port of OpenTX for the venerable. You can find in the [RCGroups thread](https://www.rcgroups.com/forums/showthread.php?3916435-FlySky-I6X-port-of-OpenTX) the latest build and some instructions and videos contributed by fellow early adopters. 

## Navigation:

| Key | Function |
| --- | --- |
| UP     | Hold for stats |                              
| DOWN   | Hold for telemetry |                                  
| OK     | Hold for Model menu |
| CANCEL | EXIT. Hold for graceful shutdown (Needed to save timers) |                      
| BIND   | Go RIGHT. Go to next PAGE. Hold for general menu. |
| RIGHT POT | Scroll right or left |

## Next tasks/Known issues:

* External module support
    - PPM
    - Crossfire
* Buzzer: battery, rssi default alarms do not work
* Buzzer: various sound effects selectable from functions.
* Buzzer: pitch adjustment
* Can't restart internal module after turning it off.
* Add settings for ExpressLRS, since no LUA.
* USB support (board.h usbplugged)
* UART DMA fifo transfers.
* Bootloader if it fits. Mass storage to backup EEPROM and update firmware.
* SDCARD support if it fits in flash.
* Go over the radio menus and correct blank spaces, remove unused options.

## Completed tasks:

* Buzzer support (basic beeps and volume adjustment)
* Latency down from 40.1 ms to 15.4 ms (Stock firmware latency: 22.1 ms) Measured with [ExpressLRS RCLatencyTester](https://github.com/ExpressLRS/RClatencyTester)
* Backported mixer scheduler
* AFHDS2A - Remaining settings complete: Subtype: PWM/PPM-IBUS/SBUS, Servo frequency
* AFHDS2A - Telemetry (RSSI/LQI, RX voltage, etc.)
* AFHDS2A - Binding
* AFHDS2A - Channels transmission
* AFHDS2A - Range test (low power mode with RSSI indication)
* AFHDS2A - Failsafe working
* LCD contrast setting
* Trims working correctly.
* Set good calibration defaults on factory reset, taken from erfly6.
* Fixed radio and model settings garbled text and crashes.
* Switches warnings now dissapear by moving the switches.
* Switches working.
* Removed graceful shutdown with right horizontal trim up. Not needed it seems.
* Factory reset fixed on version screen.
* Throttle on left. Default mode: 2
* EEPROM compression enabled. No more errors, lots of free space.
* EEPROM reading and writing.
* Serial port debug working on PA9/PA10 pads.
* ADC readings implemented, from OpenGround.
* Mixer enabled.
* Enable WDT reset every 500ms.
* Fix inverted sticks and POTS.
* Fix battery voltage calculation.
* Fix crash in calibration checksum. 

## ST-Link pinout

![ST-Link pinout](https://raw.githubusercontent.com/marianomd/opentx/2.3_M0/doc/flysky/flysky-i6x%20st-link%20pinout%20small.png)

## Linux instructions
### Compile
docker run --rm -it -e "BOARD_NAME=I6X" -e "CMAKE_FLAGS=PCB=I6X HELI=NO GVARS=NO LUA_COMPILER=NO MULTIMODULE=NO DEBUG=YES" -v $PWD:/opentx vitass/opentx-fw-build

### Flash
sudo st-flash write <file_to_flash>.bin 0x08000000
sudo st-flash reset

### Debug tty:
sudo cat /dev/ttyUSB0 115200

## Contributors

* Janek (@ajjjjjjjj) ongoing collaboration.
* All the RF code was taken from the great KotelloRC's erfly6: Er9X for i6 and i6x. You can find his project here: https://bitbucket.org/KotelloRC/erfly6/src/master/
* ADC code taken from OpenGround: https://github.com/fishpepper/OpenGround
* This work is based on Jakub's (qba667) work and is forked from his repo.
* All the contributors of OpenTX. 
