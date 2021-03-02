/*
MIT License

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

#include <iostream>
#include <vector>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>

#include <uart.h>
#include <loggingLib/log.h>
#include <elapsedTime.h>

using namespace std;

static std::vector<int> uarts;
static int getTermiosBaudrate ( const int baudrate );
static void signal_handler_IO ( int status );


/**
    @brief  signal handler
    @return void
*/
void signal_handler_IO ( int signalNumber ) {
    logme ( kLogError, LINEINFOFORMAT "Cought Signal %i\n", LINEINFO, signalNumber );

    //  wait_flag = FALSE;
}


/**
    @brief  convert baudrate to getTermiosBaudrate

    @param  @param  baudrate        115200 or 400000
    @return getTermiosBaudrate
*/
int getTermiosBaudrate ( const int baudrate ) {
    switch ( baudrate ) {
    case  115200:
        return B115200;
    case  230400:
        return B230400;
    case  460800:
        return B460800;
    case  500000:
        return B500000;
    case  921600:
        return B921600;

    default:
        logme ( kLogError, LINEINFOFORMAT "Invalid baudrate %i, returning default 115200\n", LINEINFO, B115200 );
        return B115200;
    }

    return B115200;
}

/**
    @brief  open uart device

    @param  device:         null terminated sring to the device file, platform specific
    @param  baudrate        115200 or 400000
    @return bool:           uart index or < 0 if error
*/
int8_t uartInit ( const char* device, const int baudRate ) {
    struct sigaction saio;           /* definition of signal action */

    int serial_port = open ( device, O_RDWR | O_EXCL | O_NDELAY | O_NOCTTY );

    if ( serial_port < 0 ) {
        logme ( kLogError, LINEINFOFORMAT "Error %i from open():  %s, device %s\n", LINEINFO, errno, strerror ( errno ), device );
        return  serial_port;
    }

    uarts.push_back ( serial_port );
    uint8_t uartNum = uarts.size() - 1;

    /* install the signal handler before making the device asynchronous */
    saio.sa_handler = signal_handler_IO;
    sigemptyset ( &saio.sa_mask );
    saio.sa_flags = 0;
    saio.sa_restorer = NULL;
    if ( sigaction ( SIGIO, &saio, NULL ) < 0 ) {
        logme ( kLogError, LINEINFOFORMAT "Error %i from sigaction():  %s\n", LINEINFO, errno, strerror ( errno ) );
        return -1;
    }


    if ( uartSetBaud ( uartNum, baudRate ) > 0 ) {
        return -1;
    }

    return uartNum;
}

/**
    @brief  close open uart device

    @param  uart:           uart number
    @return status:         0 is OK, < 0 failure
*/
void uartClose ( uint8_t uartNum ) {
    if ( uartNum > uarts.size() - 1 ) {
        logme ( kLogError, LINEINFOFORMAT "Error invalid uart %i:  %s\n", LINEINFO, uartNum );
        return;
    }
    int & serial_port = uarts.at ( uartNum );
    close ( serial_port );
}

/**
    @brief  set baudrate

    @param  uart:           uart number
    @param  baudRate:       baudrate
    @return void
*/
int8_t uartSetBaud ( uint8_t uartNum, uint32_t baudRate ) {

    if ( uartNum > uarts.size() - 1 ) {
        logme ( kLogError, LINEINFOFORMAT "Error invalid uart %i:  %s\n", LINEINFO, uartNum );
        return -1;
    }

    int & serial_port = uarts.at ( uartNum );

    // Create new termios struc, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    if ( tcgetattr ( serial_port, &tty ) != 0 ) {
        logme ( kLogError, LINEINFOFORMAT "Error %i from tcgetattr():  %s\n", LINEINFO, errno, strerror ( errno ) );
        close ( serial_port );
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

    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed ( &tty, getTermiosBaudrate ( baudRate ) );
    cfsetospeed ( &tty, getTermiosBaudrate ( baudRate ) );

    tcflush ( serial_port, TCIFLUSH );

    // Save tty settings, also checking for error
    if ( tcsetattr ( serial_port, TCSANOW, &tty ) != 0 ) {
        logme ( kLogError, LINEINFOFORMAT "Error %i from tcgetattr():  %s\n", LINEINFO, errno, strerror ( errno ) );
        close ( serial_port );
        return -1;
    }

    return 0;
}



