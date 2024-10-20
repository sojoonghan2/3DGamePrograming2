#include "pitches.h"

int melody[] = {
  F4, G4, A4, F4, C5, A4, G4, C5, G4,
  F4, D4, A4, F4, E4,
  F4, E4, D4, E4, F4, G4,
  C4, F4, G4, A4,
  AS4, AS4, A4, G4, F4, G4,

  F4, G4, A4, F4, C5, A4, G4, C5, G4,
  F4, D4, A4, F4, E4,
  F4, E4, D4, E4, F4, G4,
  C4, F4, G4, A4,
  AS4, AS4, A4, G4, F4, F4, 0,

  A4, AS4, C5, C5, C5, C5, C5, D5, C5, AS4,
  A4, A4, A4, A4, A4, AS4, A4, G4,
  F4, F4, F4, E4, D4, E4, E4, F4,
  G4, G4, A4, G4, A4, G4,

  A4, AS4, C5, C5, C5, C5, C5, D5, C5, AS4,
  A4, A4, A4, A4, AS4, A4, G4, F4, E4,
  D4, D4, E4, F4, G4, C4, F4, G4, A4,
  G4, G4, G4, F4, F4,
};

int noteDurations[] = {
  8, 8, 8, 8, 4, 4, 4, 4, 4,
  8, 8, 4, 4, 2,
  8, 8, 4, 4, 8, 8, 
  4, 4, 8, 8,
  4, 8, 8, 8, 8, 2,

  8, 8, 8, 8, 4, 4, 4, 4, 4,
  8, 8, 4, 4, 2,
  8, 8, 4, 4, 8, 8, 
  4, 4, 8, 8,
  4, 8, 8, 8, 8, 2, 8,

  8, 8, 4, 4, 4, 4, 8, 8, 8, 8,
  4, 4, 4, 4, 8, 8, 8, 8,
  4, 4, 8, 8, 4, 4, 8, 8,
  4, 8, 8, 8, 8, 2,

  8, 8, 4, 4, 4, 4, 8, 8, 8, 8,
  4, 4, 4, 8, 8, 8, 8, 8, 8,
  4, 8, 8, 8, 8, 4, 4, 8, 8,
  4, 8, 8, 8, 4,
};

void setup() {
  int numNotes = sizeof(melody) / sizeof(melody[0]);  // 배열 크기 계산
  for (int thisNote = 0; thisNote < numNotes; ++thisNote) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(8, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    noTone(8);
  }

}

void loop() {

}
