#include <string.h>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <uart.h>

#include <loggingLib/log.h>

using namespace std;

int main ( int argc, char **argv ) {
    int baudrate = 115200;
    char deviceFile[32] = "/dev/ttyUSB0";
    printme(NEWLINE, TIMESTAMP, "SRXL2 Spy Linux Rev 0.1");
    
    int c;

    opterr = 0;

    while ( ( c = getopt ( argc, argv, "pb:h" ) ) != -1 ) {
        switch ( c ) {
        case 'b':
            baudrate = std::stoi ( optarg );
            break;
        case 'p':
            strcpy ( deviceFile, optarg );
            break;
        case 'h':
            fprintf ( stderr, "usage: %s -b baudrate -p portDevice\n", argv[0] );
            break;
        case '?':
            fprintf ( stderr, "invalid command line option %c\n", c );
        default:
            abort ();
        }
    }

    fprintf ( stderr, "starting serial device %s at baudrate %d\n", deviceFile, baudrate );

   // if(!srxlInitBus(0, uartHandle, SRXL_SUPPORTED_BAUD_RATES))

    int8_t uart = uartInit ( deviceFile, baudrate );
    if (uart < 0) return -1;
    
    while (1) {
        uint8_t buffer[32];
        int ret = uartReceiveBytes(uart, buffer, sizeof(buffer), 100);
        if (ret) {
            hexdump(buffer,ret, true, kLogError);
        }
    }
    
    uartClose ( uart );
    printme(NEWLINE, NO_TIMESTAMP, "uart %s is closed", deviceFile);
    return 0;

}
