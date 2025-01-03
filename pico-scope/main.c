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

/* This example demonstrates WebUSB as web serial with browser with WebUSB support (e.g Chrome).
 * After enumerated successfully, browser will pop-up notification
 * with URL to landing page, click on it to test
 *  - Click "Connect" and select device, When connected the on-board LED will litted up.
 *  - Any charters received from either webusb/Serial will be echo back to webusb and Serial
 *
 * Note:
 * - The WebUSB landing page notification is currently disabled in Chrome
 * on Windows due to Chromium issue 656702 (https://crbug.com/656702). You have to
 * go to landing page (below) to test
 *
 * - On Windows 7 and prior: You need to use Zadig tool to manually bind the
 * WebUSB interface with the WinUSB driver for Chrome to access. From windows 8 and 10, this
 * is done automatically by firmware.
 *
 * - On Linux/macOS, udev permission may need to be updated by
 *   - copying '/examples/device/99-tinyusb.rules' file to /etc/udev/rules.d/ then
 *   - run 'sudo udevadm control --reload-rules && sudo udevadm trigger'
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

// For ADC input:
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/irq.h"

#include "bsp/board_api.h"
#include "tusb.h"
#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED     = 1000,
  BLINK_SUSPENDED   = 2500,

  BLINK_ALWAYS_ON   = UINT32_MAX,
  BLINK_ALWAYS_OFF  = 0
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

#define URL  "example.tinyusb.org/webusb-serial/index.html"

const tusb_desc_webusb_url_t desc_url =
{
  .bLength         = 3 + sizeof(URL) - 1,
  .bDescriptorType = 3, // WEBUSB URL type
  .bScheme         = 1, // 0: http, 1: https
  .url             = URL
};

static bool web_serial_connected = false;

// Define buffer size and buffers
#define BUFFER_SIZE 1024
uint8_t buffer1[BUFFER_SIZE];
uint8_t buffer2[BUFFER_SIZE];

// DMA channel and configuration
uint dma_channel;
dma_channel_config dma_config;

// Pointer to the current buffer being filled
uint8_t* current_buffer;
uint8_t* next_buffer;

// Flag to indicate buffer state
volatile bool buffer_filled = false;

//------------- prototypes -------------//
void echo_all(uint8_t buf[], uint32_t count);
void led_blinking_task(void);
void cdc_task(void);
void webserial_task(void);

void dma_handler(void);
void setup_adc_dma(void);
void send_data_to_usb(void);

#define PIN 4

bool led_value = false;

/*------------- MAIN -------------*/
int main(void)
{

  // stdio_init_all();
  gpio_init(PIN);
  gpio_set_dir(PIN, GPIO_OUT);

  board_init();

  // init device stack on configured roothub port
  tud_init(BOARD_TUD_RHPORT);

  if (board_init_after_tusb) {
    board_init_after_tusb();
  }

  //Config & Enabled ADC, DMA.
  //Must be enabled after vendor connected..

  setup_adc_dma();

  while (1)
  {
    tud_task(); // tinyusb device task
    cdc_task();
    webserial_task();
    led_blinking_task();
  }
}
//--------------------------------------------------------------------+
// ADC & DMA Function
//--------------------------------------------------------------------+

void dma_handler(void){
  // Clear the interrupt
  dma_hw->ints0 = 1u << dma_channel;

  // Toggle the buffers
  if (current_buffer == buffer1) {
      current_buffer = buffer2;
      next_buffer = buffer1;
  } else {
      current_buffer = buffer1;
      next_buffer = buffer2;
  }

  buffer_filled = true;  // Mark the buffer as filled

  // Reconfigure DMA to use the next buffer
  dma_channel_configure(
      dma_channel,
      &dma_config,
      current_buffer,             // Destination buffer
      &adc_hw->fifo,           // Source: ADC FIFO
      BUFFER_SIZE,             // Number of transfers
      true                     // Start the transfer immediately
  );
}
void setup_adc_dma(void){
  // Initialize ADC
  adc_init();
  adc_gpio_init(26);  // Use GPIO 26 as ADC input
  adc_select_input(0); // Select ADC input channel 0

  // Configure ADC FIFO
  adc_fifo_setup(
      true,          // Enable FIFO
      true,          // Enable DMA data request (DREQ)
      4,             // Number of samples in FIFO before request
      false,         // Disable error bit
      true          // No shift results to 12-bit
  );

  // Enable ADC
  adc_run(true);

  // Initialize DMA
  dma_channel = dma_claim_unused_channel(true);
  dma_config = dma_channel_get_default_config(dma_channel);
  channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_8);  // 16-bit transfers
  channel_config_set_read_increment(&dma_config, false);            // No increment for ADC FIFO
  channel_config_set_write_increment(&dma_config, true);            // Increment destination buffer
  channel_config_set_dreq(&dma_config, DREQ_ADC);                   // Trigger on ADC FIFO

  // Set initial buffer pointers
  current_buffer = buffer1;
  next_buffer = buffer2;

  // Configure DMA to use buffer1 initially
  dma_channel_configure(
      dma_channel,
      &dma_config,
      current_buffer,          // Destination buffer
      &adc_hw->fifo,           // Source: ADC FIFO
      BUFFER_SIZE,             // Number of transfers
      true                     // Start the transfer immediately
  );

  // Set up DMA interrupt
  irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
  irq_set_enabled(DMA_IRQ_0, true);
  dma_channel_set_irq0_enabled(dma_channel, true);
}
void send_data_to_usb(void){
  if (buffer_filled) {
    // gpio_put(PIN, led_value);
    // led_value = !led_value;
    // Send current_buffer data via USB CDC
    // (Replace with your USB CDC transmission code)
    // echo_all((uint8_t *)next_buffer, BUFFER_SIZE * sizeof(uint16_t));
    if ( web_serial_connected )
    {
      tud_vendor_write((uint8_t *)next_buffer, BUFFER_SIZE * sizeof(uint8_t));
      tud_vendor_write_flush();
    }
    buffer_filled = false;  // Mark buffer as free for the next DMA transfer
  }
}

