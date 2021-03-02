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


// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <unistd.h> // write(), read(), close()
#include <sys/time.h>
#include <sys/types.h>

#include "elapsedTime.h"


static uint64_t startTime = getElapseTime();
/**
    @brief  get elapsed time since start up

    @return elepsed time in uSec
*/
uint64_t getElapseTime() {
    
    struct timeval timeNow;
    
    gettimeofday(&timeNow, NULL);
    
    uint64_t ret = timeNow.tv_sec * 1000000 + timeNow.tv_usec  - startTime;
    return ret;
}

/**
    @brief  reset  elapsed time

    @return elepsed time in uSec
*/
void resetElapseTime() {
    
}
