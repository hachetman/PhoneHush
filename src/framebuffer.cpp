#include "framebuffer.h"

framebuffer::framebuffer(display & display)
{
display_buffer = &display;
display_buffer->size_x = 240;
display_buffer->size_y = 240;
}

auto framebuffer::test() -> void
{
    uint8_t color[3];
    color[0] = 0xFF;
    color[1] = 0xFF;
    color[2] = 0x00;
    for (int x = 0; x < 240; x++) {
        for (int y = 0; y < 240; y++) {
            display_buffer->write_pixel(color, x, y);
        }
    }
}

auto framebuffer::update_display() -> void
{
    display_buffer->update_display();
}

auto framebuffer::fill_display(const uint32_t *buffer) -> void
{
    uint32_t counter = 0;
    for (int x = 0; x < display_buffer->size_x; x++) {
        for (int y = 0; y < display_buffer->size_y; y++) {
            display_buffer->write_pixel(buffer[counter++], x, y);
        }
    }
}

auto framebuffer::fill_rect(const uint32_t *buffer, uint8_t x_start,
                            uint8_t y_start, uint8_t x_size, uint8_t y_size)
    -> void
{
    uint32_t counter = 0;
    for (int x = x_start; x < x_start + x_size; x++) {
        for (int y = y_start; y < y_start + y_size; y++) {
            display_buffer->write_pixel(buffer[counter++], x, y);
        }
    }
}
