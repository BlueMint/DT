#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
 
/*
This sketch receives strings from sending unit via nrf24 
and prints them out via serial.  The sketch waits until
it receives a specific value (2 in this case), then it 
prints the complete message and clears the message buffer.
*/
int messageLength = 12;
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int lastmsg = 1;
String theMessage = "";
String testMessage = "";

const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;
const int ground1 = 4;
const int ground2 = 8;
const int whiteLed = 7;

String redBrightness = 0;
String greenBrightness = 0;
String blueBrightness = 0;

void setup(void){
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 
  pinMode(ground1, OUTPUT);
  pinMode(ground2, OUTPUT);
  pinMode(whiteLed, OUTPUT);
  digitalWrite(ground1, LOW);
  digitalWrite(ground2, LOW);
}

void loop(void){
  digitalWrite(whiteLed, LOW);
  if (radio.available()){
    digitalWrite(whiteLed, HIGH);
    bool done = false;  
      done = radio.read(msg, 1); 
      char theChar = msg[0];
      if (msg[0] != 2){
        theMessage.concat(theChar);
        Serial.println(theMessage);
        }
      }
      if (theMessage.length() == messageLength) { 
           redBrightness = "";
           greenBrightness = "";
           blueBrightness = "";
            Serial.println("Recieved!");
            for (int i = 0; i < 3; i++){
              redBrightness += theMessage[i];
            }
            for (int i = 4; i < 7; i++){
              greenBrightness += theMessage[i];
            }
            for (int i = 8; i < 11; i++){
              blueBrightness += theMessage[i];
            }
            analogWrite(redPin, redBrightness.toInt());
            analogWrite(greenPin, greenBrightness.toInt());
            analogWrite(bluePin, blueBrightness.toInt());
           theMessage= ""; 
       }
}


