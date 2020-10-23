/*==========================================================================
// Author : Handson Technology
// Project : BTD7960 Motor Control Board driven by Arduino.
// Description : Speed and direction controlled by a potentiometer attached
// to analog input A0. One side pin of the potentiometer (either one) to
// ground; the other side pin to +5V
// Source-Code : BTS7960.ino
// Program: Control DC motors using BTS7960 H Bridge Driver.
//==========================================================================
// Connection to the BTS7960 board:
// BTS7960 Pin 1 (RPWM) to Arduino pin 5(PWM)
// BTS7960 Pin 2 (LPWM) to Arduino pin 6(PWM)
// BTS7960 Pin 3 (R_EN), 4 (L_EN), 7 (VCC) to Arduino 5V pin
// BTS7960 Pin 8 (GND) to Arduino GND
// BTS7960 Pin 5 (R_IS) and 6 (L_IS) not connected
*/
int SENSOR_PIN = A0; // center pin of the potentiometer
int RPWM_Output = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)
void setup()
{
 pinMode(RPWM_Output, OUTPUT);
 pinMode(LPWM_Output, OUTPUT);
}
void loop()
{
 int sensorValue = analogRead(SENSOR_PIN);
 // sensor value is in the range 0 to 1023
 // the lower half of it we use for reverse rotation; the upper half for forward rotation

 // reverse rotation
 int reversePWM = map(sensorValue,0,1023,0,255);
 analogWrite(LPWM_Output, 0);
 analogWrite(RPWM_Output, reversePWM);
 


 Serial.print("Sensor Value: ");
 Serial.print(sensorValue);
 Serial.println("");
 Serial.print("reversePWM: ");
 Serial.print(reversePWM);
 Serial.println("");
}
