#ifndef SSD1306_CORE_H
#define SSD1306_CORE_H

#include <stdint.h>
#include <stdbool.h>

#ifndef SSD1306_ADDR
#define SSD1306_ADDR 0x3C
#endif

#define SSD1306_WIDTH 128

#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT 64
#endif

#define SSD1306_PAGE_HEIGHT 8

#define SSD1306_PAGES (SSD1306_HEIGHT / SSD1306_PAGE_HEIGHT)

#define SSD1306_MAX_SCALE 8

typedef struct {
  const uint8_t width;
  const uint8_t height;
  const uint8_t space;
  const uint8_t first_char;
  const uint8_t last_char;
  const uint8_t *data;
} font_def;

typedef enum {
  SSD1306_MEMORY_ADDRESSING_MODE_HORIZONTAL,
  SSD1306_MEMORY_ADDRESSING_MODE_VERTICAL,
  SSD1306_MEMORY_ADDRESSING_MODE_PAGE
} ssd1306_mam_t;

typedef enum {
  SSD1306_MULTIPLEX_RATIO_16 = 0x0F,
  SSD1306_MULTIPLEX_RATIO_32 = 0x1F,
  SSD1306_MULTIPLEX_RATIO_64 = 0x3F
} ssd1306_mux_t;

typedef enum {
  SSD1306_COM_PINS_SEQUENTIAL_NO_REMAP = 0x02,
  SSD1306_COM_PINS_ALTERNATIVE_NO_REMAP = 0x12,
  SSD1306_COM_PINS_SEQUENTIAL_REMAP = 0x22,
  SSD1306_COM_PINS_ALTERNATIVE_REMAP = 0x32
} ssd1306_com_pins_t;

typedef enum {
  SSD1306_VCOMH_065 = 0x00,
  SSD1306_VCOMH_077 = 0x20,
  SSD1306_VCOMH_083 = 0x30
} ssd1306_vcomh_t;

extern const font_def Font5x8;

#ifdef __cplusplus
extern "C" {
#endif

  void ssd1306_i2c_write(uint8_t control, const uint8_t *data, uint16_t len);
  void ssd1306_delay(uint16_t ms);
  void ssd1306_print(const char *msg);

  void ssd1306_init(void);
  void ssd1306_set_font(const font_def *font);
  void ssd1306_set_scale(uint8_t scale);
  void ssd1306_clear(void);
  void ssd1306_set_caret(uint8_t x, uint8_t y);
  void ssd1306_set_text(const char *str);
  void ssd1306_update(void);

#ifdef __cplusplus
}
#endif

#endif
