#include <stdio.h>
#include <string.h>
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

void printmem(void *, const int);

// Math function
int add(int, int);
int sub(int, int);

typedef struct _math_table_t{
    int (*add)(int, int);
    int (*sub)(int, int);
} math_table_t;

__attribute__((section(".math"))) math_table_t math_table;

uint16_t fibbonacci[] = {
    0xb570, // push	{r4, r5, r6, lr}
    0x0004, // movs	r4, r0
    0x2500, // movs	r5, #0
    0x2c00, // cmp	r4, #0
    0xd007, // beq.n	1a <fibbonacci+0x1a>
    0x2c01, // cmp	r4, #1
    0xd005, // beq.n	1a <fibbonacci+0x1a>
    0x1e60, // subs	r0, r4, #1
    0xf7ff, 0xfff6, // bl	0 <fibbonacci>
    0x3c02, // subs	r4, #2
    0x182d, // adds	r5, r5, r0
    0xe7f5, // b.n	6 <fibbonacci+0x6>
    0x1960, //adds	r0, r4, r5
    0xbd70, //pop	{r4, r5, r6, pc}
};
// uint8_t fibbonacci[] = {
//      0x40, 0x43, 0x70, 0x47,
// };

typedef int (*fib)(int);

int main() {
    init_uart();
    
    // printf("Hello, World!\n\r");

    math_table.add = add;
    math_table.sub = sub;
    
    volatile math_table_t * math = (volatile math_table_t *)0x2003D800;

    uart_putc_raw(UART_ID, math->add(5, 4) + 0x30 );
    uart_putc_raw(UART_ID, '\n' );
    uart_putc_raw(UART_ID, '\r' );
    uart_putc_raw(UART_ID, math->sub(5, 4) + 0x30 );
    uart_putc_raw(UART_ID, '\n' );
    uart_putc_raw(UART_ID, '\r' );

    uint16_t * fibbonacci_ram = (uint16_t *)0x2003D804;
    // for(int i = 0; i < sizeof(fibbonacci) / sizeof(fibbonacci[0]); i++){
    //     fibbonacci_ram[i] = fibbonacci[i];
    // }
    memcpy(fibbonacci_ram, fibbonacci, sizeof(fibbonacci));

    printmem(fibbonacci_ram, sizeof(fibbonacci));

    fib ram_fib = (fib)0x2003D804;
    int x = ram_fib(8);

    char local_buff[25];
    sprintf(local_buff, "Number: %d\n\r", x);
    uart_puts(UART_ID, local_buff);

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

int add(int a, int b){
    return a + b;
}
int sub(int a, int b){
    return a - b;
}

void printmem(void *start, const int size) {
    char local_buff[10]; // Buffer to store the hex string
    uint8_t *ptr = (uint8_t *)start; // Cast start to a uint8_t pointer for byte-wise access

    for (int i = 0; i < size; ++i) {
        // Format the current byte as a hex string
        sprintf(local_buff, "0x%02X\n\r", *ptr);
        // Output the string using UART
        uart_puts(UART_ID, local_buff);
        // Move to the next byte in memory
        ptr++;
    }
}