int pinA = 3; // Connected to CLK 
int pinB = 4; // Connected to DT
int encoderPosCount = 0;
int escEnableA = 5; //enableMotor
int escEnableB = 6; 
int escEnable = A1;
int pinALast;
int aVal;
boolean bCW;
void setup() 
{
pinMode (pinA,INPUT);
pinMode (pinB,INPUT);
pinMode (escEnable,OUTPUT);
pinMode (escEnableA,OUTPUT);
pinMode (escEnableB,OUTPUT);
/* Read Pin A
Whatever state it's in will reflect the last position
*/
pinALast = digitalRead(pinA); 
digitalWrite(escEnableA, HIGH);
digitalWrite(escEnableB, LOW);
Serial.begin (9600);
}


void loop() {
aVal = digitalRead(pinA);
if (aVal != pinALast)
{                                         // Means the knob is rotating
                                         // if the knob is rotating, we need to determine direction
                                        // We do that by reading pin B.
if (digitalRead(pinB) != aVal)
{                                     // Means pin A Changed first - We're Rotating Clockwise
    if(encoderPosCount<255)
      encoderPosCount = encoderPosCount+10 ; 
    analogWrite(escEnable,encoderPosCount);
    bCW = true;
} 
else {  
    // Otherwise B changed first and we're moving CCW bCW = false;
    if(encoderPosCount>0)
      encoderPosCount = encoderPosCount-10;
    
    analogWrite(escEnable,encoderPosCount);
}

Serial.print ("Rotated: "); 
if (bCW)
{
  Serial.println ("clockwise");
}else
{
 
Serial.println("counterclockwise");
}
Serial.print("Encoder Position: ");
Serial.println(encoderPosCount);
}
pinALast = aVal;
}
