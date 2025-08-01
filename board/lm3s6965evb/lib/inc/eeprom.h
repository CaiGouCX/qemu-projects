#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

/* 初始化 EEPROM（基于 Flash，起始地址 0x0003F000，1KB） */
void eeprom_init(void);

/* 读取 EEPROM 数据（字节级） */
void eeprom_read(uint32_t offset, uint8_t *data, uint32_t len);

/* 写入 EEPROM 数据（字节级） */
void eeprom_write(uint32_t offset, const uint8_t *data, uint32_t len);

#endif /* EEPROM_H */