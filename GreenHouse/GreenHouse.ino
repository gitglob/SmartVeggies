//WiFi and Thingspeak
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Replace with your network details
const char* ssid = "Memo";
const char* password = "12345678";
//WiFiClient client;

unsigned long channelID = 1710181; //your channel
const char * myWriteAPIKey = "1FZ6IC3WPT4UIVLO"; // your WRITE API key
const char* server = "api.thingspeak.com";


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
#define ReadMoist 13     //D7  GPIO13  Turns on and off the moisture sensor
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
  //Serial.println("Setup is complete");

  //Wifi setup
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //WiFi.begin(ssid, password);

  /*
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
  */



}

//-----------------------------------------------------------------------Void Loop------------------------------------------------
void loop() {
  //WiFi things
  /*
    ThingSpeak.begin(client);
    if (client.connect(server, 80)) {

    // Measure Signal Strength (RSSI) of Wi-Fi connection
    long rssi = WiFi.RSSI();
    Serial.print("RSSI: ");
    Serial.println(rssi);
    ThingSpeak.setField(5, rssi);
  */
  //Read the sensors
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

  delay(10000);

  //Reading the moisture
  ReadMoisture();
  Serial.println((String)"Moisture is " + MoistLevel + "%");

  /*
      ThingSpeak.setField(1, t);    //Temperature
      ThingSpeak.setField(2, h);    //Humidity
      ThingSpeak.setField(3, MoistLevel);    //Soil Moisture
      ThingSpeak.setField(4, waterPercentage);    //Water deposit


      ThingSpeak.writeFields(channelID, myWriteAPIKey);
    }
    client.stop();

    // wait and then post again
    delay(20000);
  */
  //Logic and greenhouse functionality---------------------------------------------------------------------------------------
  //Water deposit
  if (waterPercentage <= 20) {
    Serial.println("Please refill the water deposit");
    delay(5000);
  }

  //Soil moisture
  if (MoistLevel >= 1000) {
    PumpWater(10);
    delay(5000);
  }

  //Humidity and temperature
  if (t >= 26 || h >= 35) {
    Serial.println("Opening window");
    OpenWindow(7);
    Serial.println("Closing window");
    delay(5000);
  }
  
  delay(5000);






}


//-----------------------------------------------------------------------Functions for each sensor------------------------------------------------
//Reads the moisture sensor 100 times and gets the average to ensure a good reading
int ReadMoisture() {
  digitalWrite(ReadMoist, HIGH);
  delay(10000);
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
  delay(opTime * 1000);
  window.write(80);  //Close the window
}

//Activates the pump for a certain amount of time
//Recieves the amount of seconds you want to activate it (parameter given in seconds)
void PumpWater(int opTimeP) {
  Serial.println("Turning ON the water pump");
  digitalWrite(pump, HIGH);
  delay(opTimeP * 1000);
  Serial.println("Turning OFF the water pump");
  digitalWrite(pump, LOW);

}
