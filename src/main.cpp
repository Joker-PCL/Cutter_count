#define LGFX_LCD4_3
// #define LGFX_LCD7_0

//.................ฟังชั่น.....Library...........
#include <Arduino.h>
#include <Wire.h>
#include <lv_conf.h>
#include <lvgl.h>
#include "./gui/gui.h"
#include <EEPROM.h>

#define CUTTER_SENSOR 13

int CUR_CUTTER_SENSOR = 0;  // ค่าเซ็นเซอร์ปัจจุบัน
int PREV_CUTTER_SENSOR = 0; // ค่าเซ็นเซอร์ก่อนหน้า
unsigned int LED_CUTTER_SENSOR_TIME = 0;
unsigned long CYCLE_TIME = 0; // เก็บเวลาเวลาในแต่ละรอบ
unsigned long TIMEOUT = 0;    // เก็บเวลา TIMEOUT

bool isRunning = false;           // สถานะการทำงาน
unsigned long CPM_COUNT_TIME = 0; // เก็บเวลาตัวนับจำนวนใน 1 นาที
unsigned int LED_CPM_COUNT_TIME = 0;
int CPM_COUNT = 0; // เก็บจำนวนการตัดใน 1 นาที

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
  // delay(5);

  CUR_CUTTER_SENSOR = digitalRead(CUTTER_SENSOR); // อ่านค่าจากเซ็นเซอร์

  if (CUR_CUTTER_SENSOR == 0 && PREV_CUTTER_SENSOR == 1)
  {
    unsigned long CPS = millis() - CYCLE_TIME; // เปรียบเทียบเวลาในรอบการตัดเป็น มิลลิวินาที
    float CPM_F = float(60UL * 1000UL) / float(CPS);  // คำนวนจำนวนตัดต่อนาที 1 นาที หาร กับเวลาในรอบการตัด
    int CPM = ceil(CPM_F);                             // คำนวนจำนวนตัดต่อนาที 1 นาที หาร กับเวลาในรอบการตัด
    CYCLE_TIME = millis();                     // รีเซ็ตตัวจับเวลา
    TIMEOUT = millis();                        // รีเซ็ตเวลา timeout

    isRunning = true;                                          // เปลี่ยนสถานะการทำงาน -> true
    CPM_COUNT++;                                               // เพิ่มค่าจำนวนการตัดใน 1 นาที
    lv_label_set_text(ui_CPMcount, String(CPM_COUNT).c_str()); // อัพเดทหน้าจอ CPM_COUNT

    Serial.print("CPS: ");
    Serial.print(CPS);
    Serial.print(", CPM_F: ");
    Serial.print(CPM_F);
    Serial.print(", CPM: ");
    Serial.print(CPM);
    Serial.print(", Total CPM: ");
    Serial.println(CPM_COUNT);

    LED_CUTTER_SENSOR_TIME = millis();
    lv_obj_set_style_bg_color(ui_LedCPMcount, lv_color_hex(0x53E903), 0);
    lv_label_set_text(ui_CPM, String(CPM).c_str()); // อัพเดทหน้าจอ CPM
  }

  PREV_CUTTER_SENSOR = CUR_CUTTER_SENSOR;

  // ปิดไฟสถานะ CUTTER
  if (millis() - LED_CUTTER_SENSOR_TIME >= 300)
  {
    lv_obj_set_style_bg_color(ui_LedCPMcount, lv_color_hex(0x1E3D0E), 0);
  }

  // หากไม่มีสัญญานจากเซ็นเซอร์ภายใน 10 วินาที ให้รีเซ็ตค่า CPM เป็น 0 และ สถานะการทำงาน -> true
  if (millis() - TIMEOUT >= 3000UL && isRunning)
  {
    isRunning = false;                                 // เปลี่ยนสถานะการทำงาน -> false
    lv_label_set_text(ui_CPM, String(0).c_str());      // อัพเดทหน้าจอ CPM
    lv_label_set_text(ui_CPMtime, String(60).c_str()); // อัพเดทหน้าจอ CPM_COUNT
    lv_label_set_text(ui_CPMcount, String(0).c_str()); // อัพเดทหน้าจอ CPM_COUNT
  }

  //  เก็บค่าเซ็นต์เซอร์ ทุกๆ 1 นาที หาก สถานะการทำงาน -> true
  if (millis() - CPM_COUNT_TIME <= 60000UL && isRunning)
  {
    // เปิดไฟสถานะตัวจับเวลา
    if ((millis() - LED_CPM_COUNT_TIME) >= 1000)
    {
      lv_obj_set_style_bg_color(ui_LedCPMtime, lv_color_hex(0x53E903), 0);
      LED_CPM_COUNT_TIME = millis();
      lv_label_set_text(ui_CPMtime, String((millis() - CPM_COUNT_TIME) / 1000).c_str()); // อัพเดทหน้าจอ CPM_COUNT
    }
  }
  else
  {
    CPM_COUNT = 0;             // รีเซ็ตค่าจำนวนการตัดใน 1 นาที
    CPM_COUNT_TIME = millis(); // รีเซ็ตเวลาตัวนับจำนวนใน 1 นาที
  }

  // ปิดไฟสถานะตัวจับเวลา
  if (millis() - LED_CPM_COUNT_TIME >= 300)
  {
    lv_obj_set_style_bg_color(ui_LedCPMtime, lv_color_hex(0x1E3D0E), 0);
  }
}
