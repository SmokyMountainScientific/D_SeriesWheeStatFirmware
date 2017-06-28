//digiPot tab:   / StellarisWheeStat sketch
//  controlls MCP 4231 digital potentiostat
//  working as of 10-23-13
//  resistance between wiper and terminal B, Rwb =  10K * n/128  + Rw (page 38 in doc)
//  terminal B is closer to pin 1 than wiper, terminal A is furthest.
//  Pot 0 is wired with pin B tied to wiper, R4 = 10 * (1-F0) Kohm, where F0 is the fraction of pot 0 max.  
//  Pot 1 is wired wiht pin A tied to wiper, R5 = 10 * F1 Kohm, where F1 is the fraction of the pot 1 max.
//  Current amplification = R4 * R6 / R5 (R6 is currently 10 Kohms).
//  Voltage at Iread pin = 10 * (1-F0) / F1 

void  setupDigiPot()  {             

  altSPI.setModule(0);
  altSPI.begin();
  altSPI.setClockDivider(SPI_CLOCK_DIV64);  // max clock speed = 10 MHz
  altSPI.setBitOrder(MSBFIRST);
  altSPI.setDataMode(SPI_MODE0);            // clock idles low, read on rising edge

  altSPI.trans2ByteA(0);   // command 0; write to channel 0
  altSPI.trans2ByteB(gain0);   // 

  altSPI.trans2ByteA(16);   // command 16; write to channel 1
  altSPI.trans2ByteB(gain1);  // 
  
  altSPI.end();
}



