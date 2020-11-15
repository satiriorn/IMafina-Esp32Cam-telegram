#pragma once
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void wifi();
void take_send_photo();

void wifi(){
    Serial.print("Connecting Wifi: ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
  
void take_send_photo()
{
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  currentByte = 0;
  fb_length = fb->len;
  fb_buffer = fb->buf;
  bot.sendPhotoByBinary(chat_id, "image/jpeg", fb->len, isMoreDataAvailable, photoNextByte, nullptr, nullptr);
  esp_camera_fb_return(fb);
  fb_length = NULL;
  fb_buffer = NULL;
}