// send characters to both CDC and WebUSB
void echo_all(uint8_t buf[], uint32_t count)
{
  // TODO: always send raw data no conversation.
  // echo to web serial
  if ( web_serial_connected )
  {
    tud_vendor_write(buf, count);
    tud_vendor_write_flush();
  }

  // echo to cdc
  if ( tud_cdc_connected() )
  {
    for(uint32_t i=0; i<count; i++)
    {
      tud_cdc_write_char(buf[i]);

      if ( buf[i] == '\r' ) tud_cdc_write_char('\n');
    }
    tud_cdc_write_flush();
  }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}

//--------------------------------------------------------------------+
// WebUSB use vendor class
//--------------------------------------------------------------------+

// Invoked when a control transfer occurred on an interface of this class
// Driver response accordingly to the request and the transfer stage (setup/data/ack)
// return false to stall control endpoint (e.g unsupported request)
bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request)
{
  // nothing to with DATA & ACK stage
  if (stage != CONTROL_STAGE_SETUP) return true;

  switch (request->bmRequestType_bit.type)
  {
    case TUSB_REQ_TYPE_VENDOR:
      switch (request->bRequest)
      {
        case VENDOR_REQUEST_WEBUSB:
          // match vendor request in BOS descriptor
          // Get landing page url
          return tud_control_xfer(rhport, request, (void*)(uintptr_t) &desc_url, desc_url.bLength);

        case VENDOR_REQUEST_MICROSOFT:
          if ( request->wIndex == 7 )
          {
            // Get Microsoft OS 2.0 compatible descriptor
            uint16_t total_len;
            memcpy(&total_len, desc_ms_os_20+8, 2);

            return tud_control_xfer(rhport, request, (void*)(uintptr_t) desc_ms_os_20, total_len);
          }else
          {
            return false;
          }

        default: break;
      }
    break;

    case TUSB_REQ_TYPE_CLASS:
      if (request->bRequest == 0x22)
      {
        // Webserial simulate the CDC_REQUEST_SET_CONTROL_LINE_STATE (0x22) to connect and disconnect.
        web_serial_connected = (request->wValue != 0);

        // Always lit LED if connected
        if ( web_serial_connected )
        {
          board_led_write(true);
          blink_interval_ms = BLINK_ALWAYS_ON;

          tud_vendor_write_str("\r\nWebUSB interface connected\r\n");
          tud_vendor_write_flush();
        }else
        {
          blink_interval_ms = BLINK_MOUNTED;
        }

        // response with status OK
        return tud_control_status(rhport, request);
      }
    break;

    default: break;
  }

  // stall unknown request
  return false;
}

void webserial_task(void)
{
  if ( web_serial_connected )
  {
    send_data_to_usb();
    // if ( tud_vendor_available() )
    // {
    //   send_data_to_usb();
    //   // uint8_t buf[64];
    //   // uint32_t count = tud_vendor_read(buf, sizeof(buf));

    //   // // echo back to both web serial and cdc
    //   // echo_all(buf, count);
    // }
  }
}


//--------------------------------------------------------------------+
// USB CDC
//--------------------------------------------------------------------+
void cdc_task(void)
{
  if ( tud_cdc_connected() )
  {
    send_data_to_usb();
    // connected and there are data available
    // if ( tud_cdc_available() )
    // {
    //   send_data_to_usb();
    //   // uint8_t buf[64];

    //   // uint32_t count = tud_cdc_read(buf, sizeof(buf));

    //   // // echo back to both web serial and cdc
    //   // echo_all(buf, count);
    // }
  }
}

// Invoked when cdc when line state changed e.g connected/disconnected
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
  (void) itf;

  // connected
  if ( dtr && rts )
  {
    // print initial message when connected
    tud_cdc_write_str("\r\nTinyUSB WebUSB device example\r\n");
  }
}

// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf)
{
  (void) itf;
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if ( board_millis() - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}
