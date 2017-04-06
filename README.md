[![Build Status](https://travis-ci.org/silver13/H8mini_blue_board.svg?branch=master)](https://travis-ci.org/silver13/H8mini_blue_board)

###H8mini Blue board version###

Specific Blue instructions:
https://www.rcgroups.com/forums/showthread.php?2721755-H8-blue-board-flashing-instructions#post35501995


From rcg user __kmtam__
> Hi all, I finally got the h8mini blue board erased and flashed to Silverxxx firmware after zillion times of tried and error.
> The board's labelled wrong with the CLK and DAT got swapped....
> 
> This is how I got it work from the STLINK to the H8mini blue board:
> SWDIO->CLK
> GND->GND
> SWCLK->DAT
> 
> Thanks Silverxxx for this wonderful firmware! Cheers! 

From rcg user __ketsa__
> Thanks!!! I finally managed to flash my blue board.
> 
> It was not straightforward, I tried the suggested connections and was still getting "ST-LINK USB communication error."
> 
> I then found a file in my keil directory named "ST-LinkUpgrade.exe" So I Upgraded the FW of my ST-link to "V2.J24.S4 > > > STM32+STM8 Debugger"
> 
> Then i was getting a different message : "no target" hmm. progress. but still not flashing.
> Then I removed the battery, plugged it in and it erased !
> 
> I tried to flash : "no target."
> 
> Replug battery, and it flashed !

From rcg user __silverxxx__
> I made a blue board quad to see what's happening with the flashes.
> 
> So far I found out that it does indeed stop working after the st-link gets disconnected from usb.
> This seems some software issue, maybe with keil.
> 
> The st-link starts working after it is used in some other way , such as: flash another quad with keil(not blue board) , use > the "st-link utility" and connect to a board(even blue), use openocd and connect to a board (even blue).
> 
> After the "st-link usb communication error" comes up , you have to disconnect the st-link and reconnect it, or the above > fixes do not work.
> 
> Another find was that the nucleo built in st-link does not have this issue. It also does not get detected by the st-link firmware updater. A nucleo board is about $12
> 
> I'll have to find a more permanent solution, although openocd could be made a shortcut. 

_The last 2 posts refer to windows and keil only_

This port supports both acro only and dual mode. To enable acro only, an option exist in config.h.

The H8 board - blue version - uses a stm32f030 and a XN297L 3 wire radio. The XN297L is differerent from the XN297 in that is supports 250K rate, and the debug registers are different.  [XN297L datasheet](https://drive.google.com/file/d/0B3AKcbg1PFrnbHRXMzUzUUFmUFk/view?pref=2&pli=1) (use google translate)

### Flashing instructions
CG023 instructions:
http://www.rcgroups.com/forums/showthread.php?t=2634611

Specific Blue instructions:
https://www.rcgroups.com/forums/showthread.php?2721755-H8-blue-board-flashing-instructions#post35501995

###Linux/gcc version
The gcc version compiles close to 16k, and may need turning off features in order to make it fit. Read __install.md__ for additional information.

###Wiki
http://sirdomsen.diskstation.me/dokuwiki/doku.php?id=start

###Updates:

###01.12.16
* devo telemetry
* high angle - up to 90 in level mode
* autovdrop
* trims on stock tx for switching

###11.08.16
* added tx autocenter for stock tx
* added extra devo channels that were MIA
* bluetooth beacon was added previously

###03.07.16
* added updates from cg023 build
* now using hardware i2c for full speed (1 Mhz)

###30.05.16
* fixed low rates control issue in yaw+roll

###19.05
* added gestures
* changed imu from the experimental one that somehow made it in, to the old one

###11.05
* added linux compilation support

###10.05
* minor update
* additional protocols now work
