.syntax unified
    .cpu cortex-m4
    .thumb

    .section .vectors, "a"
    .align 2
    .global vectors
vectors:
    .word 0x2000ffff    // 栈顶指针（LM3S6965 SRAM 64KB）
    .word _start        // 重置处理程序

    .section .text
    .global _start
    .thumb_func
_start:
    ldr r0, =message    // 加载字符串地址
    bl uart_puts        // 调用 C 函数
    b .                 // 死循环

    .section .rodata
message:
    .asciz "Hello, QEMU Bare Metal!\n"