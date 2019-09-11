#include <Arduino.h>
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

  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
}

void loop()
{
  mySwitch.send(1, 24);
  mySwitch.send(3, 24);
  delay(2000);
  mySwitch.send(0, 24);
  mySwitch.send(2, 24);
  delay(2000);

// /* See Example: TypeA_WithDIPSwitches */
// mySwitch.switchOn("11111", "00010");
// delay(1000);
// mySwitch.switchOff("11111", "00010");
// delay(1000);

// /* Same switch as above, but using decimal code */
// mySwitch.send(1, 24);
// mySwitch.send(3, 24);
// delay(1000);
// mySwitch.send(0, 24);
// mySwitch.send(2, 24);
// delay(1000);
// mySwitch.send(5396, 24);
// delay(1000);

// /* Same switch as above, but using binary code */
// mySwitch.send("000000000001010100010001");
// delay(1000);
// mySwitch.send("000000000001010100010100");
// delay(1000);

// /* Same switch as above, but tri-state code */
// mySwitch.sendTriState("00000FFF0F0F");
// delay(1000);
// mySwitch.sendTriState("00000FFF0FF0");
// delay(1000);

// delay(20000);
}


// Pines para motores
// #define MOTOR_IZQUIERDA_A 3
// #define MOTOR_IZQUIERDA_B 9
// #define MOTOR_DERECHA_A 10
// #define MOTOR_DERECHA_B 11

// void setup()
// {
//   pinMode(MOTOR_DERECHA_A, OUTPUT);
//   pinMode(MOTOR_DERECHA_B, OUTPUT);
//   pinMode(MOTOR_IZQUIERDA_A, OUTPUT);
//   pinMode(MOTOR_IZQUIERDA_B, OUTPUT);

//   mySwitch.enableReceive(0); // Receiver on interrupt 0 => that is pin #2
//   // Serial.begin(9600);
// }

// void loop()
// {
//   if (mySwitch.available())
//   {
//     byte vel = mySwitch.getReceivedValue();
    
//     if(vel >= 2) {
//       vel -= 2;
//       analogWrite(MOTOR_DERECHA_A, LOW);
//       analogWrite(MOTOR_DERECHA_B, vel);
//     } else {
//       analogWrite(MOTOR_IZQUIERDA_A, LOW);
//       analogWrite(MOTOR_IZQUIERDA_B, vel);
//     }

//     // Serial.print(mySwitch.getReceivedValue());
//     // Serial.print(mySwitch.getReceivedBitlength());
//     // Serial.println(mySwitch.getReceivedProtocol());

//     mySwitch.resetAvailable();
//   }
// }