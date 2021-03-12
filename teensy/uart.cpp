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

#ifdef ARDUINO

// C library headers
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <uart.h>
#include <log.h>
#include <elapsedTime.h>

using namespace std;

static std::vector<int> uarts;

  
/**
    @brief  open uart device

    @param  device:         null terminated sring to the device file, platform specific
    @param  baudrate        115200 or 400000
    @return bool:           uart index or < 0 if error
*/
int8_t uartInit ( const char* device, const int baudRate ) {
    
    return 0;
}

/**
    @brief  close open uart device

    @param  uart:           uart number
    @return status:         0 is OK, < 0 failure
*/
void uartClose ( uint8_t uartNum ) {
  
}

/**
    @brief  set baudrate

    @param  uart:           uart number
    @param  baudRate:       baudrate
    @return void
*/
int8_t uartSetBaud ( uint8_t uartNum, uint32_t baudRate ) {
 
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
int8_t uartTransmit ( uint8_t uartNum, uint8_t* pBuffer, uint8_t bytesToSend ) {
   
    return 0;
}

/**
    @brief  get uart raw fd

    @param  uart:           uart number
    @return fd:         0 is OK, < 0 failure
*/
int uartGetFd(uint8_t uartNum) {
    
    return 0;
    
}

#endif
