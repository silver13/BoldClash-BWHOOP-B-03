[![Build Status](https://travis-ci.org/silver13/BoldClash-BWHOOP-B-03.svg?branch=master)](https://travis-ci.org/silver13/BoldClash-BWHOOP-B-03)

## SilverWare - BoldClash BWHOOP B-03 version

_Also works with B03 Pro_

Currently does not account for centered throttle! ( no altitude hold ) It uses normal throttle, low = motors off. That means it should be used with Devo / multimodule or another tx.

Programming connector is a Microjst 1.25 pitch connector. Don't wire positive to st-link as you may overcharge the battery if connected or damage the st-link.

Programming instructions ( uses St-link Utility program ):
https://www.rcgroups.com/forums/showthread.php?2876797-Boldclash-bwhoop-B-03-opensource-firmware

Compiling instructions ( uses Keil uVision IDE):
https://www.rcgroups.com/forums/showthread.php?2877480-Compilation-instructions-for-silverware#post37391059

### Level / acro mode change
By default "gestures" are used to change modes, move the stick *left-left-down* for acro mode , and *right-right-down* for level mode.

### Pid gestures and save
Pid gestures allow the pilot to change the acro mode pids by a percentage, and such tune the quadcopter without a computer. The new pids can be saved so that they will be restored after the quad is power cycled.The new pids will remain active if saved, until the pid values in file pid.c are changed. If the values are not changed, flashing the quad will not erase the pids unless the erase command is manually issued when programming.

*See also*
http://sirdomsen.diskstation.me/dokuwiki/doku.php?id=pidgesture

### Accel calibration / pid save
The gesture for accel calibration is down - down - down. If pids have been changed using the respective gestures since the last powerup or save, the pids will be saved instead.

### Telemetry
DeviationTx and multimodule+taranis can support telemetry, this requires no changes from the defaults on the quad. For devo, when selecting the Bayang protocol, hit Enter to see options, and enable telemetry there. Telemetry currently contains received number of data and telemetry packets, and 2 voltages, battery raw voltage and compensated voltage ( against voltage drop )

For multimodule, you need to add telemetry ( as an option ) to the bayang protocol in the protocol table ( configuration file ). Note the telemetry protocol won't work with stock quads.

*See also*
http://sirdomsen.diskstation.me/dokuwiki/doku.php?id=telemetry

### Android App telemetry
The Android app "SilverVISE" by SilverAG (not me) is able to receive telemetry from the quad using BLE packets. To use, the app protocol should be set in the quadcopter. The app will also show the pids.

*More information*
http://sirdomsen.diskstation.me/dokuwiki/doku.php?id=silvervise

### Linux/gcc version
The gcc version compiles close to 16k, and may need turning off features in order to make it fit. It's also possible to flash up to 32K with changes. Read __install.md__ for additional information. 

The boldclash settings may need changes to work with gcc well. Turn off the software lpf filters except the last 2, the loop time is longer, at 1400, which affects the frequency of other soft lpf filters. You could set loop time to 1500 but it may not be needed.

### Wiki
http://sirdomsen.diskstation.me/dokuwiki/doku.php?id=start

### 01.18
* 2 new D term filters, 1st and 2nd order with custom frequency
* improved led flash timing
* the throttle smooth feature is gone 
* nrf24 support added, bayang protocol + telemetry
* softi2c support for 1 pullup only ( e011 )

### 29.08.17
* moved flash save to 31k
* added 2 looptime independent soft lpf
* added lowerthrottlemix3 from h8
* added motor filter 2 replacing original,much better performance
* pid save has own led flash now
* cleanup of unused features: headless and some other


### xx.06.17
* pid gestures - pids can be changed by gestures at the field
* pid save - new pids are saved to flash by the d-d-d gesture

### 02.05.17
* level pid is tuned more aggressive
* added filter between level pid and  acro pid
* fixed issue where "Overclock" option broke level mode (i2c) - overclock can be used on the bwhoop now
* removed level mode I term - this was not usually used
* minor cleanup and efficiency
* GCC autovdrop now functional
* LVC throttle lowering disabled by default just in case it malfunctions

### initial changes

* autovdrop improved, and better compensation
* new measured motor curve at 24K pwm
* added a new way of lowering throttle below a certain voltage
* lvc now flashes below an "uncompensated" voltage, too, just in case



## brianquad Notes - Additions for Analog Aux channels
Last major update 2018-08-19

This option (controlled in config.h) adds support for Analog Aux channels to control certain pre-programmed features. These are intended to be used with a transmitter with knobs/sliders to easily alter parameters while flying. These are controlled by #define lines in config.h. Enable these features by uncommenting the "#define USE_ANALOG_AUX" line. Commenting that line disables all analog aux channels at the compiler level, meaning they do not make the built firmware bigger or slower than it was before.

Initially, these features include:
1. Analog Rate Multiplier (ANALOG_RATE_MULT)
   - Use a tranmitter knob to control your rates to help find your sweet spot without flashing in between
   - Set your MAX_RATE and MAX_RATE_YAW to the highest rate you might want
   - Use the assigned knob to adjust beween 0 and 100% of that rate in a linear scale
     - Putting the knob at its middle point will give you half of your MAX_RATE
2. Analog Max Angle for Level mode (ANALOG_MAX_ANGLE)
   - When in Angle/Level mode, the maximum angle the quad is allowed to tilt (controlling your max speed, etc.) is set by the LEVEL_MAX_ANGLE define
   - When enabling ANALOG_MAX_ANGLE, the LEVEL_MAX_ANGLE define is ignored
   - The knob controls the maximum angle from 0 to 90 degrees in a linear scale
     - Putting the knob at its middle point will give you a maximum angle of 45 degrees
3. PID adjustments (ANALOG_RP_P, etc.)
   - Assigning a knob to one of these defines lets you alter that PID setting from 0.5X to 1.5X of the current setting in pid.c
   - Each of the P, I, or D for Roll, Pitch, and Yaw can be selected in config.h, or Roll and Pitch P, I, or D can be selected together on one knob
   - The PID adjustments can be saved, just like the classic Silverware gesture PID adjustments. To save a new value, use the Down Down Down (DDD) gesture to write the current PID values to flash (including your new one(s)) and re-center your adjusted values. This means to keep your new value after saving, you must re-center your knob/slider.

These initial features are mostly meant to start a conversation on how Analog Aux channels could be used. For example, I'm sure there are better ways to do live PID adjustment with a couple of analog knobs!

###How do you access/assign analog channels? What channels can be used?

For Sbus and DSM, you can assign any of the channels to use as analog aux channels.

For Bayang, you can use a modified version of the Bayang protocol I've made to the Multiprotocol Tx and Deviation Tx firmware that adds two 8-bit analog channels to the protocol.

The Multiprotocol module uses channels 14 and 15 for these analog channels. Set the "Option/Telemetry" value for the Bayang protocol on the Taranis to 2 or 3 (2 to get only the analog channels, 3 to get both Telemetry and the analog channels).

Deviation uses channels 11 and 12. Enable the Aux Analog option for the Bayang protocol.

For both the Multiprotocol module and Deviation, Silverware will not bind with a transmitter that does not have matching options (both Telemetry and Analog Aux channels).

These modifications can be found on the analog aux branch in my forks on GitHub (for now, you _must_ select the branch rather than master):
	https://github.com/brianquad/DIY-Multiprotocol-TX-Module/tree/bayang-analog-aux
	https://github.com/brianquad/deviation/tree/bayang-analog-aux

####How to use Trim Switches for Silverware Analog Aux channels in Deviation
VIRTUAL POT (Example, by Ian444):

1. Say you want to assign LV (left vertical) trim switch to drive the radio channel 11 as if it were a pot, each click driving the pot in one direction or the other, using Virt3 as the intermediate link. (It could be any radio channel and use any free virtual channel.)

2. Go into the TRIMS menu and set the LV trim to Virt3, so replace "whatever-is-there" with Virt3 i.e. it should look like: Virt3/step size/TRIMLV+. You can highlight "Virt3" and hit enter to get into the submenu if you need to. Set step size, trim+, trim- etc. Step size 10 gives 10 clicks up and 10 clicks down. Save it.

3. Go into the MIXER menu and find Virt3, set the mixer type as "simple", Src is Virt3, curve is 1-to-1, scale is 0 and offset is 0. Save it. The output of Virt3 is -100 to +100 with 0 being centre. You can check this in the Transmitter Menu/Channel Monitor Virt3 bar graph display.

4. Go into the mixer menu and find the Ch11 mixer and set this: mixer type is complex, Mixers is 1, Page is 1, Switch is none, Mux is replace, Src is Virt3, Curve is 1-to-1, scale is 100, offest is 0, and set NO TRIM instead of TRIM. You can also set limits in the Ch11 first menu if required.

5. Check the radio output in the Transmitter Menu/Channel Monitor, you should see the CH11 output following the trim switch.

