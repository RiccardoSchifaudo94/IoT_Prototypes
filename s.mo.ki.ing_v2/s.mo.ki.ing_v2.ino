//include library to control bluetooth communication
#include <SoftwareSerial.h>
//include library to enabled radio fm
#include <TEA5767.h>
#include <Wire.h>

//setting tx and rx bluetooth module
SoftwareSerial BTSerial(11,10); // RX | TX
//create radio FM object instance
TEA5767 Radio; //Pinout SLC and SDA - Arduino uno pins A5 and A4

char cmd = ' ';
const int led_pin = 0;
const int led_array[] = {2,3,4,5,6,7,8,9,A0,A1,A2,A3,A4,A5};
int counter_led[]  =       {0,0,0,0,0,0,0,0,0,  0, 0, 0, 0, 0};
const int microphone_sensor = A6;
int counter_clap = 0;
int value_microphone = 0;

int timer = 1000;
//variables control radio fm 
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
char inByte = ' ';
int flag=0;
char BTSignal = ' ';

//VARIABLES OF STATES 
bool first_initialize_radio = true;
bool first_start = true;
bool shift_channel = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin,OUTPUT);
  for(int k=0;k<16;k++){
    pinMode(led_array[k],OUTPUT);
  }
  pinMode(microphone_sensor,INPUT);
  digitalWrite(led_pin,LOW);
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  if(first_start){
    BTSerial.println("----------------------------------------------------");
    BTSerial.println("----------------------------------------------------");
    BTSerial.println("WELCOME IN S.MO.KICK.ING ");
    BTSerial.println("----------------------------------------------------");
    BTSerial.println("----------------------------------------------------");
    BTSerial.println("Choose one of the following options:");
    BTSerial.println("0 - LED strips - SWITCH OFF ");
    BTSerial.println("1 - LED strips - NORMAL MODE");
    BTSerial.println("2 - LED strips - SINUSOIDAL ");
    BTSerial.println("3 - LED strips - STROBO LIGHT (15Hz)");
    BTSerial.println("4 - LED strips - STROBO HEAVY(20Hz)");
    BTSerial.println("5 - LED strips - WAVES EFFECT ");
    BTSerial.println("6 - LED strips - BRIGTHNESS EFFECT ");
    BTSerial.println("7 - Radio FM   - ENABLE");
    BTSerial.println("8 - Radio FM   - SHIFT CH. AHEAD");
    BTSerial.println("9 - Radio FM   - SHIFT CH. BACK");
    BTSerial.println("----------------------------------------------------");
    
    first_start = false;
  }
  // put your main code here, to run repeatedly:
   if(BTSerial.available()){
        cmd = BTSerial.read();
         if(cmd == '0'){
           BTSerial.println("LED OFF!");
         }
         if(cmd == '1'){
           BTSerial.println("Normal LED");
         }
         if(cmd == '2'){
           BTSerial.println("Sinusoidal LED");
         }
         if(cmd == '3'){
           BTSerial.println("Strobo LED Light");
         }
         if(cmd == '4'){
            BTSerial.println("Strobo LED Strong");
         }
         if(cmd == '5'){
            BTSerial.println("Waves LED");
         }
         
         if(cmd == '6'){
            BTSerial.println("Brigthness LED");
         }
         
         if(cmd == '7'){
             BTSerial.println("Enable Radio FM");
         }

         if(cmd == '8'){
           BTSerial.println("Shift ahead one radio station");
           search_mode = 1;
           shift_channel = true;
           flag = 0;
           inByte = '8';
           radio_fm();
         }
         if(cmd == '9'){
           BTSerial.println("Shift behind one radio station");
           search_mode = 1;
           shift_channel = true;
           flag = 0;
           inByte = '8';
           radio_fm();
         }
       
   }

   if(cmd == ' ') 
      led_snake_sinusuoidal(); // led_snake(); // //led_snake_random(); //led_pulse_start();
  

 /* if(analogRead(microphone_sensor)>12)
  {
    //Serial.println("CLAP");
      Serial.print("microphone value = ");
      value_microphone = analogRead(microphone_sensor);
      Serial.println(value_microphone);
      delay(500);

  }else{
    //Serial.println("No clap");
  }*/
  
   if(cmd == '0')
     led_off();
     
   if(cmd == '1')
     led_normal();

   if(cmd == '2'){
     led_sinusoidal();
   }
   
   if(cmd == '3')
       led_strobo_light();

   if(cmd == '4') 
       led_strobo_strong();   

   if(cmd == '5') 
       //led_waves();  
   
   if(cmd == '6') 
       led_pulsar();      
       
   if(cmd == '7')
     radio_fm();


   


}

