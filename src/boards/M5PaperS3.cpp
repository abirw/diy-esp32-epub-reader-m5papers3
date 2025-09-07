#include "M5PaperS3.h"
#include <Renderer/M5PaperS3Renderer.h>
#include <regular_font.h>
#include <bold_font.h>
#include <italic_font.h>
#include <bold_italic_font.h>
#include <hourglass.h>
#include <SPIFFS.h>
#include "controls/GPIOButtonControls.h"

#define M5EPD_MAIN_PWR_PIN GPIO_NUM_2
//setup the pins to use for navigation
#define BUTTON_UP_GPIO_NUM GPIO_NUM_37
#define BUTTON_DOWN_GPIO_NUM GPIO_NUM_39
#define BUTTON_SELECT_GPIO_NUM GPIO_NUM_38
// buttons are low when pressed
#define BUTONS_ACTIVE_LEVEL 0

void M5PaperS3::power_up()
{
  // nothing to do for this board - should probably move the power up from
  // the driver to here?
}
void M5PaperS3::prepare_to_sleep()
{
  // keep the power on as we are going into deep sleep not shutting down completely
  rtc_gpio_init(M5EPD_MAIN_PWR_PIN);
  rtc_gpio_set_direction(M5EPD_MAIN_PWR_PIN, RTC_GPIO_MODE_OUTPUT_ONLY);
  rtc_gpio_set_level(M5EPD_MAIN_PWR_PIN, 1);
  rtc_gpio_hold_en(M5EPD_MAIN_PWR_PIN);
}
Renderer *M5PaperS3::get_renderer()
{
  return new M5PaperS3Renderer(
      &regular_font,
      &bold_font,
      &italic_font,
      &bold_italic_font,
      hourglass_data,
      hourglass_width,
      hourglass_height);
}
void M5PaperS3::stop_filesystem()
{
#ifdef USE_SPIFFS
  delete spiffs;
#else
  // seems to cause issues with the M5 Paper
  // delete sdcard;
#endif
}
ButtonControls *M5PaperS3::get_touch_controls(Renderer *renderer, xQueueHandle ui_queue)
{
  #ifdef USE_L58_TOUCH
    return new M5PaperS3TouchControls(
        renderer,
        CONFIG_TOUCH_INT,
        EPD_WIDTH,
        EPD_HEIGHT,
        3,
        [ui_queue](UIAction action)
        {
          xQueueSend(ui_queue, &action, 0);
        });
  #endif
    // dummy implementation
    return new TouchControls();
  }