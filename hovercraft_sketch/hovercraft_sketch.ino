//include library to control bluetooth communication
#include <SoftwareSerial.h>
//include library to control servo to pilot directio air flow of the hovercraft
#include <Servo.h>
//instance of servo library
Servo myServo;
//declare pins of eletronic components
const int pontPin = A0;
const int enableSkirt = 6;
const int enablePropulsor = 5;

const int channelSkirtA = 7;
const int channelSkirtB = 8;

const int channelPropulsorA = 12;
const int channelPropulsorB = 13;

//clare status variables
int potVal = 0;
int angle = 90;
int valueSpeedSkirt = 0;
int valueSpeedPropulsor = 0;
//set if first start of motors
bool firstStart = true;

//setting tx and rx bluetooth module
SoftwareSerial BTSerial(10,11); // RX | TX

//default command 'block' arrest all system
char cmd = 'B';

void setup() {
  //setting outputs
  myServo.attach(A1);
  pinMode(enableSkirt,OUTPUT);
  pinMode(enablePropulsor,OUTPUT);
  
  pinMode(channelSkirtA,OUTPUT);
  pinMode(channelSkirtB,OUTPUT);
  
  pinMode(channelPropulsorA,OUTPUT);
  pinMode(channelPropulsorB,OUTPUT);
  
  analogWrite(enableSkirt,valueSpeedSkirt);
  analogWrite(enablePropulsor,valueSpeedPropulsor);
  
  digitalWrite(channelSkirtA,LOW);
  digitalWrite(channelSkirtB,HIGH);

   digitalWrite(channelPropulsorA,HIGH);
  digitalWrite(channelPropulsorB,LOW);
  
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
     delay(100);
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
      case 'X':
       Serial.println("Regola Sterzo");
      break;
      case 'G':
         Serial.println("Accelera!");
         
         if(firstStart){
          valueSpeedSkirt = 120;
          valueSpeedPropulsor = 120;
          firstStart = false;
         }
          
         if(valueSpeedSkirt<250){
             valueSpeedSkirt = valueSpeedSkirt + 10;
             analogWrite(enableSkirt,valueSpeedSkirt);
             delay(100);
             Serial.print("Value Speed Skirt : ");
             Serial.println(valueSpeedSkirt);
         }else{
            Serial.println("Massima velocita' raggiunta!"); 
         }

         if(valueSpeedPropulsor<250){
             valueSpeedPropulsor = valueSpeedPropulsor + 10;
             analogWrite(enablePropulsor,valueSpeedPropulsor);
             delay(100);
             Serial.print("Value Speed Propulsor : ");
             Serial.println(valueSpeedPropulsor);
         }else{
            Serial.println("Massima velocita' raggiunta!"); 
         }
      break;
      case 'R':
        Serial.println("Rallenta!");
        
         if(valueSpeedSkirt>0){
         valueSpeedSkirt = valueSpeedSkirt - 10;
         analogWrite(enableSkirt,valueSpeedSkirt);
         delay(100);
         Serial.print("Value Speed Skirt : ");
         Serial.println(valueSpeedSkirt);
         }else{
          Serial.println("Motore Fermo!");
          firstStart = true; 
         }
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
  // GO AHEAD with the Servo
  if(cmd == 'A'){    
    myServo.write(angle);
  }
  //Block/Total Arrest of Hovercraft;
  if(cmd == 'B'){
    //fix straght the servo 
    myServo.write(angle);
    //swith off all motors
    valueSpeedSkirt = 0;
    analogWrite(enableSkirt,valueSpeedSkirt);
    valueSpeedPropulsor = 0;
    analogWrite(enablePropulsor,valueSpeedPropulsor);
  }
  //GO TO THE RIGHT
  if(cmd == 'D'){
    /*int max_angle = 175;
    int min_angle = 30;
    int diff_angle = angle; 
    for(int i = diff_angle;diff_angle>=min_angle;diff_angle--){
       myServo.write(angle);
       delay(15);
    }*/
    
    int min_angle = 0;
    int diff_angle = angle - min_angle;
    
    myServo.write(angle - diff_angle);
    
    Serial.print("Servo shift a destra di gradi : ");
    Serial.print(angle - diff_angle);
    Serial.println("");
  }
  //GO TO THE LEFT
  if(cmd == 'S'){
    /*int max_angle = 175;
    int min_angle = 30;
    int diff_angle = angle;
    for(int i = diff_angle;diff_angle<=max_angle;diff_angle++){
       myServo.write(diff_angle);
       delay(15);
    }*/
    int max_angle = 150;
    int diff_angle = max_angle - angle;
    myServo.write(angle + diff_angle);

    Serial.print("Servo shift a sinitra di gradi : ");
    Serial.print(angle + diff_angle);
    Serial.println("");
  }

  //setting angle radiant of servo motor reading the value from the potentiameter 
  if(cmd == 'X'){
      potVal = analogRead(pontPin);
      Serial.print("potVal: ");
      Serial.print(potVal);
      Serial.println(" ");
    
      angle = map(potVal,0,1023,0,179);
      
      Serial.print("angle: ");
      Serial.print(angle);
      Serial.println(" ");
      
      myServo.write(angle);
      delay(15);
    }
    //start the hovercraft motor skirt with half of power
    if(cmd == 'V'){
         valueSpeedSkirt = 125;
         analogWrite(enableSkirt,valueSpeedSkirt);
         Serial.println("Decollo!");
         delay(100);
    }
    if(cmd == 'B'){ 
         if(!firstStart){     
         valueSpeedSkirt = 0;
         analogWrite(enableSkirt,valueSpeedSkirt);
         myServo.write(angle);
         Serial.println("Arresto Totale");
         delay(100);
         firstStart = true;
         }
    }
    if(cmd== 'I'){

      Serial.println("---------------------------");
      Serial.print("Velocita' motore della scafo: ");
      Serial.print(valueSpeedSkirt);
      Serial.println("Angolo curvatura del servomotore: ");
      Serial.print(angle);
      Serial.println("--------------------------");
    
    }
}
