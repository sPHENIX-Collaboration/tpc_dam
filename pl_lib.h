#ifndef PL_LIB_H
#define PL_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dam_ioctl.h"

#define DEVNAME "/dev/dam"
#define PAGE_SIZE 4096

int pl_open(int *fd);
int pl_close(int fd);
uint32_t pl_register_read(int fd, uint32_t addr);
int pl_register_write(int fd, uint32_t addr, uint32_t data);
int fee_register_write(int fd, uint32_t id, uint32_t addr, uint32_t data);
uint16_t fee_register_read(int fd, uint32_t id, uint32_t addr);
int pl_trigger_dma(int fd);
uint32_t pl_get_dma_status(int fd);
int pl_set_rate(int fd, uint32_t data);
int pl_set_buff_len(int fd, uint32_t len);
int pl_set_brust_len(int fd, uint32_t len);
int pl_set_debug_level(int fd, uint32_t level);
int dam_wait_for_dma(int fd, int *data);
int dam_enable_irq(int fd);
int dam_disable_irq(int fd);
int dam_reset_dma_engine(int fd);

#ifdef __cplusplus
}
#endif

#endif // PL_LIB_H
