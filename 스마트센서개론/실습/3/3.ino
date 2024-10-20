#define WAITTIME 500

void setup() {
  Serial.begin(9600);
  for(int i{2}; i < 6; i++) {
    pinMode(i, OUTPUT);
  }
}

void sol1() {
  for(int i{2}; i < 6; ++i) {
    digitalWrite(i, HIGH);
    delay(WAITTIME);
    digitalWrite(i, LOW);
    delay(WAITTIME);
  }
}

void sol2() {
  for(int i{2}; i < 5; i++) {
    digitalWrite(i, HIGH);
    digitalWrite(i+1, HIGH);
    delay(WAITTIME);
    digitalWrite(i, LOW);
    digitalWrite(i+1, LOW);
    delay(WAITTIME);
  }
}

void sol3() {
  for(int i =2; i < 6; ++i) {
    digitalWrite(i, HIGH);
    delay(WAITTIME);
    digitalWrite(i, LOW);
    delay(WAITTIME);
  }
  for(int i = 4; i > 1; --i) {
    digitalWrite(i, HIGH);
    delay(WAITTIME);
    digitalWrite(i, LOW);
    delay(WAITTIME);
  }
}

void loop() {
  sol1();
  sol2();
  sol3();
}
