#include <xc.h>
#include <stdint.h>
#include "config.h"
#include "dht22.h"
#include "ow_slave.h"

#define _XTAL_FREQ 8000000UL

// global measurement values
int16_t temperature = 0; // 0.1C
uint16_t humidity = 0;   // 0.1%

static const uint8_t rom[8] = {
    OW_ROM_FAMILY,
    OW_ROM_SERIAL1,
    OW_ROM_SERIAL2,
    OW_ROM_SERIAL3,
    OW_ROM_SERIAL4,
    OW_ROM_SERIAL5,
    OW_ROM_SERIAL6,
    OW_ROM_CRC
};

static uint8_t page0[8];
static uint8_t page0_crc;

static uint8_t crc8(const uint8_t *data, uint8_t len) {
    uint8_t crc = 0;
    while(len--) {
        uint8_t inbyte = *data++;
        for(uint8_t i=0;i<8;i++) {
            uint8_t mix = (crc ^ inbyte) & 0x01;
            crc >>=1;
            if(mix) crc ^= 0x8C;
            inbyte >>=1;
        }
    }
    return crc;
}

static void update_page0(void) {
    page0[0] = temperature & 0xFF;
    page0[1] = (temperature >> 8) & 0xFF;
    page0[2] = humidity & 0xFF;
    page0[3] = (humidity >> 8) & 0xFF;
    page0[4] = page0[5] = page0[6] = page0[7] = 0;
    page0_crc = crc8(page0,8);
}

static void bus_release(void) { OW_TRIS = 1; }
static void bus_low(void) { OW_PIN = 0; OW_TRIS = 0; }
static uint8_t bus_read(void) { return OW_PIN; }

// wait for reset pulse and send presence
static uint8_t ow_wait_reset(void) {
    // wait for line low
    if(bus_read()) return 0; // bus idle high expected
    while(!bus_read()); // wait for rising edge
    __delay_us(15); // presence after 15us
    bus_low();
    __delay_us(240);
    bus_release();
    __delay_us(240);
    return 1;
}

static uint8_t read_bit(void) {
    while(bus_read()); // wait for master start
    __delay_us(15);
    uint8_t b = bus_read();
    while(!bus_read());
    return b;
}

static void write_bit(uint8_t b) {
    while(bus_read()); // wait for master read slot
    if(b) {
        __delay_us(6);
        bus_release();
        __delay_us(64);
    } else {
        bus_low();
        __delay_us(60);
        bus_release();
    }
    while(!bus_read());
}

static uint8_t read_byte(void) {
    uint8_t val=0;
    for(uint8_t i=0;i<8;i++) {
        if(read_bit()) val |= (1<<i);
    }
    return val;
}

static void write_byte(uint8_t val) {
    for(uint8_t i=0;i<8;i++) {
        write_bit(val & 0x01);
        val >>=1;
    }
}

static void send_page0(void) {
    for(uint8_t i=0;i<8;i++) write_byte(page0[i]);
    write_byte(page0_crc);
}

void ow_init(void) {
    bus_release();
    update_page0();
}

void ow_process(void) {
    if(!ow_wait_reset()) return;
    uint8_t cmd = read_byte();

    if(cmd == 0x33) { // READ ROM
        for(uint8_t i=0;i<8;i++) write_byte(rom[i]);
        return;
    } else if(cmd == 0xF0) { // SEARCH ROM (single device)
        for(uint8_t i=0;i<64;i++) {
            uint8_t b = (rom[i/8] >> (i%8)) & 1;
            write_bit(b);
            write_bit(!b);
            uint8_t master = read_bit();
            if(master != b) return; // not selected
        }
        return;
    } else if(cmd == 0x55) { // MATCH ROM
        for(uint8_t i=0;i<8;i++) {
            uint8_t b = read_byte();
            if(b != rom[i]) return;
        }
        cmd = read_byte();
    } else if(cmd == 0xCC) { // SKIP ROM
        cmd = read_byte();
    } else {
        return;
    }

    switch(cmd) {
        case 0x44: // Convert T
            dht22_read(&temperature,&humidity);
            update_page0();
            break;
        case 0xBE: // Read Scratchpad (page0)
            send_page0();
            break;
        default:
            break;
    }
}
