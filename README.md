## FlySky i6x port of OpenTX

Continuing Jakub's (qba667) work on 2.3_M0 Branch

Next to tackle:

* Internal module
    - AFHDS2A
        - Binding [done]
        - Channels transmission [done]
        - Range test (low power mode with RSSI indication) [done]
        - Roll and Yaw channels are sent swapped
        - Failsafe not set up correctly
        - Missing menu settings: Subtype: PWM/PPM-IBUS/SBUS, Servo frequency
        - Improve mixer scheduling
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

* LCD contrast setting
* Trims working correctly.
* Set good calibration defaults on factory reset, taken from erfly6.
* Restored BIND as SHIFT. SHIFT+UP for LEFT, SHIFT+DN for RIGHT. Trims freed.
* Fixed radio and model settings garbled text and crashes.
* Switches warnings now dissapear by moving the switches.
* Switches working!
* Removed graceful shutdown with right horizontal trim up. Not needed it seems.
* Restored Enter key as MENU. Hold for model settings.
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
