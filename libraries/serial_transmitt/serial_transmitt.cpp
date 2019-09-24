#include "serial_transmitt.h"

#include <cstring>
#include <climits>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <termios.h>

namespace {
    bool SetInterfaceAttributes(int fd, int speed, int parity) {
        struct termios tty;
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            return false;
        }

        cfsetospeed(&tty, speed);
        cfsetispeed(&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            return false;
        }
        return true;
    }

    bool SetBlocking(int fd, int should_block) {
        struct termios tty;
        memset(&tty, 0, sizeof tty);
        if (tcgetattr(fd, &tty) != 0) {
            return false;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout
        return true;
    }

} // namespace

bool SerialTransmitt::Configure(std::string port){
    file_descriptor_ = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (file_descriptor_ < 0 
      || !SetInterfaceAttributes(file_descriptor_, B9600, 0)
      || !SetBlocking(file_descriptor_, 0)) {
        file_descriptor_ = INT_MIN;
        return false;
    }
    return true;
}

bool SerialTransmitt::SendPacket(uint32_t packet) {
    if (file_descriptor_ == INT_MIN){
        std::cout << "Make a successful call to Configure first" << std::endl;
        return false;
    }
    write (file_descriptor_, static_cast<void*>(&packet), sizeof(packet));

    return true;
}