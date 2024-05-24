#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define BUFFER_SIZE 512 

char buff[BUFFER_SIZE];

void init_uart(void);
char uart_read_char(void);
void send_back_buffer(void);

int main() {
    init_uart();

    int buffer_index = 0;
    while (true) {
        char c = uart_read_char();

        if (c == '\b') {  // Handle backspace
            if (buffer_index > 0) {
                buffer_index--;
                uart_putc_raw(UART_ID, '\b');
            }
            continue;
        }

        if (c == '\r') {  // Handle enter
            buff[buffer_index] = '\0';
            uart_putc_raw(UART_ID, '\n');
            uart_putc_raw(UART_ID, '\r');
            send_back_buffer();
            uart_putc_raw(UART_ID, '\n');
            uart_putc_raw(UART_ID, '\r');
            buffer_index = 0;
            continue;
        }

        if (buffer_index < BUFFER_SIZE - 1) {  // Prevent buffer overflow
            buff[buffer_index++] = c;
            uart_putc_raw(UART_ID, c);
        }
    }
}

void init_uart(void) {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

char uart_read_char(void) {
    while (!uart_is_readable(UART_ID)) {
        tight_loop_contents();  // Optional: Put CPU in low-power mode
    }
    return uart_getc(UART_ID);
}

void send_back_buffer(void) {
    int len = 0;
    while (buff[len] != '\0' && len < BUFFER_SIZE) {
        uart_putc_raw(UART_ID, buff[len]);
        len++;
    }
}
