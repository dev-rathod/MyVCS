Hello world!
#include "Arduino.h"
#include <TimerOne.h>
#include <AltSoftSerial.h>

const byte enableTrans = 1;
const byte resetCount = 3;
const byte enableMux = 5;
const byte zMux = 2;
const byte bitsMux[] = {4, 6, 8, 10};

int z_val;
int bits_val[4];


bool turnOnCount = true;
bool turnOnTranslator = false;

void interuptHandler(){
    // Turning the enable pin of Translator -> LOW
    digitalWrite(enableTrans, LOW);
    // Turning the Enable pin of the multiplexer -> LOW 
    digitalWrite(enableMux, LOW);
    // Delay required to turn on the mux

    readVals();
    
    // Small delay to execute the read operation
    delay(1);

    digitalWrite(enableMux, HIGH);
    // Delay of the time taken to reset the values
    
    turnOnCount = true;
}

void readVals(){
    z_val = digitalRead(zMux);
    for (int i = 0; i < 4; i++) {
        bits_val[i] = digitalRead(bitsMux[i]);
    }
}

void delay37ns() {
  for (int i = 0; i < 22; i++) {
    __asm__ __volatile__("nop\n\t");
  }
}

void setup()
{
    Serial.begin(9600);
    pinMode(enableTrans, OUTPUT);
    pinMode(resetCount, OUTPUT);
    pinMode(enableMux, OUTPUT);
    pinMode(zMux, INPUT);
    for (int i = 0; i < 4; i++) {
        pinMode(bitsMux[i], INPUT);
    }

    // Creating a timer for handling interupts
    Timer1.initialize(timeDelay);
    Timer1.attachInterrupt(interuptHandler);
}

void loop(){
    if(turnOnCount){
        digitalWrite(resetCount, HIGH);
        delay37ns();
        digitalWrite(enableTrans, HIGH);
        digitalWrite(resetCount, LOW);
        // Enter delay for setting trans
        turnOnCount = false;
    }
}