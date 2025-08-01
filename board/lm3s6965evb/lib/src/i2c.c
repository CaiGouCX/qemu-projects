#include "i2c.h"

/* ========================== 寄存器定义 ========================== */
#define SYSCTL_BASE         0x400FE000
#define SYSCTL_RCGCI2C      (*((volatile uint32_t *)(SYSCTL_BASE + 0x620)))

#define I2C0_BASE           0x40020000
#define I2C0_MSA            (*((volatile uint32_t *)(I2C0_BASE + 0x000)))
#define I2C0_MCS            (*((volatile uint32_t *)(I2C0_BASE + 0x004)))
#define I2C0_MDR            (*((volatile uint32_t *)(I2C0_BASE + 0x008)))
#define I2C0_MTPR           (*((volatile uint32_t *)(I2C0_BASE + 0x00C)))
#define I2C0_MIMR           (*((volatile uint32_t *)(I2C0_BASE + 0x014)))
#define I2C0_MMIS           (*((volatile uint32_t *)(I2C0_BASE + 0x01C)))
#define I2C0_MICR           (*((volatile uint32_t *)(I2C0_BASE + 0x024)))
#define I2C0_MCR            (*((volatile uint32_t *)(I2C0_BASE + 0x020)))

#define NVIC_EN0            (*((volatile uint32_t *)(0xE000E100)))

/* ========================== 中断处理函数指针 ========================== */
static void (*i2c0_handler)(void) = 0;

/* ========================== I2C 初始化 ========================== */
void i2c_init(uint32_t speed) {
    /* [QEMU 无效] 使能 I2C0 模块时钟 */
    SYSCTL_RCGCI2C |= 0x00000001;

    /* [QEMU 无效] 等待时钟稳定 */
    for (volatile int i = 0; i < 1000; i++);

    /* [QEMU 有效] 配置 I2C0 主站模式 */
    I2C0_MCR = 0x00000010;        /* 启用主站功能 */
    I2C0_MTPR = (16000000 / (20 * speed)) - 1; /* 设置速度，16MHz 系统时钟 */
}

/* ========================== 向从设备写入数据 ========================== */
void i2c_write(uint8_t addr, uint8_t *data, uint32_t len) {
    /* [QEMU 有效] 设置从设备地址（写模式） */
    I2C0_MSA = (addr << 1) & ~0x1; /* 地址左移 1 位，写操作（最低位=0） */

    /* [QEMU 有效] 发送数据 */
    for (uint32_t i = 0; i < len; i++) {
        I2C0_MDR = data[i];       /* 写入数据 */
        I2C0_MCS = 0x00000007;    /* 启动传输（START + RUN + STOP） */
        while (I2C0_MCS & 0x00000001); /* 等待传输完成 */
    }
}

/* ========================== 注册中断处理函数（QEMU 无效） ========================== */
void i2c_register_handler(void (*handler)(void)) {
    i2c0_handler = handler;

    /* [QEMU 无效] 使能 I2C0 中断 */
    I2C0_MIMR |= 0x00000001;

    /* [QEMU 无效] 启用 NVIC 中 I2C0 中断（IRQ#8） */
    NVIC_EN0 |= (1 << 8);
}

/* ========================== I2C0 中断处理函数（QEMU 无效） ========================== */
void I2C0_Handler(void) {
    if (i2c0_handler) {
        i2c0_handler();
    }

    /* [QEMU 无效] 清除中断标志 */
    I2C0_MICR = 0x00000001;
}