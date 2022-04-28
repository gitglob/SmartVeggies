// port 9 -> upper left

#define reciever A1
#define pump 12
#define led 13

void setup() {
  pinMode(reciever, INPUT);
  pinMode(led, OUTPUT);
  pinMode(pump, OUTPUT);
  digitalWrite(pump, HIGH);     //de-Activate the pump
  Serial.begin(9600);

}

void loop() {
  long sig = analogRead(reciever);
  Serial.println("Received value: ");
  Serial.println(sig);
  if (sig > 600) {
    Serial.println("Watering the plant...");

    // blink the led
    digitalWrite(led, HIGH);      // turn the led ON
    delay(100);

    digitalWrite(pump, LOW);     //Activate the pump
    delay(1500);
    digitalWrite(pump, HIGH);     //de-Activate the pump

    delay(100);
    digitalWrite(led, LOW);      // turn the led ON

    long sig = analogRead(reciever);
  }
  //Serial.println("Out of the while loop");
  digitalWrite(pump, HIGH);     //de-Activate the pump
  digitalWrite(led, LOW);     //Turn the led OFF
  //delay(10000);


}
