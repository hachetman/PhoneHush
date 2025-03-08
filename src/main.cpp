#include <cstdint>
#include <cstdio>

#include "hardware/timer.h"
#include "pico/time.h"
#include "tusb.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "GC9A01A.h"
#include "CST816S.h"
#include "../images/free.h"
#include "../images/in_call.h"
#include "../images/mic_muted.h"
#include "../images/mic.h"

void hid_task();

void cdc_task();

void serial_print(const char *str);

bool serialReady = false;
bool mic_muted = false;
bool in_call = false;
GC9A01A display;

void core1_entry() {
    while (true) {
        tud_task();
    }
}

/*------------- MAIN -------------*/
int main() {
    bool touch_happened = false;
    CST816S_Touch touch_res;
    uint8_t gesture;
    set_sys_clock_khz(125000, true);

    tusb_init();
    stdio_init_all();
    multicore_launch_core1(core1_entry);
    serial_print("Initializing\r\n");
    clock_configure(clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS, 125 * MHZ, 125 * MHZ);
    uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    uint baudrate = display.init();
    CST816S touch;
    struct GC9A01_frame frame = {{0,   0},
                                 {239, 239}};
    display.set_frame(frame);
    touch.init(CST816S_Mode::CST816S_ALL_Mode);

    display.fill_display(free_image);
    display.update_display();
    while (true) {
        sleep_ms(100);
        gesture = touch.get_Gesture();
        touch_res = touch.get_Point();
        if (touch_res.interrupt == 0) {
            touch_happened = false;
        }
        if ((touch_res.y_point < 120) & touch_res.interrupt & (touch_happened == 0)) {
            uint8_t report = 3;
            tud_hid_report(0x20, &report, 1);
            report = 1;
            tud_hid_report(0x20, &report, 1);
            touch_happened = true;
        }
        if ((touch_res.y_point > 120) & touch_res.interrupt & (touch_happened == 0)) {
            uint8_t report = 0;
            tud_hid_report(0x20, &report, 1);
            touch_happened = true;
        }
    }
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer,
                               uint16_t reqlen) {
    // TODO not Implemented
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;
    char buffer2[80];
    snprintf(buffer2, 80, "got a report request, report_type: %d, report_id %d, len: %x buff %x:\r\n", report_type,
             report_id,
             reqlen, buffer[0]);
    serial_print(buffer2);
    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer,
                           uint16_t bufsize) {
    (void) instance;
    char buffer2[80];
    snprintf(buffer2, 80, "got a report, report_type: %d, report_id %d, buff %x:, bufsize: %x\r\n", report_type,
             report_id,
             buffer[0], bufsize);
    serial_print(buffer2);
    if ((report_id == 36) && (*buffer == 1)) {
        display.fill_rect(in_call_image, 0, 0, 240, 240);
        if (mic_muted) {
            display.fill_rect(mic_muted_image, 0, 120, 240, 120);
            display.update_display();
        } else {
            display.fill_rect(mic_image, 0, 120, 240, 120);
            display.update_display();
        }
        uint8_t report = 0;
        tud_hid_report(0x20, &report, 1);
    }
    if ((report_id == 36) && (*buffer == 0)) {
        display.fill_rect(free_image, 0, 0, 240, 240);
        display.update_display();
        uint8_t report = 0;
        tud_hid_report(0x20, &report, 1);

    }
    if (report_id == 35) {
        if (*buffer == 1) {
            display.fill_rect(mic_muted_image, 0, 120, 240, 120);
            display.update_display();
            mic_muted = true;
        } else {
            display.fill_rect(mic_image, 0, 120, 240, 120);
            display.update_display();
            mic_muted = false;
        }
    }

}

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+

// Invoked when cdc when line state changed e.g connected/disconnected
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
    (void) itf;
    (void) rts;

    serialReady = false;
    if (dtr) {
        serialReady = true;
        tud_cdc_write_str("Connected\n");
        tud_cdc_write_flush();
    }
}

void cdc_task() {
    // connected() check for DTR bit
    // Most but not all terminal client set this when making connection
    // if ( tud_cdc_connected() )
    {
        // connected and there are data available
        if (tud_cdc_available()) {
            // read data
            char buf[80];
            uint32_t count = tud_cdc_read(buf, sizeof(buf));
            (void) count;

            // Echo back
            // Note: Skip echo by commenting out write() and write_flush()
            // for throughput test e.g
            //    $ dd if=/dev/zero of=/dev/ttyACM0 count=10000
            tud_cdc_write(buf, count);
            tud_cdc_write_str("\r\n");
            tud_cdc_write_flush();
        }
    }
}


void serial_print(const char *str) {
    if (serialReady) {
        tud_cdc_write_str(str);
        tud_cdc_write_flush();
    }
}
