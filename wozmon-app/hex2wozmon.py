def load_hex(source: str, target: bytearray, base_address: int = 0):
    high_address_bytes = 0
    for line in source.split('\n'):
        if line.startswith(':'):
            if line[7:9] == '04':
                high_address_bytes = int(line[9:13], 16)
            elif line[7:9] == '00':
                byte_count = int(line[1:3], 16)
                addr = ((high_address_bytes << 16) | int(line[3:7], 16)) - base_address
                for i in range(byte_count):
                    target[addr + i] = int(line[9 + i * 2:11 + i * 2], 16)

def print_memory(target: bytearray):
    for i in range(0, len(target), 8):
        # Format memory address
        address = f'{i:04X}'
        # Format 8 bytes in hexadecimal
        data = ' '.join(f'{byte:02X}' for byte in target[i:i+8])
        # Print formatted line
        print(f'{address}: {data}')

# Example usage
hex_data = """
:1000000006A0006806A10A780131002A05D083699C
:1000100080242342FBD00270F5E7FEE70040034056
:0E00200048656C6C6F2C20576F726C640A0080
:00000001FF
"""
target_array = bytearray(256)
load_hex(hex_data, target_array)

# Print the filled target array for verification
print_memory(target_array)

