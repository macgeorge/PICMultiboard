/*
Plaketa miniboard
Hardware mapping:
Input:
Button1: RC15 (pin14), pressed=5V
Button2: RC14 (pin16), pressed=5V
Button3: RC13 (pin15), pressed=5V
Button4: RE8 (pin17), pressed=5V
Switch1: RD3 (pin19), ON=5V
Switch2: RD2 (pin22), ON=5V
Trimmer1: AN0 (pin2)
Trimmer2: AN1 (pin3)
Analog Input: AN2-6 (pin4-8)
Output:
Display1: RB7(pin9), RB8(pin10),RD0(pin23), RD1(pin18), DP:RF6(pin24, ON=0)
Display2: RF5(pin27), RF4(pin28), RF1(pin29), RF0(pin30)
PWM: 1L/RE0(pin38), 1H/RE1(pin37), 2L/RE2(pin36), 2H/RE3(pin35), 3L/RE4(pin34), 3H/RE5(pin33)
*/

#include "miniboard.h"

float PWMFREQ=50000.;
int i=0;
/////

int main() {
initIO();
initInterrupts();
initTimer2();
initAD();
i=10;
initPWM(PWMFREQ,10.);
startPWM();
display2digit(i,0);

while(1);

}

void __attribute__((interrupt,auto_psv)) _T2Interrupt(void) {
checkbuttons();
checkswitches();
}

void __attribute__((interrupt,auto_psv)) _ADCInterrupt(void) {
//New sample
if (switch1state()==1) updatePWMbyADC();
}

void button1pressed(){
if (switch1state()==0){ 
if (i<81) i+=10;
changePWM(PWMFREQ,(float)i);
display2digit(i,0);
}
}

void button2pressed(){
if (switch1state()==0){
if (i>10) i-=10;
changePWM(PWMFREQ,(float)i);
display2digit(i,0);
}
}

void button3pressed(){
if (switch1state()==0){
if (i<90) i++;
changePWM(PWMFREQ,(float)i);
display2digit(i,0);
}
}

void button4pressed(){
if (switch1state()==0){
if (i>1) i--;
changePWM(PWMFREQ,(float)i);
display2digit(i,0);
}
}

void switch1off2on(){
}

void switch1on2off(){
}

void switch2off2on(){
PWMFREQ=50000.;
changePWM(PWMFREQ,(float)i);
display2digit(i,0);
}

void switch2on2off(){
PWMFREQ=500000.;
changePWM(PWMFREQ,(float)i);
display2digit(i,0);

}

void switch1on(){
float analogduty;
analogduty=ADCBUF1*100./1023.;
if (analogduty>90.) analogduty=90.;
if (analogduty<i+10 && analogduty>i-10) {
changePWM(PWMFREQ,analogduty);
display2digit((int)analogduty,0);
i=(int)analogduty;
}
}

void switch2on(){
}
