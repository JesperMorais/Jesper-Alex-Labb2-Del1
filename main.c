#include <avr/io.h>

volatile uint8_t *ddrb = (uint8_t *)0x24; // DDRB register address
volatile uint8_t *portb = (uint8_t *)0x25; // PORTB register address


byte tick = 1; // Sätter tick till 1
byte pin = 1; // Sätter pin till 1

void setup() {
    *ddrb &= ~(1 << PB5); // Sätter pin 13 som input
    *ddrb |= (1 << PB4); // Sätter pin 12 som output
    *ddrb |= (1 << PB3); // Sätter pin 11 som output
    *ddrb |= (1 << PB2); // Sätter pin 10 som output
    *ddrb |= (1 << PB1); // Sätter pin 9 som output
}

void loop() {
    
    *portb |= (1 << pin); // Toggle pin x (HIGH)
    delay(100); //blink delay
    *portb &= ~(1 << pin); // Toggle pin x (LOW)
    delay(tick*1000);

    pin++; // Ökar pin med 1
    tick++; // Ökar tick med 1

    if(tick > 9) // Om tick är större än 9
         tick = 1; // Sätt tick till 1
    if(pin > 4) // Om pin är större än 4
        pin = 1; // Sätt pin till 1

  
}