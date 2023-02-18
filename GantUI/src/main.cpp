#include <Arduino.h>
#include <string>

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potA = 34;
const int potB = 35;
const int potC = 32;


// variable for storing the potentiometer value
int potValue1 = 0;
int potValue2 = 0;
int potValue3 = 0;

void SetupPin();
void lecturePot(String* ser); 
void lectureIMU(String* ser);


void setup() {
  SetupPin(); 
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // Reading potentiometer value

  String comm = "<";

  lecturePot(&comm);
  lectureIMU(&comm);

  
  comm += ">";

  Serial.println(comm);
  delay(50);
}


void SetupPin()
{
  pinMode(potA, INPUT);
  pinMode(potB, INPUT);
  pinMode(potC, INPUT);
}


void lecturePot(String* ser)
{
  /* Lit et transmet les donnée des potentiomètre en angle  */
  float angle1; 
  float angle2; 
  float angle3;

  String toSend;

  angle1 = analogRead(potA) / 4095.0 * 360;
  angle2 = analogRead(potB) / 4095.0 * 360;
  angle3 = analogRead(potC) / 4095.0 * 360;

  toSend = String(angle1) + "A";
  toSend += String(angle2) + "B";
  toSend += String(0) + "C";

  *ser += toSend;
}

void lectureIMU(String* ser)
{
  
}

