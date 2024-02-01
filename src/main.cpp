
#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <utility>

#include "hardware/timer.h"
#include "tusb.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "usb_descriptors.h"
#include "GC9A01A.h"
#include "framebuffer.h"
#include "CST816S.h"
#include "../images/free.h"
#include "../images/in_call.h"

void hid_task(void);
void cdc_task(void);
void serial_print(const char* str);
bool serialReady = false;

void core1_entry() {
    while (1) {
        tud_task();
    }
}

/*------------- MAIN -------------*/
int main(void)
{
    char buffer[50];
    CST816S_Touch touch_res;
    uint8_t gesture;
    set_sys_clock_khz(133000, true);
    tusb_init();
    stdio_init_all();
    multicore_launch_core1(core1_entry);
    serial_print("Initializing\r\n");
    GC9A01A display;
    display.init();
    framebuffer framebuffer(display);
    CST816S touch;
    struct GC9A01_frame frame = {{0,0},{239,239}};
    display.set_frame(frame);
    uint8_t color[3];
    touch.init(CST816S_Mode::CST816S_ALL_Mode);
    // Triangle
    color[0] = 0xFF;
    color[1] = 0xFF;
    color[2] = 0x00;

    framebuffer.fill_display(free_image);
    framebuffer.update_display();
    sleep_ms(1000);
    framebuffer.test();
    framebuffer.update_display();
    sleep_ms(1000);
    framebuffer.fill_rect(in_call_image, 40, 30, 80, 80);
    framebuffer.update_display();
    while (1)
    {
        sleep_ms(1000);
        gesture = touch.get_Gesture();
        snprintf(buffer, 20, "Gesture: %03d\r\n", gesture);
        serial_print(buffer);
        touch_res = touch.get_Point();
        snprintf(buffer, 25, "x: %03d; y: %03d, I: %1d\r\n", touch_res.x_point, touch_res.y_point, touch_res.interrupt);
        serial_print(buffer);
        serial_print("hello worlding\r\n");
    }
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    (void) instance;
    char buffer2[50];
    snprintf(buffer2, 49, "got a report, report_type: %d, report_id %d:\r\n", report_type, report_id);
    serial_print(buffer2);

}

//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+

// Invoked when cdc when line state changed e.g connected/disconnected
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
	(void)itf;
	(void)rts;

	serialReady = false;
	if (dtr) {
		serialReady = true;
		tud_cdc_write_str("Connected\n");
        tud_cdc_write_flush();
	}
}

void cdc_task(void)
{
    // connected() check for DTR bit
    // Most but not all terminal client set this when making connection
    // if ( tud_cdc_connected() )
    {
        // connected and there are data available
        if ( tud_cdc_available() )
        {
            // read data
            char buf[64];
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


void serial_print(const char* str) {
	if (serialReady) {
		tud_cdc_write_str(str);
        tud_cdc_write_flush();
	}
}
