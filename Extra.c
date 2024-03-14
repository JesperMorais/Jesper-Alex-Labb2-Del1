#include <Arduino.h>

typedef struct Led {
  byte pin; //säger vilken pinne lampan är kopplad till
  unsigned int blink; //säger hur länge lampan lyser
  int offTimerArray[2]; //array av ints som säger hur länge lampan skall vara avstängs
  int currentIndex ; //säger vilket index i offTimerArray som vi är på just nu
  bool isOn; //säger om lampan är tänd eller släckt
  unsigned long ledOfftimer; //säger när lampan släcktes
  unsigned long blinkTimer; //säger när lampan började blinka
} Led;

void setup() {
    DDRB &= ~(1 << PB5); 
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4); 

    DDRD &= ~(1 << PD7); // Sätter pin 7 som input
    DDRD &= ~(1 << PD6); // Sätter pin 6 som input
    DDRD &= ~(1 << PD5); // Sätter pin 5 som input
    DDRD &= ~(1 << PD4); // Sätter pin 4 som input  
}


int anyLedsOn(){
  if(PINB & (1 << PB1) || PINB & (1 << PB2) || PINB & (1 << PB3) || PINB & (1 << PB4)){
    return 1;
  }
  return 0;
}

void turnOnLamp(Led *led){ //tänder lampan samt sätter uppdaterar previousMillis och isOn variablerna
  PORTB |= (1 << led->pin);
  led->isOn = true;
  led->blinkTimer = millis();
}

void controllLed(Led *led) {
    if(led->isOn){
      if(millis() - led->blinkTimer >= led->blink){ //om tiden som gått sedan lampan började blinka är större än blinktiden
        PORTB &= ~(1 << led->pin); //stänger av leden
        led->isOn = false; //sätter isOn till false
        led->ledOfftimer = millis(); //sätter ledOfftimer till millis
      }
  } 
    if(!led->isOn){ //lampan är inte på
      if(millis() - led->ledOfftimer >= led->offTimerArray[led->currentIndex] && !anyLedsOn()){ //lampan har varit avstängd i tillräckligt lång tid och ingen annan lampa är tänd
        turnOnLamp(led); //tänder lampan
        led->currentIndex++; //ökar indexet i offTimerArray
        if(led->currentIndex > 1){ //om indexet är större än 1
          led->currentIndex = 0; //sätt indexet till 0
      } 
    }
  }
}

void loop() {
  static Led _PB1 = {1, 500,{1000, 2000}, 0, false, 0,0};
  static Led _PB2 = {2, 500,{1500, 3000}, 0, false,0,0 };
  static Led _PB3 = {3, 500,{6000, 2000}, 0, false,0,0};
  static Led _PB4 = {4, 500,{2000, 10000}, 0, false,0,0};

    if(PIND & (1 << PD7) || PORTB |= (1 << PB1)) { // Om knapp 7 är nedtryckt eller lampan är tänd
        controllLed(&_PB1); 
    }
    if(PIND & (1 << PD6) || PORTB |= (1 << PB2)) { // om knapp 6 är nedtryckt eller lampan är tänd
        controllLed(&_PB2);
    }
    if(PIND & (1 << PD5) || PORTB |= (1 << PB3)) { // Om knapp 5 är nedtryckt eller lampan är tänd 
        controllLed(&_PB3);
    }
    if(PIND & (1 << PD4) || PORTB |= (1 << PB4)) { // Om knapp 4 är nedtryckt eller lampan är tänd
        controllLed(&_PB4);
    }
}