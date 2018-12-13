/********************************************************************************************
AAL024 current sensor interface to an ATtiny85 MCU, with 4-segment piecewise-linear 
calibration based on measurements of uncalibrated output. Sensor OUT+ to ADC2 (PB4);  
OUT- to ADC3 (PB3); calibrated PWM out on PB0; uncalibrated on PB1; Zeroing switch on PB2.
0.05"-wide trace under sensor ==> 48 mV/A = 170 mV nominal at 3.5A Full Scale.
3.5A Full Scale ==> 350 ADC with 20x ATTiny preamp. Voltage divider scales 5V PWM 
to read 35 mV (3.50A) on a three-digit meter.                                   Rev. 12/10/18
********************************************************************************************/
#include <EEPROM.h>
int cal[]={0, 100, 200, 300, 350}; //Uncal. (PB1) readings at 0, 1, 2, 3, & 3.5A (no dec pts)
unsigned char i=0; //Array index
int sensor; //Sensor output
int8_t offset=0; //"offset" is a signed byte (-127 to +127)
char zeroSwitch = HIGH; //Previous state of the zeroing switch
void setup() {
if(EEPROM.read(0) == 255) EEPROM.write(0, 0); //Offset stored in EEPROM (0); initialized to 0
CLKPR=0x80; //Enable changing the internal clock
CLKPR=4; //Slow internal clock to 1 Mhz (16 Mhz/16) to reduce power and minimize ADC noise
pinMode(2, INPUT_PULLUP); //Set up the "Zero" switch on PB2 
ADCSRB=0x80; //Set ADC to bipolar mode (ADCSRB bit 7 = 1); 250 mV ==> 512 ADC
ADMUX=7; //Enable preamp so ADC = 20*(PB4-PB3); reference = Vcc = 5V
cal[4]+=cal[4]-cal[3]; //Extrapolate last array point to 4A
}
void loop() {
ADCSRA |=_BV(ADSC)|3; //Start ADC with clock at 125 KHz (divided by 8; 50-200 kHz recommended)
while(ADCSRA & _BV(ADSC)); //Wait for ADC conversion
sensor = ADC-((ADC & 0x200)<<1); //Read sensor and correct negative readings
if(digitalRead(2) != zeroSwitch) { //Zero switch state changed
 offset = sensor;
 if(offset != EEPROM.read(0)) EEPROM.write(0, offset); //Update offset if changed
 zeroSwitch = !zeroSwitch; //Update switch state
}
delay(15); //67 samples/sec; also allows for EEPROM write and switch debounce
offset = EEPROM.read(0); //Read stored offset; cast into signed byte 
sensor -= offset; //Correct for offset
if(sensor < 0) sensor = 0; if(sensor > 350) sensor = 350; //Clamp output to prevent wrapping
analogWrite(1, (long(sensor)*255+175)/350); //Normalized output on PB1 ("175" is for rounding)

//Calibrated output
i = sensor/100; //Uncalibrated current in amps rounded down (<1A, 1-2A, 2-3A, or >=3A)
sensor = i*73 + ((i*100-cal[i])*73+50)/100 + (long((sensor-i*100))*7300/(cal[i+1]-100*i)+50)/100;
if(sensor < 0) sensor = 0; if(sensor > 255) sensor = 255; //Prevent wrapping
analogWrite(0, sensor); //Write calibrated output to PB0
}