void led_strobo_light(){
  Serial.print("Enabled led strobo light");
  digitalWrite(led_pin,HIGH);
  delay(67);
  digitalWrite(led_pin,LOW);
  delay(67);
}
void led_strobo_strong(){
  Serial.print("Enabled led strobo strong");
  digitalWrite(led_pin,HIGH);
  delay(50);
  digitalWrite(led_pin,LOW);
  delay(50);
}
void led_normal(){
  digitalWrite(led_pin,HIGH);
}
void led_off(){
  digitalWrite(led_pin,LOW);
}
void led_pulse_start(){
  
   if(round(timer)>0){
    digitalWrite(led_pin,LOW);
    delay(timer);
    digitalWrite(led_pin,HIGH);
    delay(timer);
   
    if(timer == 500){
      BTSerial.println("----------------------------------------------------");
      BTSerial.println("Initializing smart module ...");
    }
    if(timer>500)
       timer = timer - 100;
    if(timer>250&&timer<=500) 
       timer = timer - 50;
    if(timer>100&&timer<=250)
       timer = timer - 25;
    if(timer>50&&timer<=100)
        timer = timer - 5;
    if(timer<=50) 
        timer = timer - 1;

    Serial.println(timer);
  }  
}
void led_waves(){
    for(int j = 25;j>5;j--){
       digitalWrite(led_pin,HIGH);
       delay(j);
       digitalWrite(led_pin,LOW);
       delay(j);
       Serial.println(j);
    }  
    for(int j = 5;j<25;j++){
       digitalWrite(led_pin,HIGH);
       delay(j);
       digitalWrite(led_pin,LOW);
       delay(j);
       Serial.println(j);
    }  
}
void led_pulsar(){
 
    for(int j = 0;j<255;j++){
       analogWrite(led_pin,j);
       delay(5);
       //Serial.println(j);
    }  

    for(int j = 255;j>0;j--){
       analogWrite(led_pin,j);
       delay(5);
       //Serial.println(j);
    }  
    delay(100);
}
void led_sinusoidal(){
  float something = millis()/1000.0;
  int value = 128.0 + 128 * sin( something * 2.0 * 255 );
  analogWrite(led_pin,value);
  delay(100);
}

void led_snake(){
  int lenght_array = 16;
  for(int i=0;i<lenght_array;i++){
     digitalWrite(led_array[i],HIGH);
     //Serial.print("Switch LED ");
     //Serial.println(i);
     delay(50);
  }
  for(int j=0;j<lenght_array;j++){
     digitalWrite(led_array[j],LOW);
     delay(50);
  }
}
void led_snake_sinusuoidal(){
  int lenght_array = 16;
  for(int i=0;i<lenght_array;i++){    
     float something = millis()/1000.0;
     int value = 128.0 + 128 * sin( something * 2.0 * 255 );
     analogWrite(led_array[i],value);
    // Serial.print("Switch LED ");
    // Serial.println(i);
     delay(10);
  }

   for(int j=lenght_array;j>=0;j--){    
     float something = millis()/1000.0;
     int value = 128.0 + 128 * sin( something * 2.0 * 255 );
     analogWrite(led_array[j],value);
     //Serial.print("Switch LED ");
     //Serial.println(j);
     delay(10);
  }

}
void led_snake_random(){
  int lenght_array = 16;
  int random_led = random(0,lenght_array);
  for(int i=0;i<lenght_array;i++){    
     digitalWrite(led_array[i],LOW);
  }
  digitalWrite(led_array[random_led],HIGH);

  /*counter_led[random_led]++;
  Serial.println("-------------------------------");
  for(int i=0;i<lenght_array;i++){
    
    Serial.print("counter_led ");
    Serial.print(i);
    Serial.print(" on = ");
    Serial.println(counter_led[i]);
  }
  Serial.println("-------------------------------");*/
  
  delay(100); 
}
void radio_fm(){
  
  if(first_initialize_radio){
     BTSerial.println("----------------------------------------------------");
     BTSerial.println("Radio Enabled!");
     Radio.init();
     Radio.set_frequency(96.2); //On power on go to station 95.2
     first_initialize_radio = false;
  }

  
  if (Radio.read_status(buf) == 1 || shift_channel == true) {
     current_freq =  floor (Radio.frequency_available (buf) / 100000 + .5) / 10;
     stereo = Radio.stereo(buf);
     signal_level = Radio.signal_level(buf);
     //By using flag variable the message will be printed only one time. 
       
      if(flag == 0){
      
          Serial.print("Current freq: ");
          BTSerial.println("Current freq: ");
          
          Serial.print(current_freq);
          Serial.print("MHz Signal: ");
          
          BTSerial.print(current_freq);
          BTSerial.print("MHz Signal: ");
          
          //Strereo or mono ?
          if (stereo){
            Serial.print("STEREO "); 
            BTSerial.println("STEREO ");
          } 
          else{
            Serial.print("MONO ");
            BTSerial.println("MONO ");
          } 
      
          Serial.print(signal_level);
          Serial.println("/15");
          BTSerial.print(signal_level);
          BTSerial.println("/15");
          flag=1;
     }
  }
     
  //When button pressed, search for new station
  if (search_mode == 1) {
      if (Radio.process_search (buf, search_direction) == 1) {
          search_mode = 0;
          shift_channel = false;
      }
  }
  //If forward button is pressed, go up to next station
  if (inByte == '8') {
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_UP;
    Radio.search_up(buf);
  }
  //If backward button is pressed, go down to next station
  if (inByte == '9') {
    last_pressed = current_millis;
    search_mode = 1;
    search_direction = TEA5767_SEARCH_DIR_DOWN;
    Radio.search_down(buf);
  } 
  delay(500);

}
