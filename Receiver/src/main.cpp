#include <Arduino.h>
#include "RCSwitch.h"

RCSwitch mySwitch = RCSwitch();

// Pines para motores
#define MOTOR_IZQUIERDA_A 10
#define MOTOR_IZQUIERDA_B 11
#define MOTOR_DERECHA_A 9
#define MOTOR_DERECHA_B 3

void setup()
{
  pinMode(MOTOR_DERECHA_A, OUTPUT);
  pinMode(MOTOR_DERECHA_B, OUTPUT);
  pinMode(MOTOR_IZQUIERDA_A, OUTPUT);
  pinMode(MOTOR_IZQUIERDA_B, OUTPUT);

  mySwitch.enableReceive(0); // Receiver on interrupt 0 => that is pin #2
  Serial.begin(9600);
}

void loop()
{
  if (mySwitch.available())
  {
    unsigned long total = mySwitch.getReceivedValue();
    unsigned long velDer = (total & 0x000000FF);
    unsigned long velIzq = ((total & 0x0000FF00) >> 8);
    Serial.println(total, BIN);
    Serial.println((int)velDer, BIN);
    Serial.println((int)velIzq, BIN);

    analogWrite(MOTOR_DERECHA_A, LOW);
    analogWrite(MOTOR_DERECHA_B, (int)velDer);
    analogWrite(MOTOR_IZQUIERDA_A, LOW);
    analogWrite(MOTOR_IZQUIERDA_B, (int)velIzq);

    mySwitch.resetAvailable();
  }
}