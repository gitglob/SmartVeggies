#define reciever A1
#define pump 7

void setup() {
  pinMode(reciever, INPUT);
  pinMode(pump, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  long sig = analogRead(reciever);
  //Serial.println(sig);
  if (sig > 600) {
    digitalWrite(pump, HIGH);     //Activate the pump
    delay(9000);
    long sig = analogRead(reciever);
    //Serial.println(sig);
  }
  //Serial.println("Out of the while loop");
  digitalWrite(pump, LOW);     //Turn off the pump
  delay(1000);
  

}
