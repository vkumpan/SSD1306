#include "ssd1306_core.h"
#include "ssd1306_font5x8.h"
#include <stdint.h>
#include <stdbool.h>

static uint8_t height = SSD1306_HEIGHT_64;
static uint8_t pages = SSD1306_HEIGHT_64 / SSD1306_PAGE_HEIGHT;

const font_t Font5x8 = { 5, 8, 1, 32, 126, (const uint8_t *)font5x8 };

static uint8_t cur_scale = 1;
static const font_t *cur_font = &Font5x8;

static uint16_t cmd_buf_caret;
static uint8_t cmd_buf[31];
static uint16_t data_buf_caret;
static uint16_t update_data_buf_caret;
static uint8_t *data_buf = 0;

static uint8_t data_buf_16[SSD1306_WIDTH * SSD1306_HEIGHT_16 / SSD1306_PAGE_HEIGHT];
static uint8_t data_buf_32[SSD1306_WIDTH * SSD1306_HEIGHT_32 / SSD1306_PAGE_HEIGHT];
static uint8_t data_buf_64[SSD1306_WIDTH * SSD1306_HEIGHT_64 / SSD1306_PAGE_HEIGHT];

static void add_command(uint8_t cmd) {
  cmd_buf[cmd_buf_caret] = cmd;
  cmd_buf_caret += 1;
}

static void add_data(uint8_t *data, uint8_t len) {
  memcpy(&data_buf[data_buf_caret], data, len);
  data_buf_caret += len;
  if (data_buf_caret > update_data_buf_caret) {
    update_data_buf_caret = data_buf_caret;
  }
}

// 1. Fundamental Command Table

// (RESET = 50%)
static void set_contrast_control(uint8_t percent) {
  if (percent > 100) {
    percent = 100;
  }
  add_command(0x81);
  add_command(percent * 255 / 100);
}

// (RESET = false)
static void set_entire_display_on(bool on) {
  add_command(on ? 0xA5 : 0xA4);
}

// (RESET = false)
static void set_normal_inverse_display(bool inverse) {
  add_command(inverse ? 0xA7 : 0xA6);
}

// (RESET = false)
static void set_display_on_off(bool on) {
  add_command(on ? 0xAF : 0xAE);
}

// 2. Scrolling Command Table

//

// 3. Addressing Setting Command Table

// (0d after RESET)
// Note: This command is only for page addressing mode
static void set_column_start_address(uint8_t column) {
  if (column > SSD1306_WIDTH - 1) {
    column = SSD1306_WIDTH - 1;
  }
  uint8_t low = column & 0x0F;
  uint8_t high = column >> 4;
  add_command(0x00 | low);
  add_command(0x10 | high);
}

// (RESET = SSD1306_MEMORY_ADDRESSING_MODE_PAGE)
static void set_memory_addressing_mode(ssd1306_mam_t mode) {
  add_command(0x20);
  add_command(mode);
}

// start: 0-127d, (RESET = 0d)
// end: 0-127d, (RESET = 127d)
// Note: This command is only for horizontal or vertical addressing mode.
static void set_column_address(uint8_t start, uint8_t end) {
  if (end > SSD1306_WIDTH - 1) {
    end = SSD1306_WIDTH - 1;
  }
  if (start > end) {
    start = end;
  }
  add_command(0x21);
  add_command(start);
  add_command(end);
}

// start: 0-7d, (RESET = 0d)
// end: 0-7d, (RESET = 7d)
// Note: This command is only for horizontal or vertical addressing mode.
static void set_page_address(uint8_t start, uint8_t end) {
  if (end > pages - 1) {
    end = pages - 1;
  }
  if (start > end) {
    start = end;
  }
  add_command(0x22);
  add_command(start);
  add_command(end);
}

// (0d after RESET)
// Note: This command is only for page addressing mode
static void set_page_start_address(uint8_t page) {
  if (page > pages - 1) {
    page = pages - 1;
  }
  add_command(0xB0 | page);
}

// 4. Hardware Configuration (Panel resolution & layout related) Command Table

// (0d during RESET)
static void set_display_start_line(uint8_t line) {
  if (line > height - 1) {
    line = height - 1;
  }
  add_command(0x40 | line);
}

