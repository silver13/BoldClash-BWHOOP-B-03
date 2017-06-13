[![Build Status](https://travis-ci.org/silver13/BoldClash-BWHOOP-B-03.svg?branch=master)](https://travis-ci.org/silver13/BoldClash-BWHOOP-B-03)

### BoldClash BWHOOP B-03

Currently does not account for centered throttle! ( no altitude hold ) It uses normal throttle, low = motors off.

Programming connector is a Microjst 1.25 pitch connector. Don't wire positive to st-link as you may overcharge the battery if connected or damage st-link.

Programming instructions ( uses St-link Utility program ):
https://www.rcgroups.com/forums/showthread.php?2876797-Boldclash-bwhoop-B-03-opensource-firmware

Compiling instructions ( uses Keil uVision IDE):
https://www.rcgroups.com/forums/showthread.php?2877480-Compilation-instructions-for-silverware#post37391059

### Level / acro mode change
By default "gestures" are used to change modes, move the stick left - left - down for acro mode , and right - right - down for level mode.

### Accel calibration / pid save
The gesture for accel calibration is down - down - down. If pids have been changed using the respective gestures since the last powerup or save, the pids will be saved instead.

### Linux/gcc version
The gcc version compiles close to 16k, and may need turning off features in order to make it fit. Read __install.md__ for additional information.

The boldclash settings need changes to work with gcc well, turn off pid save as it uses the last K of flash, likely needed for program space. Turn off the software lpf filter, the loop time is longer, at 1400, which affects the frequency of that filter. You could set loop time to 1500 but it may not be needed.

### Wiki
http://sirdomsen.diskstation.me/dokuwiki/doku.php?id=start


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
