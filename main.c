#include <Arduino.h>

byte tick = 1; // Sätter tick till 1
byte pin = 1; // Sätter pin till 1

void setup() {
    DDRD &= ~(1 << PD7); // Sätter pin 7 som input
    DDRD &= ~(1 << PD6); // Sätter pin 6 som input
    DDRD &= ~(1 << PD5); // Sätter pin 5 som input
    DDRD &= ~(1 << PD4); // Sätter pin 4 som input
    
    DDRB &= ~(1 << PB5); // Sätter pin 13 som input
    
    DDRB |= (1 << PB4); // Sätter pin 12 som output
    DDRB |= (1 << PB3); // Sätter pin 11 som output
    DDRB |= (1 << PB2); // Sätter pin 10 som output
    DDRB |= (1 << PB1); // Sätter pin 9 som output
}

void loop() {
    
    PORTB |= (1 << pin); // Toggle pin x (HIGH)
    delay(100); //blink delay
    PORTB &= ~(1 << pin); // Toggle pin x (LOW)
    delay(tick*1000);

    pin++; // Ökar pin med 1
    tick++; // Ökar tick med 1

    if(tick > 9) // Om tick är större än 9
         tick = 1; // Sätt tick till 1
    if(pin > 4) // Om pin är större än 4
        pin = 1; // Sätt pin till 1

  
}