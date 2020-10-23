/*
  SD card datalogger

  This example shows how to log data from three analog sensors
  to an SD card using the SD library.

  The circuit:
   analog sensors on analog ins 0, 1, and 2
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 8 (for MKRZero SD: SDCARD_SS_PIN)

  created  24 Nov 2010
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

*/

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"
DS1307 rtc;

//include libraries and instance of dht sensor

#include "DHT.h"
#define DHTPIN 3
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);


#include <SPI.h>
#include <SD.h>

const int chipSelect = 8;
const int tone_buzzer = 2;

// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  //initialize dht sensor
  dht.begin();


  //initialize i2c communication 
  Wire.begin();
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {  
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  rtc.begin();

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

}

void loop() {

  //start counting date and time
  DateTime now = rtc.now();
  
  // make a string for assembling the data to log:
  String dataString = "";
  String dataTime = "";

  File dataFile = SD.open("atmoscan.txt", FILE_WRITE);

  // if the file is available, write to it:
   
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }else{

       
    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);
    
    
    Serial.print(prefixZero(now.day()));
    Serial.print('/');
    Serial.print(prefixZero(now.month()));
    Serial.print('/');
    Serial.print(now.year(), DEC);

    
    Serial.print(' ');
    Serial.print(prefixZero(now.hour()));
    Serial.print(':');
    Serial.print(prefixZero(now.minute()));
    Serial.print(':');
    Serial.print(prefixZero(now.second()));
    Serial.println();
    
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));
    dataTime = "Data: "+String(now.day(),DEC)+"/"+String(now.month(),DEC)+"/"+String(now.year(),DEC);
    dataString = " Time: "+String(prefixZero(now.hour()))+":"+String(prefixZero(now.minute()))+":"+String(prefixZero(now.second()))+" Humidity: "+ String(h)+" % Temperature: "+ String(t)+" C° Heat Index: "+ String(hic)+" C° "+ String(hif) + " F°";
   // Serial.println("DATA STRING =" +dataString);
  }

   

  if (dataFile) {
    dataFile.print(dataTime);
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
    tone(tone_buzzer, 424, 1000);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening atmoscan.txt");
  }
  
  dataFile.close();
  
  delay(30000);
}


String prefixZero(uint8_t value){
  if(value<10)
    return "0"+String(value);
  return String(value);  
}
