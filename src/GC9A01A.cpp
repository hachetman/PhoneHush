#include "GC9A01A.h"
#include "hardware/spi.h"
#include <type_traits>


#define ORIENTATION 2   // Set the display orientation 0,1,2,3

auto GC9A01A::write_command(uint8_t cmd) ->void
{
    set_data_command(0);
    set_chip_select(0);
    spi_tx(&cmd, sizeof(cmd));
    set_chip_select(1);
}

auto GC9A01A::write_data(uint8_t *data, std::size_t len) -> void
{
    set_data_command(1);
    set_chip_select(0);
    spi_tx(data, len);
    set_chip_select(1);
}

auto GC9A01A::write_byte(uint8_t val) -> void
{
    write_data(&val, sizeof(val));
}

auto GC9A01A::init(void) -> void
{
    spi_init(SPI_PORT, 24 * 1000 * 1000);
    gpio_set_function(SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_TX_PIN, GPIO_FUNC_SPI);

    gpio_init(RESET_PIN);
    gpio_set_dir(RESET_PIN, GPIO_OUT);

    gpio_init(DC_PIN);
    gpio_set_dir(DC_PIN, GPIO_OUT);

    gpio_init(CS_PIN);
    gpio_set_dir(CS_PIN, GPIO_OUT);

    gpio_init(BL_PIN);
    gpio_set_dir(BL_PIN, GPIO_OUT);
    gpio_put(BL_PIN, true);
    set_chip_select(1);
    delay(5);
    set_reset(0);
    delay(10);
    set_reset(1);
    delay(120);

    /* Initial Sequence */

    write_command(0xEF);

    write_command(0xEB);
    write_byte(0x14);

    write_command(0xFE);
    write_command(0xEF);

    write_command(0xEB);
    write_byte(0x14);

    write_command(0x84);
    write_byte(0x40);

    write_command(0x85);
    write_byte(0xFF);

    write_command(0x86);
    write_byte(0xFF);

    write_command(0x87);
    write_byte(0xFF);

    write_command(0x88);
    write_byte(0x0A);

    write_command(0x89);
    write_byte(0x21);

    write_command(0x8A);
    write_byte(0x00);

    write_command(0x8B);
    write_byte(0x80);

    write_command(0x8C);
    write_byte(0x01);

    write_command(0x8D);
    write_byte(0x01);

    write_command(0x8E);
    write_byte(0xFF);

    write_command(0x8F);
    write_byte(0xFF);


    write_command(0xB6);
    write_byte(0x00);
    write_byte(0x00);

    write_command(0x36);

#if ORIENTATION == 0
    write_byte(0x18);
#elif ORIENTATION == 1
    write_byte(0x28);
#elif ORIENTATION == 2
    write_byte(0x48);
#else
    write_byte(0x88);
#endif

    write_command(COLOR_MODE);
    write_byte(COLOR_MODE__18_BIT);

    write_command(0x90);
    write_byte(0x08);
    write_byte(0x08);
    write_byte(0x08);
    write_byte(0x08);

    write_command(0xBD);
    write_byte(0x06);

    write_command(0xBC);
    write_byte(0x00);

    write_command(0xFF);
    write_byte(0x60);
    write_byte(0x01);
    write_byte(0x04);

    write_command(0xC3);
    write_byte(0x13);
    write_command(0xC4);
    write_byte(0x13);

    write_command(0xC9);
    write_byte(0x22);

    write_command(0xBE);
    write_byte(0x11);

    write_command(0xE1);
    write_byte(0x10);
    write_byte(0x0E);

    write_command(0xDF);
    write_byte(0x21);
    write_byte(0x0c);
    write_byte(0x02);

    write_command(0xF0);
    write_byte(0x45);
    write_byte(0x09);
    write_byte(0x08);
    write_byte(0x08);
    write_byte(0x26);
    write_byte(0x2A);

    write_command(0xF1);
    write_byte(0x43);
    write_byte(0x70);
    write_byte(0x72);
    write_byte(0x36);
    write_byte(0x37);
    write_byte(0x6F);

    write_command(0xF2);
    write_byte(0x45);
    write_byte(0x09);
    write_byte(0x08);
    write_byte(0x08);
    write_byte(0x26);
    write_byte(0x2A);

    write_command(0xF3);
    write_byte(0x43);
    write_byte(0x70);
    write_byte(0x72);
    write_byte(0x36);
    write_byte(0x37);
    write_byte(0x6F);

    write_command(0xED);
    write_byte(0x1B);
    write_byte(0x0B);

    write_command(0xAE);
    write_byte(0x77);

    write_command(0xCD);
    write_byte(0x63);

    write_command(0x70);
    write_byte(0x07);
    write_byte(0x07);
    write_byte(0x04);
    write_byte(0x0E);
    write_byte(0x0F);
    write_byte(0x09);
    write_byte(0x07);
    write_byte(0x08);
    write_byte(0x03);

    write_command(0xE8);
    write_byte(0x34);

    write_command(0x62);
    write_byte(0x18);
    write_byte(0x0D);
    write_byte(0x71);
    write_byte(0xED);
    write_byte(0x70);
    write_byte(0x70);
    write_byte(0x18);
    write_byte(0x0F);
    write_byte(0x71);
    write_byte(0xEF);
    write_byte(0x70);
    write_byte(0x70);

    write_command(0x63);
    write_byte(0x18);
    write_byte(0x11);
    write_byte(0x71);
    write_byte(0xF1);
    write_byte(0x70);
    write_byte(0x70);
    write_byte(0x18);
    write_byte(0x13);
    write_byte(0x71);
    write_byte(0xF3);
    write_byte(0x70);
    write_byte(0x70);

    write_command(0x64);
    write_byte(0x28);
    write_byte(0x29);
    write_byte(0xF1);
    write_byte(0x01);
    write_byte(0xF1);
    write_byte(0x00);
    write_byte(0x07);

    write_command(0x66);
    write_byte(0x3C);
    write_byte(0x00);
    write_byte(0xCD);
    write_byte(0x67);
    write_byte(0x45);
    write_byte(0x45);
    write_byte(0x10);
    write_byte(0x00);
    write_byte(0x00);
    write_byte(0x00);

    write_command(0x67);
    write_byte(0x00);
    write_byte(0x3C);
    write_byte(0x00);
    write_byte(0x00);
    write_byte(0x00);
    write_byte(0x01);
    write_byte(0x54);
    write_byte(0x10);
    write_byte(0x32);
    write_byte(0x98);

    write_command(0x74);
    write_byte(0x10);
    write_byte(0x85);
    write_byte(0x80);
    write_byte(0x00);
    write_byte(0x00);
    write_byte(0x4E);
    write_byte(0x00);

    write_command(0x98);
    write_byte(0x3e);
    write_byte(0x07);

    write_command(0x35);
    write_command(0x21);

    write_command(0x11);
    delay(120);
    write_command(0x29);
    delay(20);

}

