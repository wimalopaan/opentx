## FlySky i6x port of OpenTX

Continuing Jakub's (qba667) work on 2.3_M0 Branch

Navigation:

| --- | --- |
| UP     | Hold for STATS                                    
| DOWN   | Hold for DEBUG                                    
| OK     | ENTER. Hold for Model menu                        
| CANCEL | EXIT                                              
| BIND   | Go RIGHT. Go to next PAGE. Hold for general menu. 

Next tasks:

* Internal module
    - AFHDS2A
        - Missing menu settings: Subtype: PWM/PPM-IBUS/SBUS, Servo frequency
        - Improve mixer scheduling (currently low refresh rate)
        - Can't restart internal module after turning it off.
        - Telemetry (RSSI/LQI, RX voltage, etc.)
* External module support
    - PPM
    - Crossfire
* Add settings for ExpressLRS, since no LUA.
* USB support (board.h usbplugged)
* UART DMA fifo transfers.
* Buzzer

Completed tasks:
* AFHDS2A - Binding
* AFHDS2A - Channels transmission
* AFHDS2A - Range test (low power mode with RSSI indication)
* AFHDS2A - Failsafe working
* LCD contrast setting
* Trims working correctly.
* Set good calibration defaults on factory reset, taken from erfly6.
* Fixed radio and model settings garbled text and crashes.
* Switches warnings now dissapear by moving the switches.
* Switches working!
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
