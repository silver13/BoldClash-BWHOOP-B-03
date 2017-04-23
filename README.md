[![Build Status](https://travis-ci.org/silver13/BoldClash-BWHOOP-B-03.svg?branch=master)](https://travis-ci.org/silver13/BoldClash-BWHOOP-B-03)

### BoldClash BWHOOP B-03

Currently does not account for centered throttle! ( no altitude hold )

### work in progress
Programming connector is a Microjst 1.25 pitch connector. Don't wire positive to st-link as you may overcharge the battery if connected or damage st-link.


The programming has the "keil usb bug" and as such it needs a workaround. The bug concerns windows only.


Specific instructions same as: BlueH8 instructions:
https://www.rcgroups.com/forums/showthread.php?2721755-H8-blue-board-flashing-instructions#post35501995


### Linux/gcc version
The gcc version compiles close to 16k, and may need turning off features in order to make it fit. Read __install.md__ for additional information.

### Wiki
http://sirdomsen.diskstation.me/dokuwiki/doku.php?id=start

### changes

* added a new way of lowering throttle below a certain voltage
* lvc now flashes below an "uncompensated" voltage, too, just in case
