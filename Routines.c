#include "miniboard.h"

// Configuration Bits//
_FOSC(CSW_FSCM_OFF & FRC_PLL16);	// Set up for Internal Fast RC by 16x PLL
_FWDT(WDT_OFF);						// Turn off the Watch-Dog Timer
_FBORPOR(MCLR_EN & PWRT_OFF);		// Enable MCLR reset pin and turn off the power-up timers
_FGS(CODE_PROT_OFF);				// Disable Code Protection

int switch1state2=0;
int switch2state2=0;

void initIO(){
TRISB=0b001111111;
TRISC=0xFFFF;
TRISD=0b1100;
TRISE=0xFFFF;
TRISF=0b0001100;
}

void initInterrupts() {
_NSTDIS=0; //enable nesting
_T2IP=2; //T2 Interrupt Priority
_ADIP=1; //ADC Interrupt Priority
}

void initTimer2() {
//TIMER5 setup, gia elegxo koumpion, diakopton//
//praktika kathe 250ms
T2CON=0x0030;//prescale 256
TMR2=0; //midenismos metriti
PR2=1150;  
_T2IE=1; //enable tis Timer2 interrupt
_T2IF=0; //svisimo interrupt flag
T2CONbits.TON=1; //enarksi timer2
}

void initPWM(float freq, float dc) {
//i freq einai se Hz
//to dc (duty cycle) einai 0-100
PWMCON1=0x0011; //enable PWM1L,1H complementary
PWMCON2=0x0000;
PTCON=0x0000;
PTPER=FCY/freq-1.;
PDC1=PTPER*dc/50.+1.;
}

changePWM(float freq, float dc) {
//i freq einai se Hz
//to dc (duty cycle) einai 0-100
PWMCON1=0x0011; //enable PWM1L,1H complementary
PWMCON2=0x0000;
PTCONbits.PTCKPS=4;
PTPER=64*(FCY/freq)-1.;
PDC1=PTPER*dc/50.+1.;
}

void startPWM(){
_PTEN=1;
}

void stopPWM(){
_PTEN=0;
}

void initAD(){
//Initialization gia ta esoterika trimmer
//ANO=>CH1=>ADCBUF1, AN1=>CH0=>ADCBUF0
ADCSSL=0x0003;	//Select AN0,AN1
ADCHS=0x0001;	//Multiplexing apo pano
ADPCFG=0x01FC;	//AN0,AN1 analog ports
ADCON3=0x1F3F;	//Tsamp = 31*TAD , TAD = 32*TCY , internal clock, Fsamp=29,7kHz
ADCON2=0x0200;	//Convert Ch0,Ch1, interrupt se kathe conv
ADCON1=0x80EC;
}


void checkbuttons() {
if (PORTCbits.RC15==1) {
	if (button1flag==0) {
		button1pressed();
		button1flag=1;
		}
	else ; //do nothing an exei idi patithi ston proigoumeno kiklo
	}
else button1flag=0;

if (PORTCbits.RC14==1) {
	if (button2flag==0) {
		button2pressed();
		button2flag=1;
		}
	else ; //do nothing an exei idi patithi ston proigoumeno kiklo
	}
else button2flag=0;

if (PORTCbits.RC13==1) {
	if (button3flag==0) {
		button3pressed();
		button3flag=1;
		}
	else ; //do nothing an exei idi patithi ston proigoumeno kiklo
	}
else button3flag=0;

if (PORTEbits.RE8==1) {
	if (button4flag==0) {
		button4pressed();
		button4flag=1;
		}
	else ; //do nothing an exei idi patithi ston proigoumeno kiklo
	}
else button4flag=0;
}

void checkswitches(){

if (PORTDbits.RD3==1 && switch1state2==0) {
	switch1state2=1;
	switch1off2on();
}
if (PORTDbits.RD3==0 && switch1state2==1) {
	switch1state2=0;
	switch1on2off();
}
if (PORTDbits.RD2==1 && switch2state2==0) {
	switch2state2=1;
	switch2off2on();
}
if (PORTDbits.RD2==0 && switch2state2==1) {
	switch2state2=0;
	switch2on2off();
}
if (switch1state2==1) switch1on();
if (switch2state2==1) switch2on();
}

int switch1state(){
return switch1state2;
}

int switch2state(){
return switch2state2;
}

