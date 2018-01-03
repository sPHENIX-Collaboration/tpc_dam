#include "fee.h"

#include <iostream>

Fee::Fee(int id)
{
    this->id = id;
    pl_open(&this->fd);

    for (int i = 0; i < 2; i++) {
        sampa.push_back(new Sampa(this, i));
    }

    this->expected_length = 7*8*1010;
    this->sampa_data = new SampaData(this->fd, 7*8, 1010);

    pl_register_write(fd, 10, 0xcf400000);
    pl_register_write(fd, 0x8, 0xf);
    pl_register_write(fd, 0x8, 0x3);
}

Fee::~Fee()
{
    pl_close(this->fd);
}

void Fee::setupSampa()
{
    for (auto &s : this->sampa) {
        std::cout << "BX Count:" << s->getBxCount() << " Trig Count:" << s->getTrigCount() << std::endl;
    }

}

int Fee::registerWrite(uint32_t addr, uint16_t data)
{
    return fee_register_write(this->fd, addr, data);
}

int Fee::registerRead(uint32_t addr, uint16_t *data)
{
    *data = fee_register_read(this->fd, addr);
    return 0;
}

int Fee::dma_wait(uint32_t len, uint32_t seed, int chan_expected)
{
    pl_register_write(fd, 11,  len);
    uint32_t data = 0;
    while ((seed+chan_expected) != data) {
            data = pl_register_read(fd, 7);
    }

    data = pl_register_read(fd, 10);
    return (size_t)data;
}

int Fee::trigger()
{
    // Reset DMA controller
    pl_register_write(fd, 0x8, 0xb);
    pl_register_write(fd, 0x8, 0x7);
    pl_register_write(fd, 0x8, 0x3);

    // Get current channel counter
    uint32_t seed = pl_register_read(fd, 7);

    // FEE Trigger
    fee_register_write(fd, 0x5, 0x8);
    usleep(3000);
    fee_register_write(fd, 0x5, 0x0);
    usleep(3000);

    // Wait until 56 channels recv.
    this->data_length = dma_wait(this->expected_length, seed, 56);

    return this->data_length;
}

uint32_t *Fee::getRawData()
{
    return this->sampa_data->getRawData();
}

size_t Fee::getDataLength()
{
    return this->data_length;
}
