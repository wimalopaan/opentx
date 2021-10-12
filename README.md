# FlySky i6x port of OpenTX

This is a port of OpenTX for the venerable Flysky I6X RC radio transmitter. You can find the latest build and some instructions and videos contributed by fellow early adopters in the [RCGroups thread](https://www.rcgroups.com/forums/showthread.php?3916435-FlySky-I6X-port-of-OpenTX)  

## Features implemented

* AFHDS2A protocol
* PPM out
* CRSF working with ExpressLRS at 115 and 400k bauds and with Crossfire modules at 115k bauds.
* Telemetry
* Buzzer audio alarms and indications.

## Users Telegram group

https://t.me/otx_flysky_i6x

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

* Add settings menu for ExpressLRS, since no LUA is supported.
* Reboots with Crossfire modules at 400k bauds.
* Enable mixer sync for CRSF.
* USB joystick mode.
* Bootloader. USB mass storage to backup EEPROM and update firmware.
* SDCARD support if it fits in flash.
* PPM in (Trainer)
* HELI support.

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
