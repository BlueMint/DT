#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int fireButton = 6;
int fireGround = 7;
int doorButton = 3;
int doorGround = 4;

void setup(void){
Serial.begin(9600);
radio.begin();
pinMode(fireButton, INPUT);
digitalWrite(fireButton, HIGH);
pinMode(fireGround, OUTPUT);
digitalWrite(fireGround, LOW);
pinMode(doorButton, INPUT);
digitalWrite(doorButton, HIGH);
pinMode(doorGround, OUTPUT);
digitalWrite(doorGround, LOW);
radio.openWritingPipe(pipe);
}

int incomingByte = 0; 
String content = "";
char character;


void loop(void){

    while(Serial.available()) {
        character = Serial.read();
        content.concat(character);
    }
    if (digitalRead(fireButton) == HIGH){
        content = "fire*.*000000000";
        Serial.println("fire");
    }
    if (digitalRead(doorButton) == LOW){
        content = "door*.*000000000";
        Serial.println("door");
    }
    if (content != "") {
        int messageSize = content.length();
        for (int i = 0; i < messageSize; i++) {
            int charToSend[1];
            charToSend[0] = content.charAt(i);
            radio.write(charToSend,1);
        }  
        msg[0] = 2; 
        radio.write(msg,1);
        radio.powerDown(); 
        delay(1000);
        radio.powerUp();
        content = "";
    }
}