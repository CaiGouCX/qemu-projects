.syntax unified
    .cpu cortex-m3
    .thumb

    .section .vectors, "a"
    .align 2
    .global vectors
vectors:
    .word 0x20007fff    // 栈顶指针（LM3S811 SRAM 32KB）
    .word main + 1      // 重置处理程序（Thumb 模式）
    