//Temperature and humidity sensor
#include "DHT.h"
#define DHTPIN 5    //GPIO5 D1
#define DHTTYPE DHT11

//Water level sensor
#define ReadWatLev 4     //GPIO4 D2  Turns on and off the water level sensor
int WaterLevel = 0;
int waterPercentage = 0;

#define AnalogInput A0          //ESP only has one analog input gonna be used for moisture and water level

//Moisture sensor
#define ReadMoist 12     //D6  GPIO12  Turns on and off the moisture sensor
int MoistLevel = 0;
int MoistPercentage = 0;
#define WetThres 450
#define DryThres 100 

//Servo for opening and closing the window
#include <Servo.h>
Servo window;
int servoPin = 2;     //GPIO2 D4 Signal for the servomotor

//Water pump (to water plants)
#define pump 15     //D8  GPIO15  Turns on and off the pump/relay



DHT dht(DHTPIN, DHTTYPE);

void setup() {
  //pinMode(blue, OUTPUT);  
  pinMode(DHTPIN, INPUT);
  pinMode(AnalogInput, INPUT);
  pinMode(ReadWatLev, OUTPUT);
  pinMode(ReadMoist, OUTPUT);
  pinMode(pump, OUTPUT);
  window.attach(servoPin);


  Serial.begin(115200);
  dht.begin();
  Serial.println("Setup is complete");
}

//-----------------------------------------------------------------------Void Loop------------------------------------------------
void loop() {
  Serial.println("Hello the code is running now :D");
  //Reading the humidity and temperature
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.println((String)"Humidity: " + h + " %\t");
  Serial.println((String)"Temperature: " + t + 0xC2 + 0xB0 + "C");

  delay(5000);
    
  //Reading the water level
  ReadWaterLevel();
  Serial.println((String)"Water Level is " + waterPercentage + "%");

  delay(5000);

  //Reading the moisture
  ReadMoisture();
  Serial.println((String)"Moisture is " + MoistLevel + "%");

  delay(5000);
  Serial.println("Opening window");
  OpenWindow(7);
  Serial.println("Closing window");
  delay(5000);

  Serial.println("Turning ON the water pump");
  digitalWrite(pump, HIGH);
  delay(10000);
  Serial.println("Turning OFF the water pump");
  digitalWrite(pump, LOW);
  
  


}


//-----------------------------------------------------------------------Functions for each sensor------------------------------------------------
//Reads the moisture sensor 100 times and gets the average to ensure a good reading
int ReadMoisture() {
  digitalWrite(ReadMoist, HIGH);
  for (int i = 0; i <= 100; i++)
  {
    MoistLevel = MoistLevel + analogRead(AnalogInput);
    delay(1);
  }
  MoistLevel = MoistLevel / 100.0;
  delay(30);
  digitalWrite(ReadMoist, LOW);
  return MoistLevel;
}

//Turns the reading into a 0-100% level
int ReadWaterLevel() {
  digitalWrite(ReadWatLev, HIGH);
  WaterLevel = analogRead(AnalogInput);
  waterPercentage = (int)((((double)WaterLevel) / 512) * 100.0);
  digitalWrite(ReadWatLev, LOW);
  return waterPercentage;
}

//Opens and closes the window for a certain amount of time
//Recieves the amount of seconds you want to open the window (parameter given in seconds)
void OpenWindow(int opTime) {
  window.write(180);  //Open the window
  delay(opTime*1000);
  window.write(80);  //Close the window
}
