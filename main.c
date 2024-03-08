#include <Arduino.h>

int isButtonPressed() {
    if ((PIND & (1 << PD7) && PIND & (1 << PD6)) ||
        (PIND & (1 << PD7) && PIND & (1 << PD5)) ||
        (PIND & (1 << PD7) && PIND & (1 << PD4)) ||
        (PIND & (1 << PD6) && PIND & (1 << PD5)) ||
        (PIND & (1 << PD6) && PIND & (1 << PD4)) ||
        (PIND & (1 << PD5) && PIND & (1 << PD4))) {
        return 1;
    } else {
        return 0;
    }
}

int countPresses() {
    if (tick == 11) {
        return 1;
    } else {
        return 0;
    }
}

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

    Serial.begin(9600);
}

bool hasBeenPressed[4] = {false, false, false, false}; // Initialize an array to track button presses on different pins


void loop() {
     if ((PIND & (1 << PD7)) && !hasBeenPressed[0]) {
        PORTB |= (1 << PB1);
        hasBeenPressed[0] = true;
        tick++;
    } else if (!(PIND & (1 << PD7)) && hasBeenPressed[0]) {
        hasBeenPressed[0] = false;
     	PORTB&= ~(1 << PB1);
    }
  
    if ((PIND & (1 << PD6)) && !hasBeenPressed[1]) {
        PORTB |= (1 << PB2);
        hasBeenPressed[1] = true;
        tick++;
    } else if (!(PIND & (1 << PD6)) && hasBeenPressed[1]) {
        hasBeenPressed[1] = false;
      	PORTB &= ~(1 << PB2);
    }  
  
    if ((PIND & (1 << PD5)) && !hasBeenPressed[2]) {
        PORTB |= (1 << PB3);
        hasBeenPressed[2] = true;
        tick++;
    } else if (!(PIND & (1 << PD5)) && hasBeenPressed[2]) {
        hasBeenPressed[2] = false;
        PORTB &= ~(1 << PB3);
    }
  
    if ((PIND & (1 << PD4)) && !hasBeenPressed[3]) {
        PORTB |= (1 << PB4);
        hasBeenPressed[3] = true;
        tick++;
    } else if (!(PIND & (1 << PD4)) && hasBeenPressed[3]) {
        hasBeenPressed[3] = false;
        PORTB &= ~(1 << PB4);
    }
  
    Serial.print(tick);
    Serial.print("\n");

}