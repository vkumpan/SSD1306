#include "ssd1306_core.h"
#include <stdint.h>
#include <WString.h>

#pragma once

class SSD1306 {
public:
  SSD1306();
  SSD1306(ssd1306_height_t ssd1306_height);
  void init();
  void clear();
  void setFont(const font_t *font);
  void setScale(uint8_t scale);
  void setCaret(uint8_t x, uint8_t y);
  void setText(const __FlashStringHelper *fstr);
  void setText(const char *str);
  void setText(int val);
  void setText(float val, uint8_t decimals = 2);
  void setText(bool val);
  void update(void);

private:
};