void display(int d1, int dp, int d2){
//d1: proti dipslay, dp: mesaio decimal point, d2: defteri display
//0-9: arithmoi, 10:c, 11:anapodo c, 12: pano Y, 13: sxedon E, 14: t, 15: svista
switch (d1)
	{
	case 0:
		_LATB7=0;
		_LATB8=0;
		_LATD0=0;
		_LATD1=0;
		break;
	case 1:
		_LATB7=0;
		_LATB8=0;
		_LATD0=0;
		_LATD1=1;
		break;
	case 2:
		_LATB7=0;
		_LATB8=0;
		_LATD0=1;
		_LATD1=0;
		break;
	case 3:
		_LATB7=0;
		_LATB8=0;
		_LATD0=1;
		_LATD1=1;
		break;
	case 4:
		_LATB7=0;
		_LATB8=1;
		_LATD0=0;
		_LATD1=0;
		break;
	case 5:
		_LATB7=0;
		_LATB8=1;
		_LATD0=0;
		_LATD1=1;
		break;
	case 6:
		_LATB7=0;
		_LATB8=1;
		_LATD0=1;
		_LATD1=0;
		break;
	case 7:
		_LATB7=0;
		_LATB8=1;
		_LATD0=1;
		_LATD1=1;
		break;
	case 8:
		_LATB7=1;
		_LATB8=0;
		_LATD0=0;
		_LATD1=0;
		break;
	case 9:
		_LATB7=1;
		_LATB8=0;
		_LATD0=0;
		_LATD1=1;
		break;
	case 10:
		_LATB7=1;
		_LATB8=0;
		_LATD0=1;
		_LATD1=0;
		break;
	case 11:
		_LATB7=1;
		_LATB8=0;
		_LATD0=1;
		_LATD1=1;
		break;
	case 12:
		_LATB7=1;
		_LATB8=1;
		_LATD0=0;
		_LATD1=0;
		break;
	case 13:
		_LATB7=1;
		_LATB8=1;
		_LATD0=0;
		_LATD1=1;
		break;
	case 14:
		_LATB7=1;
		_LATB8=1;
		_LATD0=1;
		_LATD1=0;
		break;
	case 15:
		_LATB7=1;
		_LATB8=1;
		_LATD0=1;
		_LATD1=1;
		break;
	}
if (dp==0) _LATF6=1;
else _LATF6=0;
switch (d2)
	{
	case 0:
		_LATF5=0;
		_LATF4=0;
		_LATF1=0;
		_LATF0=0;
		break;
	case 1:
		_LATF5=0;
		_LATF4=0;
		_LATF1=0;
		_LATF0=1;
		break;
	case 2:
		_LATF5=0;
		_LATF4=0;
		_LATF1=1;
		_LATF0=0;
		break;
	case 3:
		_LATF5=0;
		_LATF4=0;
		_LATF1=1;
		_LATF0=1;
		break;
	case 4:
		_LATF5=0;
		_LATF4=1;
		_LATF1=0;
		_LATF0=0;
		break;
	case 5:
		_LATF5=0;
		_LATF4=1;
		_LATF1=0;
		_LATF0=1;
		break;
	case 6:
		_LATF5=0;
		_LATF4=1;
		_LATF1=1;
		_LATF0=0;
		break;
	case 7:
		_LATF5=0;
		_LATF4=1;
		_LATF1=1;
		_LATF0=1;
		break;
	case 8:
		_LATF5=1;
		_LATF4=0;
		_LATF1=0;
		_LATF0=0;
		break;
	case 9:
		_LATF5=1;
		_LATF4=0;
		_LATF1=0;
		_LATF0=1;
		break;
	case 10:
		_LATF5=1;
		_LATF4=0;
		_LATF1=1;
		_LATF0=0;
		break;
	case 11:
		_LATF5=1;
		_LATF4=0;
		_LATF1=1;
		_LATF0=1;
		break;
	case 12:
		_LATF5=1;
		_LATF4=1;
		_LATF1=0;
		_LATF0=0;
		break;
	case 13:
		_LATF5=1;
		_LATF4=1;
		_LATF1=0;
		_LATF0=1;
		break;
	case 14:
		_LATF5=1;
		_LATF4=1;
		_LATF1=1;
		_LATF0=0;
		break;
	case 15:
		_LATF5=1;
		_LATF4=1;
		_LATF1=1;
		_LATF0=1;
		break;
	}
}

int hex2bcd( int hexnumber) {
//metatropi se dipsifio arithmo
int c,d; // psifia dekadon, monadon
int b1,c1; //diafores
int result;

c=hexnumber/10;
d=hexnumber-c*10;
result=c*16 +d;
return result;
}

void display2digit(int dig, int dp) {
int dek, mon;
dek=(hex2bcd(dig)&0xF0)>>4;
mon=hex2bcd(dig)&0xF;
display(dek,dp,mon);
}

void updatePWMbyADC(){
changePWM(50000.,ADCBUF1*100./1023.);
display2digit((int) ADCBUF1*100./1023., 0);
}
