/*------------------------------------------------------------------------------
 * LM3S6965 QEMU Linker Script
 *----------------------------------------------------------------------------*/

ENTRY(Reset_Handler)

/* 内存区域定义 */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

/* 栈顶与堆边界 */
__StackTop  = ORIGIN(RAM) + LENGTH(RAM);  /* 栈顶 = RAM 末尾 */
_stack_size = 0x400;                      /* 栈大小：1KB */
_heap_size  = 0x800;                      /* 堆大小：2KB */

/* newlib 需要的符号 */
PROVIDE(__bss_start__ = __bss_start);
PROVIDE(__bss_end__   = __bss_end);
PROVIDE(end           = __HeapBase);

/* 导出符号给启动代码使用 */
PROVIDE(_sdata = __data_start);
PROVIDE(_edata = __data_end);
PROVIDE(_sbss  = __bss_start);
PROVIDE(_ebss  = __bss_end);

SECTIONS
{
    /* 中断向量表 */
    .isr_vector : {
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* 代码段 (.text) 与只读数据 (.rodata) */
    .text : {
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata*)
        *(.init)
        . = ALIGN(4);
        _etext = .;  /* 标记 .text 和 .rodata 的结束地址 */
    } > FLASH

    /* ARM 异常索引表 */
    .ARM.exidx : {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(4);
    } > FLASH

    /* FinSH 符号表段 */
    .fsymtab : {
        __fsymtab_start = .;
        KEEP(*(.fsymtab*))
        __fsymtab_end = .;
        . = ALIGN(4);
    } > FLASH

    /* 初始化数据段 (.data) */
    _sidata = .;  /* 将 _sidata 设置为 .fsymtab 结束后的地址 */
    .data : AT (_sidata) {
        . = ALIGN(4);
        __data_start = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        __data_end = .;
    } > RAM

    /* 未初始化数据段 (.bss) */
    .bss : {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > RAM

    /* 堆空间 */
    .heap : {
        . = ALIGN(4);
        __HeapBase = .;
        . = . + _heap_size;
        . = ALIGN(4);
        __HeapLimit = .;
    } > RAM

    /* 栈空间 */
    .stack (NOLOAD) : {
        . = ALIGN(8);
        __StackLimit = .;
        . = . + _stack_size;
        . = ALIGN(8);
        __StackTop = .;
    } > RAM

    /* 丢弃不必要段 */
    /DISCARD/ : {
        *(.fini)
        *(.fini_array)
        *(.fini_array*)
        *(.comment)
        *(.note*)
        *(.ARM.attributes)
    }
}