#ifndef __FEE_REG_H__
#define __FEE_REG_H__

#include <stdint.h>


int fee_reg_write(int fd, uint32_t addr, uint32_t data);
int fee_reg_read(int fd, uint32_t addr, uint16_t *data);

#endif
