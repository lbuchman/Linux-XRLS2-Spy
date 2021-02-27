#include <string.h>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>


using namespace std;

int main(int argc, char **argv) {
    
  int baudrate = 115200;
  char deviceFile[32] = "/dev/ttyOSB0";

  int c;

  opterr = 0;


  while ((c = getopt (argc, argv, "pb:h")) != -1)
    switch (c)
      {
      case 'b':
        baudrate = std::stoi(optarg);
        break;
      case 'p':
        strcpy(deviceFile, optarg);
        break;
      case '?':
        return 1;
      default:
        abort ();
      }


return 0;   
    
}
