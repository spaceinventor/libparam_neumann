#include <stdio.h>
#include <stdint.h>
#include <string.h>

// #include <driver_init.h>

// #include <spi_nor_flash_main.h>
// #include "vmem_nor_flash.h"
//Harry: Added this file to param so that nor flash functions were available to thruster.c

#define PAGE_SIZE   256
#define SECTOR_SIZE 4096
#define BLOCK_SIZE  65536

void vmem_nor_flash_read(vmem_t * vmem, uint32_t addr, void * dataout, uint32_t len) {

    intptr_t physaddr = ((intptr_t)((vmem_nor_flash_driver_t *)vmem->driver)->flash_addr) + addr;

    spi_nor_flash_read(SPI_NOR_FLASH_0, dataout, physaddr, len);
}

void vmem_nor_flash_write(vmem_t * vmem, uint32_t addr, void * datain, uint32_t len) {

    int rc;
    intptr_t physaddr = ((intptr_t)((vmem_nor_flash_driver_t *)vmem->driver)->flash_addr) + addr;

    /* Erase sector if writing to the first byte of it */
    int block_start = physaddr / BLOCK_SIZE;
    int block_end = (physaddr + len - 1) / BLOCK_SIZE;
    if ((physaddr % BLOCK_SIZE) == 0 || block_start != block_end) {
        intptr_t blockaddr = block_end * BLOCK_SIZE;
        rc = spi_nor_flash_erase(SPI_NOR_FLASH_0, blockaddr, BLOCK_SIZE);
        if (rc != ERR_NONE) {
            printf("Nor flash erase error %d\n", rc);
        }
    }

    int page_start = physaddr / PAGE_SIZE;
    int page_end = (physaddr + len - 1) / PAGE_SIZE;
    if (page_start != page_end) {
        intptr_t pageaddr = page_end * PAGE_SIZE;
        int part_len = pageaddr - physaddr;
        rc = spi_nor_flash_write(SPI_NOR_FLASH_0, (uint8_t *)datain, physaddr, part_len);
        if (rc != ERR_NONE) {
            printf("Nor flash write error %d\n", rc);
        }
        rc = spi_nor_flash_write(SPI_NOR_FLASH_0, (uint8_t *)datain + part_len, pageaddr, len - part_len);
        if (rc != ERR_NONE) {
            printf("Nor flash write error %d\n", rc);
        }
    } else {
        rc = spi_nor_flash_write(SPI_NOR_FLASH_0, (uint8_t *)datain, physaddr, len);
        if (rc != ERR_NONE) {
            printf("Nor flash write error %d\n", rc);
        }
    }
}
