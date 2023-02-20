#include <SPI.h>                                          
#include <nRF24L01.h>                                     
#include <RF24.h>
#include <DHT.h>
#define DHTPIN 3

DHT dht(DHTPIN, DHT11);
RF24 radio(9, 10); // пины радио

void setup(){                                                           
  radio.begin(); // старт радио                                                                       
  radio.setDataRate(RF24_1MBPS); // 1мбит/сек                
  radio.setPALevel(RF24_PA_HIGH);                 
  radio.openReadingPipe(1, 0x1234567890LL); // идентификатор  
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(5000);
  int analogSoilHum = analogRead(A0);
  int analogTemp = dht.readTemperature();

  radio.stopListening();

  radio.setChannel(4);
  if(radio.available()){
    radio.write(analogSoilHum, sizeof(analogSoilHum));
  }

  radio.setChannel(5);
  if(radio.available()){
    radio.write(analogTemp, sizeof(analogTemp));
  }
}
