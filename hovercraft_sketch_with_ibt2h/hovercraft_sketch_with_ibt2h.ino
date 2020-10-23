//include library to control bluetooth communication
#include <SoftwareSerial.h>
//include library to control servo to pilot directio air flow of the hovercraft
#include <Servo.h>
//instance of servo library
Servo myServo;
//declare pins of eletronic components
const int pontPin = A2;
//const int enableSkirt = 6;
//const int enablePropulsor = 5;

//const int channelSkirtA = 7;
//const int channelSkirtB = 8;

//const int channelPropulsorA = 12;
//const int channelPropulsorB = 13;

//clare status variables
int potVal = 0;
int angle = 90;
//int valueSpeedSkirt = 0;
//int valueSpeedPropulsor = 0;
//set if first start of motors
bool firstStart = true;

const int RPWM_Output = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
const int LPWM_Output = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)
int reversePWM = 0;
//setting tx and rx bluetooth module
SoftwareSerial BTSerial(10,11); // RX | TX

//default command 'block' arrest all system
char cmd = 'B';

void setup() {
  //setting outputs
   myServo.attach(A3);
  /*
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
  */
  pinMode(RPWM_Output, OUTPUT);
  pinMode(LPWM_Output, OUTPUT);
  
  analogWrite(LPWM_Output, 0);
  analogWrite(RPWM_Output, reversePWM);
  
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
      //ACCELERATION
      case 'G':
         
         Serial.println("Accelera!");
         
         if(firstStart){
          reversePWM = 10;
          firstStart = false;
         }
          
         if(reversePWM<250){
             reversePWM = reversePWM + 10;
             analogWrite(RPWM_Output,reversePWM);
             delay(100);
             Serial.println("-------------------------------");
             Serial.print("Value Speed Skirt : ");
             Serial.println(reversePWM);
             Serial.println("-------------------------------");
         }else{
            Serial.println("-------------------------------");
            Serial.println("Massima velocita' raggiunta!"); 
            Serial.println("-------------------------------");
         }

      break;
      //SLOW DOWN 
      //Reduce speed motors
      case 'R':
        Serial.println("Rallenta!");
        
         if(reversePWM>5){
           
            reversePWM = reversePWM - 10;
            analogWrite(RPWM_Output,reversePWM);
            delay(100);
           
            Serial.println("-------------------------------");
            Serial.print("Value Speed Skirt : ");
            Serial.println(reversePWM);
            Serial.println("-------------------------------");
         
         }else{
         
            firstStart = true; 
            Serial.println("-------------------------------");
            Serial.println("Motore Fermo!");
            Serial.println("-------------------------------");
         
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
    cmd = Serial.read();
  }
  // put your main code here, to run repeatedly:
  // GO AHEAD with the Servo
  if(cmd == 'A'){    
    myServo.write(angle);
  }
  //BLOCK
  //Total Arrest of Hovercraft;
  if(cmd == 'B'){ 
         //Verify if there was NOT the first start
            
           //Switch off the motors
           reversePWM = 0; 
           analogWrite(RPWM_Output, reversePWM);
           //Return the servo position straight
           //in the original position
           myServo.write(angle);
           //setting again the first starting
           firstStart = true;
           Serial.println("Arresto Totale");
           delay(100);
        
    }
  //TURN TO RIGHT
  //reduce angle of servo from current position 
  //unto 30 degrees
  if(cmd == 'D'){
    int min_angle = 0;
    int diff_angle = angle - min_angle;
    
    myServo.write(angle - diff_angle);
    Serial.println("-------------------------------");
    Serial.print("Servo shift a destra di gradi : ");
    Serial.print(angle - diff_angle);
    Serial.println("-------------------------------");
  }
  //TURN TO LEFT
  //Move Servo from fixed servo position 
  //unto the 150 degrees
  if(cmd == 'S'){

    int max_angle = 150;
    int diff_angle = max_angle - angle;
    myServo.write(angle + diff_angle);
    Serial.println("-------------------------------");
    Serial.print("Servo shift a sinitra di gradi : ");
    Serial.print(angle + diff_angle);
    Serial.println("-------------------------------");
  }

  //SETTING SERVO
  //fix angle radiant of servo motor 
  //reading the value from the potentiameter 
  if(cmd == 'X'){
      potVal = analogRead(pontPin);
      Serial.println("-------------------------------");
      Serial.print("potVal: ");
      Serial.print(potVal);
      Serial.println(" ");
    
      angle = map(potVal,0,1023,0,179);
      
      Serial.print("angle: ");
      Serial.print(angle);
      Serial.println("-------------------------------");
      
      myServo.write(angle);
      delay(15);
    }
    //START
    //Switch on the hovercraft motor skirt 
    //with half of power
    if(cmd == 'V'){
         reversePWM = 10; 
         analogWrite(RPWM_Output, reversePWM);
         Serial.println("Decollo!");
         delay(100);
    }
   //INFO MOTOR AND SERVO CURRENT POSITION
    if(cmd== 'I'){
      Serial.println("---------------------------");
      Serial.print("Velocita' motore della scafo: ");
      Serial.print(reversePWM);
      Serial.println("Angolo curvatura del servomotore: ");
      Serial.print(angle);
      Serial.println("--------------------------");
    
    }
}
