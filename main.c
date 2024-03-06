#include <Arduino.h>


void setup() {
    DDRB &= ~(1 << PB5); // Sätter pin 13 som input

    DDRB |= (1 << PB4); // Sätter pin 12 som output
}

void loop() {
  
  
}