// (RESET = false)
static void set_segment_remap(bool remap) {
  add_command(remap ? 0xA1 : 0xA0);
}

// (RESET = SSD1306_MULTIPLEX_RATIO_64)
static void set_multiplex_ratio(ssd1306_mux_t ratio) {
  add_command(0xA8);
  add_command(ratio);
}

// (RESET = false: normal mode)
static void set_com_scan_direction(bool remapped) {
  add_command(remapped ? 0xC8 : 0xC0);
}

// (RESET = 0d)
static void set_display_offset(uint8_t offset) {
  if (offset > height - 1) {
    offset = height - 1;
  }
  add_command(0xD3);
  add_command(offset);
}

// (RESET = SSD1306_COM_PINS_ALTERNATIVE_NO_REMAP)
static void set_com_pins(ssd1306_com_pins_t com_pins) {
  add_command(0xDA);
  add_command(com_pins);
}

// 5. Timing & Driving Scheme Setting Command Table

// (RESET: clock = 0d, osc_freq = 8)
static void set_display_clock(uint8_t clock, uint8_t osc_freq) {
  if (clock > 15) {
    clock = 15;
  }
  if (osc_freq > 15) {
    osc_freq = 15;
  }
  add_command(0xD5);
  add_command((osc_freq << 4) | clock);
}

// (RESET = 0x22)
static void set_precharge_period(uint8_t period) {
  add_command(0xD9);
  add_command(period);
}

// (RESET = SSD1306_VCOMH_077)
static void set_vcomh_deselect_level(ssd1306_vcomh_t level) {
  add_command(0xDB);
  add_command(level);
}

//Command for no operation
static void set_no_operation(void) {
  add_command(0xE3);
}

// Read Command Table
//
//

// Command Table for Charge Bump Setting

// (RESET = false)
static void set_charge_pump_setting(bool enable) {
  add_command(0x8D);
  add_command(enable ? 0x14 : 0x10);
}

static void scale_column(uint8_t col, uint8_t *out) {
  memset(out, 0, (cur_font->height * SSD1306_MAX_SCALE + 7) / 8);

  for (uint8_t i = 0; i < cur_font->height; i++) {
    uint8_t bit = (col >> i) & 0x01;
    if (!bit) {
      continue;
    }

    for (uint8_t j = 0; j < cur_scale; j++) {
      uint8_t dst_row = i * cur_scale + j;
      uint8_t byte_index = dst_row / 8;
      uint8_t bit_index = dst_row % 8;

      out[byte_index] |= (0x01 << bit_index);
    }
  }
}

static void ssd1306_set_char(uint8_t x, uint8_t y, char c) {
  if ((uint8_t)c < cur_font->first_char || (uint8_t)c > cur_font->last_char) {
    return;
  }
  if (x + cur_font->width > SSD1306_WIDTH || y + cur_font->height > height) {
    return;
  }

  uint16_t char_index = (uint8_t)c - cur_font->first_char;
  uint16_t base_offset = char_index * cur_font->width;

  const uint8_t *bitmap = &cur_font->data[base_offset];
  uint8_t start_page = y / SSD1306_PAGE_HEIGHT;
  uint8_t y_offset = y % SSD1306_PAGE_HEIGHT;

  for (uint8_t col = 0; col < cur_font->width; col++) {
    uint8_t col_data = pgm_read_byte(cur_font->data + base_offset + col);
    uint8_t scaled_bytes[(cur_font->height * SSD1306_MAX_SCALE + SSD1306_PAGE_HEIGHT - 1) / SSD1306_PAGE_HEIGHT];
    scale_column(col_data, scaled_bytes);

    for (uint8_t h = 0; h < cur_scale; h++) {
      for (uint8_t i = 0; i < cur_scale; i++) {
        uint8_t upper = scaled_bytes[i] << y_offset;
        uint8_t lower = scaled_bytes[i] >> (SSD1306_PAGE_HEIGHT - y_offset);

        uint16_t buf_index = (start_page + i) * SSD1306_WIDTH + (x + col * cur_scale + h);
        data_buf[buf_index] |= upper;
        if (buf_index + 1 > update_data_buf_caret) {
          update_data_buf_caret = buf_index + 1;
        }
        if (lower) {
          data_buf[buf_index + SSD1306_WIDTH] |= lower;
          if (buf_index + SSD1306_WIDTH + 1 > update_data_buf_caret) {
            update_data_buf_caret = buf_index + SSD1306_WIDTH + 1;
          }
        }
      }
    }
  }
}

