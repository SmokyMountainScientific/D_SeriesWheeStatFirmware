//Main tab / StellarisWheeStat5_4c sketch
// pins setup for spi module 0
// simultaneous pwm outputs on pins 2.1, 2.4 and 1.6 
//   Files saved as csv format
//    version 5.4c;  +400 mV cleaning for mercury film electrodes
//                   -900 mV deposition voltage
//    worked on open circuit tab
//    one and two step chronoamperometry experiments working
//    normal pulse voltammetry working
//    voltage subtraction for current zero
//    Status command;  when % transmitted by GUI, experiment stopped and board reset
//    Measured mV output between working and ref electrodes; 0.966*mV set - 14.3
//    Setup begins on ~line 111
//    Loop begins on ~line 127


#include<Energia.h>           //need this for SPI to work
#include "wiring_analog.c"    // need this for pwm at high frequency, resolution
#include <altSPI.h>           // alternative SPI library must be in "libraries" folder in sketchbook

/////////////////////////

int vcc =3294;                   // full scale voltage in mV - 6/2/14
float res6 = 9.573;             // 9.433 measured value of resistor six in Kohm
float resW = 0.27;            // 1.202 resistance of wiper on digital pot in Kohm from MCP spec sheet
int corr = 0;              // adjustment to offset, determined as value that gives best zero data in resistor test
float resPot = 50;             // resistance of potentiometer at full value in Kohm from spec sheet
int offAdj = 0; // cut this out 

/////////////// set voltage parameters ////////////////////
int mVinit;                     // initial potential in mV +2000 to replace Vinit
int dInit;                      //  pwm difference variable based on mVinit 
int dInt;
int mVfnl;                      // final voltage in mV + 2000
int dFnl;                       //  pwm difference variable based on mV final
//int dDeposit = 75;                 // deposition voltage for ASV experiments; -900 mV
int dClean = 205;                   // digital value for cleaning mercury off electrode; +400 mV 
                           // cleaning at +400 mV for 30 s, as per ASDL experiment

////////// scan parameters /////////////////
int nSteps;                     // number of steps in scan
int dSig;                       // pwm input to signal pin
int dRef;                       // pwm input to ref pin
int delay1 = 1;                 // plating delay time (seconds)
int openCirc;                   //digital value for open circuit potential
boolean oCircRun = false;       // has open circuit loop been run?
int sRead;                      // read for stop sign durring run
boolean rsStatus;              // reset status
boolean runState = false;       // tells the program to stop running
boolean gotLims = false;        // tells program to establish min and max I vals
float mVi;

int pwmClock = 40000;   // was 40 k
int pwmRes = 330;  // gives 10 mV resolution, for greater res, increase value
int halfRes;  // = pwmRes/2;
int readTime;  // time for reading chronoamperometry experiments
int readUnits;

int pwm_step = 1;           // pwm increment for step
unsigned int initial_ms = delay1*1000;   // convert initial delay ms to sec

//////////////////// experiment list  ///////////////
int RAMP = 0; 
int CV = 1;           // run mode for cyclic voltammetry
int ASV = 2;          // run mode for anodic strippinng voltammetery
int logASV = 3;       // run mode for data logging ASV experiment
int diff_Pulse = 4;  
int chronoAmp = 5;    // chronoamperometry
int chronoAmp2 = 6;    // two step chronoamperometry
int normalPulse = 7;  // normal pulse voltammetry
int multiCV = 8;      // multiple CV scans
int mode;

///////////// current gain parameters ///////////////////
int iGain;                      // current gain, 0 to 30, sets digital pot
int dB;
int gain0;                      // parameter for setting digital pot 0, R4       
int gain1;                     // sets pot 1, R5.  Amp = 10*R4/R5
  float res4;   // poteniometer resistance for R4 = (128-gain0)/128*50 + 0.3 Kohms
  float res5;   // poteniometer resistance for R5 = gain1/128*50 + 0.3 Kohms
  float amplif;  // amplification of current:  i = iRead * R5/10 R4
  
//////////// scan rate parameters  ////////////////////////////////  
int scanRate;                  // mV / sec
float stepTime;                // mseconds per step

/////////// offset parameters
int dOff;                        // digital value of offset   
int dOff1;
int pgm = 0;               //stripping_volt tab
///////////  data logging parameters //////////////////
int runs = 1;                 // number of runs for data logging
unsigned int runDelay;                // sec between runs for data logging

