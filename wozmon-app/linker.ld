/* Memory regions */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 64K  /* Flash memory starts at 0x00000000, size 64KB */
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 20K   /* RAM starts at 0x20000000, size 20KB */
}

/* Entry point definition */
ENTRY(_start)

/* Sections */
SECTIONS
{
    /* .text section starts at the beginning of FLASH */
    .text :
    {
        *(.text)    /* All .text sections from input files */
    } > FLASH

    /* .rodata section (read-only data) */
    .rodata :
    {
        *(.rodata)  /* All .rodata sections from input files */
    } > FLASH

    /* .data section (initialized data) */
    .data :
    {
        *(.data)    /* All .data sections from input files */
    } > RAM AT > FLASH  /* Place .data in RAM, initialized from FLASH */

    /* .bss section (uninitialized data) */
    .bss :
    {
        *(.bss)     /* All .bss sections from input files */
    } > RAM           /* Place .bss in RAM */

    /* Stack and heap definitions (if needed) */
    PROVIDE(__stack_end = ORIGIN(RAM) + LENGTH(RAM));
    __heap_start = .;
    __heap_end = ORIGIN(RAM) + LENGTH(RAM);
}
