#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <cstdint>

class display {
public:
virtual auto init(void) -> void = 0;
virtual auto write_pixel(uint8_t *color, uint8_t x, uint8_t y) -> void = 0;
virtual auto write_pixel(uint32_t color, uint8_t x, uint8_t y) -> void = 0;
virtual auto update_display() -> void = 0;
uint8_t size_x;
uint8_t size_y;
};
#endif // DISPLAY_H_
