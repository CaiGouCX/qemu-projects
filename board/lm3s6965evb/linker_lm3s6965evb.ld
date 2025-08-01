/* linker_lm3s6965evb.ld - LM3S6965 链接脚本 */
ENTRY(Reset_Handler)

/* 内存区域定义 */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K   /* Flash 起始地址与大小 */
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K    /* RAM 起始地址与大小 */
}

/* 栈顶与堆边界 */
_estack = ORIGIN(RAM) + LENGTH(RAM);  /* 栈顶 = RAM 末尾地址 (0x20010000) */
_stack_size = 0x400;                  /* 栈大小：1KB */
_heap_size = 0x200;                   /* 堆大小：512B */

SECTIONS
{
    /* 中断向量表 (必须位于 FLASH 起始位置) */
    .isr_vector : {
        . = ALIGN(4);
        KEEP(*(.isr_vector))          /* 保留向量表 */
        . = ALIGN(4);
    } > FLASH

    /* 代码段 (.text) 与只读数据 (.rodata) */
    .text : {
        *(.text)                      /* 程序代码 */
        *(.text.*)                    /* 子函数代码 */
        *(.rodata)                    /* 只读常量 */
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;                   /* 代码结束地址 */
    } > FLASH

    /* 初始化数据段 (.data) - 从 FLASH 复制到 RAM */
    _sidata = LOADADDR(.data);        /* .data 在 FLASH 中的加载地址 */
    .data : AT (_sidata) {
        . = ALIGN(4);
        _sdata = .;                   /* .data 在 RAM 中的起始地址 */
        *(.data)                     
        *(.data*)                    
        . = ALIGN(4);
        _edata = .;                   /* .data 在 RAM 中的结束地址 */
    } > RAM

    /* 未初始化数据段 (.bss) - 启动时清零 */
    .bss : {
        . = ALIGN(4);
        _sbss = .;                    /* .bss 起始地址 */
        *(.bss)
        *(COMMON)                     /* 全局通用变量 */
        . = ALIGN(4);
        _ebss = .;                    /* .bss 结束地址 */
    } > RAM

    /* 堆空间 */
    .heap : {
        . = ALIGN(4);
        __HeapBase = .;               /* 堆起始地址 */
        . = . + _heap_size;
        . = ALIGN(4);
        __HeapLimit = .;              /* 堆结束地址 */
    } > RAM

    /* 栈空间 */
    .stack (NOLOAD) : {
        . = ALIGN(8);
        __StackLimit = .;             /* 栈底地址 */
        . = . + _stack_size;          /* 分配栈空间 */
        . = ALIGN(8);
        __StackTop = .;               /* 栈顶地址 */
    } > RAM
}