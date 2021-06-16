## FlySky i6x port of OpenTX

Continuing Jakub's (qba667) work on 2.3_M0 Branch

Next to tackle:

* Switches warnings don't dissapear by moving the switches.
* Radio and model settings crashes. A complete review of each screen is needed.
* Internal module. Currently no A7105 support.
* External module support. PXX, CRSF. Sbus?
* Add settings for ExpressLRS, since no LUA.
* Telemetry
* USB support (board.h usbplugged)
* UART DMA fifo transfers.

Completed tasks:

* Switches working!
* Removed graceful shutdown with right horizontal trim up. Not needed it seems.
* Restored Enter key as MENU. Hold for model settings.
* Factory reset fixed on version screen. Navigate to the left as Special functions menu crashes currently.
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
* Left horizontal trim as LEFT & RIGHT keys. We don't need trims with Betaflight or iNav.
