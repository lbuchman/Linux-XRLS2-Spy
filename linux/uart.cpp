/*
MIT License

Copyright (c) 2019-2020 Horizon Hobby, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// C library headers
#include <stdio.h>
#include <string.h>
#include <cstdint>
#include <iostream>
#include <vector>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include <loggingLib/log.h>

using namespace std;

std::vector<int> uarts;        

/**
    @brief  open uart device

    @param  device:         null terminated sring to the device file, platform specific
    @param  baudrate        115200 or 400000
    @return bool:           uart index or < 0 if error
*/
uint8_t uartInit ( const char* device, const int baudrate ) {
    int serial_port = open ( device, O_RDWR );
    
    if (serial_port < 0) {
        logme(kLogError, LINEINFOFORMAT "IError %i from open():  %s\n", LINEINFO, errno, strerror ( errno ) );
        return  serial_port;   
    }

    // Create new termios struc, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if ( tcgetattr ( serial_port, &tty ) != 0 ) {
        logme(kLogError, LINEINFOFORMAT "Error %i from tcgetattr():  %s\n", LINEINFO, errno, strerror ( errno ) );
        close(serial_port);
        return -1;
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~ ( IXON | IXOFF | IXANY ); // Turn off s/w flow ctrl
    tty.c_iflag &= ~ ( IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL ); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed ( &tty, B9600 );
    cfsetospeed ( &tty, B9600 );

    // Save tty settings, also checking for error
    if ( tcsetattr ( serial_port, TCSANOW, &tty ) != 0 ) {
        logme(kLogError, LINEINFOFORMAT "Error %i from tcgetattr():  %s\n", LINEINFO, errno, strerror ( errno ) );
        close(serial_port);
        return -1;
    }

    
    uarts.push_back(serial_port);
    return uarts.size() - 1;
}

/**
    @brief  close open uart device

    @param  uart:           uart number
    @return status:         0 is OK, < 0 failure
*/
void uartClose ( uint uart) {
    if (uart > uarts.size() - 1) {
        logme(kLogError, LINEINFOFORMAT "Error %i from close():  %s\n", LINEINFO, errno, strerror ( errno ) );
        return;
    }
    close(uart);
}
