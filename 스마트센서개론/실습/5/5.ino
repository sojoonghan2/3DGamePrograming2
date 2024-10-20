void setup() {
  Serial.begin(74880);
}

int led = 9;

void loop() {
  float sensorValue = analogRead(A0);

  // 밝기 값을 0-255 범위로 매핑
  int mappedBrightness = map(sensorValue, 0, 1023, 0, 255);
  analogWrite(led, mappedBrightness);
  Serial.println(mappedBrightness);
  delay(30);  // 30ms 딜레이
}