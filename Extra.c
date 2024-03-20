#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

volatile uint8_t *ddrb = (uint8_t *)0x24; // pekar på DDRB som sätter vilka pinnar som är input och output
volatile uint8_t *portb = (uint8_t *)0x25; // pekar på PORTB som sätter vilka pinnar som är höga och låga
volatile uint8_t *pinb = (uint8_t *)0x23; // pekar på PINB som läser av vilka pinnar som är höga och låga
volatile uint8_t *ddrd = (uint8_t *)0x2A; // pekar på DDRD som sätter vilka pinnar som är input och output
volatile uint8_t *pind = (uint8_t *)0x29; // pekar på PIND som läser av vilka pinnar som är höga och låga
volatile uint8_t *portb = (uint8_t *)0x2B; // pekar på PORTD som sätter vilka pinnar som är höga och låga

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

void setup() {
    *ddrb &= ~(1 << PB5); 
    *ddrb |= (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4);  // Sätter pin 1, 2, 3 och 4 som Digital output

    *ddrd &= ~(1 << PD7); // Sätter pin 7 som Digital input 
    *ddrd &= ~(1 << PD6); // Sätter pin 6 som Digital input
    *ddrd &= ~(1 << PD5); // Sätter pin 5 som Digital input
    *ddrd &= ~(1 << PD4); // Sätter pin 4 som Digital input  
}

//funktion som retunrerar 1 om någon av lamporna är tända
int anyLedsOn() {
    if(*pinb & (1 << PB1) || *pinb & (1 << PB2) || *pinb & (1 << PB3) || *pinb & (1 << PB4)){
        return 1;
    }
    return 0;
}

//funktion som tänder lampan samt sätter uppdaterar relevant variabler
void turnOnLamp(Led *led) {
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
    static Led _PB1 = {1,500,{1000, 2000}, 0, false, 0,0}; // skapar en led struct som är kopplad till pin 1
    static Led _PB2 = {2, 500,{1500, 3000}, 0, false,0,0 }; // skapar en led struct som är kopplad till pin 2
    static Led _PB3 = {3, 500,{6000, 2000}, 0, false,0,0}; // skapar en led struct som är kopplad till pin 3
    static Led _PB4 = {4, 500,{2000, 10000}, 0, false,0,0}; // skapar en led struct som är kopplad till pin 4

    if(*pind & (1 << PD7) || *pinb |= (1 << PB1)) { // Om knapp 7 är nedtryckt eller lampan är tänd
        controllLed(&_PB1); 
    }
    if(*pind & (1 << PD6) || *pinb |= (1 << PB2)) { // om knapp 6 är nedtryckt eller lampan är tänd
        controllLed(&_PB2);
    }
    if(*pind & (1 << PD5) || *pinb |= (1 << PB3)) { // Om knapp 5 är nedtryckt eller lampan är tänd 
        controllLed(&_PB3);
    }
    if(*pind & (1 << PD4) || *pinb |= (1 << PB4)) { // Om knapp 4 är nedtryckt eller lampan är tänd
        controllLed(&_PB4);
    }

}