# D_SeriesWheeStatFirmware
Updated WheeStat firmware
As of May 2017, there are two versions of WheeStat hardware released into the wild.  These are the 5 and 7 series hardware.  The 5 series hardware uses the WheeStat6 microcontroller software and the 7 series hardware uses the 7 series software.   
The original code shipped was version 6_c and 7_c.  As of May 2017, we began shipping with versions 6_d and 7_d.  The version of the hardware that you have can be determined by looking at printing on the green printed circuit board.  It should say either WheeStat5 or WheeStatE_7.  
To flash the firmware onto your microcontroller, you will need to have the altSPI folder and its contents in the 'libraries' folder of your Energia sketchbook.  The altSPI code can be found in the SmokyMountainScientific GitHub site.
The User interface will work with either version of the hardware.  The current version of the user interface is WheeStat6_e. The earlier version of the interface cannot be compiled by Processing 3.  It will also not be compatible with the new analysis software, WheeStatAnalysis1.  