auto GC9A01A::set_frame(struct GC9A01_frame frame) -> void
{

    uint8_t data[4];

    write_command(COL_ADDR_SET);
    data[0] = (frame.start.X >> 8) & 0xFF;
    data[1] = frame.start.X & 0xFF;
    data[2] = (frame.end.X >> 8) & 0xFF;
    data[3] = frame.end.X & 0xFF;
    write_data(data, sizeof(data));

    write_command(ROW_ADDR_SET);
    data[0] = (frame.start.Y >> 8) & 0xFF;
    data[1] = frame.start.Y & 0xFF;
    data[2] = (frame.end.Y >> 8) & 0xFF;
    data[3] = frame.end.Y & 0xFF;
    write_data(data, sizeof(data));

}

auto GC9A01A::write(uint8_t *data, std::size_t len) -> void
{
    write_command(MEM_WR);
    write_data(data, len);
}

auto GC9A01A::write_continue(uint8_t *data, std::size_t len) -> void
{
    write_command(MEM_WR_CONT);
    write_data(data, len);
}

auto GC9A01A::set_reset(uint8_t val) -> void
{
  gpio_put(RESET_PIN, val);

}

auto GC9A01A::set_data_command(uint8_t val) -> void
{
  gpio_put(DC_PIN, val);
}

auto GC9A01A::set_chip_select(uint8_t val) -> void
{
  gpio_put(CS_PIN, val);

}

auto GC9A01A::spi_tx(uint8_t *data, size_t len) -> void
{
  spi_write_blocking(SPI_PORT, data, len);
}

auto GC9A01A::delay(uint16_t ms) -> void
{
    sleep_ms(ms);

}
auto GC9A01A::write_pixel(uint8_t *color, uint8_t x, uint8_t y) -> void
{
    buffer[(y * WIDTH + x) * 3  + 0] = color[0];
    buffer[(y * WIDTH + x) * 3  + 1] = color[1];
    buffer[(y * WIDTH + x) * 3  + 2] = color[2];
}
auto GC9A01A::write_pixel(uint32_t color, uint8_t x, uint8_t y) -> void
{
    if ((color >> 24) & 0xff) {
        buffer[(y * WIDTH + x) * 3  + 0] = color & 0xff;
        buffer[(y * WIDTH + x) * 3  + 1] = (color >> 8) & 0xff;
        buffer[(y * WIDTH + x) * 3  + 2] = (color >> 16) & 0xff;
    }
}
auto GC9A01A::update_display() -> void
{
    write(buffer, HEIGHT*WIDTH*3);
}

auto GC9A01A::test() -> void
{
    uint8_t color[3];
    color[0] = 0xFF;
    color[1] = 0xFF;
    color[2] = 0x00;
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            write_pixel(color, x, y);
        }
    }
}

auto GC9A01A::fill_display(const uint32_t *buffer) -> void
{
    uint32_t counter = 0;
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            write_pixel(buffer[counter++], x, y);
        }
    }
}

auto GC9A01A::fill_rect(const uint32_t *buffer, uint8_t x_start,
                            uint8_t y_start, uint8_t x_size, uint8_t y_size)
    -> void
{
    uint32_t counter = 0;
    for (int x = x_start; x < x_start + x_size; x++) {
        for (int y = y_start; y < y_start + y_size; y++) {
            write_pixel(buffer[counter++], x, y);
        }
    }
}
