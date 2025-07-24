ENTRY(main)
SECTIONS
{
    . = 0x00000000;
    .vectors : { KEEP(*(.vectors)) }
    .text : { *(.text*) }
    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : {
        __bss_start = .;
        *(.bss*)
        __bss_end = .;
    }
    . = ALIGN(16);
    __stack_bottom = .;
    . = . + 0x1000;
    __stack_top = .;
}