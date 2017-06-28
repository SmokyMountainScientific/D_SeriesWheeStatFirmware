//  chronoAmperometry tab / WheeStat5_4 sketch in publicEnergiaSketches folder
//  chronoamperometry and two step chronoamperometry experiments
//     both working as of May 20, 2014

void chronAmp()  {
//  Serial.print(rdTime);
  //Serial.print(",");
  //Serial.println(readTime);
  ////////// ////////////////////////////////////////////////  
  PWMWrite(signal_pin,pwmRes,dInit,pwmClock);  // set signal voltage to 512 + 1/2 Vinit

///// from Ben's old stuff ////
  int Iread = 0;
  long inTime = 0;
//  readTime = readTime*1000;        // micros durring read 
//  fnlV_time = fnlV_time*1000; 
//  long time1 = micros();

  long time2;             // = time1;
//  readTime = 50000;              
  delay(delay1*1000);
  long time1 = millis();
  ///////// Step and read ////////////////////////////////////////////////

    PWMWrite(signal_pin,pwmRes,dFnl,pwmClock);
    
    while(inTime <= readTime){    
//    while(time2 <= time1+readTime){    
    Iread = 0;
//    time2 = micros();
//    inTime = (time2-time1)/1000;  //inTime is in ms, time1 and time2 are micros      
    time2 = millis();
    inTime = time2-time1;  //inTime is in ms, time1 and time2 are micros      
//    Serial.print("4 ,");          // not working here
    Serial.print(inTime);
    Serial.print(",");          // working here
  //  Serial.println(Iread2,4);  // holdover from Ben's old stuff
 //   time2 = micros();    
//    Serial.print("1 ,");
    readCurrent(true);
    Serial.print(iMin);
    Serial.print(",");
    Serial.println(iMax);//    Serial.println("");
 //   Serial.println("");
    delay(runDelay);
    }
 if(mode == chronoAmp2) {
   PWMWrite(signal_pin,pwmRes,dInit,pwmClock);  // step back to initial voltage
     while(inTime <= readTime*2){    
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
 }
  
  openCircuit();    // go to open circuit
 }

