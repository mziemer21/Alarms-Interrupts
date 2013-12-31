Alarms-Interrupts
=================
alarm.c sets a software interrupt alarm to go off each time a specified interval passes. When the alarm 
goes off, signalling the running program, it invokes a function that acts as an interrupt handler. 
This handler function prints out the current time, in the same format as the Unix date program.
Args: <intervalSeconds> <totalRunTime>
Sample output from September 30 2013 with 2 second intervals:
    current time is Mon Sep 30 16:51:52 2013
    current time is Mon Sep 30 16:51:54 2013
    current time is Mon Sep 30 16:51:56 2013
    current time is Mon Sep 30 16:51:58 2013

kbint.c builds off of alarm.c's interrupt feature, but also catches keyboard interrupts.  kbint.c takes
in an extra string arg and changes it's case every time a interrupt is sent from the keyboard (Control-c).
Args: <intervalSeconds> <totalRunTime> <string> 
Sample output from September 30 2013 with 3 second intervals and string arg of abCDeFGhijk:
	current time is Mon Sep 30 17:02:46 2013
	^CABCDEFGHIJK
	^Cabcdefghijk
	^CABCDEFGHIJK
	current time is Mon Sep 30 17:02:49 2013
	current time is Mon Sep 30 17:02:52 2013
	^Cabcdefghijk

All programs were compiled with: gcc -Wall -m32 -O