#include "SSD1306.h"
#include "ssd1306_core.h"
#include "Arduino.h"
#include <Wire.h>
#include <stdint.h>

void ssd1306_delay(uint16_t ms) {
  delay(ms);
}

void ssd1306_print(const char *msg) {
  Serial.println(msg);
}

void ssd1306_i2c_write(uint8_t control, const uint8_t *data, uint16_t len) {
  uint16_t i = 0;
  while (i < len) {
    uint8_t chunkSize = min(len - i, 31);

    Wire.beginTransmission(SSD1306_ADDR);
    Wire.write(control);
    Wire.write(&data[i], chunkSize);
    uint8_t result = Wire.endTransmission();

    if (result != 0) {
      Serial.print(F("ssd1306_i2c_write error: "));
      Serial.println(result);
      return;
    }

    i += chunkSize;
  }
}

SSD1306::SSD1306() {
}

void SSD1306::init() {
  ssd1306_init();
}

void SSD1306::clear() {
  ssd1306_clear();
}

void SSD1306::setFont(const font_def *font) {
  ssd1306_set_font(font);
}

void SSD1306::setScale(uint8_t scale) {
  ssd1306_set_scale(scale);
}

void SSD1306::setCaret(uint8_t x, uint8_t y) {
  ssd1306_set_caret(x, y);
}

void SSD1306::setText(const __FlashStringHelper *fstr) {
  PGM_P p = (PGM_P)fstr;
  uint8_t chunkSize = 21;
  char tmp[chunkSize + 1];
  uint8_t len = 0;
  char c;
  while ((c = (char)pgm_read_byte(p++))) {
    tmp[len++] = c;
    if (len == chunkSize) {
      tmp[len] = '\0';
      ssd1306_set_text(tmp);
      len = 0;
    }
  }
  if (len > 0) {
    tmp[len] = '\0';
    ssd1306_set_text(tmp);
  }
}

void SSD1306::setText(const char *str) {
  ssd1306_set_text(str);
}

void SSD1306::setText(int val) {
  char buf[16];
  snprintf(buf, sizeof(buf), "%d", val);
  ssd1306_set_text(buf);
}

void SSD1306::setText(float val, uint8_t decimals = 2) {
  char buf[32];
  dtostrf(val, 0, decimals, buf);
  ssd1306_set_text(buf);
}

void SSD1306::setText(bool val) {
  ssd1306_set_text(val ? "true" : "false");
}

void SSD1306::update() {
  ssd1306_update();
}
