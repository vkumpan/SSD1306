#include <SSD1306.h>
#include <Wire.h>

SSD1306 display;
// use this for 128x32 displays
// SSD1306 display(SSD1306_HEIGHT_32);

void setup() {
  Serial.begin(9600);
  Wire.begin();

  display.init();
  display.clear();

  display.setText(F("!\"#$%&'()*+,-./"));
  display.setCaret(15, 10);
  display.setScale(2);
  display.setText(F("0123456789"));
  display.setCaret(40, 28);
  display.setScale(1);
  display.setText(F(":;<=>?@"));
  display.setCaret(0, 40);
  display.setText(F("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
  display.setText(F("[\]^_`"));
  display.setText(F("abcdefghijklmnopqrstuvwxyz"));
  display.setText(F("{|}~"));

  display.update();
}

void loop() {
}
