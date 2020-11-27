#define CAMERA_MODEL_AI_THINKER
#define EEPROM_SIZE 1

#include "esp_camera.h"
#include "camera_pins.hpp"
#include "badge.hpp"
#include "detect.hpp"
#include "telegramBot.hpp"

#include <EEPROM.h>
#include <Arduino.h>

uint16_t count = 0;
bool SetCam;
void setupCamera(bool statusformat);

void setup() {
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(0, SetCam);
    setupCamera(SetCam);
}
   
void loop() {
  Serial.println(String(SetCam));
  if(SetCam){
    if (!capture_still()) {
        Serial.println("Failed capture");
        delay(3000);
        return;
    }
    if (motion_detect()) {
      if (count>=3){
          Serial.println("Motion detected");
          SetCam = false;
          EEPROM.write(0, false);
          EEPROM.commit();
          ESP.restart();
        }
      else
          count++;    
    }
    update_frame();
    Serial.println("=================");
  }
  else{      
      wifi();
      SetCam = true;
      for(int i = 0; i<4;i++){
         take_send_photo();
         i++;
         delay(15000);
        }
      EEPROM.write(0, true);
      EEPROM.commit();
      ESP.restart();
    }
}

void setupCamera(bool statusformat){
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    if(statusformat){
      config.pixel_format = PIXFORMAT_GRAYSCALE; 
      config.frame_size = FRAMESIZE_QVGA;
    }
    else{
      config.pixel_format = PIXFORMAT_JPEG;
      config.frame_size = FRAMESIZE_VGA;
    }
    config.jpeg_quality = 12;
    config.fb_count = 1;
    esp_camera_init(&config);

    sensor_t *sensor = esp_camera_sensor_get();
    sensor->set_framesize(sensor, config.frame_size);
}
