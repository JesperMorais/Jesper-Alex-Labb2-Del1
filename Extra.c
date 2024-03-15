#include <Arduino.h>

#include <stdbool.h>
#include <stdint.h>
int tick = 1; //variable som håller koll på antalet knapptryckningar

//struct som innehåller all information om en led
typedef struct Led {
  uint8_t pin; //säger vilken pinne lampan är kopplad till
  unsigned int blink; //säger hur länge lampan lyser
  int offTimerArray[2]; //array av ints som säger hur länge lampan skall vara avstängs
  uint8_t currentIndex ; //säger vilket index i offTimerArray som vi är på just nu
  bool isOn; //säger om lampan är tänd eller släckt
  unsigned long ledOfftimer; //säger när lampan släcktes
  unsigned long blinkTimer; //säger när lampan började blinka
} Led;

void turnOfAllRedLeds(){ //Funktion som släcker alla röda lampor, Detta för att den görna lede ska kunna overrida de röda
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
void setup() {
    DDRB &= ~(1 << PB5); 
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB0);  // Sätter pin 12, 11, 10, 9 och 8 som Digital output
    
    DDRD &= ~(1 << PD7); // Sätter pin 7 som Digital input 
    DDRD &= ~(1 << PD6); // Sätter pin 6 som Digital input
    DDRD &= ~(1 << PD5); // Sätter pin 5 som Digital input
    DDRD &= ~(1 << PD4); // Sätter pin 4 som Digital input  
}

//funktion som retunrerar 1 om någon av lamporna är tända
int anyLedsOn(){
    if(PINB & (1 << PB1) || PINB & (1 << PB2) || PINB & (1 << PB3) || PINB & (1 << PB4)){
        return 1;
    }
    return 0;
}

void greenFunc() { //Funktion som sätter på den gröna lampan enligt specifikationerna
        turnOfRed();  //stänger av alla röda lampor
        PORTB |= (1 << PB0); //Sätter på den gröna lampan
        delay(3000); //Lys i 3 sekunder
        tick = 1;
        PORTB &= ~(1 << PB0); //Stänger av den gröna lampan  
}


//funktion som tänder lampan samt sätter uppdaterar relevant variabler
void turnOnLamp(Led *led){
    PORTB |= (1 << led->pin);
    led->isOn = true;
    led->blinkTimer = millis();
}

//funktion som kontrollerar om lampan skall tändas eller släckas
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
        
        if(led->currentIndex > 1){ //om indexet är större än 1, detta för att vi har 2 värden i offTimerArray
          led->currentIndex = 0; //sätt indexet till 0
      } 
    }
  }
}

void loop() {
    static Led _PB1 = {1,500,{1000, 2000}, 0, false, 0,0};
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

    if(isButtonPressed() || ) { //Om 2 knappar är nedtryckta samtidigt så 
        greenFunc();
    }

}