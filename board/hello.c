#include <stdint.h>
#if defined(__riscv)
#define UART_BASE 0x10000000 // RISC-V virt UART
#elif defined(__arm__)
#define UART_BASE 0x4000C000 // Cortex-M3/M4 lm3s811evb, lm3s6965evb UART0
#endif
void _start() {
    volatile uint8_t * const UART0_DR = (uint8_t *)UART_BASE;
    const char *msg = "Hello, QEMU Bare Metal!\n";
    while (*msg) {
        *UART0_DR = *msg++;
    }
    while (1);
}