#ifndef _GLIBCXX_IOSTREAM

#include <Arduino.h>

namespace std {
void __throw_bad_alloc() {
    Serial.println("Unable to allocate memory");

    while(1);
}

void __throw_length_error(char const*e) {
    Serial.print("Length Error :");
    Serial.println(e);

    while(1);
}

void __throw_bad_function_call() {
    Serial.println("Bad function call!");

    while(1);
}

void __throw_out_of_range_fmt(const char*, ...) {
    Serial.println("Bad function call!");

    while(1);
}
}
#endif
