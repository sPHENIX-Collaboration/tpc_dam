#include "sampadata.h"
#include <stdio.h>
#include <string.h>

SampaData::SampaData(int damfd, int channels, int channel_length)
{
    this->expected_length = channels * channel_length;
    size_t size = (getpagesize() * 245 * 3);
    this->buffer = (uint32_t *) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, damfd, 0);
    if (this->buffer == MAP_FAILED) {
        perror("mmap");
    }

    this->data = new uint32_t**[2];
    for (int x = 0; x < 2; ++x) {
        this->data[x] = new uint32_t*[32];
        for (int y = 0; y < 32; ++y) {
            this->data[x][y] = new uint32_t[1010];
            for (int z = 0; z < 1010; ++z) {
                this->data[x][y][z] = 0;
            }
        }
    }
}

uint32_t *SampaData::getRawData()
{
    if (this->buffer == MAP_FAILED) {
        return NULL;
    }

    return this->buffer;
}

uint32_t ***SampaData::getFormattedData()
{
    this->channels_reported = 0;

    uint32_t marker_key = buffer[0];
    uint32_t payload_len = buffer[1] & 0xffff;
    this->bx_count = ((buffer[4] & 0xffff) << 4) | (buffer[5] & 0xf);
    for (unsigned i = 0; i < this->recv_length; i += (payload_len+1)) {
            payload_len = (buffer[i+1] & 0xffff);
            uint8_t  packet_type = buffer[i+2] & 0xffff;
            uint16_t channel     = buffer[i+3] & 0x1f;
            uint8_t  sampa_addr  = (buffer[i+3] >> 5) & 0xf;
            uint32_t bx_count    = ((buffer[i+4] & 0xffff) << 4) | (buffer[i+5] & 0xf);
            this->channels_reported++;

            // TODO: Figure out a better way to deal with this?
            if (bx_count != this->bx_count)
                return NULL;

            // FIXME: Check for packet type and length types against expected length
            if (channel >= 32 || sampa_addr >= 2)
                return NULL;

            memcpy(this->data[sampa_addr][channel], &buffer[i+HEADER_LENGTH], (payload_len-HEADER_LENGTH)*sizeof(uint32_t));
    }

    return this->data;
}

uint32_t SampaData::getBxCount()
{
   return this->bx_count;
}

