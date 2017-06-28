// Open_Circuit tab

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
 // while (mVi < zeroI-6) {
  else if (mVi < zeroI-6) {
    fixed = false;
 --openCirc;
  }
 else {
   fixed = true;
   }
 oCircRun = true;
// }
}
