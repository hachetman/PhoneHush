#ifndef __GC9A01A_H
#define __GC9A01A_H

#include <cstdint>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"

class GC9A01A  {
public:
auto init(void) -> uint;
auto set_frame(struct GC9A01_frame frame) -> void;
auto write(uint8_t *data, std::size_t len) -> void;
auto write_continue(uint8_t *data, std::size_t len) -> void;
auto write_pixel(uint8_t *color, uint8_t x, uint8_t y) -> void;
auto write_pixel(uint32_t color, uint8_t x, uint8_t y) -> void;
auto update_display() -> void;
auto test() -> void;
auto fill_display(const uint32_t *buffer) -> void;
auto fill_rect(const uint32_t *buffer, uint8_t x_start, uint8_t y_start,
                   uint8_t x_size, uint8_t y_size) -> void;

private:
auto set_reset(uint8_t val) -> void;
auto set_data_command(uint8_t val) -> void;
auto set_chip_select(uint8_t val) -> void;
auto delay(uint16_t ms) -> void;
auto spi_tx(uint8_t *data, std::size_t len) -> void;
auto write_command(uint8_t cmd) -> void;
auto write_data(uint8_t *data, std::size_t len) -> void;
auto write_byte(uint8_t val) -> void;
const uint dma_tx = dma_claim_unused_channel(true);

spi_inst_t *SPI_PORT = spi1;
static const uint8_t HEIGHT = 240;
static const uint8_t WIDTH = 240;
const uint8_t RESET_PIN = 13;
const uint8_t DC_PIN = 8;
const uint8_t CS_PIN = 9;
const uint8_t SPI_RX_PIN = 12;
const uint8_t SPI_TX_PIN = 11;
const uint8_t SPI_SCK_PIN = 10;
const uint8_t BL_PIN = 25;
const uint8_t COL_ADDR_SET = 0x2A;
const uint8_t ROW_ADDR_SET = 0x2B;
const uint8_t MEM_WR = 0x2C;
const uint8_t COLOR_MODE = 0x3A;
const uint8_t COLOR_MODE__12_BIT = 0x03;
const uint8_t COLOR_MODE__16_BIT = 0x05;
const uint8_t COLOR_MODE__18_BIT = 0x06;
const uint8_t MEM_WR_CONT = 0x3C;
const uint8_t ORIENTATION = 2;
uint8_t buffer[HEIGHT*WIDTH*3];
};

struct GC9A01_point {
    uint16_t X, Y;
};

struct GC9A01_frame {
    struct GC9A01_point start, end;
};

#endif
