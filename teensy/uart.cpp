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

//#ifdef ARDUINO

// C library headers
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <uart.h>
#include <log.h>
#include <elapsedTime.h>

using namespace std;

static std::vector<HardwareSerial*> uarts;

  
/**
    @brief  open uart device

    @param  device:         null terminated sring to the device file, platform specific
    @param  baudrate        115200 or 400000
    @return bool:           uart index or < 0 if error
*/
int8_t uartInit ( const char* device, const int baudRate ) {
    HardwareSerial* hwSerial;
    if (!strcmp(device, "Serial1")) {
         hwSerial = &Serial1;
         logme ( kLogInfo, LINEINFOFORMAT "Initializing serial port -> %s, baudRate = %d", LINEINFO, device, baudRate);
    }
    else if (!strcmp(device, "Serial2")) {
      
         hwSerial = &Serial2;
         logme ( kLogInfo, LINEINFOFORMAT "Initializing serial port -> %s, baudRate = %d", LINEINFO, device, baudRate);
    }
    else if (!strcmp(device, "Serial3")) {
      
         hwSerial = &Serial3;
         logme ( kLogInfo, LINEINFOFORMAT "Initializing serial port -> %s, baudRate = %d", LINEINFO, device, baudRate);
    }
    else {
        logme ( kLogError, LINEINFOFORMAT "%s is not valid teensy port", LINEINFO, device);
        while (1);
    }
    uarts.push_back ( hwSerial );
    uint8_t uartNum = uarts.size() - 1;
    
    hwSerial->begin(baudRate, SERIAL_HALF_DUPLEX);
    return uartNum;
}
/**
    @brief  close open uart device

    @param  uart:           uart number
    @return status:         0 is OK, < 0 failure
*/
void uartClose ( uint8_t uartNum ) {
    logme ( kLogError, LINEINFOFORMAT "Not implemented function uartClose()", LINEINFO);
}

/**
    @brief  set baudrate

    @param  uart:           uart number
    @param  baudRate:       baudrate
    @return void
*/
int8_t uartSetBaud ( uint8_t uartNum, uint32_t baudRate ) {
    logme ( kLogError, LINEINFOFORMAT "Not implemented function uartSetBaud()", LINEINFO);
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
    
    if ( uartNum > uarts.size() - 1 ) {
        logme ( kLogError, LINEINFOFORMAT "Error invalid uart %i:  %s\n", LINEINFO, uartNum );
        return -1;
    }
    
    uint8_t inBytes = 0;
    
    HardwareSerial* serial_port = uarts.at ( uartNum );
    if (!serial_port->available()) {
        return 0;
    }

    while (serial_port->available() && inBytes < bufferSize) {
        pBuffer[inBytes] = serial_port->read();
        inBytes += 1;
    }
    return inBytes;
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
    unsigned long timeoutAt = micros() + timeout_ms * 1000;
    int bytesReceived = 0;

    while ((timeoutAt > micros()) && (bytesReceived < bufferSize)) {
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

    HardwareSerial* serial_port = uarts.at ( uartNum );
    return serial_port->write(pBuffer, bytesToSend);
}

/**
    @brief  get uart raw fd

    @param  uart:           uart number
    @return fd:         0 is OK, < 0 failure
*/
int uartGetFd(uint8_t uartNum) {
    logme ( kLogError, LINEINFOFORMAT "Not implemented function uartGetFd()", LINEINFO);
    return 0;
}

//#endif
