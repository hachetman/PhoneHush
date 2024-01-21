/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "pico/unique_id.h"
#include "tusb.h"
#include "usb_descriptors.h"

/* A combination of interfaces must have a unique product id, since PC will save device driver after the first plug.
 * Same VID/PID with different interface e.g MSC (first), then CDC (later) will possibly cause system error on PC.
 *
 * Auto ProductID layout's Bitmap:
 *   [MSB]         HID | MSC | CDC          [LSB]
 */
#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )
#define USB_PID           (0x4000 | _PID_MAP(CDC, 0) | _PID_MAP(MSC, 1) | _PID_MAP(HID, 2) | \
                           _PID_MAP(MIDI, 3) | _PID_MAP(VENDOR, 4) )

#define USB_VID   0xCafe
#define USB_BCD   0x0200

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device =
{
  .bLength            = sizeof(tusb_desc_device_t),
  .bDescriptorType    = TUSB_DESC_DEVICE,
  .bcdUSB             = USB_BCD,
  .bDeviceClass       = 0x00,
  .bDeviceSubClass    = 0x00,
  .bDeviceProtocol    = 0x00,
  .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

  .idVendor           = USB_VID,
  .idProduct          = USB_PID,
  .bcdDevice          = 0x0100,

  .iManufacturer      = 0x01,
  .iProduct           = 0x02,
  .iSerialNumber      = 0x03,

  .bNumConfigurations = 0x01
};

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const * tud_descriptor_device_cb(void)
{
  return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// HID Report Descriptor
//--------------------------------------------------------------------+

uint8_t const desc_hid_report[] =
{
  TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(REPORT_ID_KEYBOARD         )),
  TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(REPORT_ID_MOUSE            )),
  TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL )),
  TUD_HID_REPORT_DESC_GAMEPAD ( HID_REPORT_ID(REPORT_ID_GAMEPAD          )),
  0x06, 0xA0, 0xFF,  // Usage Page (Vendor Defined 0xFFA0)
  0x09, 0x03,        // Usage (0x03)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x01,        //   Report ID (1)
  0x09, 0x09,        //   Usage (0x09)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x01,        //   Report Count (1)
  0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x95, 0x07,        //   Report Count (7)
  0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x06,        //   Report ID (6)
  0x09, 0xA1,        //   Usage (0xA1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x01,        //   Report Count (1)
  0x91, 0x22,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0xA1,        //   Usage (0xA1)
  0x75, 0x01,        //   Report Size (1)
  0x81, 0x22,        //   Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
  0x09, 0xB7,        //   Usage (0xB7)
  0x09, 0xB5,        //   Usage (0xB5)
  0x09, 0xB3,        //   Usage (0xB3)
  0x09, 0xB1,        //   Usage (0xB1)
  0x09, 0xB2,        //   Usage (0xB2)
  0x95, 0x05,        //   Report Count (5)
  0x81, 0x06,        //   Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
  0x75, 0x02,        //   Report Size (2)
  0x95, 0x01,        //   Report Count (1)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x09, 0xA1,        //   Usage (0xA1)
  0x09, 0xB5,        //   Usage (0xB5)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x02,        //   Report Count (2)
  0xB1, 0xA2,        //   Feature (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
  0x75, 0x06,        //   Report Size (6)
  0x95, 0x01,        //   Report Count (1)
  0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x09,        //   Report ID (9)
  0x09, 0x9E,        //   Usage (0x9E)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x22,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x0A,        //   Report ID (10)
  0x09, 0x8D,        //   Usage (0x8D)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x22,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x0B,        //   Report ID (11)
  0x09, 0x8F,        //   Usage (0x8F)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x22,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x0C,        //   Report ID (12)
  0x09, 0xDC,        //   Usage (0xDC)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x22,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x0D,        //   Report ID (13)
  0x09, 0x9C,        //   Usage (0x9C)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x01,        //   Report Count (1)
  0x81, 0x06,        //   Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
  0x75, 0x07,        //   Report Size (7)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x10,        //   Report ID (16)
  0x09, 0x30,        //   Usage (0x30)
  0x15, 0x00,        //   Logical Minimum (0)
  0x26, 0xFF, 0x00,  //   Logical Maximum (255)
  0x75, 0x08,        //   Report Size (8)
  0x95, 0x3F,        //   Report Count (63)
  0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0x30,        //   Usage (0x30)
  0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0xC0,              // End Collection
  0x06, 0xA2, 0xFF,  // Usage Page (Vendor Defined 0xFFA2)
  0x09, 0x03,        // Usage (0x03)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x02,        //   Report ID (2)
  0x09, 0x09,        //   Usage (0x09)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x01,        //   Report Count (1)
  0x91, 0x06,        //   Output (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x95, 0x07,        //   Report Count (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0xCE,        //   Usage (0xCE)
  0x26, 0xFF, 0x00,  //   Logical Maximum (255)
  0x75, 0x08,        //   Report Size (8)
  0x95, 0x01,        //   Report Count (1)
  0xB1, 0x23,        //   Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x85, 0x05,        //   Report ID (5)
  0x09, 0x7D,        //   Usage (0x7D)
  0x75, 0x10,        //   Report Size (16)
  0xB1, 0x23,        //   Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x85, 0x07,        //   Report ID (7)
  0x09, 0xBE,        //   Usage (0xBE)
  0x75, 0x08,        //   Report Size (8)
  0x95, 0x3F,        //   Report Count (63)
  0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0xBE,        //   Usage (0xBE)
  0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x08,        //   Report ID (8)
  0x09, 0x99,        //   Usage (0x99)
  0x95, 0x01,        //   Report Count (1)
  0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0x99,        //   Usage (0x99)
  0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x12,        //   Report ID (18)
  0x09, 0x30,        //   Usage (0x30)
  0x75, 0x08,        //   Report Size (8)
  0x95, 0x3F,        //   Report Count (63)
  0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0x30,        //   Usage (0x30)
  0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x13,        //   Report ID (19)
  0x09, 0x8F,        //   Usage (0x8F)
  0x95, 0x01,        //   Report Count (1)
  0xB1, 0xA3,        //   Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
  0x09, 0x8F,        //   Usage (0x8F)
  0x15, 0xFE,        //   Logical Minimum (-2)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x02,        //   Report Size (2)
  0x91, 0x46,        //   Output (Data,Var,Rel,No Wrap,Linear,Preferred State,Null State,Non-volatile)
  0x75, 0x06,        //   Report Size (6)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0x8F,        //   Usage (0x8F)
  0x15, 0x00,        //   Logical Minimum (0)
  0x75, 0x01,        //   Report Size (1)
  0x81, 0x2E,        //   Input (Data,Var,Rel,Wrap,Linear,No Preferred State,No Null Position)
  0x75, 0x07,        //   Report Size (7)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x14,        //   Report ID (20)
  0x09, 0xD2,        //   Usage (0xD2)
  0x15, 0x01,        //   Logical Minimum (1)
  0x75, 0x02,        //   Report Size (2)
  0x25, 0x02,        //   Logical Maximum (2)
  0x95, 0x01,        //   Report Count (1)
  0x91, 0x62,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,Null State,Non-volatile)
  0x75, 0x06,        //   Report Size (6)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0x17,        //   Usage (0x17)
  0x09, 0x09,        //   Usage (0x09)
  0x09, 0x18,        //   Usage (0x18)
  0x09, 0x1E,        //   Usage (0x1E)
  0x09, 0x20,        //   Usage (0x20)
  0x09, 0x3A,        //   Usage (0x3A)
  0x09, 0x37,        //   Usage (0x37)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x07,        //   Report Count (7)
  0xB1, 0xA3,        //   Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
  0x09, 0x3F,        //   Usage (0x3F)
  0x95, 0x01,        //   Report Count (1)
  0xB1, 0x23,        //   Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x09, 0x8A,        //   Usage (0x8A)
  0x75, 0x01,        //   Report Size (1)
  0x81, 0x2E,        //   Input (Data,Var,Rel,Wrap,Linear,No Preferred State,No Null Position)
  0x75, 0x07,        //   Report Size (7)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x15,        //   Report ID (21)
  0x09, 0xD3,        //   Usage (0xD3)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x62,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,Null State,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0x8C,        //   Usage (0x8C)
  0x16, 0x00, 0x80,  //   Logical Minimum (-32768)
  0x26, 0xFF, 0x7F,  //   Logical Maximum (32767)
  0x75, 0x10,        //   Report Size (16)
  0x81, 0x22,        //   Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
  0x85, 0x16,        //   Report ID (22)
  0x09, 0x22,        //   Usage (0x22)
  0x15, 0x01,        //   Logical Minimum (1)
  0x25, 0x02,        //   Logical Maximum (2)
  0x75, 0x02,        //   Report Size (2)
  0x91, 0x62,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,Null State,Non-volatile)
  0x75, 0x06,        //   Report Size (6)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0x22,        //   Usage (0x22)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x03,        //   Logical Maximum (3)
  0x75, 0x02,        //   Report Size (2)
  0xB1, 0xA3,        //   Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
  0x75, 0x06,        //   Report Size (6)
  0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0x22,        //   Usage (0x22)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x81, 0x2E,        //   Input (Data,Var,Rel,Wrap,Linear,No Preferred State,No Null Position)
  0x75, 0x07,        //   Report Size (7)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x17,        //   Report ID (23)
  0x09, 0xB5,        //   Usage (0xB5)
  0x15, 0x01,        //   Logical Minimum (1)
  0x25, 0x02,        //   Logical Maximum (2)
  0x75, 0x02,        //   Report Size (2)
  0x91, 0x62,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,Null State,Non-volatile)
  0x15, 0x00,        //   Logical Minimum (0)
  0x75, 0x06,        //   Report Size (6)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0xB5,        //   Usage (0xB5)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x81, 0x06,        //   Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
  0x75, 0x07,        //   Report Size (7)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x09, 0xB5,        //   Usage (0xB5)
  0x25, 0x03,        //   Logical Maximum (3)
  0x75, 0x02,        //   Report Size (2)
  0xB1, 0xA3,        //   Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
  0x75, 0x06,        //   Report Size (6)
  0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x18,        //   Report ID (24)
  0x09, 0xD9,        //   Usage (0xD9)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x06,        //   Output (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0x9C,        //   Usage (0x9C)
  0x75, 0x01,        //   Report Size (1)
  0x81, 0x2E,        //   Input (Data,Var,Rel,Wrap,Linear,No Preferred State,No Null Position)
  0x75, 0x07,        //   Report Size (7)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x19,        //   Report ID (25)
  0x09, 0xDD,        //   Usage (0xDD)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x06,        //   Output (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0xDD,        //   Usage (0xDD)
  0x75, 0x01,        //   Report Size (1)
  0x81, 0x06,        //   Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
  0x75, 0x07,        //   Report Size (7)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x1A,        //   Report ID (26)
  0x09, 0x9E,        //   Usage (0x9E)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x22,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x1B,        //   Report ID (27)
  0x09, 0xDC,        //   Usage (0xDC)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x22,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x1C,        //   Report ID (28)
  0x09, 0xB7,        //   Usage (0xB7)
  0x75, 0x01,        //   Report Size (1)
  0x81, 0x06,        //   Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
  0x75, 0x07,        //   Report Size (7)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x1D,        //   Report ID (29)
  0x09, 0xB1,        //   Usage (0xB1)
  0x09, 0xB2,        //   Usage (0xB2)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x02,        //   Report Count (2)
  0x81, 0x06,        //   Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
  0x75, 0x06,        //   Report Size (6)
  0x95, 0x01,        //   Report Count (1)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x1E,        //   Report ID (30)
  0x09, 0xB3,        //   Usage (0xB3)
  0x75, 0x01,        //   Report Size (1)
  0x81, 0x06,        //   Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
  0x75, 0x07,        //   Report Size (7)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x1F,        //   Report ID (31)
  0x09, 0xE2,        //   Usage (0xE2)
  0x15, 0x01,        //   Logical Minimum (1)
  0x25, 0x02,        //   Logical Maximum (2)
  0x75, 0x02,        //   Report Size (2)
  0x91, 0x62,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,Null State,Non-volatile)
  0x75, 0x06,        //   Report Size (6)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0xE2,        //   Usage (0xE2)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x03,        //   Logical Maximum (3)
  0x75, 0x02,        //   Report Size (2)
  0xB1, 0xA3,        //   Feature (Const,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Volatile)
  0x75, 0x06,        //   Report Size (6)
  0xB1, 0x01,        //   Feature (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x09, 0xE2,        //   Usage (0xE2)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x81, 0x2E,        //   Input (Data,Var,Rel,Wrap,Linear,No Preferred State,No Null Position)
  0x75, 0x07,        //   Report Size (7)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0xC0,              // End Collection
  0x05, 0x0C,        // Usage Page (Consumer)
  0x09, 0x01,        // Usage (Consumer Control)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0xFF,        //   Report ID (-1)
  0x09, 0xE9,        //   Usage (Volume Increment)
  0x09, 0xEA,        //   Usage (Volume Decrement)
  0x09, 0xB0,        //   Usage (Play)
  0x09, 0xB1,        //   Usage (Pause)
  0x09, 0xCD,        //   Usage (Play/Pause)
  0x09, 0xB6,        //   Usage (Scan Previous Track)
  0x09, 0xB5,        //   Usage (Scan Next Track)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x07,        //   Report Count (7)
  0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x95, 0x01,        //   Report Count (1)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0xC0,              // End Collection
  0x05, 0x0B,        // Usage Page (Telephony)
  0x09, 0x05,        // Usage (Headset)
  0xA1, 0x01,        // Collection (Application)
  0x85, 0x20,        //   Report ID (32)
  0x15, 0x00,        //   Logical Minimum (0)
  0x25, 0x01,        //   Logical Maximum (1)
  0x75, 0x01,        //   Report Size (1)
  0x95, 0x01,        //   Report Count (1)
  0x09, 0x20,        //   Usage (Hook Switch)
  0x81, 0x22,        //   Input (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position)
  0x09, 0x2F,        //   Usage (Phone Mute)
  0x81, 0x06,        //   Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
  0x09, 0x21,        //   Usage (Flash)
  0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x09, 0x24,        //   Usage (Redial)
  0x81, 0x06,        //   Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
  0x09, 0x07,        //   Usage (Programmable Button)
  0x05, 0x09,        //   Usage Page (Button)
  0x09, 0x01,        //   Usage (0x01)
  0x81, 0x06,        //   Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
  0x75, 0x03,        //   Report Size (3)
  0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
  0x85, 0x21,        //   Report ID (33)
  0x05, 0x08,        //   Usage Page (LEDs)
  0x09, 0x18,        //   Usage (Ring)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x22,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x22,        //   Report ID (34)
  0x09, 0x1E,        //   Usage (Speaker)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x22,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x23,        //   Report ID (35)
  0x09, 0x09,        //   Usage (Mute)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x22,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x24,        //   Report ID (36)
  0x09, 0x17,        //   Usage (Off-Hook)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x22,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0x85, 0x25,        //   Report ID (37)
  0x09, 0x20,        //   Usage (Hold)
  0x75, 0x01,        //   Report Size (1)
  0x91, 0x22,        //   Output (Data,Var,Abs,No Wrap,Linear,No Preferred State,No Null Position,Non-volatile)
  0x75, 0x07,        //   Report Size (7)
  0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
  0xC0,              // End Collection
};

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance)
{
  (void) instance;
  return desc_hid_report;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

enum
{
  ITF_NUM_HID,
	ITF_NUM_CDC,
	ITF_NUM_CDC_DATA,
  ITF_NUM_TOTAL
};

#define  CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN + TUD_CDC_DESC_LEN)

#define EPNUM_HID   0x84
#define EPNUM_CDC_NOTIF 0x81
#define EPNUM_CDC_OUT   0x02
#define EPNUM_CDC_IN    0x82
uint8_t const desc_configuration[] =
{
  // Config number, interface count, string index, total length, attribute, power in mA
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

  // Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
  TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 5),
	TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64)};