/////////// Pin definitions //////////////////////
#define signal_pin  PB_4            // Originaly on PF_1 (LED1), move to PB_4, J1 pin 7
#define ref_pin  PB_6            // Originaly PF_2 (LED2), plan move to PB_6, J2 pin 14 
#define offset_pin PB_0            // Originaly PF_3 (LED3), plan move to PB_0, J1 pin 3;   
#define pulse_pin  PA_6          // PA_6; J1, Pin 9  only digital read and write. 
#define Iread_pin  A11              //J1 pin 2, Analog read current, was A3  
#define Vread_pin  A8            //J1, pin 6, Analog read, was A4    
#define reset_pin PF_2           //spi clock on pin 1.5, mosi on pin 1.7
#define stir_pin PF_1           // stirring motor attached to pin PB_2 (19)? RED LED on PF_1

/////////////// data read parameters /////////////////
unsigned int inVolt = 0;                 // voltage read
int mVread;                            // can go to negative values
unsigned int Iread1 = 0;                 // hight pulse current read 
unsigned int Iread2 = 0;                 // low pulse curent read 
unsigned int vRead = 0;                 // voltage read
unsigned int iRead = 0;                 // high pulse current read 
unsigned int param =0;


////////////////  current range scale bar parameters ///////////////////////  
  float iMin;    // value for minimum of current range bar, added July 7, 14
  float iMax;
  
/****************************************************************************/
void setup() {
  pinMode (signal_pin,OUTPUT);      // set pin 2.1 for output   
  pinMode (ref_pin,OUTPUT);         // new 9/10/13  
  pinMode (offset_pin,OUTPUT);     // new 9/10/13
  pinMode (pulse_pin,OUTPUT);      // set pulse pin as output
  pinMode (Vread_pin,INPUT);
  pinMode (Iread_pin,INPUT);
  pinMode (stir_pin,OUTPUT);
  halfRes = pwmRes/2;        // half scale for setting reference voltage, offset
  PWMWrite(offset_pin,330,165,pwmClock);  // sets current offset to VCC/2
  PWMWrite(ref_pin,330,165,pwmClock);     // set reference voltage to VCC/2
 PWMWrite(signal_pin,330,165,pwmClock);   
  Serial.begin(9600);             // begin serial comm. at 9600 baud
}

/******************************************************************************/
void loop (){                          

  setupRun();
  nSteps = abs((dFnl - dInit)/pwm_step);   //number of steps is positive value
  setupDigiPot();               

   PWMWrite(offset_pin,pwmRes,dOff,pwmClock);  // sets current offset

///////////  Ramp and CV experiments  ////////////////////
  if (mode == RAMP || mode == CV || mode == multiCV) {
    ramp();  
    stop(3);   // executes stop command and 3 following zero transmits
   }
  if (mode == diff_Pulse) {
      digitalWrite(pulse_pin,LOW); 
      PWMWrite(signal_pin,pwmRes,dInit,pwmClock);  // set signal voltage to initial value
      delay(delay1*1000);
      diffPulse();
        stop(4);
}

///////////////  ASV experiments ////////////////////////
if (mode == ASV || mode == logASV) {
    cleanDepos();  // Clean for 2 sec at Vfnl, deposit for delay 1 at Vinit
    diffPulse();  

    if (mode == logASV) {

      for (int q = 2; q<=runs; ++q) {

        Serial.print("55555,");  
        Serial.print(q);  
        Serial.println(",0,0,0");
        delay (runDelay*1000);          // run delay at open circuit
  
        cleanDepos();
        diffPulse();
        status();           // check for reset

      }
    }
        stop(4);
 }
 
 /////////////// normal pulse experiment ///////////////////
  if (mode == normalPulse) {            // status check in normPulse() routine
    digitalWrite(pulse_pin,LOW);              // set pulse pin to low
//    PWMWrite(signal_pin,pwmRes,dFnl,pwmClock);  // electrode cleaning step
//    delay(2000);
    normPulse(); 
        stop(3);
  }
  
  /////////////////// chronoamperometry experiments//////////////////
  if (mode == chronoAmp||mode == chronoAmp2) {
    digitalWrite(pulse_pin,LOW);              // set pulse pin to low
//    PWMWrite(signal_pin,pwmRes,dFnl,pwmClock);  // electrode cleaning step
//    delay(2000);
    chronAmp();     //changed
    stop(3);
  }
}

void stop(int s) {
      Serial.print(99999);
  for (int r = 0; r<s; r++) {
    Serial.print(",0");
    }
    Serial.println("");
  status();
}

