#include <SPI.h>
 
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
 
#include <DigitalIO.h>
#include <DigitalPin.h>
#include <I2cConstants.h>
#include <nRF24L01.h>
#include <PinIO.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SoftI2cMaster.h>
#include <SoftSPI.h>
 
/*
This sketch sends a string to a corresponding Arduino
with nrf24 attached.  It appends a specific value 
(2 in this case) to the end to signify the end of the
message.
*/
 
int msg[1];
RF24 radio(6,7);
const uint64_t pipe = 0xE8E8F0F0E1LL;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;

//Ethernet setup...
byte mac[] = { 
  0xDE, 0xAD, 0x00, 0xCC, 0xFE, 0xAA };
EthernetServer server(10020);


void setup(void){
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
  Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}
void loop(void){
  inputString = "";
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  int messageSize = inputString.length();
  for (int i = 0; i < messageSize; i++) {
    int charToSend[1];
    charToSend[0] = inputString.charAt(i);
    Serial.println(inputString.charAt(i));
    radio.write(charToSend,1);
  }  
//send the 'terminate string' value...  
  msg[0] = 2; 
  radio.write(msg,1);
/*delay sending for a short period of time.  radio.powerDown()/radio.powerupp
//with a delay in between have worked well for this purpose(just using delay seems to
//interrupt the transmission start). However, this method could still be improved
as I still get the first character 'cut-off' sometimes. I have a 'checksum' function
on the receiver to verify the message was successfully sent.
*/
  radio.powerDown(); 
  delay(1000);
  radio.powerUp();
  
}
