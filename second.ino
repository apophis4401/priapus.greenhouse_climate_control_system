#include <SPI.h>                                          
#include <nRF24L01.h>                                     
#include <RF24.h>
#include <DHT.h>
#define DHTPIN 3

int data = 0;

DHT dht(DHTPIN, DHT11);
RF24 radio(9, 10); // пины радио

void setup(){                                                           
  radio.begin(); // старт радио                                                                       
  radio.setDataRate(RF24_1MBPS); // 1мбит/сек                
  radio.setPALevel(RF24_PA_HIGH);                 
  radio.openReadingPipe(1, 0x1234567890LL); // идентификатор  
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  dht.begin();
}

void loop() {
  delay(5000);
  int analogTemp = dht.readTemperature();
  int analogHumAir = dht.readHumidity();

  radio.stopListening();

  radio.setChannel(2);
  if(radio.available()){
    radio.write(analogHumAir, sizeof(analogHumAir));
  }

  radio.setChannel(3);
  if(radio.available()){
    radio.write(analogTemp, sizeof(analogTemp));
  }

  radio.startListening();
  radio.setChannel(10);
  if(radio.available()){
    radio.read(data, sizeof(data));
    if(data = 1){
      digitalWrite(3, HIGH);
    } else{
      digitalWrite(3, LOW);
    }
  }
}
