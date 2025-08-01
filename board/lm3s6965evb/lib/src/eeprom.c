#include "eeprom.h"
#include "flash.h"

/* ========================== 定义 ========================== */
#define EEPROM_BASE_ADDR    0x0003F000  /* EEPROM 模拟区域，1KB */
#define EEPROM_SIZE         1024        /* 1KB 页面 */
#define FLASH_PAGE_SIZE     1024        /* Flash 页面大小 */

/* 静态缓冲区，用于页面读写 */
static uint8_t eeprom_buffer[FLASH_PAGE_SIZE];

/* ========================== EEPROM 初始化 ========================== */
void eeprom_init(void) {
    /* [QEMU 有效] 初始化 Flash 控制器 */
    flash_init();
}

/* ========================== 读取 EEPROM 数据 ========================== */
void eeprom_read(uint32_t offset, uint8_t *data, uint32_t len) {
    if (offset + len > EEPROM_SIZE) return; /* 边界检查 */

    /* [QEMU 有效] 直接从 Flash 读取 */
    uint32_t addr = EEPROM_BASE_ADDR + offset;
    for (uint32_t i = 0; i < len; i++) {
        data[i] = *(volatile uint8_t *)(addr + i);
    }
}

/* ========================== 写入 EEPROM 数据 ========================== */
void eeprom_write(uint32_t offset, const uint8_t *data, uint32_t len) {
    if (offset + len > EEPROM_SIZE) return; /* 边界检查 */

    /* [QEMU 有效] 读取当前页面到缓冲区 */
    uint32_t page_addr = EEPROM_BASE_ADDR & ~(FLASH_PAGE_SIZE - 1);
    for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i++) {
        eeprom_buffer[i] = *(volatile uint8_t *)(page_addr + i);
    }

    /* 更新缓冲区中的数据 */
    for (uint32_t i = 0; i < len; i++) {
        eeprom_buffer[offset + i] = data[i];
    }

    /* [QEMU 有效] 擦除页面 */
    flash_erase_page(page_addr);

    /* [QEMU 有效] 按字编程（4 字节对齐） */
    for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i += 4) {
        uint32_t word = (eeprom_buffer[i] |
                        (eeprom_buffer[i + 1] << 8) |
                        (eeprom_buffer[i + 2] << 16) |
                        (eeprom_buffer[i + 3] << 24));
        flash_write_word(page_addr + i, word);
    }
}