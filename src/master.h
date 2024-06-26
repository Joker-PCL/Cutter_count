// ห้ามลบ
#include <Arduino.h>
#include <lv_conf.h>
#include <lvgl.h>
#include "./gui/gui.h"

int btn = 13;
int Air_cylinder_output = 11;
int Bottle_cap = 12;
int sensor = 0;

int count = 0;
bool state1 = false;
bool state2 = false;
bool state_sensor = false;
bool state = false;
bool stop_state = false;
unsigned long timer = 0;
unsigned long timer2 = 0;
int start_time = 0;
int stop_timer2 = 0;

// ฟังชั่นรับอีเว้นท์
//  void ui_event_counter(lv_event_t *e)
//  {
//    lv_event_code_t event_code = lv_event_get_code(e);
//    if (event_code == LV_EVENT_CLICKED)
//    {
//      // ดึงค่าที่ถูกส่งมาจาก user_data
//      // const char *_ui_VALUES = (const char *)lv_event_get_user_data(e);
//      // Serial.println(_ui_VALUES);

//     if (_ui_VALUES == "ui_Button1btn")
//     {
//       digitalWrite(Bottle_cap,HIGH);
//       state1 = !state1;
//       Serial.println(state1);
//     }

//     if (_ui_VALUES == "ui_Button3btn2")
//     {
//       digitalWrite(Bottle_cap,HIGH);
//       state1 = !state1;
//       Serial.println(state1);
//     }
//     }
//  }

void ui_event_Button1btn(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  if (event_code == LV_EVENT_CLICKED)
  {
    state1 = !state1;
    Serial.println(state1);
  }
}

void ui_event_Button3btn(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  if (event_code == LV_EVENT_CLICKED)
  {
    state2 = !state2;
    Serial.println(state2);
  }
}

void ui_event_Button4homeA(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  if (event_code == LV_EVENT_CLICKED)
  {
    state1 = false;
    state2 = false;
  }
}

void ui_event_Settimer2(lv_event_t *e)
{
  lv_obj_t *obj = lv_event_get_target(e);
  const char *value = lv_textarea_get_text(obj);
  stop_timer2 = atoi(value);
  lv_label_set_text(ui_messageTimer2, value);
  Serial.print("stop_timer: ");
  Serial.println(stop_timer2);
}

void ui_Buttonresetcount1(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  if (event_code == LV_EVENT_CLICKED)
  {
    count = 0;
    lv_label_set_text(ui_Label10, String(count).c_str());
    Serial.println(count);
  }
}

// ฟังชั่นเซ็ตหารหน่วงเวลา
int cur_timerPanel;
lv_obj_t *timerPanel(int panel)
{
  switch (panel) {
    case 1:
      return ui_timerLabel1;
    case 2:
      return ui_timerLabel2;
  }
  
}

void ui_event_timerPanel(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);

  const int _timerPanel = (const int)lv_event_get_user_data(e);
  cur_timerPanel = _timerPanel;

  _ui_flag_modify(ui_timerInputPanel, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
}

void ui_event_Keyboard(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  lv_obj_t *target = lv_event_get_target(e);
  if (event_code == LV_EVENT_READY)
  {
    _ui_flag_modify(ui_timerInputPanel, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
    const char *value = lv_textarea_get_text(ui_timerInput);
    lv_label_set_text(timerPanel(cur_timerPanel), value);
    lv_textarea_set_text(ui_timerInput, "");
  }
}

void setup()
{
  pinMode(Air_cylinder_output, OUTPUT);
  pinMode(Bottle_cap, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  // ห้ามลบ
  Serial.begin(115200);
  gui_start();
  gfx.setBrightness(128);

  // ประกาศอีเว้นท์ ui_Start == ปุ่มstart stop, ui_event_counter == //ฟังชั่นรับอีเว้นท์, LV_EVENT_ALL, (void *)"Start" ==ค่าที่ส่งไปให้ฟังชั่น
  //  lv_obj_add_event_cb(ui_Button1btn, ui_event_counter, LV_EVENT_ALL, (void *)"ui_Button1btn");
  //  lv_obj_add_event_cb(ui_Button3btn2, ui_event_counter, LV_EVENT_ALL, (void *)"ui_Button3btn2");

  // All Events
  lv_obj_add_event_cb(ui_Button1btn, ui_event_Button1btn, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button3btn2, ui_event_Button3btn, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Button4home, ui_event_Button4homeA, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui_Buttonresetcount, ui_Buttonresetcount1, LV_EVENT_ALL, NULL);

  // ประกาศอีเว้นท์ ui_Start == ปุ่มstart stop, ui_event_counter == //ฟังชั่นรับอีเว้นท์, LV_EVENT_ALL, (void *)"Start" ==ค่าที่ส่งไปให้ฟังชั่น
  //  lv_obj_add_event_cb(ui_TextArea1setting2, ui_event_Settimer1, LV_EVENT_READY, (void *)"ui_Button1btn");
  //  lv_obj_add_event_cb(ui_TextArea2setting, ui_event_Settimer2, LV_EVENT_READY, (void *)"ui_Button3btn2");

  // Single Event
  lv_obj_add_event_cb(ui_timerPanel1, ui_event_timerPanel, LV_EVENT_CLICKED, (void *)1);
  lv_obj_add_event_cb(ui_timerPanel2, ui_event_timerPanel, LV_EVENT_CLICKED, (void *)2);
  lv_obj_add_event_cb(ui_Keyboard, ui_event_Keyboard, LV_EVENT_ALL, NULL);
}

void loop()
{
  // ห้ามลบ
  lv_timer_handler();
  delay(5);
  digitalWrite(Bottle_cap, state1);
  digitalWrite(Air_cylinder_output, state2);

  sensor = digitalRead(btn);

  if (sensor == 0 && !state_sensor)
  {
    state_sensor = true;
    state = true;
    timer = millis();
  }

  if (sensor == 1)
  {
    state_sensor = false;
    delay(10);
  }

  if (state)
  {
    if (millis() - timer >= 1000)
    {
      digitalWrite(Air_cylinder_output, HIGH);
    }

    if (millis() - timer >= start_time * 1000)
    {
      digitalWrite(Bottle_cap, HIGH);
      timer2 = millis();
      state = false;
      stop_state = true;
    }
  }

  if (stop_state)
  {
    if (millis() - timer2 >= stop_timer2 * 1000)
    {
      digitalWrite(Bottle_cap, LOW);
    }

    if (millis() - timer2 >= 1700)
    {
      digitalWrite(Air_cylinder_output, LOW);
      stop_state = false;
      count++;
      lv_label_set_text(ui_Label10, String(count).c_str());
      Serial.println(count);
    }
  }
}