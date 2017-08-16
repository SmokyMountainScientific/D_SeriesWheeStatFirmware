// Open_Circuit tab, WheeStatE_7d
// if kill switch incorporated, comment out lines 5-26, uncomment lines 27 to end

void openCircuit() {
  boolean fixed;                                  
  if (oCircRun == false) {          // use existing open circuit value?
    openCirc = (dInit + dFnl)/2;
  }  
  PWMWrite(offset_pin,pwmRes,halfRes,pwmClock);   //added Jan 2, 2014
 PWMWrite(signal_pin,pwmRes,openCirc,pwmClock);  // set signal pin to open circuit
 readCurrent(false);        //reads current, does not Serial print
 int zeroI = vcc/2;
// while (mVi > zeroI+6) {
    if (mVi > zeroI+6) {
    fixed = false;
 ++openCirc;
 }
  else if (mVi < zeroI-6) {
    fixed = false;
 --openCirc;
  }
 else {
   fixed = true;
   }
 oCircRun = true;
}
/*
digitalWrite(kill,HIGH);
 PWMWrite(offset_pin,pwmRes,halfRes,pwmClock);   //added Jan 2, 2014
 PWMWrite(signal_pin,pwmRes,halfRes,pwmClock);  // set signal pin to open circuit
 }
 */
