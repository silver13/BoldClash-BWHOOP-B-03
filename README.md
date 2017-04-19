[![Build Status](https://travis-ci.org/silver13/BoldClash-BWHOOP-B-03.svg?branch=master)](https://travis-ci.org/silver13/BoldClash-BWHOOP-B-03)

### BoldClash BWHOOP B-03


### work in progress

The CLK and DAT pads on the board are marked in reverse. The same pads in the connector are marked correctly. The connector is wired ( Batt+) ( Gnd ) ( DAT ) ( SCK ) . It's a Microjst 1.25 pitch connector. Don't wire positive to st-link as you may overcharge the battery if conencted or damage st-link.


The programming has the "keil usb bug" and as such it needs a workaround.


Specific Blue instructions ( same flashing procedure):
https://www.rcgroups.com/forums/showthread.php?2721755-H8-blue-board-flashing-instructions#post35501995


### Linux/gcc version
The gcc version compiles close to 16k, and may need turning off features in order to make it fit. Read __install.md__ for additional information.

### Wiki
http://sirdomsen.diskstation.me/dokuwiki/doku.php?id=start

