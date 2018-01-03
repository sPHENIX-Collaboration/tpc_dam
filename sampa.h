#ifndef SAMPA_H
#define SAMPA_H

#include "fee.h"
#include <stdint.h>
#include <iostream>

class Fee;

class Sampa
{
public:
    Sampa(Fee *fee, int i2c_addr);
    ~Sampa();
    int write(char *key, uint16_t value);
    int read(char *key, uint16_t *value);
    int channelWrite(char *key, uint16_t value);
    int channelRead(char *key, uint16_t *value);
    int getBxCount();
    int getTrigCount();
    void dumpRegisters();
    friend std::ostream& operator<<(std::ostream& os, const Sampa& s);

private:
    int find_register(char *key);
    int i2c_write(uint8_t addr, uint16_t data);
    int i2c_read(uint8_t addr, uint16_t *data);
    int begin_bitmask(uint32_t mask);

    int i2c_address;
    int bx_count;
    int trig_count;
    Fee *fee;

    typedef struct {
        uint8_t  chip_address;
        uint32_t trigger_count;
        uint32_t bx_count;
        uint8_t  pre_trig_delay;
        uint16_t time_window;
        uint16_t wait_before_acq_cycles;
        uint16_t trig_to_acq_cycles;
        bool     cont_mode;
        bool     raw_data_mode;
        bool     cluster_sum_mode;
        bool     huffman_codes;
        bool     gen_header_for_empty_chan;
        bool     power_save;
    } registers_t;

    typedef struct {
        const char *name;
        uint8_t addr;
        bool rw;
        bool gated;
        uint8_t data;
        uint8_t mask;
        const char *description;
    } global_register_t;

