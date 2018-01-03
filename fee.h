#ifndef FEE_H
#define FEE_H

#include "fee_global.h"
#include "pl_lib.h"
#include "sampa.h"
#include "sampadata.h"
#include <stdint.h>
#include <vector>

class Sampa;

class Fee
{

public:
    Fee(int id);
    ~Fee();
    void setupSampa();
    int registerWrite(uint32_t addr, uint16_t data);
    int registerRead(uint32_t addr, uint16_t *data);
    int trigger();
    uint32_t *getRawData();
    size_t getDataLength();

private:
    int fd;
    int address;
    int id;
    int expected_length;
    size_t data_length;

    SampaData *sampa_data;
    std::vector<Sampa *> sampa;
    int dma_wait(uint32_t len, uint32_t seed, int chan_expected);
};

#endif // FEE_H
