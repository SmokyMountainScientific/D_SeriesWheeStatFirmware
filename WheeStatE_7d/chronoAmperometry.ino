//  chronoAmperometry tab / WheeStatE_7d on GitHub SmokyMountainScientific
//  chronoamperometry and two step chronoamperometry experiments


void chronAmp()  {
  ////////// ////////////////////////////////////////////////  
  PWMWrite(signal_pin,pwmRes,dInit,pwmClock);  // set signal voltage to 512 + 1/2 Vinit

///// from Ben's old stuff ////
  int Iread = 0;
  long inTime = 0;
  long time2;                 
  delay(delay1*1000);
  long time1 = millis();
  ///////// Step and read ////////////////////////////////////////////////
    PWMWrite(signal_pin,pwmRes,dFnl,pwmClock);    
    while(inTime <= readTime && runState == true){     
    Iread = 0;
    time2 = millis();
    inTime = time2-time1;  //inTime is in ms, time1 and time2 are micros      
    Serial.print(inTime);
    Serial.print(",");             
    readCurrent(true);
    Serial.print(iMin);
    Serial.print(",");
    Serial.println(iMax);
    delay(runDelay);

    // If "Stop" is pressed in GUI, stop the program 
    if(Serial.available()>0) {
    sRead = Serial.read();
    if (sRead == '%') {
      runState = false;
    }
  }
    }
 if(mode == chronoAmp2) {
   PWMWrite(signal_pin,pwmRes,dInit,pwmClock);  // step back to initial voltage
     while(inTime <= readTime*2 && runState == true){    
       Iread = 0;     
    time2 = millis();
    inTime = time2-time1;  //inTime is in ms, time1 and time2 are micros      
    Serial.print(inTime);
    Serial.print(",");          // working here
    readCurrent(true);
    Serial.print(iMin);
    Serial.print(",");
    Serial.println(iMax);//    Serial.println("");
    delay(runDelay);
    }

    // If "Stop" is pressed in GUI, stop the program 
    if(Serial.available()>0) {
    sRead = Serial.read();
    if (sRead == '%') {
      runState = false;
    }
  }
 }
  
  openCircuit();    // go to open circuit
 }

