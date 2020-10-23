// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"

DS1307 rtc;

void setup () {
  Serial.begin(57600);
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop () {
    DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(prefixZero(now.month()));
    Serial.print('/');
    Serial.print(prefixZero(now.day()));
    Serial.print(' ');
    Serial.print(prefixZero(now.hour()));
    Serial.print(':');
    Serial.print(prefixZero(now.minute()));
    Serial.print(':');
    Serial.print(prefixZero(now.second()));
    Serial.println();
    
    Serial.println();
    delay(3000);
}

String prefixZero(uint8_t value){
  if(value<10)
    return "0"+String(value);
  return String(value);  
}
