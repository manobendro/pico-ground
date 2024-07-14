#include <stdint.h>
#include <stdbool.h>
#include "wozmon.h"
// Define constants
#define IN_BUFFER_SIZE  128
#define BASE_ADDR 0x20030000

// Define variables
uint8_t in_buffer[IN_BUFFER_SIZE];
uint8_t text_index = 0;
uint8_t bytes_count = 0;
uint8_t XAML = 0, XAMH = 0, STL = 0, STH = 0;
uint8_t addr_str[5];  // Buffer to accumulate address string

// Define lookup table for hex conversion
const uint8_t hex_to_val[16] = {
    0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF
};

// External function
extern void echo(uint8_t ch);
extern uint8_t get_keyboard_input();

// Function prototypes
void reset();
void prbyte(uint8_t byte);
void parse_hex(uint8_t *y);
void run_program(uint16_t addr);
void print_address(uint16_t addr);  // Added function

typedef void (*run_program_t)(void);

void reset() {
    text_index = 0;
    XAML = XAMH = STL = STH = 0;

    echo('\\');
    echo('\n');

    while (true) {
        uint8_t ch = get_keyboard_input();
        switch (ch) {
            case '\b':
                if (text_index > 0) {
                    text_index--;
                    echo(ch);
                }
                break;
            case '\e':
                reset();
                break;
            case '\r':
                in_buffer[text_index] = '\0';
                echo('\n');
                text_index = 0;
                parse_and_execute_command();
                break;
            default:
                in_buffer[text_index++] = ch;
                echo(ch);
                break;
        }
    }
}

void parse_and_execute_command() {
    uint8_t y = 0;
    while (true) {
        uint8_t ch = in_buffer[y];
        if (ch == '\0') {
            reset();
        } else if (ch == '.') {
            MODE = 0xAE;
            y++;
        } else if (ch == ':') {
            MODE = 0x7F;
            STL = L;
            STH = H;
            y++;
        } else if (ch == 'R') {
            run_program((XAMH << 8) | XAML);
        } else if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F')) {
            parse_hex(&y);

            if (MODE & 0x40) {
                uint16_t addr = (STH << 8) | STL;
                *((uint8_t *)(BASE_ADDR | addr)) = L;
                STL++;
                if (STL == 0) STH++;
            } else {
                if (MODE == 0x00) {
                    XAML = L;
                    XAMH = H;
                    bytes_count = 0;
                }
                uint16_t XAM = (XAMH << 8) | XAML;
                uint16_t TEMP = (H << 8) | L;

                const uint8_t num_to_print = (TEMP - XAM >= (8 - bytes_count)) ? (8 - bytes_count) : TEMP - XAM + 1;
                uint8_t remaining = num_to_print;

                while (remaining) {
                    prbyte(*((uint8_t *)(BASE_ADDR | XAM)));
                    echo(' ');
                    XAM++;
                    bytes_count++;
                    remaining--;
                }
                if (MODE & 0x20) { // block xam mood
                    bytes_count = 0;
                    echo('\n');
                } else {
                    // Print the address after the loop
                    print_address(XAM);
                }
                XAMH = XAM >> 8;
                XAML = XAM;
                MODE = 0x00;
            }
        } else {
            y++;
        }
    }
}

void parse_hex(uint8_t *y) {
    L = 0;
    H = 0;

    while (true) {
        uint8_t ch = in_buffer[*y];
        if (ch >= '0' && ch <= '9') {
            ch -= '0';
        } else if (ch >= 'A' && ch <= 'F') {
            ch = ch - 'A' + 10;
        } else {
            break;
        }

        (*y)++;
        H = (H << 4) | (L >> 4);
        L = (L << 4) | hex_to_val[ch];  // Use lookup table for faster conversion
    }
}

inline void prbyte(uint8_t byte) {
    prhex(byte >> 4);
    prhex(byte & 0x0F);
}

inline void prhex(uint8_t hex) {
    hex += (hex > 9) ? 'A' - 10 : '0';
    echo(hex);
}

void run_program(uint16_t addr) {
    ((run_program_t)((BASE_ADDR | addr) | 0x01))();
}

// Added function to print address in a formatted way
void print_address(uint16_t addr) {
    addr_str[0] = (addr & 0xF000) >> 12;
    addr_str[1] = (addr & 0x0F00) >> 8;
    addr_str[2] = (addr & 0x00F0) >> 4;
    addr_str[3] = (addr & 0x000F);
    for (int i = 0; i < 4; i++) {
        addr_str[i] += (addr_str[i] > 9) ? 'A' - 10 : '0';
    }
    addr_str[4] = '\0';
    echo(addr_str);
}
