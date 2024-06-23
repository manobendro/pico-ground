#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/sync.h"
#include "hardware/structs/mpu.h"

#include "wozmon.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define BUFFER_SIZE 512 

// Define MPU RASR bit positions
#define MPU_RASR_ENABLE_Pos     0U
#define MPU_RASR_SIZE_Pos       1U
#define MPU_RASR_AP_Pos         24U
#define MPU_RASR_XN_Pos         28U

// Define MPU RASR bit masks
#define MPU_RASR_ENABLE_Msk     (1UL << MPU_RASR_ENABLE_Pos)
#define MPU_RASR_SIZE_Msk       (31UL << MPU_RASR_SIZE_Pos)
#define MPU_RASR_AP_Msk         (7UL << MPU_RASR_AP_Pos)
#define MPU_RASR_XN_Msk         (1UL << MPU_RASR_XN_Pos)

#define RAM_EXEC_START 0x20030000
#define RAM_EXEC_SIZE  (64 * 1024)  // 64KB

char buff[BUFFER_SIZE];

void init_uart(void);
char uart_read_char(void);
void send_back_buffer(void);
void print_fault_status(void);
void printmem(void *, const int);

// Math function
int add(int, int);
int sub(int, int);
int sqrtt(int);
int fibbonaccii(int);
void enable_mpu(void);

//WOZMON function
void echo(uint8_t ch);
uint8_t get_keyboard_input();

// typedef struct _math_table_t{
//     int (*add)(int, int);
//     int (*sub)(int, int);
// } math_table_t;

// __attribute__((section(".math"))) math_table_t math_table;
// void HardFault_Handler(void);

// __attribute__((section(".isr_vector")))
// void (* const vector_table[])(void) = {
//     // Other vectors...
//     [3] = HardFault_Handler,
//     // Other vectors...
// };

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
// uint16_t fibbonacci[] = {
//      0x4340, //muls	r0, r0
//      0x4770, //bx	lr
// };

typedef int (*fib)(int);