    global_register_t raw_registers[71] = {
        // Name,        Addr,  RW,   Gated, Data, Mask, Description
        {"HWADD",       0x00, false, true,  0x00, 15,  "Chip address (hardware address)"},
        {"TRCNTL",      0x01, false, false, 0x00, 255, "Trigger count, lower byte"},
        {"TRCNTH",      0x02, false, false, 0x00, 255, "Trigger count, upper byte"},
        {"BXCNTLL",     0x03, false, false, 0x00, 255, "Bunch crossing count, lower byte"},
        {"BXCNTLH",     0x04, false, false, 0x00, 255, "Bunch crossing count, mid byte"},
        {"BXCNTHL",     0x05, false, false, 0x00, 15,  "Bunch crossing count, upper byte"},
        {"PRETRG",      0x06, true,  false, 0x00, 255, "Number of pre-samples (Pre-trigger delay), max 192"},
        {"TWLENL",      0x07, true,  false, 0xE7, 255, "Number of cycles for time window +1, lower byte"},
        {"TWLENH",      0x08, true,  false, 0x03, 3,   "Number of cycles for time window +1, upper byte"},
        {"ACQSTARTL",   0x09, true,  false, 0x00, 255, "Number of cycles to wait before acquisition starts, lower byte"},
        {"ACQSTARTH",   0x0A, true,  false, 0x00, 3,   "Number of cycles to wait before acquisition starts, upper byte"},
        {"ACQENDL",     0x0B, true,  false, 0xFF, 255, "Number of cycles elapsed from trigger to acquisition end +1, lower byte"},
        {"ACQENDH",     0x0C, true,  false, 0x03, 3,   "Number of cycles elapsed from trigger to acquisition end +1, upper byte"},
        {"VACFG",       0x0D, true,  true,  0x31, 255, "Various configuration settings"},
        {"VACFG[1]",    0x0D, true,  false, 0x01, 1,   "Continuous mode enabled"},
        {"VACFG[2]",    0x0D, true,  false, 0x00, 2,   "Raw data enable"},
        {"VACFG[3]",    0x0D, true,  false, 0x00, 4,   "Cluster sum enable"},
        {"VACFG[4]",    0x0D, true,  false, 0x00, 8,   "Huffman enable"},
        {"VACFG[5]",    0x0D, true,  false, 0x01, 16,  "Enable header generation for empty channels"},
        {"VACFG[6]",    0x0D, true,  false, 0x01, 32,  "Power save enable"},
        {"VACFG[7]",    0x0D, true,  true,  0x00, 64,  "Enable automatic clock gating on I2C block"},
        {"VACFG[8]",    0x0D, true,  false, 0x00, 128, "Enable clock gating on neighbour block when number of neighbour is 0"},
        {"CMD",         0x0E, true,  true,  0x00, 7,   "Commands, see table 3.3"},
        {"NBCFG",       0x0F, true,  false, 0x40, 255, "Neighbor configuration settings"},
        {"NBCFG[1]",    0x0F, true,  false, 0x00, 63,  "Neighbor input delay, ca. 0.2 ns per bit for a total of ca. 12.5 ns"},
        {"NBCFG[2]",    0x0F, true,  false, 0x01, 192, "Number of neighbors"},
        {"ADCDEL",      0x10, true,  true,  0x00, 255, "ADC sampling clock delay"},
        {"ADCDEL[1]",   0x10, true,  true,  0x00, 63,  "ADC sampling clock delay, ca. 1.5 ns per bit for a total of ca. 94.5 ns"},
        {"ADCDEL[2]",   0x10, true,  true,  0x00, 64,  "Invert ADC sampling clock"},
        {"ADCTRIM",     0x11, true,  true,  0x04, 7,   "Voltage reference trimming"},
        {"SOCFG",       0x12, true,  true,  0x14, 255, "Serial link configuration"},
        {"SOCFG[1]",    0x12, true,  false, 0x04, 15,  "Number of serial out, 0-11"},
        {"SOCFG[2]",    0x12, true,  true,  0x01, 16,  "Disable internal termination of input differential links"},
        {"SODRVST",     0x13, true,  true,  0x55, 255, "Serial link drive strength configuration, see table 3.8"},
        {"SODRVST[1]",  0x13, true,  true,  0x01, 3,   "Drive strength of serial out 4-0"},
        {"SODRVST[2]",  0x13, true,  true,  0x01, 12,  "Drive strength of neighbor flow stop out/serial out 5"},
        {"SODRVST[3]",  0x13, true,  true,  0x01, 48,  "Drive strength of serial out 6,8,10"},
        {"SODRVST[4]",  0x13, true,  true,  0x01, 192, "Drive strength of serial out 7,9"},
        {"ERRORS",      0x14, false, false, 0x00, 255, "Errors accumulated"},
        {"ERRORS[1]",   0x14, false, false, 0x00, 31,  "Correctable header hamming errors"},
        {"ERRORS[2]",   0x14, false, false, 0x00, 224, "Uncorrectable header hamming errors"},
        {"PMADDL",      0x15, true,  false, 0x00, 255, "Pedestal memory address, lower byte"},
        {"PMADDH",      0x16, true,  false, 0x00, 3,   "Pedestal memory address, upper byte"},
        {"CHRGADD",     0x17, true,  false, 0x00, 31,  "Channel register address"},
        {"CHRGWDATL",   0x18, true,  false, 0x00, 255, "Channel register write data, lower byte"},
        {"CHRGWDATH",   0x19, true,  false, 0x00, 31,  "Channel register write data, upper byte"},
        {"CHRGCTL",     0x1A, true,  false, 0x00, 255, "Channel register control"},
        {"CHRGCTL[1]",  0x1A, true,  false, 0x00, 31,  "Channel number"},
        {"CHRGCTL[2]",  0x1A, true,  false, 0x00, 32,  "Broadcast to all channels (channel number ignored)"},
        {"CHRGCTL[3]",  0x1A, true,  false, 0x00, 64,  "Write, not read from register address (returns to read after write)"},
        {"CHRGCTL[4]",  0x1A, true,  false, 0x00, 128, "Increment PMADD (returns automatically to zero)"},
        {"CHRGRDATL",   0x1B, false, false, 0x00, 255, "Channel register read data, lower byte"},
        {"CHRGRDATH",   0x1C, false, false, 0x00, 31,  "Channel register read data, upper byte"},
        {"CHORDAT",     0x1D, true,  false, 0x00, 31,  "Channel readout order data"},
        {"CHORDCTL",    0x1E, true,  false, 0x00, 255, "Channel readout order control"},
        {"CHORDCTL[1]", 0x1E, true,  false, 0x00, 31,  "Position in order"},
        {"CHORDCTL[2]", 0x1E, true,  false, 0x00, 32,  "Write enable"},
        {"BYPASS",      0x1F, true,  true,  0x00, 15,  "Bypass inputs to serial 0, see table 3.13"},
        {"SERCHSEL",    0x20, true,  false, 0x00, 31,  "Channel select for ADC test serializer mode in bypass"},
        {"RINGCNT",     0x21, false, true,  0x00, 255, "Ring oscillator counter difference from reference ADC clock"},
        {"CLKCONF",     0x22, false, true,  0x00, 127, "Clock configuration pin status"},
        {"BOUNDARY",    0x23, false, true,  0x00, 255, "Status of differential input pins"},
        {"BOUNDARY[1]", 0x23, false, true,  0x00, 1,   "NBflowstop_in"},
        {"BOUNDARY[2]", 0x23, false, true,  0x00, 2,   "DinN"},
        {"BOUNDARY[3]", 0x23, false, true,  0x00, 4,   "hb_trg"},
        {"BOUNDARY[4]", 0x23, false, true,  0x00, 8,   "trg"},
        {"BOUNDARY[5]", 0x23, false, true,  0x00, 16,  "bx_sync_trg"},
        {"CHEN0",       0x24, true,  true,  0xFF, 255, "Channel enable 7-0"},
        {"CHEN1",       0x25, true,  true,  0xFF, 255, "Channel enable 15-8"},
        {"CHEN2",       0x26, true,  true,  0xFF, 255, "Channel enable 23-16"},
        {"CHEN3",       0x27, true,  true,  0xFF, 255, "Channel enable 31-24"}
    };
};

#endif // SAMPA_H
