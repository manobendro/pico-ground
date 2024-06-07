# Compiler
CC = arm-none-eabi-gcc

# Assembler
AS = arm-none-eabi-as

# Compiler Flags
CFLAGS = -mthumb -mcpu=cortex-m0plus -Os -Wall -fPIC -nostd

# Assembler Flags
ASFLAGS = -mcpu=cortex-m0plus

# Source Files
SRCS = main.c

# Object Files
OBJS = $(SRCS:.c=.o)

# Output Binary
TARGET = output.elf

# Directory for generated files
GEN_DIR = gen

.PHONY: all assemble disassemble clean

all: assemble disassemble

assemble: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

disassemble:
	@echo "Disassembling object files:"
	@mkdir -p $(GEN_DIR)
	@for file in $(OBJS); do \
		echo "Disassembling $$file"; \
		arm-none-eabi-objdump -d $$file > $(GEN_DIR)/$(basename $$file).S; \
	done

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(GEN_DIR)
