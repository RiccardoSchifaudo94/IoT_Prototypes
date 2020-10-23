/*
 ARDUINO BLUETOOTH SMART WATCH

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.


  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

*/

// include the library code:
#include <LiquidCrystal.h>

//include library to control bluetooth communication
#include <SoftwareSerial.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
LiquidCrystal lcd(7, 8, 9, 5, 6, 12);

//setting tx and rx bluetooth module
SoftwareSerial BTSerial(11,10); // RX | TX

int minutes = 0;
int hours = 0;
char cmd = 'T';
String cx_cmd = "";

//booleans for machine states
bool print_header = true;
bool enable_switch = true;
bool hours_to_set = false;
bool minutes_to_set = false;
bool set_clock_alarm = false;
bool enabled_clock_alarm = false;
bool enabled_led_strip = false;

//settings variables
int hours_setted = 0;
int minutes_setted = 0;
int clock_alarm_hours = 0;
int clock_alarm_minutes = 0;

const int led_pin = 2;
const int button_pin = 3;

void setup() {
  //set led pin
  pinMode(led_pin,OUTPUT);
  digitalWrite(led_pin,LOW);
  pinMode(button_pin,INPUT);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("BLUETOOTH");
  lcd.setCursor(0,1);
  lcd.print("SMART WATCH");
  Serial.begin(9600);
  BTSerial.begin(9600);  // HC-05 default speed in AT command more
 
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):

//led_strobo();
bluetooth_listener();

}


  


void bluetooth_listener(){
    
    if(digitalRead(button_pin)==HIGH)
         digitalWrite(led_pin,HIGH);
      else 
         digitalWrite(led_pin,LOW);     

    if(enabled_led_strip)
      led_strobo();
         

    if(print_header){
       BTSerial.println("--------------------------------------");
       BTSerial.println("SETTINGS");
       BTSerial.println("--------------------------------------");
       BTSerial.println("Choose one of following options:");
       BTSerial.println("1-Timer");
       BTSerial.println("2-Countdown");
       BTSerial.println("3-Clock Alarm");
       BTSerial.println("4-Enabled/Disabled Clock Alarm");
       BTSerial.println("5-Enabled/Disabled Led Strobo");
       print_header = false;  
    }
    if(BTSerial.available()){
      
      if(enable_switch)
       cmd = BTSerial.read();
      else 
       cx_cmd = BTSerial.readString();
         
       if(enable_switch==true)
       {
           switch(cmd){
            case '1':
              BTSerial.println("You choose CLOCK MODE");
              enabled_led_strip = false;
              enable_switch = false;
              hours_to_set = true;
              BTSerial.println("Set hour from 0 to 23");
              //timer(21,30);
            break;
            case '2':
              BTSerial.println("You choose COUNTDOWN MODE");
              enabled_led_strip = false;
              enable_switch = false;
              hours_to_set = true;
              BTSerial.println("Set hour from 0 to 23");
            break;
            case '3':
             BTSerial.println("You choose CLOCK ALARM MODE");
             enabled_led_strip = false;
             enable_switch = false;
             hours_to_set = true;
            break;
            case '4':
              if(enabled_clock_alarm){
                enabled_clock_alarm = false;
                digitalWrite(led_pin,LOW);
                BTSerial.println("CLOCK ALARM Disabled");
              }//close if enabled clock alarm
              else{
                enabled_clock_alarm = true;
                BTSerial.println("CLOCK ALARM Enabled");
              }
            break;
            case '5':
            if(!enabled_led_strip){
              enabled_led_strip = true;
            }else{
              enabled_led_strip = false;
            }  
            break;
            case 'R':
              loop();
            break;  
            default:
              print_header = true;
              BTSerial.println("Incorrect choice! change command please...");
            break;     
         }//close switch
       }//close enable_switch 
       else{
          //if block of setting minutes range 
          if(minutes_to_set){
              int park_minute = 0;
                  park_minute = cx_cmd.toInt();
                   
               if(park_minute>=0&&park_minute<=59){
                 minutes_setted = park_minute;
                 //disable block setting minutes and restore main menu
                 minutes_to_set = false;
                 enable_switch = true;
                 //give alert of success operation setting minutes
                 BTSerial.println("Minutes setted successfully!");
               }//close if cmd  
               else{
                BTSerial.println("You are out of range. Please reset againg from 0 to 59");
               }

               //pass parameters of hours and minutes if you choose clock mode
               if(cmd=='1'){ 
                 
                 BTSerial.println("------------------------------");
                 BTSerial.println("CLOCK MODE Activate!");
                 BTSerial.println("------------------------------");
               
                 timer(hours_setted,minutes_setted);
               }
               //pass parameters of hours and minutes if you choose countdown mode
               if(cmd=='2'){
                 
                 BTSerial.println("------------------------------");
                 BTSerial.println("COUNTDOWN MODE Activate!");
                 BTSerial.println("------------------------------");
                 
                 countdown(hours_setted,minutes_setted);
               }  

               if(cmd=='3'){
                 BTSerial.println("------------------------------");
                 BTSerial.println("CLOCK ALARM MODE Activate!");
                 BTSerial.println("------------------------------");    
                 set_clock_alarm = false;
                 enabled_clock_alarm = true;   
                 clock_alarm(hours_setted,minutes_setted,set_clock_alarm,enabled_clock_alarm);
               }  //close if cmd == 3

                if(cmd=='5'){
                 BTSerial.println("------------------------------");
                 BTSerial.println("LED Strips Activate!");
                 BTSerial.println("------------------------------");    
                 led_strobo();
               }  //close if cmd == 3
               
               print_header = true;
          }// close if minute_to_set
          
          if(hours_to_set){
            
               int park_hour = 0;
                   park_hour = cx_cmd.toInt();
            
               if(park_hour>=0&&park_hour<=23){
                 hours_setted = park_hour;
                 BTSerial.println("Hour setted successfully!");
                 BTSerial.println("Set minutes in a range of 0 to 59");
                 cx_cmd = "";
                 hours_to_set = false;
                 minutes_to_set = true;
               }//close if cmd  
               else{
                  BTSerial.println("You are out of range. Please reset againg from 0 to 23"); 
               }
               
          }//close if hours_to_set


       }  
   }//close Bluetooth listener

}

