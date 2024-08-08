#include <stdint.h>
#include <stdbool.h>

// Define constants
#define IN_BUFFER_SIZE  128
#define BASE_ADDR       0x20030000

// Define variables
uint8_t input_buffer[IN_BUFFER_SIZE];  // Buffer to store user input
uint8_t input_index = 0;               // Index for the input buffer
uint8_t byte_count = 0;                // Counter for bytes processed
uint8_t addr_low = 0, addr_high = 0;   // Variables to store low and high parts of the address
uint8_t start_low = 0, start_high = 0; // Variables to store low and high parts of the start address
uint8_t data_low = 0, data_high = 0;   // Variables to store low and high parts of the data
uint8_t mode = 0;                      // Variable to store the current mode

// External function declarations
extern void echo(uint8_t ch);
extern uint8_t get_keyboard_input();

// Function prototypes
void reset();
void print_byte(uint8_t byte);
void print_hex(uint8_t hex);
void parse_and_execute_command();
void parse_hex(uint8_t *index);
void execute_program(uint16_t addr);

typedef void (*program_runner_t)(void);

void reset() {
    // Reset input index and all relevant variables
    input_index = 0;
    addr_low = addr_high = start_low = start_high = data_low = data_high = mode = 0;

    echo('\\');
    echo('\n');

    while (true) {
        uint8_t ch = get_keyboard_input();  // Read input from the keyboard
        switch (ch) {
            case '\b':  // Handle backspace
                if (input_index > 0) {
                    input_index--;
                    echo(ch);
                }
                break;
            case '\e':  // Handle escape key to reset
                reset();
                break;
            case '\r':  // Handle carriage return (Enter key)
                input_buffer[input_index] = '\0';  // Null-terminate the input string
                echo('\n');
                input_index = 0;  // Reset input index
                parse_and_execute_command();  // Parse and execute the command
                break;
            default:  // Handle regular characters
                input_buffer[input_index++] = ch;  // Store character in the buffer
                echo(ch);
                break;
        }
    }
}

void parse_and_execute_command() {
    uint8_t index = 0;  // Index for traversing the input buffer
    while (true) {
        uint8_t ch = input_buffer[index];  // Get the current character from the buffer
        if (ch == '\0') {  // End of input string
            reset();  // Reset the program
        } else if (ch == '.') {  // Set mode to 0xAE on encountering '.'
            mode = 0xAE;
            index++;
        } else if (ch == ':') {  // Set mode to 0x7F on encountering ':'
            mode = 0x7F;
            start_low = data_low;  // Store current data in start variables
            start_high = data_high;
            index++;
        } else if (ch == 'R') {  // Execute program on encountering 'R'
            execute_program((addr_high << 8) | addr_low);
        } else if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F')) {  // Handle hexadecimal input
            parse_hex(&index);

            if (mode & 0x40) {  // Memory write mode
                uint16_t addr = (start_high << 8) | start_low;
                *((uint8_t *)(BASE_ADDR | addr)) = data_low;  // Write data to memory
                if (++start_low == 0) start_high++;  // Increment start address
            } else {  // Address calculation and memory read mode
                if (mode == 0x00) {  // Initial setup for address
                    addr_low = data_low;
                    addr_high = data_high;
                    byte_count = 0;
                }
                uint16_t addr = (addr_high << 8) | addr_low;
                uint16_t temp = (data_high << 8) | data_low;

                // Output bytes from memory until address matches temp
                while (addr <= temp) {
                    if (byte_count == 0) {  // Print address at the start of a line
                        print_byte(addr >> 8);
                        print_byte(addr);
                        echo(':');
                        echo(' ');
                    }

                    uint8_t num_bytes = (temp - addr >= (8 - byte_count)) ? (8 - byte_count) : temp - addr + 1;

                    while (num_bytes--) {
                        print_byte(*((uint8_t *)(BASE_ADDR | addr)));  // Print byte from memory
                        echo(' ');
                        addr++;
                        byte_count++;
                    }
                    if (mode & 0x20) {  // Check if block mode is set
                        byte_count = 0;
                        echo('\n');
                    }
                }
                addr_high = addr >> 8;  // Update address high byte
                addr_low = addr;        // Update address low byte
                mode = 0x00;            // Reset mode
            }
        } else {
            index++;  // Move to the next character in the buffer
        }
    }
}

void parse_hex(uint8_t *index) {
    data_low = 0;
    data_high = 0;

    // Parse hex digits and convert to binary
    while (true) {
        uint8_t ch = input_buffer[*index];
        if (ch >= '0' && ch <= '9') {
            ch -= '0';
        } else if (ch >= 'A' && ch <= 'F') {
            ch -= 'A' - 10;
        } else {
            break;  // Exit loop if the character is not a valid hex digit
        }

        (*index)++;  // Move to the next character in the buffer
        data_high = (data_high << 4) | (data_low >> 4);  // Shift high part and move low part
        data_low = (data_low << 4) | ch;  // Shift low part and add current digit
    }
}

inline void print_byte(uint8_t byte) {
    // Print a byte in hexadecimal format
    print_hex(byte >> 4);  // Print high nibble
    print_hex(byte & 0x0F);  // Print low nibble
}

inline void print_hex(uint8_t hex) {
    // Convert a 4-bit value to its corresponding hexadecimal character
    hex += (hex > 9) ? 'A' - 10 : '0';
    echo(hex);
}

void execute_program(uint16_t addr) {
    // Call the program at the specified address. In branching 0th address bit needs to be set
    ((program_runner_t)((BASE_ADDR | addr) | 0x01))();
}
