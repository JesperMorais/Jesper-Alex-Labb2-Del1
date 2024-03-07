#include <Arduino.h>

byte tick = 1;
byte pin = 1;

void setup() {
    DDRB &= ~(1 << PB5); // Sätter pin 13 som input
    DDRB |= (1 << PB4); // Sätter pin 12 som output
    DDRB |= (1 << PB3); // Sätter pin 11 som output
    DDRB |= (1 << PB2); // Sätter pin 10 som output
    DDRB |= (1 << PB1); // Sätter pin 9 som output
}

void loop() {
    
    PORTB |= (1 << pin); // Toggle pin 12 (HIGH)
    delay(100); //blink delay
    PORTB &= ~(1 << pin); // Toggle pin 12 (LOW)
    delay(tick*1000);

    pin++;
    tick++;

    if(tick > 9)
         tick = 1;
    if(pin > 4)
        pin = 1;

  
}