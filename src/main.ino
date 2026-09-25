#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

void setup() {
  Serial.begin(115200);
  delay(1500);

  WiFi.mode(WIFI_STA);
  delay(300);

  Serial.println();
  Serial.println("================================");
  Serial.println("ESP32-C3 KONEKTOR");
  Serial.println("DIAGNOSTIC TEST");
  Serial.println("================================");

  Serial.print("Chip : ");
  Serial.println(ESP.getChipModel());

  Serial.print("Revision : ");
  Serial.println(ESP.getChipRevision());

  Serial.print("MAC : ");
  Serial.println(WiFi.macAddress());

  Serial.println("Initializing ESP-NOW...");
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESP-NOW : OK");
  } else {
    Serial.println("ESP-NOW : FAILED");
  }

  Serial.println("READY");
  Serial.println("================================");
}

void loop() {
  static uint32_t last = 0;
  if (millis() - last >= 5000) {
    last = millis();
    Serial.print("ONLINE | uptime=");
    Serial.print(millis() / 1000);
    Serial.println("s");
  }
}
