#include <EEPROMVar.h>
#include <EEPROMex.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <TEA5767.h>

#define btn1 8
#define btn2 9
#define btnMemo 13
#define btnSel 3

float frequenza = 0;
float frSint;
int indiceLettura = 2;
int indiceScrittura;
short selezione = 0;
int indic;
unsigned long t;
unsigned long t1 = 0;


TEA5767 radio = TEA5767();
LiquidCrystal lcd(12, 11, 5, 4, 7, 6);



/*
    Caratteri personalizzati per il display
*/

byte scala[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
  B00100,
  B10101,
  B10101
};
byte cursore[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B10101
};
byte fm1[] = {
  B00000,
  B00000,
  B00000,
  B00111,
  B00100,
  B00111,
  B00100,
  B00100
};
byte fm2[] = {
  B00000,
  B00000,
  B00000,
  B10001,
  B11011,
  B10101,
  B10001,
  B10001
};
byte audioON[] = {
  B00001,
  B00011,
  B00101,
  B11001,
  B11001,
  B00101,
  B00011,
  B00001
};
byte modoRadio[] = {
  B10000,
  B01000,
  B00100,
  B00010,
  B11111,
  B10001,
  B10001,
  B11111
};
byte modoMemoria[] = {
  B00000,
  B00111,
  B00101,
  B01111,
  B01010,
  B11110,
  B10100,
  B11100
};
byte modoReset[] = {
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B00000,
  B01110
};


/*
    Funzione di riavvio usata dopo il reset della EEprom
*/

void(* Riavvia)(void) = 0;


/*
    Disegno della scala graduata sul display
*/

void scalaGrad()
{
  indic = map(frequenza, 87, 108, 3, 12);

  if (frequenza <= 109 || frequenza >= 87 ) {
    for (int j = 2; j <= 13; j++)
    {
      lcd.setCursor(j, 1);
      lcd.write(3);
    }
    lcd.setCursor(indic, 1);
    lcd.write(4);
  }
}

/*
    Aggiornamento della frequenza dopo
    aver premuto i pulsanti avanti-indietro
*/

void aggiornaFrequenza()
{
  frequenza = constrain(frequenza, 87.5, 108.5);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.write(1);
  lcd.setCursor(2, 0);
  lcd.write(2);
  lcd.setCursor(4, 0);
  lcd.print(frequenza);
  lcd.setCursor(11, 0);
  lcd.print("Mhz");
  if (radio.isStereo()) {
    lcd.setCursor(0, 1);
    lcd.print("s");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("m");
  }
  radio.setFrequency(frequenza);
  scalaGrad();
}

/*
    Ricerca manuale delle stazioni tramite i
    pulsanti di aumento e decremento della frequenza

*/
void ricerca()
{
  while (digitalRead(btn1) == LOW)
  {
    frequenza += 0.1;
    aggiornaFrequenza();
    scalaGrad();
    delay(200);
  }

  while (digitalRead(btn2) == LOW)
  {
    frequenza -= 0.1;
    aggiornaFrequenza();
    scalaGrad();
    delay(200);
  }
}

/*
    Procedura di memorizzazione di una stazione radio su EEPROM.
    Nelle posizioni 0-1 si memorizza con un valore int il
    numero delle stazioni contenute nella memoria,mentre a partire
    dalla posizione 3 vengono memorizzate le frequenze con un valore
    float. Da notare che si tiene conto della lunghezza delle variabili
    usate, cioè int = 2 byte e float = 4 byte.
*/

void memorizzaSuEeprom()
{
  frSint = radio.getFrequency();
  delay(100);
  indiceScrittura = EEPROM.readInt(0);
  EEPROM.writeFloat(indiceScrittura, radio.getFrequency());
  delay(5);
  lcd.setCursor(0, 1);
  lcd.print("Memoriz. OK !");
  indiceScrittura = indiceScrittura + 4;
  EEPROM.writeInt(0, indiceScrittura);
  delay(1000);
  frequenza = frSint;
  aggiornaFrequenza();
}

/*
    Lettura delle stazioni memorizzate su EEPROM

*/

