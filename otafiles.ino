#include <WiFi.h>
#include <HTTPClient.h>
#include <FS.h>
#include <SPIFFS.h>
#include "demo.h"  // Başta dosyayı dahil ediyoruz

const char* ssid = "aaaabbbb";
const char* password = "bg12as7p";
const char* fileURL = "https://raw.githubusercontent.com/omorfx/otafiles/refs/heads/main/demo.h";  // Dosyanın indirileceği URL

void setup() {
  Serial.begin(115200);

  // Wi-Fi'ye bağlan
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // SPIFFS dosya sistemini başlat
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount file system");
    return;
  }
}

void updateFile(){
  // Dosyayı HTTP üzerinden indir
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(fileURL);  // URL'yi belirt
    int httpCode = http.GET();  // GET isteği gönder
    if (httpCode == 200) {  // Başarılı yanıt alındı
      String payload = http.getString();  // Dosya içeriğini al

      // Dosya aç ve güncelle
      File file = SPIFFS.open("/demo.h", "w");
      if (file) {
        file.print(payload);  // İçeriği yaz
        file.close();
        Serial.println("File updated successfully");
      } else {
        Serial.println("Failed to open file for writing");
      }
    } else {
      Serial.println("Error in HTTP request");
    }

    http.end();  // HTTP bağlantısını sonlandır
  }
}

void loop() {
  if (Serial.available()) {
    String receivedCommand = Serial.readStringUntil('\n');  // Komutu al
    receivedCommand.trim();  // Gereksiz boşlukları temizle
    if (receivedCommand == "update") {
      // UPDATE komutu alındı, dosyayı indirip güncelle
      Serial.println("Updating the file...");
      updateFile();
    }
  }else{
    printConstant();    
  }
  delay(1000);
}
