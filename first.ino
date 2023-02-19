#include <SPI.h>                                          
#include <nRF24L01.h>                                     
#include <RF24.h>

int data = 0;

RF24 radio(9, 10); // пины радио

void setup(){                                                           
  radio.begin(); // старт радио                                                                       
  radio.setDataRate(RF24_1MBPS); // 1мбит/сек                
  radio.setPALevel(RF24_PA_HIGH);                 
  radio.openReadingPipe(1, 0x1234567890LL); // идентификатор  
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop() {
  delay(5000);
  int waterLVL = analogRead(A0);

  radio.stopListening();

  radio.setChannel(1);
  if(radio.available()){
    radio.write(waterLVL, sizeof(waterLVL));
  }

  radio.startListening();
  radio.setChannel(9);
  if(radio.available()){
    radio.read(data, sizeof(data));
    if(data = 1){
      digitalWrite(3, HIGH);
      delay(10000);
      digitalWrite(3, LOW);
    } else{
      digitalWrite(3, LOW);
    }
  }

  radio.startListening();
  radio.setChannel(8);
  if(radio.available()){
    radio.read(data, sizeof(data));
    if(data = 1){
      digitalWrite(3, HIGH);
      delay(20000);
      digitalWrite(3, LOW);
    } else{
      digitalWrite(3, LOW);
    }
  }
}