int main() {
    init_uart();
    enable_mpu();
    // printf("Hello, World!\n\r");

    // math_table.add = add;
    // math_table.sub = sub;
    
    // volatile math_table_t * math = (volatile math_table_t *)0x21030000;

    // uart_putc_raw(UART_ID, math->add(5, 4) + 0x30 );
    // uart_putc_raw(UART_ID, '\n' );
    // uart_putc_raw(UART_ID, '\r' );
    // uart_putc_raw(UART_ID, math->sub(5, 4) + 0x30 );
    // uart_putc_raw(UART_ID, '\n' );
    // uart_putc_raw(UART_ID, '\r' );

    uint16_t * fibbonacci_ram = (uint16_t *)0x20030000;
    // for(int i = 0; i < sizeof(fibbonacci) / sizeof(fibbonacci[0]); i++){
    //     fibbonacci_ram[i] = fibbonacci[i];
    // }
    memcpy(fibbonacci_ram, fibbonacci, sizeof(fibbonacci));

    printmem(fibbonacci_ram, sizeof(fibbonacci));

    fib ram_fib = (fib)0x20030001;
    int x = ram_fib(0x5);
    // int x = fibbonaccii(0x5);
    int y = sqrtt(0x5);

    char local_buff[25];
    sprintf(local_buff, "Number: %d, %d\n\r", x, y);
    uart_puts(UART_ID, local_buff);

    int buffer_index = 0;

    //WOZMON reset handler.
    reset();

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

void print_fault_status(void) {
    volatile uint32_t *SCB_HFSR = (volatile uint32_t *)0xE000ED2C;
    volatile uint32_t *SCB_CFSR = (volatile uint32_t *)0xE000ED28;

    uint32_t hfsr = *SCB_HFSR;
    uint32_t cfsr = *SCB_CFSR;

    printf("HFSR: 0x%08X\n", hfsr);
    printf("CFSR: 0x%08X\n", cfsr);

    if (cfsr & 0xFFFF0000) {
        printf("Memory Management Fault Status:\n");
        if (cfsr & (1 << 16)) printf("  - Instruction Access Violation\n");
        if (cfsr & (1 << 17)) printf("  - Data Access Violation\n");
        if (cfsr & (1 << 18)) printf("  - MemManage Fault on Unstacking\n");
        if (cfsr & (1 << 19)) printf("  - MemManage Fault on Stacking\n");
        if (cfsr & (1 << 24)) printf("  - MemManage Fault Address Register (MMAR) Valid\n");
    }

    if (cfsr & 0x0000FF00) {
        printf("Bus Fault Status:\n");
        if (cfsr & (1 << 8)) printf("  - Instruction Bus Error\n");
        if (cfsr & (1 << 9)) printf("  - Precise Data Bus Error\n");
        if (cfsr & (1 << 10)) printf("  - Imprecise Data Bus Error\n");
        if (cfsr & (1 << 11)) printf("  - Bus Fault on Unstacking\n");
        if (cfsr & (1 << 12)) printf("  - Bus Fault on Stacking\n");
        if (cfsr & (1 << 15)) printf("  - Bus Fault Address Register (BFAR) Valid\n");
    }

    if (cfsr & 0x000000FF) {
        printf("Usage Fault Status:\n");
        if (cfsr & (1 << 0)) printf("  - Undefined Instruction\n");
        if (cfsr & (1 << 1)) printf("  - Invalid State\n");
        if (cfsr & (1 << 2)) printf("  - Invalid PC Load Usage Fault\n");
        if (cfsr & (1 << 3)) printf("  - No Coprocessor\n");
        if (cfsr & (1 << 8)) printf("  - Unaligned Access Usage Fault\n");
        if (cfsr & (1 << 9)) printf("  - Divide by Zero\n");
    }
}

void __isr HardFault_Handler(void) {
    printf("HardFault detected!\n");
    print_fault_status();

    // Infinite loop to halt execution
    while (1) {
        // Optionally add code to log fault information or reset the system
        sleep_ms(1000);
    }
}
int fibbonaccii(int n) {
   if(n == 0){
      return 0;
   } else if(n == 1) {
      return 1;
   } else {
      return (fibbonaccii(n-1) + fibbonaccii(n-2));
   }
}

int sqrtt(int n){
   return n * n;
}

void enable_mpu(void) {
    // Disable MPU
    mpu_hw->ctrl = 0;

    // Configure region 0 for existing RAM (0x20000000 to 0x20030000, 192KB)
    mpu_hw->rnr = 0; // Select region 0
    mpu_hw->rbar = 0x20000000; // Base address
    mpu_hw->rasr = (0 << MPU_RASR_XN_Pos)          // Execution enabled
                 | (3UL << MPU_RASR_AP_Pos)        // Full access
                 | (18UL << MPU_RASR_SIZE_Pos)     // 192KB region size
                 | (1UL << MPU_RASR_ENABLE_Pos);   // Enable region

    // Configure region 1 for new execution region in RAM (0x20030000, 64KB)
    mpu_hw->rnr = 1; // Select region 1
    mpu_hw->rbar = RAM_EXEC_START; // Base address
    mpu_hw->rasr = (0 << MPU_RASR_XN_Pos)          // Execution enabled
                 | (3UL << MPU_RASR_AP_Pos)        // Full access
                 | (16UL << MPU_RASR_SIZE_Pos)     // 64KB region size
                 | (1UL << MPU_RASR_ENABLE_Pos);   // Enable region

    // Enable MPU
    mpu_hw->ctrl = (1UL << M0PLUS_MPU_CTRL_PRIVDEFENA_LSB) | (1UL << M0PLUS_MPU_CTRL_ENABLE_LSB);

    // Memory barrier to ensure MPU settings take effect
    __dsb();
    __isb();
}

void echo(uint8_t ch){
    uart_putc_raw(UART_ID, ch);
}
uint8_t get_keyboard_input(){
    while (!uart_is_readable(UART_ID)) {
        tight_loop_contents();  // Optional: Put CPU in low-power mode
    }
    return (uint8_t)uart_getc(UART_ID);
}