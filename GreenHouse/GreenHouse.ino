//WiFi and Thingspeak
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Replace with your network details
const char* ssid = "Test";
const char* password = "liam1234";
WiFiClient client;

unsigned long channelID = 1710181; //your channel
const char * myWriteAPIKey = "1FZ6IC3WPT4UIVLO"; // your WRITE API key
const char* server = "api.thingspeak.com";


//Temperature and humidity sensor
#include "DHT.h"
#define DHTPIN 5    //GPIO5 D1
#define DHTTYPE DHT11
float h;
float t;

//Water level sensor
//#define ReadWatLev 4     //GPIO4 D2  Turns on and off the water level sensor
int WaterLevel = 0;
int waterPercentage = 0;

#define AnalogInput A0          //ESP only has one analog input gonna be used for moisture and water level


//Servo for opening and closing the window
#include <Servo.h>
Servo window;
int servoPin = 2;     //GPIO2 D4 Signal for the servomotor



DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);

  //pinMode(blue, OUTPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(AnalogInput, INPUT);
  //pinMode(ReadWatLev, OUTPUT);

  // initialize the servo
  Serial.println("Initializing servo...");
  window.attach(servoPin);
  window.write(90);  //Initial position of the window

  dht.begin();
  //Serial.println("Setup is complete");

  //Wifi setup
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");




}

//-----------------------------------------------------------------------Void Loop------------------------------------------------
void loop() {
  //WiFi things

  ThingSpeak.begin(client);
  if (client.connect(server, 80)) {

    // Measure Signal Strength (RSSI) of Wi-Fi connection
    long rssi = WiFi.RSSI();
    Serial.print("RSSI: ");
    Serial.println(rssi);
    ThingSpeak.setField(5, rssi);

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
    Serial.println((String)"Temperature: " + t + "C");

    delay(5000);

    //Reading the water level
    ReadWaterLevel();
    Serial.println((String)"Water Level is " + waterPercentage + "%");

    delay(10000);



    ThingSpeak.setField(1, t);    //Temperature
    ThingSpeak.setField(2, h);    //Humidity
    //ThingSpeak.setField(3, MoistLevel);    //Soil Moisture
    ThingSpeak.setField(4, waterPercentage);    //Water deposit

    //Write to Thingspeak
    ThingSpeak.writeFields(channelID, myWriteAPIKey);




    //Logic and greenhouse functionality---------------------------------------------------------------------------------------
    //Water deposit
    if (waterPercentage <= 20) {
      Serial.println("Please refill the water deposit");
      delay(5000);
    }

    //Humidity and temperature
    if (t >= 20 || h >= 35) {
      OpenWindow(7);
      delay(5000);
    }








  }
  client.stop();

  // wait and then post again
  delay(20000);

  //  delay(5000);
  Serial.println("Ended the loop");


}


//-----------------------------------------------------------------------Functions for each sensor------------------------------------------------
//Reads the moisture sensor 100 times and gets the average to ensure a good reading

//Turns the reading into a 0-100% level
int ReadWaterLevel() {
  //digitalWrite(ReadWatLev, HIGH);
  WaterLevel = analogRead(AnalogInput);
  waterPercentage = (int)((((double)WaterLevel) / 1023) * 100.0);
  //digitalWrite(ReadWatLev, LOW);
  return waterPercentage;
}

//Opens and closes the window for a certain amount of time
//Recieves the amount of seconds you want to open the window (parameter given in seconds)
void OpenWindow(int opTime) {
  Serial.println("Opening the window...");
  window.write(180);  //Open the window
  delay(opTime * 1000);
  window.write(90);  //Close the window
  Serial.println("Closing the window...");
  delay(1000);
}
//}
