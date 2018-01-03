#include "sampa.h"
#include <string>
#include <unistd.h>
#include <iostream>

Sampa::Sampa(Fee *fee, int i2c_addr)
{
    this->fee = fee;
    this->i2c_address = i2c_addr;
}

int Sampa::begin_bitmask(uint32_t mask)
{
    int cnt = 0;
    while (mask > 0) {
        if ((mask & 0x1) == 1) {
            return cnt;
        }
        mask = mask >> 1;
        cnt++;
    }

    return -1;
}

int Sampa::find_register(char *key)
{
    for (unsigned idx = 0; idx < sizeof(this->raw_registers); idx++) {
        if (std::string(this->raw_registers[idx].name) == std::string(key))
            return idx;
    }

    return -1;
}

int Sampa::i2c_write(uint8_t addr, uint16_t data)
{
    fee->registerWrite(0x2, (data & 0xff));
    fee->registerWrite(0x3, (1 << 11) | (1 << 10) | ((this->i2c_address & 0xf) << 6) | (addr & 0x3f));

    // FIXME: Data strobe for now...
    fee->registerWrite(0x2, 0x0);
    fee->registerWrite(0x3, 0x0);

   return 0;
}

int Sampa::i2c_read(uint8_t addr, uint16_t *data)
{
   uint16_t dat;

   fee->registerWrite(0x3, (1 << 11) | ((this->i2c_address & 0xf) << 6) | (addr & 0x3f));

   // FIXME: check status register for ack/fail
   usleep(1000);

   fee->registerRead(0x4, &dat);
   fee->registerWrite(0x3, 0x0);

   *data = dat;
   return 0;
}

int Sampa::write(char *key, uint16_t value)
{
    int rc = 0;
    int idx = this->find_register(key);
    if (idx < 0) {
        return -1;
    }
    global_register_t *reg = &this->raw_registers[idx];
    uint8_t addr = reg->addr;
    uint8_t mask = reg->mask;

    uint16_t val;
    rc = this->i2c_read(addr, &val);
    val = (val & ~mask);
    val = (value << this->begin_bitmask(mask));
    rc = this->i2c_write(addr, val);

    return rc;
}

int Sampa::read(char *key, uint16_t *value)
{
    int rc = 0;
    int idx = this->find_register(key);
    if (idx < 0) {
        return -1;
    }
    global_register_t *reg = &this->raw_registers[idx];
    uint8_t addr = reg->addr;
    uint8_t mask = reg->mask;

    uint16_t val;
    rc = this->i2c_read(addr, &val);
    *value = (val & mask) >> this->begin_bitmask(mask);

    return rc;
}

int Sampa::getBxCount()
{
    uint16_t upper, mid, lower;
    this->read("BXCNTHL", &upper);
    this->read("BXCNTLH", &mid);
    this->read("BXCNTLL", &lower);

    this->bx_count = (upper << 8*2) | (mid << 8*1) | (lower);
    return this->bx_count;
}

int Sampa::getTrigCount()
{
    uint16_t upper, lower;
    this->read("TRCNTL", &lower);
    this->read("TRCNTH", &upper);

    this->trig_count = (upper << 8) | (lower);
    return this->trig_count;
}

std::ostream& operator<<(std::ostream& os, const Sampa& s)
{
  return os << "SAMPA@" << s.i2c_address;
}
