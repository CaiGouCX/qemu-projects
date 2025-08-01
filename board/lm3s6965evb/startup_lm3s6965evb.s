/* startup_lm3s6965evb.s - Cortex-M3 启动代码 */
.syntax unified
.cpu cortex-m3
.thumb

/* 内存布局定义 - 需与链接器脚本匹配 */
.equ  Stack_Size, 0x00000400     /* 1KB 栈空间 */
.equ  Heap_Size,  0x00000200      /* 512B 堆空间 */

/* 中断向量表 手册83页*/
.section .isr_vector, "a"
.global __isr_vector
__isr_vector:
    .word   __StackTop           /* 栈顶指针 (0x00) */
    .word   Reset_Handler        /* 复位处理程序 (0x04) */
    .word   NMI_Handler          /* NMI 处理程序 (0x08) */
    .word   HardFault_Handler    /* 硬件错误处理 (0x0C) */
    .word   MemManage_Handler    /* 内存管理错误 (0x10) */
    .word   BusFault_Handler     /* 总线错误 (0x14) */
    .word   UsageFault_Handler   /* 用法错误 (0x18) */
    .word   0                    /* 保留 (0x1C) */
    .word   0                    /* 保留 (0x20) */
    .word   0                    /* 保留 (0x24) */
    .word   0                    /* 保留 (0x28) */
    .word   SVC_Handler          /* SVC 调用 (0x2C) */
    .word   DebugMon_Handler     /* 调试监控 (0x30) */
    .word   0                    /* 保留 (0x34) */
    .word   PendSV_Handler       /* PendSV 中断 (0x38) */
    .word   SysTick_Handler      /* SysTick 中断 (0x3C) */
    /* 外设中断向量 (LM3S6965 特有) */
    .word   GPIOA_Handler        /* 0x40: IRQ#0 GPIO Port A */
    .word   GPIOB_Handler        /* 0x44: IRQ#1 GPIO Port B */
    .word   GPIOC_Handler        /* 0x48: IRQ#2 GPIO Port C */
    .word   GPIOD_Handler        /* 0x4C: IRQ#3 GPIO Port D */
    .word   GPIOE_Handler        /* 0x50: IRQ#4 GPIO Port E */
    .word   UART0_Handler        /* 0x54: IRQ#5 UART0 */
    .word   UART1_Handler        /* 0x58: IRQ#6 UART1 */
    .word   SSI0_Handler         /* 0x5C: IRQ#7 SSI0 */
    .word   I2C0_Handler         /* 0x60: IRQ#8 I2C0 */
    .word   PWM0_Handler         /* 0x64: IRQ#9 PWM0 */
    .word   TMR0A_Handler        /* 0x68: IRQ#10 Timer0A */
    .word   TMR0B_Handler        /* 0x6C: IRQ#11 Timer0B */
    .word   TMR1A_Handler        /* 0x70: IRQ#12 Timer1A */
    .word   TMR1B_Handler        /* 0x74: IRQ#13 Timer1B */
    .word   TMR2A_Handler        /* 0x78: IRQ#14 Timer2A */
    .word   TMR2B_Handler        /* 0x7C: IRQ#15 Timer2B */
    .word   COMP0_Handler        /* 0x80: IRQ#16 Analog Comparator 0 */
    .word   ADC0_Handler         /* 0x84: IRQ#17 ADC Sequence 0 */
    .word   ADC1_Handler         /* 0x88: IRQ#18 ADC Sequence 1 */
    .word   ADC2_Handler         /* 0x8C: IRQ#19 ADC Sequence 2 */
    .word   ADC3_Handler         /* 0x90: IRQ#20 ADC Sequence 3 */
    .word   WATCHDOG0_Handler    /* 0x94: IRQ#21 Watchdog Timer */
    .word   SYSCTL_Handler       /* 0x98: IRQ#22 SysCtrl */
    .word   FLASH_Handler        /* 0x9C: IRQ#23 Flash */
    .word   QEI0_Handler         /* 0xA0: IRQ#24 QEI0 */
    .word   0                    /* 0xA4: IRQ#25 保留 */
    .word   0                    /* 0xA8: IRQ#26 保留 */
    .word   HIB_Handler          /* 0xAC: IRQ#27 Hibernation */
    .word   0                    /* 0xB0: IRQ#28 保留 */
    .word   0                    /* 0xB4: IRQ#29 保留 */
    .size  __isr_vector, . - __isr_vector

