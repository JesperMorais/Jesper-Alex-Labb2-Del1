#include <Arduino.h>

int tick = 1; //variable som håller koll på antalet knapptryckningar

void turnOfRed(){ //Funktion som släcker alla röda lampor / sätter dem till 0
  PORTB &= ~(1 << PB1);
  PORTB &= ~(1 << PB2);
  PORTB &= ~(1 << PB3);
  PORTB &= ~(1 << PB4);
}

int isButtonPressed() { //Retunerar 1 om 2 kanppar är nedtryckta samtidigt annars 0
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

int countPresses() { //retunerar 1 om knapparna tryckts 10 gånger annars 0
    if (tick == 11) {
        return 1;
    } else {
        return 0;
    }
}

void greenFunc(int buttonPresses, int buttonPressed) { //Funktion som sätter på den gröna lampan enligt specifikationerna
    if (buttonPresses) {  
        PORTB |= (1 << PB0);
      turnOfRed();  
      delay(3000);
        tick = 1;
        PORTB &= ~(1 << PB0); 
    } else if (buttonPressed) {
        PORTB |= (1 << PB0);
    } else {
        PORTB &= ~(1 << PB0); 
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

bool hasBeenPressed[4] = {false, false, false, false}; //array med 4 bools som håller koll på om knapparna har tryckts


void loop() {
    //if sats som kollar om knapparna är nedtryckta och sätter på respektive lampa
    //och ökar tick med 1 för varje knapptryckning
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
    
    //printar tick för debugging
    Serial.print(tick);
    Serial.print("\n");
    
    //kallar på greenFunc för att eventuellt sätta på den gröna lampan om det är dags
    greenFunc(countPresses(), isButtonPressed()); /*countpresses är 1 om knapparna tryckts 10 gånger annars 0
                                                    isButtonPressed är 1 om 2 knappar är nedtryckta samtidigt annars 0
                                                */
}