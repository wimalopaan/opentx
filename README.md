## FlySky i6x port of OpenTX

Continuing Jakub's (qba667) work on 2.3_M0 Branch

Currently working on:

* EEPROM Data corruption: General settings are overwritten by model settings. Pseudo fat needs rework.
* Throttle is on right stick. On calibration screen sticks appear correctly, but throttle warning operates on right stick.
* Radio and model settings crashes. A complete review of each screen is needed.
* Pulses. Currently no A7105 support.
* Switches. Swithes are analog in i6x, This needs discrete switch emulation or treating them as POTs.
* USB support (board.h usbplugged)
* UART DMA fifo transfers.

Completed tasks:

* EEPROM reading and writing.
* Serial port debug working.
* ADC readings enabled
* Mixer enabled
* Enable WDT reset every 100ms (countdown was broken)
* Fix inverted sticks and POTS.
* Removed switches as sliders.
* Fix battery voltage calculation
* Fix crash in calibration checksum 
* Bind key as MENU 
* Left horizontal trim as LEFT & RIGHT keys. We don't need trims with Betaflight or iNav.
* Graceful shutdown with right horizontal trim up