/* 栈和堆区域定义 */
.section .stack
.align 3
__StackLimit:
    .space Stack_Size
__StackTop:

.section .heap
.align 3
__HeapBase:
    .space Heap_Size
__HeapLimit:

/* 复位处理程序 */
.section .text.Reset_Handler
.global Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    /* 初始化栈指针 */
    ldr r0, =__StackTop
    mov sp, r0

    /* 复制.data段 (从FLASH到RAM) */
    ldr r0, =_sidata    /* .data初始值在FLASH中的地址 */
    ldr r1, =_sdata     /* .data段在RAM中的起始地址 */
    ldr r2, =_edata     /* .data段在RAM中的结束地址 */
    
    subs r3, r2, r1     /* 计算.data段长度 */
    ble .L_data_copy_done
    
.L_data_copy_loop:
    ldr r4, [r0], #4    /* 从FLASH加载数据 */
    str r4, [r1], #4    /* 存储到RAM */
    subs r3, #4         /* 递减计数器 */
    bgt .L_data_copy_loop
    
.L_data_copy_done:
    
    /* 清零.bss段 */
    ldr r0, =_sbss      /* .bss段起始地址 */
    ldr r1, =_ebss      /* .bss段结束地址 */
    movs r2, #0
    
    subs r3, r1, r0     /* 计算.bss段长度 */
    ble .L_bss_zero_done
    
.L_bss_zero_loop:
    str r2, [r0], #4    /* 存储0 */
    subs r3, #4         /* 递减计数器 */
    bgt .L_bss_zero_loop
    
.L_bss_zero_done:
    
    /* 调用系统初始化 */
    bl SystemInit
    
    /* 调用主函数 */
    bl main
    
    /* 主函数返回后进入死循环 */
.Loop_After_Main:
    b .Loop_After_Main
.size Reset_Handler, . - Reset_Handler

/* 默认中断处理程序 */
.macro Default_Handler handler_name
.weak \handler_name
.thumb_set \handler_name, Default_Handler_Impl
.endm

Default_Handler NMI_Handler
Default_Handler HardFault_Handler
Default_Handler MemManage_Handler
Default_Handler BusFault_Handler
Default_Handler UsageFault_Handler
Default_Handler SVC_Handler
Default_Handler DebugMon_Handler
Default_Handler PendSV_Handler
Default_Handler SysTick_Handler
Default_Handler GPIOA_Handler
Default_Handler GPIOB_Handler
Default_Handler GPIOC_Handler
Default_Handler GPIOD_Handler
Default_Handler GPIOE_Handler
Default_Handler UART0_Handler
Default_Handler UART1_Handler
Default_Handler SSI0_Handler
Default_Handler I2C0_Handler
Default_Handler PWM0_Handler
Default_Handler TMR0A_Handler
Default_Handler TMR0B_Handler
Default_Handler TMR1A_Handler
Default_Handler TMR1B_Handler
Default_Handler TMR2A_Handler
Default_Handler TMR2B_Handler
Default_Handler COMP0_Handler
Default_Handler ADC0_Handler
Default_Handler ADC1_Handler
Default_Handler ADC2_Handler
Default_Handler ADC3_Handler
Default_Handler WATCHDOG0_Handler
Default_Handler SYSCTL_Handler
Default_Handler FLASH_Handler
Default_Handler QEI0_Handler
Default_Handler HIB_Handler

/* 默认中断处理实现 */
.section .text.Default_Handler_Impl
.type Default_Handler_Impl, %function
Default_Handler_Impl:
    b .  /* 死循环 */
.size Default_Handler_Impl, . - Default_Handler_Impl

/* 系统初始化函数 (弱定义) */
.section .text.SystemInit
.weak SystemInit
.type SystemInit, %function
SystemInit:
    bx lr  /* 默认不执行操作 */
.size SystemInit, . - SystemInit

.end