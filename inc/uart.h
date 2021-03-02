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

#ifndef __UART_H__
#define __UART_H__

#include <cstdint>

// #ifdef __cplusplus
// extern "C"
// {
// #endif


int8_t uartInit(const char* device, const int baudRate); 
void uartClose ( uint8_t uartNum);
int8_t uartSetBaud(uint8_t uartNum, uint32_t baudRate);
int8_t uartReceiveBytes(uint8_t uartNum, uint8_t* pBuffer, uint8_t bufferSize, uint8_t timeout_ms);
int8_t uartReceive ( uint8_t uartNum, uint8_t* pBuffer, uint8_t bufferSize, uint8_t timeout_ms );
int8_t uartTransmit(uint8_t uartNum, uint8_t* pBuffer, uint8_t bytesToSend); 

// #ifdef __cplusplus
// } // extern "C"
// #endif

 

#endif //__SRXL_H__
