//  ASV_CSV tab / StellarisWheeStat sketch
//  In anodic stripping voltammetry, the working electrode is initially held
//    at a voltage negative of that where the analyte is reduced for a 
//    given plating time.  The reduced analyte is precipitated onto the
//    electrode and the amount on the electrode is analyzed by measuring 
//    current passed as the voltage is scanned positive through the potential
//    at which the material is re-oxidized.

void ramp()  {

    int nThru = 1;    // number of times through the 
  if (mode == CV){
    nThru = 2;
  }
  if (mode == multiCV){
    nThru = runs*2;
  }
  int start;
  boolean slopeDir = 0;   // initially slope is assumed negative
  boolean evenOdd = 0;    // keeps track of where to start 
  if (dInit<dFnl) {
  slopeDir = 1;           // slopeDir = 1 if positive slope
  }
////////// Plating /////////////////////////////////////////////////  
 
  digitalWrite(pulse_pin,LOW);              // set pulse pin to low
  PWMWrite(signal_pin,pwmRes,dInit,pwmClock);  // set signal voltage to 512 + 1/2 Vinit

  delay(delay1*1000);

  
  for(int p = 0; p<nThru; p++){
    if(evenOdd == 0){
    start = dInit;
    }
    else{
      start = dFnl;
    }
      if (slopeDir == 1) {
    pRamp(start);
  }
  else {
    nRamp(start);
  }
  slopeDir =! slopeDir;   // toggle slopeDir
  evenOdd =! evenOdd;
  }

  openCircuit();    // go to open circuit
}
/*  if (dInit<dFnl) {
    pRamp(dInit);
  }
  else {
    nRamp(dInit);
  }
  if (mode == CV) {              ///// CV experiment
    if (dInit<dFnl) {
      nRamp(dFnl);
    }
    else {
      pRamp(dFnl);
    }
  }
  openCircuit();    // go to open circuit
}*/
///////// Ramping ////////////////////////////////////////////////
void pRamp(int start)  {
 //  if (runState == true){
     for (int dC = 0; dC <= nSteps; ++dC){  
     if (runState == true){
       dSig = start + (dC*pwm_step);      // calculate digital Signal value, step =3
    PWMWrite(signal_pin,pwmRes,dSig,pwmClock);

    delay(stepTime);  
    int calcV = dSig*vcc/1024 - dRef*vcc/1024;
    readVolts();
    readCurrent(true);
 //   Serial.print(",");
    Serial.print(iMin);
    Serial.print(",");
    Serial.println(iMax);

//   Serial.println("");

  if(Serial.available()>0) {
    sRead = Serial.read();
    if (sRead == '%') {
      runState = false;
    }
  }  
}
}
//else {}
}

void nRamp(int start)  {
 //if (runState == true){
   for (int dC = 0; dC <= nSteps; ++dC){  
    if (runState == true){
      dSig = start - (dC*pwm_step);      // calculate digital Signal value, step =3
    PWMWrite(signal_pin,pwmRes,dSig,pwmClock);

    delay(stepTime);  
    int calcV = dSig*vcc/1024 - dRef*vcc/1024;
    readVolts();
    readCurrent(true);
 //   Serial.print(",");
    Serial.print(iMin);
    Serial.print(",");
    Serial.println(iMax);

 //   Serial.println("");
 //   status();
   if(Serial.available()>0) {
     sRead = Serial.read();
     if (sRead == '%') {
       runState = false;
     }
 /*  rsStatus = Serial.find("%");  }
      if (rsStatus == true){
     HWREG(NVIC_APINT) = NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ; */       
      }
  } 
}
//else {}
}

