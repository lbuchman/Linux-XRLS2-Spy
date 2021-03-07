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

#ifndef __SERIALL_H__
#define __SERIALL_H__

#include <cstdint>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
 
#ifndef ARDUINO

class Stream {
    public:
         Stream() {};
         int available() { return 0; };
         virtual int printf(const char *format, ...) = 0;
         virtual char read() = 0;
};

class _Serial: public Stream {
    public:
        _Serial() {};
        char read() override { return 0; };
        int printf(const char *format, ...) override {
            va_list args;
            va_start(args, format);
            int ret = vprintf(format, args);
            va_end(args);
            return ret;
        };
};

extern _Serial Serial;

#endif



#endif //__SERIALL_H__
