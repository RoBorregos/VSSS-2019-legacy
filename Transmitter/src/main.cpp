/* RoBorregos
 * Tecnológico de Monterrey
 * Monterrey, Nuevo León, México
 * 
 * RoBorregos Foxtrot, competing on:
 * LARC - Latin American and Brazilian Robotics Competition
 * VSSS - Very Small Size Soccer
 * 
 * 
 * MAIN FILE FOR TRANSMITTER
 * main.cpp
 * 
 * Last update: September 21, 2019
 *              Ernesto Cervantes
 */

#include <Arduino.h>
#include <String.h>
#include "RCSwitch.h"
#include "Information.h"

RCSwitch transmitter = RCSwitch();

void setup()
{
  Serial.begin(9600);

  // Transmitter is connected to Arduino Pin #10
  transmitter.enableTransmit(10);

  // Optional set protocol (default is 1, will work for most outlets)
  // transmitter.setProtocol(2);

  // Optional set pulse length.
  // transmitter.setPulseLength(320);

  // Optional set number of transmission repetitsions.
  // transmitter.setRepeatTransmit(15);
}

void loop()
{

  // byte velIzq = 155;  
  // byte velDer = 154;
  // unsigned long total = 0b0000000000000011;
  // total = (total << 8) | velIzq;
  // total = (total << 8) | velDer;
  // transmitter.send(total, 32);
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
  transmitter.send(total, 32);
  Serial.println("");
}