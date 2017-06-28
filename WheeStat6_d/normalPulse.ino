//  normalPulse tab / StellarisWheeStat5_4a sketch
//    status check added before second long delay;  5/28/14
//  Normal pulse voltammetry, experiment begun May 1, 2014
//    the working electrode is initially held at a starting voltage (vInit),
//    it is then stepped up an increment where the current is sampled   
//    after current sampling, the voltage is stepped back to the starting voltage.
//    Teh voltage is then stepped to a different voltage and the current is sampled there  
//    before being stepped back to the original starting voltage.  In successive steps,
//    the sampling voltage is incremented until the entire voltage range is sampled.

void normPulse()  {

  ////////// ////////////////////////////////////////////////  
  PWMWrite(signal_pin,pwmRes,dInit,pwmClock);  // set signal voltage to 512 + 1/2 Vinit

  delay(delay1*1000);

  ///////// Stepping ////////////////////////////////////////////////

  for (int dC = 0; dC <= nSteps; ++dC){  
   if (runState == true){
     if (dInit < dFnl){
      dSig = dInit + (dC*pwm_step);      // calculate digital Signal value
    }
    else {
      dSig = dInit - (dC*pwm_step);
    }
    PWMWrite(signal_pin,pwmRes,dSig,pwmClock);
    delay(readTime);        //stepTime/2 or readTime
    readVolts();
    readCurrent(true);
   Serial.print(iMin);
    Serial.print(",");
    Serial.println(iMax);

    PWMWrite(signal_pin,pwmRes,dInit,pwmClock);  // step back to initial voltage
    status();           // added 5/28/14, copied from titraumatic sketch 

    if(Serial.available()>0) {
    sRead = Serial.read();
    if (sRead == '%') {
      runState = false;
    }
  }  

    delay(delay1*1000);  
//    Serial.println("");
   }
  }
  openCircuit();    // go to open circuit
 }


