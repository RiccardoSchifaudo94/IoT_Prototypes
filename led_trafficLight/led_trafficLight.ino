#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10,11); // RX | TX

const int ledRed = 13;
const int ledYellow = 12;
const int ledGreen = 9;
const int ledBlue = 8;
const int buttonPin = 2;
const int motorPin = 7;
int array_led[] = {ledBlue,ledGreen,ledYellow,ledRed};
int conta = 0;
char cmd = 'B';

void setup() {
  // put your setup code here, to run once:
  pinMode(ledRed,OUTPUT);
  pinMode(ledYellow,OUTPUT);
  pinMode(ledGreen,OUTPUT);
  pinMode(ledBlue,OUTPUT);
  pinMode(motorPin,OUTPUT);
  pinMode(buttonPin,INPUT);
  Serial.begin(9600);
  BTSerial.begin(9600);  // HC-05 default speed in AT command more
}

void loop() {
  // put your main code here, to run repeatedly:

  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if(BTSerial.available()){
     cmd = BTSerial.read();
     Serial.write(cmd);
     Serial.println("");
     delay(1000);
     switch(cmd){
      case 'A':
       Serial.println("Avanti/Dritto");
      break;
      case 'B':
       Serial.println("Blocco/Spegni");
      break;
      case 'D':
       Serial.println("Destra");
      break;
      case 'S':
       Serial.println("Sinistra");
      break;
      case 'V':
       Serial.println("Via/Motore");
      break;
      default:
         cmd = 'B';
         //Serial.println("Blocco/Spegni");
      break;  
     }
  
  }

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available()){
    BTSerial.write(Serial.read());
  }
  // put your main code here, to run repeatedly:
  if(digitalRead(buttonPin)==HIGH){
    
    Serial.println("Button hight!");
    Serial.println(conta);
  
    conta = conta + 1;  
    
    delay(1000);
  }
  
  if(cmd == 'A'){
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledGreen, LOW);
  }

  if(cmd == 'B'){
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(motorPin,LOW);
  }

  if(cmd == 'D'){
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledGreen, LOW);
  }

  if(cmd == 'S'){
    digitalWrite(ledRed, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledGreen, LOW);
  }

  if(cmd == 'V'){
    digitalWrite(motorPin,HIGH);
  }
  
}
