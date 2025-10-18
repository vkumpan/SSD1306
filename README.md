# SSD1306 Arduino Library  

Lightweight Arduino library for controlling **SSD1306 OLED displays (e.g., 128×64, 128×32)** with a simple text-only interface.
Includes support for scaling text, setting precise cursor positions (even between pages), and printing from RAM or flash (PROGMEM).  

---

## Features  
- Supports SSD1306-based I²C OLED displays (commonly 128×64 or 128×32).
- Text rendering with built-in 5×8 font.
- Scalable text size — zoom 1× to 8×.
- Precise caret positioning, including positions between display pages (e.g., y = 10).
- Supports multiple text sources:
	- Regular RAM strings ("Hello")
	- Flash strings (F("Hello from PROGMEM"))
	- Numbers (int, float, bool)

---

## Installation  
1. Download or clone this repository into your Arduino `libraries` folder.  
2. Restart the Arduino IDE.  
3. Open **File → Examples → SSD1306** to try the included demos.  

---

## Usage  

### Basic Example  

```cpp
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
```

---

## License  

This project is licensed under the **MIT License**.  

```
MIT License

Copyright (c) 2025 Volodymyr Kumpan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
