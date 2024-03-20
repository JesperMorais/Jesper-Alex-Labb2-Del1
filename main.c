#include <avr/io.h>

volatile uint8_t *ddrb = (uint8_t *)0x24; // DDRB register address
volatile uint8_t *portb = (uint8_t *)0x25; // PORTB register address

byte tick = 1;

void setup() {
    *ddrb &= ~(1 << PB5); // Sätter pin 13 som input

    *ddrb |= (1 << PB4); // Sätter pin 12 som output
}

void loop() {
    
    *portb |= (1 << PB4); // Toggle pin 12 (HIGH)
    delay(100); //blink delay
    *portb &= ~(1 << PB4); // Toggle pin 12 (LOW)
    delay(tick*1000);

    tick++;
    if(tick == 10)
         tick = 1;

  
}