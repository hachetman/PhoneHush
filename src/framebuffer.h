#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <cstdint>
#include "display.h"

class framebuffer{
public:
    framebuffer(display &display);
    virtual ~framebuffer() { };
    auto test() -> void;
    auto update_display() -> void;
    auto fill_display(const uint32_t *buffer) -> void;
    auto fill_rect(const uint32_t *buffer, uint8_t x_start, uint8_t y_start,
                   uint8_t x_size, uint8_t y_size) -> void;
private:
    display *display_buffer;

};
#endif // FRAMEBUFFER_H_
