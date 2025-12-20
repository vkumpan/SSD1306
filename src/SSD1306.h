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
  void setCaret(uint8_t col, uint8_t row);
  void print(char c);
  void print(const __FlashStringHelper *fstr);
  void print(const char *str);
  void print(int val);
  void print(float val, uint8_t decimals = 2);
  void print(bool val);
  void update(void);

private:
};