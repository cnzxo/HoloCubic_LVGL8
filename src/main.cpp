#include <lvgl.h>
#include <TFT_eSPI.h>
#include <lv_demo.h>

#include "sd_card.h"

SdCard tf;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[TFT_WIDTH * 10];

TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
  Serial.printf("%s\r\n", buf);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

#define LCD_BL_PWM_CHANNEL 0
#define LCD_BL_PIN 5

void setup()
{
  Serial.begin(115200); /* prepare for possible serial debug */

  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  ledcSetup(LCD_BL_PWM_CHANNEL, 5000, 8);
  ledcAttachPin(LCD_BL_PIN, LCD_BL_PWM_CHANNEL);

  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  tft.begin(); /* TFT init */
  tft.fillScreen(TFT_BLUE);
  tft.writecommand(ST7789_DISPON);
  tft.setRotation(4); /* Landscape orientation, flipped */

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, TFT_WIDTH * 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = TFT_WIDTH;
  disp_drv.ver_res = TFT_HEIGHT;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  tf.init();

  lv_obj_t *img1 = lv_img_create(lv_scr_act());
  lv_img_set_src(img1, "S:/bilibili/avatar.bin");
  lv_obj_align(img1, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_t *lbl1 = lv_label_create(lv_scr_act());
  lv_label_set_text(lbl1, "1");
  lv_obj_align(lbl1, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *img2 = lv_img_create(lv_scr_act());
  lv_img_set_src(img2, "S:/bilibili/avatar.png");
  lv_obj_align(img2, LV_ALIGN_TOP_LEFT, 50, 50);
  lv_obj_t *lbl2 = lv_label_create(lv_scr_act());
  lv_label_set_text(lbl2, "2");
  lv_obj_align(lbl2, LV_ALIGN_TOP_LEFT, 50, 50);

  lv_obj_t *img3 = lv_img_create(lv_scr_act());
  lv_img_set_src(img3, "S:/bilibili/avatar.jpg");
  lv_obj_align(img3, LV_ALIGN_TOP_LEFT, 100, 100);
  lv_obj_t *lbl3 = lv_label_create(lv_scr_act());
  lv_label_set_text(lbl3, "3");
  lv_obj_align(lbl3, LV_ALIGN_TOP_LEFT, 100, 100);

  lv_obj_t *img4 = lv_gif_create(lv_scr_act());
  lv_gif_set_src(img4, "S:/bilibili/avatar.gif");
  lv_obj_align(img4, LV_ALIGN_TOP_LEFT, 150, 150);
  lv_obj_t *lbl4 = lv_label_create(lv_scr_act());
  lv_label_set_text(lbl4, "4");
  lv_obj_align(lbl4, LV_ALIGN_TOP_LEFT, 150, 150);

  Serial.println("Setup done");
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}