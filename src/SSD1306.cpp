#include "SSD1306.h"
#include "ssd1306_core.h"
#include "Arduino.h"
#include <Wire.h>
#include <stdint.h>

void ssd1306_delay(uint16_t ms) {
  delay(ms);
}

void ssd1306_log(const char *msg) {
  Serial.println(msg);
}

void ssd1306_log_num(const uint16_t msg) {
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
  ssd1306_setup(SSD1306_HEIGHT_64);
}

SSD1306::SSD1306(ssd1306_height_t ssd1306_height) {
  ssd1306_setup(ssd1306_height);
}

void SSD1306::init() {
  ssd1306_init();
}

void SSD1306::clear() {
  ssd1306_clear();
}

void SSD1306::setFont(const font_t *font) {
  ssd1306_set_font(font);
}

void SSD1306::setScale(uint8_t scale) {
  ssd1306_set_scale(scale);
}

void SSD1306::setCaret(uint8_t col, uint8_t row) {
  ssd1306_set_caret(col, row);
}

void SSD1306::print(char c) {
  ssd1306_print_char(c);
}

void SSD1306::print(const __FlashStringHelper *fstr) {
  PGM_P p = (PGM_P)fstr;
  ssd1306_print(p);
}

void SSD1306::print(const char *str) {
  ssd1306_print(str);
}

void SSD1306::print(int val) {
  char buf[16];
  snprintf(buf, sizeof(buf), "%d", val);
  ssd1306_print(buf);
}

void SSD1306::print(float val, uint8_t decimals = 2) {
  char buf[32];
  dtostrf(val, 0, decimals, buf);
  ssd1306_print(buf);
}

void SSD1306::print(bool val) {
  ssd1306_print(val ? "true" : "false");
}

void SSD1306::update() {
  ssd1306_update();
}
