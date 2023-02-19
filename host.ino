#include <SPI.h>                                          
#include <nRF24L01.h>                                     
#include <RF24.h>
#include <LiquidCrystal_I2C.h>

RF24 radio(9, 10); // пины радио
LiquidCrystal_I2C lcd(0x27, 20, 4);

int waterLVL[1];
int humAir[1];
int humAirTwo[1];
int temp[1];
int tempTwo[1];
int soilHum[1];
int soilHumTwo[1];

const int ultimateTemp = 25;
const int ultimateHum = 60;
const int ultimateSoil = 400;
const int ultimateWaterLVL = 300;

int one = 1;
int zero = 0;

void setup(){   
  lcd.begin();
  lcd.backlight();                                                            
  radio.begin(); // старт радио                                                                       
  radio.setDataRate(RF24_1MBPS); // 1мбит/сек                
  radio.setPALevel(RF24_PA_HIGH);                 
  radio.openReadingPipe(1, 0x1234567890LL); // идентификатор                                                    
}

void loop(){
  // дисплей вывод
  delay(500);
  lcd.clear();
  printInformation(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, "");

  // обработка информации
  
  // начнем с приема информации
  radio.startListening();

  // данные с 1 модуля
  radio.setChannel(1);
  if(radio.available()){                                
    radio.read(waterLVL, sizeof(waterLVL));  
    bool stateWaterLVL = true;                                              
  } else{
    bool stateWaterLVL = false;
  }
  	
  // меняем канал на датчик влажности со второго модуля
  radio.setChannel(2);
  if(radio.available()){                                
    radio.read(humAir, sizeof(humAir));
    bool stateHumAir = true;                                               
  } else{
    bool stateHumAir = false;
  }
  
  // датчик температуры со второго модуля
  radio.setChannel(3);
  if(radio.available()){                                
    radio.read(temp, sizeof(temp)); 
    bool stateTemp = true;                                                
  } else{
    bool stateTemp = false;
  }
  
  // датчик влажности почвы с третьего модуля
  radio.setChannel(4);
  if(radio.available()){
    radio.read(soilHum, sizeof(soilHum));
    bool stateSoilHum = true; 
  } else{
    bool stateSoilHum = false;
  }

  // датчик температуры с третьего модуля
  radio.setChannel(5);
  if(radio.available()){
    radio.read(tempTwo, sizeof(tempTwo));
    bool stateTempTwo = true; 
  } else{
    bool stateTempTwo = false;
  }

  // датчик влажности почвы с четвертого модуля
  radio.setChannel(6);
  if(radio.available()){
    radio.read(soilHumTwo, sizeof(soilHumTwo));
    bool stateSoilHumTwo = true; 
  } else{
    bool stateSoilHumTwo = false;
  }

  // датчик влажности воздуха с четвертого модуля
  radio.setChannel(7);
  if(radio.available()){
    radio.read(humAirTwo, sizeof(humAirTwo));
    bool stateHumAirTwo = true; 
  } else{
    bool stateHumAirTwo = false;
  }

  // остановка радио
  radio.stopListening();

  // передача комманд и информации

  // увлажнитель воздуха бул данные
  radio.setChannel(8);
  if(radio.available()){
    if(((humAir[1] + humAirTwo[1]) / 2) < ultimateHum){
      radio.write(one, sizeof(one));
    } else{
      radio.write(zero, sizeof(zero));
    }
    String humidifier = "OK";
  } else{
    String humidifier = "NONE";
  }

  // насос
  radio.setChannel(9);
  if(radio.available()){
    if(((soilHum[1] + soilHum[2]) / 2) < ultimateSoil){
      radio.write(one, sizeof(one));
    } else{
      radio.write(zero, sizeof(zero));
    }
    String pump = "OK";
  } else{
    String pump = "NONE";
  }

  // форточка
  radio.setChannel(10);
  if(radio.available()){
    if(((temp[1] + tempTwo[2]) / 2) < ultimateTemp){
      radio.write(one, sizeof(one));
    } else{
      radio.write(zero, sizeof(zero));
    }
    String window = "OK";
  } else{
    String window = "NONE";
  }

  // уровень воды
  radio.setChannel(11);
  if(radio.available()){
    if(waterLVL[1] < ultimateWaterLVL){
      radio.write(one, sizeof(one));
    } else{
      radio.write(zero, sizeof(zero));
    }
    String stateWaterLVL = "OK";
  } else{
    String stateWaterLVL = "NONE";
  }
}

void printInformation(float temp[1], float tempTwo[1], float humAir[1], float humAirTwo[1], int humSoil[1], int humSoilTwo[1], int waterLVL[1], int pump, int humidifier, int window, String stateWaterLVL){
  // вывод температуры
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(6, 0);
  lcd.print(temp[1]);
  lcd.setCursor(12, 0);
  lcd.print(tempTwo[1]);

  // вывод влажности воздуха
  lcd.setCursor(0, 1);
  lcd.print("Air Hum: ");
  lcd.setCursor(9, 1);
  lcd.print(humAir[1]);
  lcd.setCursor(15, 1);
  lcd.print(humAirTwo[1]);

  // вывод влажности почвы
  lcd.setCursor(0, 2);
  lcd.print("Soil Hum: ");
  lcd.setCursor(10, 2);
  lcd.print(soilHum[1]);
  lcd.setCursor(16, 2);
  lcd.print(soilHumTwo[1]);

  // вывод уровня воды
  lcd.setCursor(0, 3);
  lcd.print("Water Lvl: ");
  lcd.setCursor(11, 3);
  if(stateWaterLVL = "NONE"){
    lcd.print(stateWaterLVL);
  } else{
    lcd.print(waterLVL[1]);
  } 
  delay(3000);
  lcd.clear();

  // вывод средней температуры
  lcd.setCursor(0,0);
  lcd.print("Avrge Temp: ");
  lcd.setCursor(12, 0);
  lcd.print((temp[1] + tempTwo[1]) / 2);

  // вывод средней влажности почвы
  lcd.setCursor(0, 2);
  lcd.print("Avrge HumSoil: ");
  lcd.setCursor(15, 2);
  lcd.print((soilHum[1] + soilHum[1]) / 2);
  delay(3000);
  lcd.clear();

  // вывод состояния насоса
  lcd.setCursor(0, 0);
  lcd.print("Pump: ");
  lcd.print(pump);
  lcd.setCursor(0, 1);

  // вывод состояния увлажнителя
  lcd.print("Humidifier: ");
  lcd.print(humidifier);
  lcd.setCursor(0, 2);

  // вывод состояния актуатора
  lcd.print("Actuator: ");
  lcd.print(window);
  delay(3000);
  lcd.clear();
}
