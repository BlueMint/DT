#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>


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
String myRoomAddress = "1";
String myLightAddress = "1";
String sentRoomAddress = "";
String sentLightAddress = "";
String correctAddress = "false";
String fireState = "off";
String fire = "false";
String fireCycleOn = "false";
String doorState = "false";
long previousMillis = 0;
long interval = 250;
long doorPressedAt = 0;
int currentRedBrightness = 0;
int currentGreenBrightness = 0;
int currentBlueBrightness = 0;


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
    unsigned long currentMillis = millis();
    if (radio.available()){
        bool done = false;  
        done = radio.read(msg, 1); 
        char theChar = msg[0];
        if (msg[0] != 2){
            theMessage.concat(theChar);
        }
        else{
            Serial.println("==");
            Serial.println("  ");
            Serial.println("==");
            Serial.print("Message lengthh: ");
            Serial.println(theMessage.length());
            Serial.print("The message so far: '");
            Serial.print(theMessage);
            Serial.println("'");
            sentRoomAddress = "";
            sentLightAddress = "";
            correctAddress = "false";
            String sentMode = "";
            String sentData = "";
            String redBrightness = 0;
            String greenBrightness = 0;
            String blueBrightness = 0;
            sentMode.concat(theMessage[0]);
            sentMode.concat(theMessage[1]);
            sentMode.concat(theMessage[2]);
            sentMode.concat(theMessage[3]);
            sentRoomAddress.concat(theMessage[4]); 
            sentLightAddress.concat(theMessage[6]);
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
            Serial.print("Sent Room: ");
            Serial.print(sentRoomAddress);
            Serial.print(", Sent Light: ");
            Serial.print(sentLightAddress);
            Serial.print(" || My room: ");
            Serial.print(myRoomAddress);
            Serial.print(", My light: ");
            Serial.println(myLightAddress);
            Serial.print("Sent Mode: ");
            Serial.println(sentMode);
            Serial.print("Sent Data: ");
            Serial.println(sentData);
            if (sentRoomAddress == myRoomAddress or sentRoomAddress == "*"){
                if (sentLightAddress == myLightAddress or sentLightAddress == "*"){
                    correctAddress = "true";
                }
            }
            if (correctAddress == "true") { 
                Serial.print("That's my address!");
                if (sentMode == "send"){
                    redBrightness.concat(sentData[0]);
                    redBrightness.concat(sentData[1]);
                    redBrightness.concat(sentData[2]);
                    greenBrightness.concat(sentData[3]);
                    greenBrightness.concat(sentData[4]);
                    greenBrightness.concat(sentData[5]);
                    blueBrightness.concat(sentData[6]);
                    blueBrightness.concat(sentData[7]);
                    blueBrightness.concat(sentData[8]);
                    Serial.print("Red: ");
                    Serial.print(redBrightness.toInt());
                    Serial.print(" || Green: ");
                    Serial.print(greenBrightness.toInt());
                    Serial.print(" || Blue: ");
                    Serial.println(blueBrightness.toInt());
                    currentRedBrightness = redBrightness.toInt();
                    currentGreenBrightness = greenBrightness.toInt();
                    currentBlueBrightness = blueBrightness.toInt();
                    analogWrite(redPin, redBrightness.toInt());
                    analogWrite(greenPin, greenBrightness.toInt());
                    analogWrite(bluePin, blueBrightness.toInt());
                }
                if (sentMode == "fire"){
                    Serial.println("FIRE");
                    Serial.println(fireState);
                    if (fireState == "off"){
                        Serial.println("fire");
                        fireState = "on";
                        fire = "true";
                        analogWrite(redPin, 0);
                        analogWrite(greenPin, 0);
                        analogWrite(bluePin, 0);
                    }
                    else {
                        fire = "false";
                        fireState = "off";
                        analogWrite(redPin, currentRedBrightness);
                        analogWrite(greenPin, currentGreenBrightness);
                        analogWrite(bluePin, currentBlueBrightness);
                    }
                }
                if (sentMode == "door"){
                    doorPressedAt = currentMillis;
                    doorState = "true";
                }
            }
        }
    }
    if (fire == "true"){
        
        if(currentMillis - previousMillis > interval) {
            previousMillis = currentMillis;
            if (fireCycleOn == "true"){
                analogWrite(redPin, 255);
                fireCycleOn = "false";
            }
            else{
                analogWrite(redPin, 30);
                fireCycleOn = "true";
            }    
        }        
    }
    if (doorState == "true"){
        analogWrite(redPin, 0);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, 0);
        for (int i = 0; i++, i < 50;){
            analogWrite(greenPin, (i * 5));
            delay(20); 
        }
        for (int i = 50; i--, i > 0;){
            analogWrite(greenPin, (i * 5));
            delay(20); 
        }
        for (int i = 0; i++, i < 50;){
            analogWrite(greenPin, (i * 5));
            delay(20); 
        }
        for (int i = 50; i--, i > 0;){
            analogWrite(greenPin, (i * 5));
            delay(20); 
        }
        doorState = "false";
        analogWrite(redPin, currentRedBrightness);
        analogWrite(greenPin, currentGreenBrightness);
        analogWrite(bluePin, currentBlueBrightness);
    }
}