#if TUD_OPT_HIGH_SPEED
// Per USB specs: high speed capable device must report device_qualifier and other_speed_configuration

// other speed configuration
uint8_t desc_other_speed_config[CONFIG_TOTAL_LEN];

// device qualifier is mostly similar to device descriptor since we don't change configuration based on speed
tusb_desc_device_qualifier_t const desc_device_qualifier =
{
  .bLength            = sizeof(tusb_desc_device_qualifier_t),
  .bDescriptorType    = TUSB_DESC_DEVICE_QUALIFIER,
  .bcdUSB             = USB_BCD,

  .bDeviceClass       = 0x00,
  .bDeviceSubClass    = 0x00,
  .bDeviceProtocol    = 0x00,

  .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
  .bNumConfigurations = 0x01,
  .bReserved          = 0x00
};

// Invoked when received GET DEVICE QUALIFIER DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete.
// device_qualifier descriptor describes information about a high-speed capable device that would
// change if the device were operating at the other speed. If not highspeed capable stall this request.
uint8_t const* tud_descriptor_device_qualifier_cb(void)
{
  return (uint8_t const*) &desc_device_qualifier;
}

// Invoked when received GET OTHER SEED CONFIGURATION DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
// Configuration descriptor in the other speed e.g if high speed then this is for full speed and vice versa
uint8_t const* tud_descriptor_other_speed_configuration_cb(uint8_t index)
{
  (void) index; // for multiple configurations

  // other speed config is basically configuration with type = OHER_SPEED_CONFIG
  memcpy(desc_other_speed_config, desc_configuration, CONFIG_TOTAL_LEN);
  desc_other_speed_config[1] = TUSB_DESC_OTHER_SPEED_CONFIG;

  // this example use the same configuration for both high and full speed mode
  return desc_other_speed_config;
}

#endif // highspeed

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
  (void) index; // for multiple configurations

  // This example use the same configuration for both high and full speed mode
  return desc_configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

// buffer to hold flash ID
char serial[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];

// array of pointer to string descriptors
char const* string_desc_arr [] =
{
  (const char[]) { 0x09, 0x04 }, // 0: is supported language is English (0x0409)
  "TinyUSB",                     // 1: Manufacturer
  "PhoneHush",                   // 2: Product
  serial,                        // 3: Serials, uses the flash ID
};

static uint16_t _desc_str[32];

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
  (void) langid;

  uint8_t chr_count;

  if ( index == 0)
  {
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
  }else
  {
    // Note: the 0xEE index string is a Microsoft OS 1.0 Descriptors.
    // https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors

    if (index == 3) pico_get_unique_board_id_string(serial, sizeof(serial));
    
    if ( !(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) ) return NULL;

    const char* str = string_desc_arr[index];

    // Cap at max char
    chr_count = strlen(str);
    if ( chr_count > 31 ) chr_count = 31;

    // Convert ASCII string into UTF-16
    for(uint8_t i=0; i<chr_count; i++)
    {
      _desc_str[1+i] = str[i];
    }
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2*chr_count + 2);

  return _desc_str;
}