void ssd1306_setup(ssd1306_height_t ssd1306_height) {
  height = ssd1306_height;
  pages = height / SSD1306_PAGE_HEIGHT;

  switch (ssd1306_height) {
    case 16:
      data_buf = data_buf_16;
      break;
    case 32:
      data_buf = data_buf_32;
      break;
    default:
      data_buf = data_buf_64;
      break;
  }
}

void ssd1306_init(void) {
  cmd_buf_caret = 0;

  set_contrast_control(50);
  set_entire_display_on(false);
  set_normal_inverse_display(false);
  set_display_on_off(false);

  // set_column_start_address(0);
  // set_memory_addressing_mode(SSD1306_MEMORY_ADDRESSING_MODE_PAGE);
  // set_column_address(0, SSD1306_WIDTH - 1);
  // set_page_address(0, pages - 1);
  // set_page_start_address(0);

  set_display_start_line(0);
  set_segment_remap(false);
  ssd1306_mux_t mux;
  ssd1306_com_pins_t com_pins;
  if (height == 16) {
    mux = SSD1306_MULTIPLEX_RATIO_16;
    com_pins = SSD1306_COM_PINS_SEQUENTIAL_NO_REMAP;
  } else if (height == 32) {
    mux = SSD1306_MULTIPLEX_RATIO_32;
    com_pins = SSD1306_COM_PINS_SEQUENTIAL_NO_REMAP;
  } else {
    mux = SSD1306_MULTIPLEX_RATIO_64;
    com_pins = SSD1306_COM_PINS_ALTERNATIVE_NO_REMAP;
  }
  set_multiplex_ratio(mux);
  set_com_scan_direction(false);
  set_display_offset(0);
  set_com_pins(com_pins);

  set_display_clock(0, 8);
  set_precharge_period(0x22);
  set_vcomh_deselect_level(SSD1306_VCOMH_077);

  set_charge_pump_setting(true);

  set_display_on_off(true);

  ssd1306_i2c_write(0x00, cmd_buf, cmd_buf_caret);
}

void ssd1306_set_font(const font_t *font) {
  cur_font = font;
}

void ssd1306_set_scale(uint8_t scale) {
  if (scale > 8) {
    scale = 8;
  }
  cur_scale = scale;
}

void ssd1306_set_caret(uint8_t x, uint8_t y) {
  if (x > SSD1306_WIDTH - 1) {
    x = SSD1306_WIDTH - 1;
  }
  if (y > height - 1) {
    y = height - 1;
  }
  data_buf_caret = x + y * SSD1306_WIDTH;
}

void ssd1306_clear(void) {
  memset(&data_buf[0], 0, SSD1306_WIDTH * pages);
  update_data_buf_caret = SSD1306_WIDTH * pages;
  ssd1306_set_caret(0, 0);
}

void ssd1306_set_text(const char *str) {
  uint8_t y = data_buf_caret / SSD1306_WIDTH;
  uint8_t x = data_buf_caret % SSD1306_WIDTH;

  while (*str) {
    ssd1306_set_char(x, y, *str++);
    x += cur_font->width * cur_scale + cur_font->space;
    if ((x + cur_font->width * cur_scale) > SSD1306_WIDTH) {
      x = 0;
      y += SSD1306_PAGE_HEIGHT * cur_scale;
    }
  }
  ssd1306_set_caret(x, y);
}

void ssd1306_update(void) {
  cmd_buf_caret = 0;
  set_memory_addressing_mode(SSD1306_MEMORY_ADDRESSING_MODE_HORIZONTAL);
  set_column_address(0, SSD1306_WIDTH - 1);
  set_page_address(0, height - 1);
  ssd1306_i2c_write(0x00, cmd_buf, cmd_buf_caret);

  ssd1306_i2c_write(0x40, data_buf, update_data_buf_caret);
  update_data_buf_caret = 0;
}
