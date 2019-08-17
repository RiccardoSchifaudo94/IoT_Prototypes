//include library to control bluetooth communication
#include <SoftwareSerial.h>

//setting tx and rx bluetooth module
SoftwareSerial BTSerial(11,10); // RX | TX

const int enabled_powerbank = 3;
char cmd = ' ';

void setup() {
  
  // put your setup code here, to run once:
  pinMode(enabled_powerbank,OUTPUT);
  digitalWrite(enabled_powerbank,LOW);
  
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(BTSerial.available()){
       
       cmd = BTSerial.read();

       if(cmd == '0'){
         BTSerial.println("USB OFF!");
       }

       if(cmd == '1'){
         BTSerial.println("USB ON!");
       }
  }     

   if(cmd == '0'){
     BTSerial.println("USB OFF!");
     digitalWrite(enabled_powerbank,LOW);
   }
   
   if(cmd == '1'){
     BTSerial.println("USB ON!");
     digitalWrite(enabled_powerbank,HIGH);
   }

}
