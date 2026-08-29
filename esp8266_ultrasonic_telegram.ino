#include "CTBot.h"

CTBot myBot;
String ssid = "NAMA_WIFI_ANDA";      // Ganti dengan SSID WiFi Anda
String pass = "PASSWORD_WIFI_ANDA";  // Ganti dengan password WiFi Anda
String token = "TELEGRAM_BOT_TOKEN_ANDA"; // Token bot Telegram (dapatkan dari @BotFather)

const int trigPin = D5;  // Pin trigger sensor ultrasonik
const int echoPin = D6;  // Pin echo sensor ultrasonik

long duration;
float jarakCm;
bool stopSending = true; // Mulai dalam kondisi berhenti; aktif setelah menerima "Info"

void setup() {
  Serial.begin(9600);

  // Bagian CTBot
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);

  if (myBot.testConnection())
    Serial.println("\nKoneksi Berhasil");
  else
    Serial.println("\nKoneksi Gagal");

  // Konfigurasi pin sensor ultrasonik
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  TBMessage msg;
  if (myBot.getNewMessage(msg)) {
    if (msg.text.equalsIgnoreCase("Info")) {
      stopSending = false; // Mulai mengirim pesan
    } else if (msg.text.equalsIgnoreCase("Sip")) {
      myBot.sendMessage(msg.sender.id, "Pengiriman pesan dihentikan.");
      stopSending = true; // Hentikan pengiriman pesan
    }
  }

  // Jika tidak berhenti, kirim pesan ketinggian air
  if (!stopSending) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    jarakCm = duration * 0.034 / 2.0;

    if (jarakCm >= 1 && jarakCm <= 20) {
      myBot.sendMessage(msg.sender.id, (String) "Jarak : " + String(jarakCm) + " cm");
    }
  }

  delay(2000); // Tunda 2 detik sebelum membaca ulang
}