/*------------------------------------------------------------------------------------------------------------------*/
/* -----------------------------------------------   FUNCTIONS LIST    ---------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------*/

void timer(int hours,int minutes){

  for(int seconds = 0;seconds<=60;seconds++){
    //instance of bluetooth listener for main menu
    bluetooth_listener();
    
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Current TIME:");
    lcd.setCursor(1,1);

    //algorithm of clock timer
    if(seconds==60){
      seconds = 0;
      minutes++;
      if(minutes==60){
        minutes = 0;
        hours++;
         if(hours>23){
            hours = 0;
          }//close if hours>23
         /*if(hours==0&&minutes==0&&seconds==0){
          lcd.clear();
          lcd.print("Happy NEW YEAR!");
          delay(5000);
          seconds=6;
         }*/
      }//close if minutes == 60 
    }//close if seconds == 60  
     
    //print time format on LCD display
    lcd.print(hours);
    lcd.print("H :");
    lcd.setCursor(6,1);
    lcd.print(minutes);
    lcd.print("m :");
    lcd.setCursor(11,1);
    lcd.print(seconds);
    lcd.print("s");
    //this is the magic! ;)
    delay(1000);
    clock_alarm(hours,minutes,set_clock_alarm,enabled_clock_alarm);  
  }
}//close timer clock function

void countdown(int hours,int minutes){
  
  bool first_start = true;
  
  //loop of countdown
  for(int seconds = 59;seconds>=0;seconds--){
    
    //instance bluetooth lister of main menu
    bluetooth_listener();
    
    //first init display in countdown mode
    if(first_start){
      seconds = 0;
      first_start = false;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("COUNTDOWN");
      lcd.setCursor(1,1);
      lcd.print("Ready!");
      delay(3000);
    }
   
    
    if(hours==0&&minutes==0&&seconds==0){
        
          lcd.clear();
          lcd.print("BOOM!");
          
          //switch on the led
          digitalWrite(led_pin,HIGH);
          //wait 10 seconds
          delay(10000);
          lcd.clear();
          //arrest loop of countdown
          seconds=-1;
     
    } //close if BOOM
    else{
    
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("COUNTDOWN");
    lcd.setCursor(1,1); 

    if(seconds==0){
      seconds = 59;
      if(minutes>0)
        minutes--; 
      else  {
        minutes=59;   
        if(hours>0)
              hours--; 
      }//close else of minutes>0
    }  //close seconds == 0
  
    lcd.print(hours);
    lcd.print("H :");
    lcd.setCursor(6,1);
    lcd.print(minutes);
    lcd.print("m :");
    lcd.setCursor(11,1);
    lcd.print(seconds);
    lcd.print("s");
    //this is the magic! ;)
    delay(1000);
    }//close else of if h m s == 0 BOOM  
  }
}//close countdown function

void clock_alarm(int hours,int minutes,bool set_clock_alarm ,bool enabled_clock_alarm){
     
      if(set_clock_alarm){
        clock_alarm_hours = hours;
        clock_alarm_minutes = minutes;
        BTSerial.println("Hours and minutes changed in clock alarm by setting mode");
        set_clock_alarm = false;
      }
      if(enabled_clock_alarm){
        if(clock_alarm_hours==hours&&clock_alarm_minutes==minutes){
          lcd.clear();
          lcd.print("CLOCK ALARM");
          lcd.setCursor(0,1);
          lcd.print("Wake up!");
          digitalWrite(led_pin,HIGH);
          delay(10000);
          enabled_clock_alarm = false;
        }//close if control current hours and minutes
      }
}

void led_strobo(){
  bluetooth_listener();
  Serial.print("Enabled led strobo");
   lcd.clear();
   lcd.print("EMERGENCY SYSTEM");
   lcd.setCursor(0,1);
   lcd.print("Activate!");
  digitalWrite(led_pin,HIGH);
  delay(50);
  digitalWrite(led_pin,LOW);
  delay(50);
}
