#define LGFX_LCD4_3
// #define LGFX_LCD7_0

//.................ฟังชั่น.....Library...........
#include <Arduino.h>
#include <Wire.h>
#include <lv_conf.h>
#include <lvgl.h>
#include "./gui/gui.h"
#include <EEPROM.h>

#define CUTTER_SENSOR 11

int currentCutterSensor = 0;
int previousCutterSensor = 0;
unsigned long CycleTime = 0;
unsigned long Timeout = 0;

void setup()
{
  // ห้ามลบ
  Serial.begin(115200);
  gui_start();
  gfx.setBrightness(128);

  pinMode(CUTTER_SENSOR, INPUT_PULLUP);
}

void loop()
{
  // ห้ามลบ
  lv_timer_handler();
  delay(5);

  currentCutterSensor = digitalRead(CUTTER_SENSOR);
  if (currentCutterSensor == 0 && previousCutterSensor == 1)
  {
    unsigned long CPS = millis() - CycleTime;
    int CPM = (60UL * 1000UL) / CPS;
    CycleTime = millis();
    Timeout = millis();
    
    Serial.print("Cut per minute: ");
    Serial.println(CPM);

    lv_label_set_text(ui_CPM, String(CPM).c_str());
  }

  previousCutterSensor = currentCutterSensor;

  if(millis() - Timeout >= 10000) {
    lv_label_set_text(ui_CPM, String(0).c_str());
  }

}
