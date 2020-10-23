//include library to control bluetooth communication
#include <SoftwareSerial.h>

//setting tx and rx bluetooth module
SoftwareSerial BTSerial(11,10); // RX | TX

//default command

String phrase = "";
bool print_message = false;

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   BTSerial.begin(9600);  // HC-05 default speed in AT command more

}

void loop() {
  // put your main code here, to run repeatedly:
  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if(BTSerial.available()){
     phrase = BTSerial.readString();
     Serial.println(phrase);
  }
  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available()){
     BTSerial.write(Serial.read());
  }
}
