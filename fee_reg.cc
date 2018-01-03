
#include "pl_lib.h"
#include <unistd.h>

int fee_reg_write(int fd, uint32_t addr, uint32_t data)
{
    pl_register_write(fd, 0x6, addr);
    pl_register_write(fd, 0x5, data);
    pl_register_write(fd, 0x4, 0x1);
    usleep(10);

    return 0;
}

int fee_reg_read(int fd, uint32_t addr, uint16_t *data)
{
    pl_register_write(fd, 0x6, addr);
    pl_register_write(fd, 0x4, 0x2);
    usleep(10);
    *data = (uint16_t) pl_register_read(fd, 0x5);

    return 0;
}
