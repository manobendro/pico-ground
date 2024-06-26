#include <stdint.h>
#include <stdbool.h>

// Define constants
#define IN_BUFFER_SIZE  128
#define BASE_ADDR 0x20030000

// Define variables
// uint8_t memory[0x10000];  // Emulate memory
uint8_t in_buffer[IN_BUFFER_SIZE];
uint8_t text_index = 0;
uint8_t XAML = 0;
uint8_t XAMH = 0;
uint8_t STL = 0;
uint8_t STH = 0;
uint8_t L = 0;
uint8_t H = 0;
// uint8_t YSAV = 0;
uint8_t MODE = 0;


// Extarnal function
extern void echo(uint8_t ch);
extern uint8_t get_keyboard_input();
// Function prototypes
void reset();
void prbyte(uint8_t byte);
void prhex(uint8_t hex);
void parse_and_execute_command();
void parse_hex(uint8_t *y);
void run_program(uint16_t addr);

typedef void (*run_program_t)(void);

void reset() {
    // Initialization
    // uint8_t mode = 0x00;  // XAM mode
    // uint8_t xam_l = 0x00, xam_h = 0x00;
    // uint8_t stl = 0x00, sth = 0x00;

    // Clear decimal mode, enable interrupts
    // Set up display and keyboard control registers
    // Pseudocode:
    // clearDecimalMode();
    // enableInterrupts();
    // memory[DSP] = 0x7F;  // Set display data direction register
    // memory[KBDCR] = 0xA7;  // Set keyboard control register
    // memory[DSPCR] = 0xA7;  // Set display control register

    text_index = 0;
    XAML = 0;
    XAMH = 0;
    STL = 0;
    STH = 0;
    L = 0;
    H = 0;
    MODE = 0;

    echo('\\');
    echo('\n');

    // Main input loop
    while (true) {
        uint8_t ch = get_keyboard_input();
        if (ch == '\b') {
            // Handle backspace
            if (text_index > 0) {
                text_index--;
                echo(ch);
            } else {
                // Reinitialize line
                text_index = 0;
            }
        } else if (ch == '\e') {
            // Handle escape
            // need to reset the program
            reset();
        } else if (ch == '\r') {
            // Handle carriage return
            //echo('\n')
            in_buffer[text_index] = '\0'; // Add a string tarmination char.
            echo('\n');// TODO: I think needs to return \n
            text_index = 0;  // Reset the text index for the next line
            parse_and_execute_command();
        } else {
            // Store character in input buffer and display it
            in_buffer[text_index++] = ch;
            echo(ch);
        }
    }
}

void parse_and_execute_command() {
    uint8_t y = 0;
    // uint8_t l = 0, h = 0;

    while (true) {
        uint8_t ch = in_buffer[y];
        if (ch == '\0') {
            // Handle end of line
            reset();
        } else if (ch == '.') {
            // Handle BLOCK XAM mode
            // memory[MODE] = 0xAE;
            MODE = 0xAE;
            y++;
        } else if (ch == ':') {
            // Handle STOR mode
            // memory[MODE] = 0x7F;
            MODE = 0x7F;
            STL = L;
            STH = H;
            y++;
        } else if (ch == 'R') {
            // Run user program
            uint16_t addr = (XAMH << 8) | XAML;
            run_program(addr);
        } else if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F')){
            // Handle hex parsing
            parse_hex(&y);

            if (MODE & 0x40) {
                // Store mode
                uint16_t addr = (STH << 8) | STL;
                uint8_t * str_add = (uint8_t *)(BASE_ADDR | addr);
                *str_add = L;
                STL ++;
                if (STL == 0) STH++;
            } else {
                // XAM or BLOCK XAM mode
                if(MODE == 0x00){
                    XAML = L;
                    XAMH = H;
                }
                uint16_t XAM = (XAMH << 8) | XAML;
                uint16_t TEMP = (H << 8) | L;

                while(XAM <= TEMP){
                    //Printing adress XX:
                    prbyte(XAM >> 8);
                    prbyte(XAM);
                    echo(':');
                    echo(' ');
                    // prhex 8 times or less
                    uint8_t num_b = 1;
                    if((TEMP - XAM) >= 8){
                        num_b = 8;
                    }else{
                        num_b += (TEMP - XAM);
                    }

                    while(num_b--){
                        uint8_t * value = (uint8_t *)(BASE_ADDR | XAM); // OR oparetion with actual base adress;
                        prbyte(*value);
                        echo(' ');
                        XAM ++;
                    }
                    echo('\n');

                }
                XAMH = XAMH >> 8;
                XAML = XAML;
                MODE = 0x00; // back to normal mode.
            }
        }else{
            y++;
        }
    }
}

void parse_hex(uint8_t *y) {
    // uint8_t digit_count = 0;
    // *y--;
    L = 0;
    H = 0;
    uint8_t ch;

    while (true) {
        ch = in_buffer[*y];
        if (ch >= '0' && ch <= '9') {
            ch = ch - '0';
        } else if (ch >= 'A' && ch <= 'F') {
            ch = ch - 'A' + 10;
        }else{
            break;
        }

        (*y)++;

        H = (H << 4) | (L >> 4);
        L = (L << 4) | ch;

        // digit_count++;
        // //We can skip 4 charecter cheking rather untill not number or hex char
        // if (digit_count <= 4) {
        //     *h = (*h << 4) | (*l >> 4);
        //     *l = (*l << 4) | ch;
        // }
    }
}


void prbyte(uint8_t byte) {
    uint8_t msd = byte >> 4;
    prhex(msd);
    prhex(byte & 0x0F);
}

void prhex(uint8_t hex) {
    hex = (hex & 0x0F) + '0';
    if (hex > '9') {
        hex += 7;  // Convert to A-F
    }
    echo(hex);
}

void run_program(uint16_t addr) {
    // Placeholder for running user program at the given address
    // This function should be implemented to match the actual program running mechanism
    run_program_t run_function = (run_program_t)((BASE_ADDR | addr) | 0x01); // Ensuring 0 bit of pc set for brancing opcode
    run_function();
}
