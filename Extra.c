#include <avr/io.h> //endast för att kunna använda Serial.print för debugging
#include <stdbool.h>
#include <stdint.h>

volatile uint8_t *ddrb = (uint8_t *)0x24; // pekar på DDRB
volatile uint8_t *portb = (uint8_t *)0x25; // pekar på PORTB
volatile uint8_t *pinb = (uint8_t *)0x23; // pekar på PINB
volatile uint8_t *ddrd = (uint8_t *)0x2A; // pekar på DDRD
volatile uint8_t *portd = (uint8_t *)0x2B; // pekar på PORTD
volatile uint8_t *pind = (uint8_t *)0x29; // pekar på PIND

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

int countPresses() { //retunerar 1 om knapparna tryckts 10 gånger annars 0
    if (tick == 11) {
        return 1;
    } else {
        return 0;
    }
}

void turnOfAllRedLeds(){ //Funktion som släcker alla röda lampor, Detta för att den görna lede ska kunna overrida de röda
    *portb &= ~(1 << PB1); 
    *portb &= ~(1 << PB2);
    *portb &= ~(1 << PB3);
    *portb &= ~(1 << PB4);
}

int isButtonPressed() { //Retunerar 1 om 2 kanppar är nedtryckta samtidigt annars 0
    if ((*pind & (1 << PD7) && *pind & (1 << PD6)) ||
        (*pind & (1 << PD7) && *pind & (1 << PD5)) ||
        (*pind & (1 << PD7) && *pind & (1 << PD4)) ||
        (*pind & (1 << PD6) && *pind & (1 << PD5)) ||
        (*pind & (1 << PD6) && *pind & (1 << PD4)) ||
        (*pind & (1 << PD5) && *pind & (1 << PD4))) {
        return 1;
    } else {
        return 0;
    }
}
void setup() {
    *ddrb &= ~(1 << PB5); 
    *ddrb |= (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB0);  // Sätter pin 12, 11, 10, 9 och 8 som Digital output
    
    *ddrd &= ~(1 << PD7); // Sätter pin 7 som Digital input 
    *ddrd &= ~(1 << PD6); // Sätter pin 6 som Digital input
    *ddrd &= ~(1 << PD5); // Sätter pin 5 som Digital input
    *ddrd &= ~(1 << PD4); // Sätter pin 4 som Digital input  
}

//funktion som retunrerar 1 om någon av lamporna är tända
int anyLedsOn(){
    if(*pinb & (1 << PB1) || *pinb & (1 << PB2) || *pinb & (1 << PB3) || *pinb & (1 << PB4) || *pinb & (1 << PB0)){
        return 1;
    }
    return 0;
}

void greenFunc() { //Funktion som sätter på den gröna lampan enligt specifikationerna
        turnOfRed();  //stänger av alla röda lampor
        *portb |= (1 << PB0); //Sätter på den gröna lampan
        delay(3000); //Lys i 3 sekunder
        tick = 1;
        *portb &= ~(1 << PB0); //Stänger av den gröna lampan  
}


//funktion som tänder lampan samt sätter uppdaterar relevant variabler
void turnOnLamp(Led *led){
    *portb |= (1 << led->pin);
    led->isOn = true;
    led->blinkTimer = millis();
}

//funktion som kontrollerar om lampan skall tändas eller släckas
void controllLed(Led *led) {
    if(led->isOn){
      
      if(millis() - led->blinkTimer >= led->blink){ //om tiden som gått sedan lampan började blinka är större än blinktiden        
        *portb &= ~(1 << led->pin); //stänger av leden
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


    //Updelningen av if satserna sker för att vi skall kontrollera tick och knapptryckningar
    if(*pind & (1 << PD7)) { // Om knapp 7 är nedtryckt 
        tick++;
        
        controllLed(&_PB1); //kollar om lampan på pin 9 skall tändas eller släckas

    }else if(*pinb & (1 << PB1)){//om lampan är tänd
        controllLed(&_PB1);
    }

    if(*pind & (1 << PD6)) { // om knapp 6 är nedtryckt
        tick++;
        controllLed(&_PB2); //kollar om lampan på pin 10 skall tändas eller släckas

    }else if(*pinb & (1 << PB2)){//om lampan är tänd
        controllLed(&_PB2);
    }

    if(*pind & (1 << PD5)) { // Om knapp 5 är nedtryckt 
        tick++;
        controllLed(&_PB3); //kollar om lampan på pin 11 skall tändas eller släckas
    }else if(*pinb & (1 << PB3)){//om lampan är tänd
        controllLed(&_PB3);
    }

    if(*pind & (1 << PD4)) { // Om knapp 4 är nedtryckt 
        tick++;
        controllLed(&_PB4); //kollar om lampan på pin 12 skall tändas eller släckas
    }else if(*pinb & (1 << PB4)){//om lampan är tänd
        controllLed(&_PB4);
    }

    //TODO: fixa så det är 100% säkert att knapparna inte trycks för snabbt

    delay(100); //väntar 100 ms för att inte överbelasta knapptrycken

    if(isButtonPressed() || countPresses()) { //Om 2 knappar är nedtryckta samtidigt så 
        greenFunc();
    }

}