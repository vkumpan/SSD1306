#ifndef SSD1306_CORE_H
#define SSD1306_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>

#define SSD1306_ADDR 0x3C

#define SSD1306_WIDTH 128

#define SSD1306_PAGE_HEIGHT 8

#define SSD1306_MAX_SCALE 8

typedef struct {
  const uint8_t width;
  const uint8_t height;
  const uint8_t space;
  const uint8_t first_char;
  const uint8_t last_char;
  const uint8_t *data;
} font_t;

typedef enum {
  SSD1306_HEIGHT_16 = 16,
  SSD1306_HEIGHT_32 = 32,
  SSD1306_HEIGHT_64 = 64
} ssd1306_height_t;

typedef enum {
  SSD1306_MEMORY_ADDRESSING_MODE_HORIZONTAL = 0x00,
  SSD1306_MEMORY_ADDRESSING_MODE_VERTICAL = 0x01,
  SSD1306_MEMORY_ADDRESSING_MODE_PAGE = 0x02
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

extern const font_t Font5x8;

#ifdef __cplusplus
extern "C" {
#endif

  void ssd1306_i2c_write(uint8_t control, const uint8_t *data, uint16_t len);
  void ssd1306_delay(uint16_t ms);
  void ssd1306_log(const char *msg);
  void ssd1306_log_num(const uint16_t msg);

  void ssd1306_setup(ssd1306_height_t ssd1306_height);
  void ssd1306_init(void);
  void ssd1306_set_font(const font_t *font);
  void ssd1306_set_scale(uint8_t scale);
  void ssd1306_clear(void);
  void ssd1306_set_caret(uint8_t col, uint8_t row);
  void ssd1306_print_char(char c);
  void ssd1306_print(PGM_P p);
  void ssd1306_update(void);

#ifdef __cplusplus
}
#endif

#endif
