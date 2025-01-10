#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include "demo.h"  // Başta dosyayı dahil ediyoruz

const char* ssid = "aaaabbbb";
const char* password = "bg12as7p";
const char* firmwareURL = "https://raw.githubusercontent.com/omorfx/otafiles/refs/heads/main/build/esp32.esp32.esp32doit-devkit-v1/otafiles.ino.bin";  // Dosyanın indirileceği URL

void setup() {
  Serial.begin(115200);
  delay(1000);
  // Wi-Fi'ye bağlan
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  delay(1000);
}

void updateFirmware() {
  // HTTP istemcisi ile güncelleme işlemi başlat
  HTTPClient http;
  http.begin(firmwareURL);  // Firmware dosyasının URL'sini belirtiyoruz
  int httpCode = http.GET();  // HTTP GET isteği gönderiyoruz

  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();
    WiFiClient* stream = http.getStreamPtr();

    // Eğer içerik uzunluğu geçerli ise, OTA güncellemesi yapılacak
    if (contentLength) {
      Serial.printf("Updating firmware, size: %d bytes\n", contentLength);
      if (Update.begin(contentLength)) {
        size_t written = Update.writeStream(*stream);
        if (written == contentLength) {
          Serial.println("Firmware update complete");
          if (Update.end()) {
            Serial.println("Rebooting now...");
            ESP.restart();  // Cihazı yeniden başlat
          } else {
            Serial.println("Error during update");
          }
        } else {
          Serial.println("Write failed");
        }
      } else {
        Serial.println("Not enough space for OTA update");
      }
    }
  } else {
    Serial.println("Failed to fetch firmware file");
  }
  http.end();  // HTTP bağlantısını sonlandır
}

void loop() {
  if (Serial.available()) {
    String receivedCommand = Serial.readStringUntil('\n');  // Komutu al
    receivedCommand.trim();  // Gereksiz boşlukları temizle
    if (receivedCommand == "update") {
      // UPDATE komutu alındı, dosyayı indirip güncelle
      Serial.println("Updating the file...");
      updateFirmware();
    }
  }else{
    printConstant();    
  }
  delay(1000);
}
