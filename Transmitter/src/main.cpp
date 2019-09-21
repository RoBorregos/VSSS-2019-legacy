#include <Arduino.h>
#include <String.h>
#include "RCSwitch.h"

RCSwitch mySwitch = RCSwitch();

void setup()
{

  Serial.begin(9600);

  // Transmitter is connected to Arduino Pin #10
  mySwitch.enableTransmit(10);

  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  // Optional set pulse length.
  // mySwitch.setPulseLength(320);

  // Optional set number of transmission repetitsions.
  // mySwitch.setRepeatTransmit(15);
}

void loop()
{
  // byte velIzq = 155;  
  // byte velDer = 154;
  // unsigned long total = 0b0000000000000011;
  // total = (total << 8) | velIzq;
  // total = (total << 8) | velDer;
  // mySwitch.send(total, 32);
  // Serial.println("");

  Serial.println("Izq: ");
  while(!Serial.available());
  byte velIzq = Serial.read();  
  String x = Serial.readString();  
  Serial.println("Der: ");
  while(!Serial.available());
  byte velDer = Serial.read();
  x = Serial.readString();  
  unsigned long total = 0b0000000000000011;
  total = (total << 8) | velIzq;
  total = (total << 8) | velDer;
  mySwitch.send(total, 32);
  Serial.println("");
}