#pragma once

#include <stdint.h>
#include <vmem/vmem.h>

typedef struct {
    int flash_addr;
} vmem_nor_flash_driver_t;

#define NOR_FLASH_SIZE 0x2000000

#define VMEM_DEFINE_NOR_FLASH(name_in, strname, addr_in, size_in, _vaddr) \
    static vmem_nor_flash_driver_t vmem_##name_in##_driver = {            \
        .flash_addr = addr_in,                                            \
    };                                                                    \
    __attribute__((section("vmem")))                                      \
    __attribute__((aligned(1)))                                           \
    __attribute__((used))                                                 \
    vmem_t vmem_##name_in = {                                             \
        .type = VMEM_TYPE_NOR_FLASH,                                      \
        .name = strname,                                                  \
        .size = size_in,                                                  \
        .read = vmem_nor_flash_read,                                      \
        .write = vmem_nor_flash_write,                                    \
        .driver = &vmem_##name_in##_driver,                               \
        .vaddr = (void *)_vaddr,                                          \
    };

void vmem_nor_flash_read(vmem_t * vmem, uint32_t addr, void * dataout, uint32_t len);
void vmem_nor_flash_write(vmem_t * vmem, uint32_t addr, void * datain, uint32_t len);