/**
    @brief  receive data from uart with timeout

    @param  uart:           uart number
    @param  pBuffer:        pointer to the buffer
    @param  bufferSize:     buffer size
    @param  timeout_ms:     timeout in mSec
    @return status:         0 is OK, < 0 failure
*/
int8_t uartReceive ( uint8_t uartNum, uint8_t* pBuffer, uint8_t bufferSize, uint8_t timeout_ms ) {

    struct timeval tv;
    fd_set rfds;


    if ( uartNum > uarts.size() - 1 ) {
        logme ( kLogError, LINEINFOFORMAT "Error invalid uart %i:  %s\n", LINEINFO, uartNum );
        return -1;
    }

    int & serial_port = uarts.at ( uartNum );

    FD_ZERO ( &rfds );
    FD_SET ( 0, &rfds );
    FD_SET ( serial_port, &rfds );

    tv.tv_sec = 0;
    tv.tv_usec = timeout_ms * 1000;

    int retval = select ( serial_port+1, &rfds, NULL, NULL, &tv );

        if ( retval == 0) { //timeout
            return 0;
        }
    
    if ( retval < 0) {
        logme ( kLogError, LINEINFOFORMAT "Error %i from select():  %s\n", LINEINFO, errno, strerror ( errno ) );
        return -1;
    }

    if ( retval ) {
        int size = read ( serial_port, pBuffer, bufferSize );
        if ( size < 0 ) {
            if (errno == EAGAIN) return 0;
            logme ( kLogError, LINEINFOFORMAT "Error %i from read():  %s\n", LINEINFO, errno, strerror ( errno ) );
            return -1;
        }
        return size;
    }

    return 0;
}

/**
    @brief  receive data from uart with timeout, wait till all bytes received

    @param  uart:           uart number
    @param  pBuffer:        pointer to the buffer
    @param  bufferSize:     buffer size
    @param  timeout_ms:     timeout in mSec
    @return status:         0 is OK, < 0 failure
*/
int8_t uartReceiveBytes ( uint8_t uartNum, uint8_t* pBuffer, uint8_t bufferSize, uint8_t timeout_ms ) {

    uint64_t timeoutAt = getElapseTime() + timeout_ms * 1000;
    int bytesReceived = 0;

    while ((timeoutAt > getElapseTime()) && (bytesReceived < bufferSize)) {
        
        int ret = uartReceive (uartNum, &pBuffer[bytesReceived], bufferSize - bytesReceived, timeout_ms );
        if (ret) bytesReceived += ret;
    }
    
    return bytesReceived;
}

/**
    @brief  receive data from uart with timeout

    @param  uart:           uart number
    @param  pBuffer:        pointer to the buffer
    @param  bufferSize:     buffer size
    @param  timeout_ms:     timeout in mSec
    @return status:         0 is OK, < 0 failure
*/
int8_t uartTransmit ( uint8_t uartNum, uint8_t* pBuffer, uint8_t bytesToSend ) {
    if ( uartNum > uarts.size() - 1 ) {
        logme ( kLogError, LINEINFOFORMAT "Error invalid uart %i:  %s\n", LINEINFO, uartNum );
        return -1;
    }

    int & serial_port = uarts.at ( uartNum );

    int retval = write ( serial_port, pBuffer, bytesToSend );
    if ( retval < 0 ) {
        logme ( kLogError, LINEINFOFORMAT "Error %i from write():  %s\n", LINEINFO, errno, strerror ( errno ) );
        return -1;
    }

    return retval;
}
