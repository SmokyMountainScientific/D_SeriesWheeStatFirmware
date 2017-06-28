//setupRun Tab 
// Energia Sketch for WheeStat6_0a


void setupRun() {
  gotLims = false;   // setting up limit bars
  iMin = 0;
  iMax = 0;
  /////////////// begin reading parameters //////////
  boolean startread = false;
  while (startread == false) {
    if(Serial.available() > 0) {
      int starti = Serial.read();
      ////// new start code ///////////
      if(starti == '*') {           // Gui transmits * looking for com port
        Serial.println('&');         // LaunchPad responds to Gui with &
        digitalWrite(RED_LED,HIGH);
        delay(500);
        digitalWrite(RED_LED,LOW);
      }

      if(starti == '&') {  // 38) {         //GUI transmits & character to start read
        startread = true;
        runState = true;
      }
        starti = 0;
    }
 
  }       // end of while startRead == false loop
   

  //////////// mode ////////////////  

  Serial.print(",,Mode, ");   //double comma to get header out of data columns
  // Serial.print(",");
 // readParam();
while (Serial.available()<=0){ }
delay(20);
  mode= Serial.parseInt();   
  delay(20);  

  ///////////initial voltage///////
  Serial.print("mV init, ");
  while (Serial.available() <=0){ }
delay(20);
 
  mVinit=Serial.parseInt();
//  mVinit=Serial.parseInt()-2000;     // 2000 added in GUI to transfer positive value
  dInit = mVinit/10 + halfRes;       //digital value for pwm
  delay(20);  
   /*    digitalWrite(RED_LED,HIGH);
        delay(500);
        digitalWrite(RED_LED,LOW);
        delay(500);
        digitalWrite(RED_LED,HIGH);
        delay(500);
        digitalWrite(RED_LED,LOW); */

  ///////////final voltage///////
  Serial.print("mV Final, ");
    while (Serial.available() <=0){ }
delay(20);
  //  Serial.print(",");
//  readParam();
//  mVfnl=Serial.parseInt();
    mVfnl=Serial.parseInt();
    //-2000;       // 2000 added to value in GUI
  dFnl = mVfnl/10 + halfRes;       //digital value for pwm
  delay(20);

  ///////////scan rate///////
  Serial.print("mV/s, ");
    while (Serial.available() <=0){ }
delay(20);
  // Serial.print(",");
//  readParam();
  scanRate = Serial.parseInt();     
  stepTime = 10000*pwm_step/scanRate;      // ms per step
  delay(20);

  ///////////  Current gain  ///////
  int loGain0[19] = {
    128,127,126,125,124,123,122,120,118,116,112,108,103,96,88,77,64,48,27        };
  int hiGain1[12] = {
    26,19,15,12,9,7,5,4,3,2,1,0        };

  Serial.print("gain, ");
  while (Serial.available() <=0){ }
delay(20);
  // Serial.print(",");
 // readParam();   
  dB = Serial.parseInt();                        //dB = gain in decibels
  if (dB >= 19) {
    gain0 = 0;
    gain1 = hiGain1[dB-19];
  }
  else {
    gain1 = 26;
    gain0 = loGain0[dB];

  } 
  res4 = resPot*(128-gain0)/128+resW;            // Resistor 4 in schematic, resistance from 50 Kohm pot0
  res5 = resPot*(gain1)/128+resW;                // Resistor 5, from pot1
  amplif = res6*res4/res5;                         // amplification factor


  delay(20);

  ///////////offset voltage///////
  Serial.print("offset, ");
  while (Serial.available() <=0){ }
delay(20);
  //  Serial.print(",");
//  readParam();  
  dOff1 = Serial.parseInt();
  dOff= dOff1 + offAdj; // value from GUI + 165 (half of 330 digital resolution) 
  // addition of adjustment here assumes that analog write is source of error
  // and that error in read is not the problem  
  delay(20);

  /////////// delay 1 ////////////////
  Serial.print("initial delay, ");
  while (Serial.available() <=0){ }
delay(20);
  // Serial.print(",");
//  readParam();  
  delay1=Serial.parseInt();          // global varaible for current gain
  delay(20);

  ////// number of runs for data logging //////////////
  Serial.print("runs, ");
  while (Serial.available() <=0){ }
delay(20);
  //  Serial.print(",");
 // readParam();
  runs=Serial.parseInt();                
  delay(20);

  //////// delay between runs for data logging //////////////
  Serial.print("log delay, ");
  while (Serial.available() <=0){ }
delay(20);
  // Serial.print(",");
 // readParam();
  runDelay=Serial.parseInt(); 
  
  delay(20);

  //////////// read time for chronoAmp experiments
  Serial.println("read time");
  while (Serial.available() <=0){ }
delay(20);
 // readParam();
  readTime=Serial.parseInt();                
  delay(20);
  ///////////////////////////////// new for 6_b ///////////////////
     Serial.println("read_units");
   while (Serial.available() <=0){ }
    delay(20);
    readUnits=Serial.parseInt(); 
 //   Serial.println(readUnits);               
  delay(20);
  if(readUnits == 1){
  readTime = 1000*readTime;
  }
  if (readUnits ==2){
    readTime = 60000*readTime;
  }
  status();      // check to see if % sent, if so, reset 
  /********************* print values for header ********************/
  Serial.print(",,");              // two commas to get headers out of data columns
  Serial.print(mode);
  Serial.print(",");
  Serial.print(mVinit);
  Serial.print(",");
  Serial.print(mVfnl);
  Serial.print(",");
  Serial.print(scanRate);
  Serial.print(",");
  Serial.print(dOff); 
  Serial.print(",");
  Serial.print(delay1); 
  Serial.print(",");
  Serial.print(runs);
  Serial.print(",");
  Serial.print(runDelay); 
  Serial.print(",");
  Serial.println(readTime); 

  /////// print header ///////////////
  Serial.print("Vread,");
  //  Serial.print(",");
  Serial.println("Iread");
  Serial.println("$");       //changed from &
  //  Serial.println(" ");
         digitalWrite(RED_LED,HIGH);
        delay(500);
        digitalWrite(RED_LED,LOW);
        delay(500);
        digitalWrite(RED_LED,HIGH);
        delay(500);
        digitalWrite(RED_LED,LOW);

}  
/******************************  end setupRun *******************************/

/****************************** read Paramaters ****************************/
/*void readParam() {
  digitalWrite(RED_LED,HIGH);
  delay(500);
  digitalWrite(RED_LED,LOW);*/
/*  long place[6]={
    100000,10000,1000,100,10,1};           
  int incomingByte[6] = {
    0,0,0,0,0,0          };      // serial read variable. each incoming byte (multiplied by the place)
  int n=0;
  param = 0;
  delayMicroseconds(300);  
  while (Serial.available()<= 0) {
  }
  while (Serial.available() > 0){  
    incomingByte[n] = Serial.read()-48;
    delay(2);
    ++n;          
  }

  for(int n=0;n<6;++n){ 
    param = param + (place[n]*(incomingByte[n]));  
  }*/
 /* param = Serial.parseInt();
  delay(500);
  digitalWrite(RED_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);
  delay(500);
}          */





