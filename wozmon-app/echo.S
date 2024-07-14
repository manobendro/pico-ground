@ Compile with gcc
@ arm-none-eabi-gcc -mthumb -mcpu=cortex-m0plus -Os -Wall -nostartfiles -nostdlib -o echo.elf -T linker.ld echo.S
@ #define UART0_BASE 0x40034000

#define UART_UARTDR_OFFSET 0x00000000
@ Data Register, UARTDR
@ 0x00000800 [11]    : OE (0): Overrun error
@ 0x00000400 [10]    : BE (0): Break error
@ 0x00000200 [9]     : PE (0): Parity error
@ 0x00000100 [8]     : FE (0): Framing error
@ 0x000000ff [7:0]   : DATA (0): Receive (read) data character

#define UART_UARTFR_OFFSET 0x00000018
@ Flag Register, UARTFR
@ 0x00000100 [8]     : RI (0): Ring indicator
@ 0x00000080 [7]     : TXFE (1): Transmit FIFO empty
@ 0x00000040 [6]     : RXFF (0): Receive FIFO full
@ 0x00000020 [5]     : TXFF (0): Transmit FIFO full
@ 0x00000010 [4]     : RXFE (1): Receive FIFO empty
@ 0x00000008 [3]     : BUSY (0): UART busy
@ 0x00000004 [2]     : DCD (0): Data carrier detect
@ 0x00000002 [1]     : DSR (0): Data set ready
@ 0x00000001 [0]     : CTS (0): Clear to send

.section .text.startup,"ax",%progbits
.global _start

_start:
    ADR R0, UART0_BASE             @ Load UART0_BASE memory location to R0
    LDR R0, [R0, #0]               @ Load UART base offset into R0
    ADR R1, TEXT                   @ Load address of the text string into R1
    
WRITE_LOOP:
    LDRB R2, [R1, #0]              @ Load byte from string
    ADD R1, R1, #1                 @ Increment pointer
    CMP R2, #0                     @ Compare byte to zero
    BEQ DONE                       @ If byte is zero, we're done

WAIT_TXFE:
    LDR R3, [R0, #UART_UARTFR_OFFSET]   @ Load UART Flag Register
    MOVS R4, #0x80                      @ Move 0x80 into R4
    TST R3, R4                          @ Test if Transmit FIFO Empty flag is set
    BEQ WAIT_TXFE                       @ If not set, wait

    STRB R2, [R0, #UART_UARTDR_OFFSET]  @ Write byte to UART Data Register
    B WRITE_LOOP                        @ Repeat for next byte

DONE:
    B DONE                              @ Infinite loop to end the program

UART0_BASE:                             @ UART0_BASE value is too big for #imm load to resister
    .word 0x40034000

TEXT:
    .asciz "Hello, World\n"
