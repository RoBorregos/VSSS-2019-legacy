/* RoBorregos
 * Tecnológico de Monterrey
 * Monterrey, Nuevo León, México
 * 
 * RoBorregos Foxtrot, competing on:
 * LARC - Latin American and Brazilian Robotics Competition
 * VSSS - Very Small Size Soccer
 * 
 * 
 * INFORMATION
 * Information.h
 * 
 * Last update: September 21, 2019
 *              Ernesto Cervantes
 */

#ifndef INFORMATION_H
#define INFORMATION_H

#include "stdint.h"

class Information
{
public:
    static void setStop(uint32_t &data);
    static void setSpeedLeft(uint32_t &data, bool forward, uint8_t speed);
    static void setSpeedRight(uint32_t &data, bool forward, uint8_t speed);

    static void clearStop(uint32_t &data);
    static void clearSpeedLeft(uint32_t &data);
    static void clearSpeedRight(uint32_t &data);
    
    static inline bool hasStop(uint32_t data);
    static inline bool hasSpeedLeft(uint32_t data);
    static inline bool hasSpeedRight(uint32_t data);

    static inline bool isLeftForward(uint32_t data);
    static inline bool isRightForward(uint32_t data);

    static uint8_t speedLeft(uint32_t data);
    static uint8_t speedRight(uint32_t data);
};

#endif