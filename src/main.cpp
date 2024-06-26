// ห้ามลบ
#define LGFX_LCD4_3
// #define LGFX_LCD7_0

//.................ฟังชั่น.....Library...........
#include <Arduino.h>
#include <lv_conf.h>
#include <lvgl.h>
#include "./gui/gui.h"
#include <EEPROM.h>

//..............ตัวแปรใช้สำหรับ ส่งค่าreturn......................
int cur_time_panel = 0;

//................................ส่งค่ารีเทริน......................................................
lv_obj_t *timerPanel(int panel) // รับค่าจาก lv_event_get_user_data(e) แปลงค่ามาเป็น cur_time_panel
{
  switch (panel) // ฟังชั่น รอรีเทริน
  {
  case 1:
    return ui_Labelsettime1; // ส่งค่ากลับไปยัง lv_label_get_text
  case 2:
    return ui_Labelsettime2; // ส่งค่ากลับไปยัง lv_label_get_text
  }
}

//.......................ฟังชั่นกดใส่ค่าตัวเลข.........................................
void set_time(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  const int settime1 = (const int)lv_event_get_user_data(e);                    // เก็บค่าเป็น....คอนอิ้น settimer  = เก็บค่า....คอนอิ้น....lv_event_get_user_data(e)
  cur_time_panel = settime1;                                                    //....ใช้ตัวแปร int มาเก็บค่าของ consrt int
  const char *cur_value = lv_label_get_text(timerPanel(cur_time_panel));        // สั่ง cur_value มีค่าเท่ากับ lv_label_get_text(timerPanel(cur_time_panel)
  _ui_flag_modify(ui_Panelkeyboad, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE); // สั่งแสดงคีบอร์ด
}

//.........................ฟังชั่นคีบอร์ด................................
void Keyboard1(lv_event_t *e)
{
  lv_event_code_t event_code = lv_event_get_code(e);
  if (event_code == LV_EVENT_READY) // ถ้ามีการกด enter
  {
    _ui_flag_modify(ui_Panelkeyboad, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD); // สั่งปิดคีบอร๋ด
    const char *time_textarea = lv_textarea_get_text(ui_TextAreakey);          //....ตั้งตัวแปรเป็น..const char เท่ากับ lv_textarea_get_text

    if (String(time_textarea) != "")
    {
      lv_label_set_text(timerPanel(cur_time_panel), time_textarea); //....สั่งเข้า return และ เช็ทตัวเลขบนหน้าจอ

      //..........................สั่งทำเงื่อนไข...บันทึกการตั้งค่า starttimer.......
      if (cur_time_panel == 1) // เงื่อนไข...ถ้ากดsettimerstart.....
      {
        Serial.println("1");
      }
      //.............................สั่งทำเงื่อนไข....บันทึกค่าการตั้งค่า stoptimer..........
      else if (cur_time_panel == 2) // ถ้ากดsettimerstop
      {
        Serial.println("2");
      }
      lv_textarea_set_text(ui_TextAreakey, ""); // สั่งเคลียร์คีบอร์ด
    }
  }
}

//...............................ฟังชั่นปิดคีบอร์ด......................................
void Panel8(lv_event_t *e)
{
  _ui_flag_modify(ui_Panelkeyboad, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD); // สั่งปิดคีบอร๋ด
}

//........................................................................................
void setup()
{
  // ห้ามลบ.......................................
  Serial.begin(115200);
  EEPROM.begin(500); // ประกาศมาใช้งานบันทึกข้อมูล
  gui_start();
  gfx.setBrightness(128);

  //.....................ฟังชั่น......EVENT.....................................................................
  lv_obj_add_event_cb(ui_Panelsettime1, set_time, LV_EVENT_CLICKED, (void *)1); // ถ้ากดเซ็ทtimer start ให้แสดงคีบอร์ด
  lv_obj_add_event_cb(ui_Panelsettime2, set_time, LV_EVENT_CLICKED, (void *)2); // ถ้ากดเซ็ทtimer start ให้แสดงคีบอร์ด
  lv_obj_add_event_cb(ui_Keyboard, Keyboard1, LV_EVENT_ALL, NULL);              // ส่งค่าไปยัง void ui_event_Keyboard
  lv_obj_add_event_cb(ui_Panel8, Panel8, LV_EVENT_CLICKED, NULL);               // สั่งปิดคีบอร์ด
}
//...............................................................................................
void loop()
{
  // ห้ามลบ
  lv_timer_handler();
  delay(5);
}
