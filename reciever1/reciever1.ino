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
int messageLength = 16;
int msg[1];
RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
int lastmsg = 1;
String theMessage = "";
String sentMode = "";
String sentData = "";
const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;
const int ground = 4;
String redBrightness = 0;
String greenBrightness = 0;
String blueBrightness = 0;
String myAddress = "1.1";


void setup(void){
    Serial.begin(9600);
    pinMode(redPin, OUTPUT); 
    pinMode(greenPin, OUTPUT); 
    pinMode(bluePin, OUTPUT); 
    pinMode(ground, OUTPUT);
    digitalWrite(ground, LOW);
    radio.begin();
    radio.openReadingPipe(1,pipe);
    radio.startListening();
}
void loop(void){
    if (radio.available()){
        bool done = false;  
        done = radio.read(msg, 1); 
        char theChar = msg[0];
        if (msg[0] != 2){
            theMessage.concat(theChar);
        }
        else {
            String sentAddress = "";
            String sentMode = "";
            String sentData = "";
            String redBrightness = 0;
            String greenBrightness = 0;
            String blueBrightness = 0;
            Serial.println(theMessage.length());
            if (theMessage.length() >= messageLength){
                Serial.print("Message: ");
                Serial.println(theMessage);
                sentAddress.concat(theMessage[0]); 
                sentAddress.concat(theMessage[1]); 
                sentAddress.concat(theMessage[2]);
                sentMode.concat(theMessage[3]);
                sentMode.concat(theMessage[4]);
                sentMode.concat(theMessage[5]);
                sentMode.concat(theMessage[6]);
                sentData.concat(theMessage[7]);
                sentData.concat(theMessage[8]);
                sentData.concat(theMessage[9]);
                sentData.concat(theMessage[10]);
                sentData.concat(theMessage[11]);
                sentData.concat(theMessage[12]);
                sentData.concat(theMessage[13]);
                sentData.concat(theMessage[14]);
                sentData.concat(theMessage[15]);
                theMessage= "";
                if (sentMode == "send"){
                    redBrightness.concat(sentData[0]);
                    redBrightness.concat(sentData[1]);
                    redBrightness.concat(sentData[2]);
                    Serial.print("Red: ");
                    Serial.println(redBrightness.toInt());
                    analogWrite(redPin, redBrightness.toInt());
                }
                Serial.print("Sent Address: ");
                Serial.println(sentAddress);
                Serial.print("My real address: ");
                Serial.println(myAddress);
                Serial.print("Sent Mode: ");
                Serial.println(sentMode);
                Serial.print("Sent Data: ");
                Serial.println(sentData);
                if (sentAddress == myAddress) { 
                    Serial.print("Message Fully Recieved!");
                    Serial.println(theMessage);
                    Serial.println(theMessage[0,1,2]);
                }
            } 
        }
    }
}