#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sensorPin = A0;
const int relayPin = 8;

const int AMBANG_KELEMBABAN = 71; // Batas kelembaban tanah (%)

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  int kelembaban = analogRead(sensorPin);
  int kelembabanPersen = map(kelembaban, 0, 1023, 0, 100);

  Serial.print("Kelembaban Tanah: ");
  Serial.print(kelembabanPersen);
  Serial.println("%");

  lcd.setCursor(0, 0);
  lcd.print("Kelembaban: ");
  lcd.print(kelembabanPersen);
  lcd.print("%   ");

  lcd.setCursor(0, 1);

  // --- VERSI ASLI (mengandung bug logika terbalik) ---
  // if (kelembabanPersen < AMBANG_KELEMBABAN) {
  //   lcd.print("Relay: OFF      ");
  //   digitalWrite(relayPin, LOW);
  // } else {
  //   lcd.print("Relay: ON       ");
  //   digitalWrite(relayPin, HIGH);
  // }
  // Bug: logika di atas justru menyalakan relay saat tanah SUDAH basah
  // (kelembaban >= 71%), dan mematikannya saat tanah kering (<71%) --
  // kebalikan dari perilaku yang diinginkan.

  // --- VERSI TERKOREKSI ---
  if (kelembabanPersen < AMBANG_KELEMBABAN) {
    // Tanah masih kering -> nyalakan aliran air
    lcd.print("Relay: ON       ");
    digitalWrite(relayPin, HIGH);
  } else {
    // Tanah sudah cukup basah -> matikan aliran air
    lcd.print("Relay: OFF      ");
    digitalWrite(relayPin, LOW);
  }

  delay(1000);
}
