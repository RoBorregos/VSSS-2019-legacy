#ifndef SERIAL_TRANSMIT_H
#define SERIAL_TRANSMIT_H

#include <climits>
#include <string>
#include <unistd.h>

class SerialTransmitt { 
  public:

    SerialTransmitt() {};
    ~SerialTransmitt() {
        close(file_descriptor_);
    };

    // Not copyable or movable
    SerialTransmitt(const SerialTransmitt&) = delete;
    SerialTransmitt& operator=(const SerialTransmitt&) = delete;

    bool Configure(std::string port);
    bool SendPacket(uint32_t packet);

  private:
    int file_descriptor_ = INT_MIN;
};

#endif