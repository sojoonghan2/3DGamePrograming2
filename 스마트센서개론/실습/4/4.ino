void setup() {
  Serial.begin(9600);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

}

void loop() {
  int sensorVal[3];
  sensorVal[0] = digitalRead(2);
  sensorVal[1] = digitalRead(3);
  sensorVal[2] = digitalRead(4);

  if (sensorVal[0] == HIGH) {
    digitalWrite(5, LOW);
  }
  else {
    digitalWrite(5, HIGH);
  }

  if (sensorVal[1] == HIGH) {
    digitalWrite(6, LOW);
  }
  else {
    digitalWrite(6, HIGH);
  }

  if (sensorVal[2] == HIGH) {
    digitalWrite(7, LOW);
  }
  else {
    digitalWrite(7, HIGH);
  }
}
