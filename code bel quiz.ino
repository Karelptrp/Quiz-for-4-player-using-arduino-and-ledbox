#include <Adafruit_NeoPixel.h>

// Jumlah peserta
const int numParticipants = 4;

// Pin input pushbutton untuk setiap peserta
const int participantButtons[numParticipants] = {2, 3, 4, 5};

// Pin output LED WS2812B untuk setiap peserta
const int participantLEDs[numParticipants] = {6, 7, 8, 9};

// Pin output buzzer untuk setiap peserta
const int participantBuzzers[numParticipants] = {10, 11, 12, 13};

// Pin input pushbutton untuk juri
const int juriYesButton = A0;
const int juriNoButton = A1;

// Pin status LED untuk juri
const int juriLED = A2;

// Waktu bunyi untuk buzzer
const int buzzerDuration = 5000; // 5 detik

// Objek NeoPixel
Adafruit_NeoPixel leds[numParticipants];

// Status peserta
int participantPressed[numParticipants] = {0, 0, 0, 0};

void setup() {
  // Mengatur peserta buttons sebagai input
  for (int i = 0; i < numParticipants; i++) {
    pinMode(participantButtons[i], INPUT_PULLUP);
  }

  // Mengatur juri buttons sebagai input
  pinMode(juriYesButton, INPUT_PULLUP);
  pinMode(juriNoButton, INPUT_PULLUP);

  // Mengatur LED peserta sebagai output
  for (int i = 0; i < numParticipants; i++) {
    leds[i] = Adafruit_NeoPixel(1, participantLEDs[i], NEO_GRB + NEO_KHZ800);
    leds[i].begin();
    leds[i].show();
  }

  // Mengatur juri LED sebagai output
  pinMode(juriLED, OUTPUT);
}

void loop() {
  // Mengecek tombol peserta
  for (int i = 0; i < numParticipants; i++) {
    if (digitalRead(participantButtons[i]) == LOW && participantPressed[i] == 0) {
      participantPressed[i] = 1;
      buzzerOn(i);
      ledOn(i, 0, 0, 255); // Warna biru
      
      delay(buzzerDuration);
      buzzerOff(i);
      
      resetStatusExcept(i);
    }
  }

  // Mengecek tombol juri
  if (digitalRead(juriYesButton) == LOW && checkIfParticipantPressed()) {
    juriResponse(1);
    
    for (int i = 0; i< numParticipants; i++) {
      ledOn(i, 0, 255, 0); // Warna hijau
    }
    
    buzzerOnAll();
    delay(buzzerDuration);
    buzzerOffAll();
    
    delay(5000); // Jeda 5 detik sebelum mengembalikan status ke awal
    
    resetStatus();
  }

  if (digitalRead(juriNoButton) == LOW && checkIfParticipantPressed()) {
    juriResponse(0);
    
    for (int i = 0; i< numParticipants; i++) {
      ledOn(i, 255, 0, 0); // Warna merah
    }
    
    buzzerOnAll();
    delay(500); // Setengah detik bunyi pertama
    buzzerOffAll();
    delay(500); // Setengah detik jeda
    buzzerOnAll();
    delay(500); // Setengah detik bunyi kedua
    buzzerOffAll();
    
    delay(5000); // Jeda 5 detik sebelum mengembalikan status ke awal
    
    resetStatus();
  }
}

// Mengatur buzzer peserta menyala
void buzzerOn(int participant) {
  digitalWrite(participantBuzzers[participant], HIGH);
}

// Mengatur buzzer peserta mati
void buzzerOff(int participant) {
  digitalWrite(participantBuzzers[participant], LOW);
}

// Mengatur LED peserta menyala dengan warna tertentu
void ledOn(int participant, int red, int green, int blue) {
  leds[participant].setPixelColor(0, red, green, blue);
  leds[participant].show();
}

// Mengatur semua buzzer peserta menyala
void buzzerOnAll() {
  for (int i = 0; i < numParticipants; i++) {
    digitalWrite(participantBuzzers[i], HIGH);
  }
}

// Mengatur semua buzzer peserta mati
void buzzerOffAll() {
  for (int i = 0; i < numParticipants; i++) {
    digitalWrite(participantBuzzers[i], LOW);
  }
}

// Mengatur status juri
void juriResponse(int response) {
  if (response == 1) {
    digitalWrite(juriLED, HIGH);
  } else {
    digitalWrite(juriLED, LOW);
  }
}

// Mengatur status peserta menjadi default
void resetStatus() {
  for (int i = 0; i < numParticipants; i++) {
    participantPressed[i] = 0;
    buzzerOff(i);
    ledOn(i, 0, 0, 0); // Matikan LED
  }
}

// Mengatur status peserta menjadi default, kecuali peserta tertentu
void resetStatusExcept(int exceptParticipant) {
  for (int i = 0; i < numParticipants; i++) {
    if (i != exceptParticipant) {
      participantPressed[i] = 0;
      buzzerOff(i);
      ledOn(i, 0, 0, 0); // Matikan LED
    }
  }
}

// Mengecek apakah ada peserta yang sudah menekan tombol
bool checkIfParticipantPressed() {
  for (int i = 0; i < numParticipants; i++) {
    if (participantPressed[i] == 1) {
      return true;
    }
  }
  return false;
}
