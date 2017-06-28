//  DiffPulse tab / StellarisWheeStat sketch
//  In anodic stripping voltammetry, the working electrode is initially held
//    at a voltage negative of that where the analyte is reduced for a 
//    given plating time.  The reduced analyte is precipitated onto the
//    electrode and the amount on the electrode is analyzed by measuring 
//    current passed as the voltage is scanned positive through the potential
//    at which the material is re-oxidized.

void diffPulse()  {

  ///////// Stripping ////////////////////////////////////////////////

  for (int dC = 0; dC <= nSteps; ++dC){  
   if (runState == true){
      if (dInit < dFnl){
      dSig = dInit + (dC*pwm_step);      // calculate digital Signal value
    }
    else {
      dSig = dInit - (dC*pwm_step);
    }
    PWMWrite(signal_pin,pwmRes,dSig,pwmClock);
    digitalWrite(pulse_pin,HIGH);       
    delay(stepTime/2);  

    readVolts();
    readCurrent(true);
    digitalWrite(pulse_pin,LOW);       
    delay(stepTime/2);  
    readCurrent(true);
    Serial.print(iMin);
    Serial.print(",");
    Serial.println(iMax);
    //readStop();           // if GUI transmits '&', stop read
 //   Serial.println("");
  if(Serial.available()>0) {
    sRead = Serial.read();
    if (sRead == '%') {
      runState = false;
    }
  }  

    }
  }
  openCircuit();    // go to open circuit
 }



  void cleanDepos(){
  //////Electrode cleaning ////////////////
    digitalWrite(pulse_pin,LOW);              // set pulse pin to low
    PWMWrite(signal_pin,pwmRes,dClean,pwmClock);  // electrode cleaning voltage
    digitalWrite(stir_pin,HIGH);
    delay(2000);                     // 10 seconds at cleanning voltage
 
   ////////// Preconcentration /////////////////////////////////////////////////  
//  PWMWrite(signal_pin,pwmRes,dDeposit,pwmClock);  // set signal voltage to initial value
  PWMWrite(signal_pin,pwmRes,dInit,pwmClock);  // set signal voltage to initial value
  for (int m = 0; m<delay1; m++) {
    delay(1000);
    status();
  }
   //  delay(delay1*1000);
  /////// quiet solution //////////////////////////////////////////////////////
   digitalWrite(stir_pin,LOW); 
   PWMWrite(signal_pin,pwmRes,dInit,pwmClock);  // set signal voltage to initial value
   delay(500);
  }
  
    
