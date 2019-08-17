int lucentezza = 0; // definisce la variabile “lucentezza” con valore iniziale 0
int incremento = 10; // definisce la variabile “incremento” con valore 5
int timer = 2000;
void setup ()
{
pinMode (2, OUTPUT); // definisce la porta 9 come porta di output
Serial.begin(9600);
}
void loop()
{
  if(round(timer)>0){
   
    digitalWrite(2,LOW);
    delay(timer);
    digitalWrite(2,HIGH);
    delay(timer);
    
    if(timer>1000)
       timer = timer - 250;
    if(timer>500) 
       timer = timer - 100;
    if(timer>250)
       timer = timer - 50;
    if(timer>100)
        timer = timer - 25;
    if(timer<=100) 
        timer = timer - 5;
  }
}
