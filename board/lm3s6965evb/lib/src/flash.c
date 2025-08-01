#include "flash.h"

/* ========================== 寄存器定义 ========================== */
#define FLASH_BASE          0x400FD000
#define FLASH_FMA           (*((volatile uint32_t *)(FLASH_BASE + 0x000)))
#define FLASH_FMD           (*((volatile uint32_t *)(FLASH_BASE + 0x004)))
#define FLASH_FMC           (*((volatile uint32_t *)(FLASH_BASE + 0x008)))
#define FLASH_FMIM          (*((volatile uint32_t *)(FLASH_BASE + 0x010)))
#define FLASH_FMIS          (*((volatile uint32_t *)(FLASH_BASE + 0x014)))
#define FLASH_FMC2          (*((volatile uint32_t *)(FLASH_BASE + 0x020)))

#define NVIC_EN0            (*((volatile uint32_t *)(0xE000E100)))

/* Flash 控制位 */
#define FLASH_FMC_WRKEY     0xA4420000
#define FLASH_FMC_WRITE     (FLASH_FMC_WRKEY | 0x00000001)
#define FLASH_FMC_ERASE     (FLASH_FMC_WRKEY | 0x00000002)
#define FLASH_FMC_WRBUSY    0x00010000

/* ========================== 中断处理函数指针 ========================== */
static void (*flash_handler)(void) = 0;

/* ========================== Flash 初始化 ========================== */
void flash_init(void) {
    /* [QEMU 有效] 确保 Flash 控制器就绪 */
    while (FLASH_FMC & FLASH_FMC_WRBUSY); /* 等待忙状态结束 */
}

/* ========================== 擦除 Flash 页面 ========================== */
void flash_erase_page(uint32_t address) {
    if (address >= 0x00040000 || (address & 0x3FF)) return; /* 地址检查：256KB 内，1KB 对齐 */

    /* [QEMU 有效] 设置擦除地址并触发 */
    FLASH_FMA = address;
    FLASH_FMC = FLASH_FMC_ERASE;

    /* [QEMU 有效] 等待擦除完成 */
    while (FLASH_FMC & FLASH_FMC_WRBUSY);
}

/* ========================== 编程 Flash 字 ========================== */
void flash_write_word(uint32_t address, uint32_t data) {
    if (address >= 0x00040000 || (address & 0x3)) return; /* 地址检查：256KB 内，4 字节对齐 */

    /* [QEMU 有效] 设置地址和数据并触发 */
    FLASH_FMA = address;
    FLASH_FMD = data;
    FLASH_FMC = FLASH_FMC_WRITE;

    /* [QEMU 有效] 等待编程完成 */
    while (FLASH_FMC & FLASH_FMC_WRBUSY);
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void flash_register_handler(void (*handler)(void)) {
    flash_handler = handler;

    /* [QEMU 无效] 使能 Flash 中断 */
    FLASH_FMIM |= 0x00000001; /* 编程完成中断 */

    /* [QEMU 无效] 启用 NVIC 中 Flash 中断（IRQ#3） */
    NVIC_EN0 |= (1 << 3);
}

/* ========================== Flash 中断处理函数（QEMU 无效） ========================== */
void FLASH_Handler(void) {
    if (flash_handler) {
        flash_handler();
    }

    /* [QEMU 无效] 清除中断标志 */
    FLASH_FMIS = 0x00000001;
}