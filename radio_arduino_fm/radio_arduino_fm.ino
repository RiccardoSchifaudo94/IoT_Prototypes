/* How to use the TEA5767 FM radio Module with Arduino
   More info: http://www.ardumotive.com/how-to-use-the-tea5767-fm-radio-module-en.html
   Dev: Vasilakis Michalis // Date: 21/9/2015 // www.ardumotive.com    */

//Libraries:
#include <TEA5767.h>
#include <Wire.h>
#include <SoftwareSerial.h>


//Constants:
TEA5767 Radio; //Pinout SLC and SDA - Arduino uno pins A5 and A4
SoftwareSerial BTModule(11,10); // RX/TX

//Variables:
double old_frequency;
double frequency;
int search_mode = 0;
int search_direction;
unsigned long last_pressed;
unsigned char buf[5];
int stereo;
int signal_level;
double current_freq;
unsigned long current_millis = millis();
int inByte;
int flag=0;
char BTSignal = ' ';

void setup () {
  //Init
  Serial.begin(9600);
  BTModule.begin(9600);
  Wire.begin();
  Radio.init();
  Radio.set_frequency(96.2); //On power on go to station 95.2

}

void loop () {
  
  if (Serial.available()>0) {
    inByte = Serial.read();
    if (inByte == '+' || inByte == '-'){  //accept only + and - from keyboard
      flag=0;
    }
  }

  if(BTModule.available()){
    BTSignal = BTModule.read();
    BTModule.println("Segnale ricevuto");
    if(BTSignal == '1'){
      inByte  = '+';
      flag = 0;
      BTModule.println("Avanza di una stazione");
    }  
    if(BTSignal == '2'){
      inByte = '-';
      flag = 0;
      BTModule.println("Indietro di una stazione");
    }
  }

  if (Radio.read_status(buf) == 1) {
     current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
     stereo = Radio.stereo(buf);
     signal_level = Radio.signal_level(buf);
     //By using flag variable the message will be printed only one time. 
     if(flag == 0){
      Serial.print("Current freq: ");
      Serial.print(current_freq);
      Serial.print("MHz Signal: ");
      //Strereo or mono ?
      if (stereo){
        Serial.print("STEREO "); 
      } 
    else{
      Serial.print("MONO ");
    } 
      Serial.print(signal_level);
      Serial.println("/15");
      flag=1;
     }
  }
   
  //When button pressed, search for new station
  if (search_mode == 1) {
      if (Radio.process_search (buf, search_direction) == 1) {
          search_mode = 0;
      }
  }
  //If forward button is pressed, go up to next station
  if (inByte == '+') {
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_UP;
    Radio.search_up(buf);
  }
  //If backward button is pressed, go down to next station
  if (inByte == '-') {
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_DOWN;
    Radio.search_down(buf);
  } 
  delay(500);
   
}
