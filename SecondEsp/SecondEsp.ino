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


#define AnalogInput A0          //ESP only has one analog input gonna be used for moisture and water level

//Moisture sensor
//#define ReadMoist 13     //D7  GPIO13  Turns on and off the moisture sensor
int MoistLevel = 0;
int MoistPercentage = 0;
#define WetThres 450
#define DryThres 100

//Water pump (to water plants)
#define pump 15     //D8  GPIO15  Turns on and off the pump/relay



void setup() {
  pinMode(AnalogInput, INPUT);
  //  pinMode(ReadMoist, OUTPUT);
  pinMode(pump, OUTPUT);


  Serial.begin(115200);

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
    //long rssi = WiFi.RSSI();
    //Serial.print("RSSI: ");
    //Serial.println(rssi);
    //ThingSpeak.setField(5, rssi);

    //Read the sensors
    delay(10000);

    //Reading the moisture
    ReadMoisture();
    Serial.println((String)"Moisture is " + MoistLevel + "%");


    ThingSpeak.setField(3, MoistLevel);    //Soil Moisture


    ThingSpeak.writeFields(channelID, myWriteAPIKey);
  }
  client.stop();

  // wait and then post again
  delay(20000);

  //Logic and greenhouse functionality---------------------------------------------------------------------------------------

  //Soil moisture
  if (MoistLevel >= 1000) {
    PumpWater(9);
    delay(5000);
  }

}


//-----------------------------------------------------------------------Functions for each sensor------------------------------------------------
//Reads the moisture sensor 100 times and gets the average to ensure a good reading
int ReadMoisture() {
  //digitalWrite(ReadMoist, HIGH);
  delay(10000);
  for (int i = 0; i <= 100; i++)
  {
    MoistLevel = MoistLevel + analogRead(AnalogInput);
    delay(1);
  }
  MoistLevel = MoistLevel / 100.0;
  delay(30);
  //digitalWrite(ReadMoist, LOW);
  return MoistLevel;
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
