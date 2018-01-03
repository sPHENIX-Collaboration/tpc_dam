#ifndef SAMPADATA_H
#define SAMPADATA_H

#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>

#define HEADER_LENGTH 7

class SampaData
{
public:
    SampaData(int damfd, int channels, int channel_length);
    ~SampaData();
    uint32_t *getRawData();
    uint32_t ***getFormattedData();
    uint32_t getBxCount();

private:

    size_t recv_length;
    size_t expected_length;
    uint32_t *buffer;
    uint32_t ***data;
    uint32_t bx_count;
    uint32_t channels_reported;

    const char *PACKET_TYPE_STRING[8] = {"",
                                        "Truncated Data",
                                        "",
                                        "Data, Trigger Early, Data Truncated",
                                        "Normal Data",
                                        "Normal Data 1",
                                        "Data and Trigger Early 0",
                                        "Data and Trigger Early 1"};
};


#endif // SAMPADATA_H
