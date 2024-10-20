float PDSensorValue = 0.f;
const int ledPin = 9;  // LED를 9번 핀에 연결

void setup() {
  Serial.begin(115200);
  Serial.println("PD Sensor Read Start");
  pinMode(ledPin, OUTPUT);  // 9번 핀을 출력으로 설정
}

void loop() {
  // PD 센서 값 읽기
  for(int i = 0; i < 100; ++i)
  {
    PDSensorValue += analogRead(A0);
  }
  PDSensorValue /= 100;
  
  // 센서 값을 LED 밝기 범위(0-255)로 매핑
  int brightness = map(PDSensorValue, 0, 1023, 255, 0);
  
  // LED 밝기 설정
  analogWrite(ledPin, brightness);
  
  // 시리얼 모니터에 출력
  Serial.print("PD Sensor = ");
  Serial.print(PDSensorValue);
  Serial.print(", LED Brightness = ");
  Serial.println(brightness);

  delay(100);  // 약간의 지연 시간 추가
}

// #include <DHT.h>
// DHT dht(2, DHT11);
// void setup {
//   Serial.begin(115200);
// }

// void loop() {
//   delay(1000);
//   int h = dht.readHumidity();
//   int t = dht.readTemperature();
//   Serial.print("Humidity: ");
//   Serial.print(h);
//   Serial.print("%\t");
//   Serial.print("Temperature: ");
//   Serial.print(t);
//   Serial.print(" C");
// }