void leggiEeprom()
{
  if (digitalRead(btn2) == LOW)
  {
    frequenza =  EEPROM.readFloat(indiceLettura);
    delay(5);
    if (frequenza >= 87.5  && frequenza <= 108.5) {
      aggiornaFrequenza();
      indiceLettura = indiceLettura + 4;
    } else {
      indiceLettura = 2;
    }
    scalaGrad();
    delay(300);
  }


  if (digitalRead(btn1) == LOW)
  {
    frequenza =  EEPROM.readFloat(indiceLettura);
    delay(5);
    if (frequenza >= 87.5 && frequenza <= 108.5 && indiceLettura >= 2) {
      aggiornaFrequenza();
      indiceLettura = indiceLettura - 4;
    } else {
      indiceLettura = 2;
    }
    scalaGrad();
    delay(300);
  }
}

/*
    Cancellazione EEPROM e riavvio dell'ATmega 328
    Questa procedura richiede alcuni secondi
*/

void cancellaEprom()
{
  if (digitalRead(btn2) == LOW || digitalRead(btn1) == LOW)
  {
    frSint = radio.getFrequency();
    lcd.setCursor(0, 1);
    lcd.print("Reset in corso..");
    delay(1);
    EEPROM.writeInt(0, 2);
    delay(10);

    for (int i = 2; i < 1024; i++)
    {
      EEPROM.writeInt(i, 0);
      delay(5);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reset finito    ");
    lcd.setCursor(0, 1);
    lcd.print("Riavvio in corso..");
    delay(3000);
    Riavvia();

  }
}

void setup()
{
  delay(1000);
  Wire.begin();
  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btnMemo, INPUT);
  pinMode(btnSel, INPUT);

  frequenza = 91.1;
  radio.setMuted(false);
  radio.setFrequency(frequenza);

  lcd.createChar(1, fm1);
  lcd.createChar(2, fm2);
  lcd.createChar(3, scala);
  lcd.createChar(4, cursore);
  lcd.createChar(5, audioON);
  lcd.createChar(6, modoRadio);
  lcd.createChar(7, modoMemoria);
  lcd.createChar(8, modoReset);




  lcd.begin(16, 2);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Arduino Radio FM");
  lcd.setCursor(4, 1);
  lcd.print("TEA 5767");
  delay(2000);

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.write(1);
  lcd.setCursor(2, 0);
  lcd.write(2);
  lcd.setCursor(4, 0);
  lcd.print(frequenza);
  lcd.setCursor(11, 0);
  lcd.print("Mhz");
  if (radio.isStereo()) {
    lcd.setCursor(0, 1);
    lcd.print("s");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("m");
  }
  lcd.setCursor(15, 1);
  lcd.write(6);
  scalaGrad();
}


void loop()
{


  t = millis();

  if (radio.isMuted() == false) {
    lcd.setCursor(15, 0);
    lcd.write(5);
  } else {

    if ( t > t1 + 1000) {
      lcd.setCursor(15, 0);
      lcd.print(" ");
    }
    if ( t > t1 + 2000 ) {
      lcd.setCursor(15, 0);
      lcd.write(5);
      t1 = millis();
    }
  }

  /*
      Pulsante per memorizzare la frequenza
      su cui si è sintonizzati
  */
  if (digitalRead(btnMemo) == LOW) {
    memorizzaSuEeprom();
  }

  /*
          Pulsante selezione delle funzioni :
      1 - Scorrimento avanti/indietro delle frequenze
      2 - Lettura delle stazioni memorizzate sulla EEPROM
      3 - Cancellazione EEPROM e riavvio
      4 - Audio OFF
      5 - Audio ON

  */
  if (digitalRead(btnSel) == LOW) {
    selezione++;
    delay(200);
    if (selezione > 4 )
    {
      selezione = 0;
    }
  }


  switch (selezione)
  {
    case 0 :
      lcd.setCursor(15, 1);
      lcd.write(6);
      ricerca();
      break;

    case 1:
      lcd.setCursor(15, 1);
      lcd.write(7);
      leggiEeprom();
      break;

    case 2:
      lcd.setCursor(15, 1);
      lcd.write(8);
      cancellaEprom();
      break;

    case 3:
      delay(250);
      radio.setMuted(true);
      lcd.setCursor(15, 1);
      lcd.print(" ");
      break;

    case 4:
      delay(250);
      radio.setMuted(false);
      lcd.setCursor(15, 1);
      lcd.print(" ");
      break;
  }
}
