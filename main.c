#include <Arduino.h>



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

bool hasBeenPressed[4] = {false, false, false, false}; // Initialize an array to track button presses on different pins


void loop() {
    if (PIND & (1 << PD7)) { // Om knapp 7 är nedtryckt
        PORTB |= (1 << PB1); // Sätt pin 9 till (HIGH)
    } else {
        PORTB &= ~(1 << PB1); // Sätt pin 9 till (LOW)
    }
    
    if (PIND & (1 << PD6)) { // Om knapp 6 är nedtryckt
        PORTB |= (1 << PB2); // Sätt pin 10 till (HIGH)
    } else {
        PORTB &= ~(1 << PB2); // Sätt pin 10 till (LOW)
    }
    
    if (PIND & (1 << PD5)) { // Om knapp 5 är nedtryckt
        PORTB |= (1 << PB3); // Sätt pin 11 till (HIGH)
    } else {
        PORTB &= ~(1 << PB3); // Sätt pin 11 till (LOW)
    }
    
    if (PIND & (1 << PD4)) { // Om knapp 4 är nedtryckt
        PORTB |= (1 << PB4); // Sätt pin 12 till (HIGH)
    } else {
        PORTB &= ~(1 << PB4); // Sätt pin 12 till (LOW)
    }
}