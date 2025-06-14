#include <xc.h>
#include "dht22.h"
#include "config.h"

#define _XTAL_FREQ 8000000UL

static void dht_delay_us(unsigned int us) {
    while(us--) __delay_us(1);
}

uint8_t dht22_read(int16_t *temp, uint16_t *hum) {
    uint8_t bits[5] = {0};
    uint8_t i, j;

    // send start signal
    DHT_TRIS = 0; // output
    DHT_PIN = 0;
    dht_delay_us(1200); // >1ms
    DHT_TRIS = 1; // input
    dht_delay_us(30);

    // wait for sensor response
    if (DHT_PIN) return 1;
    while(!DHT_PIN); // low 80us
    while(DHT_PIN);  // high 80us

    // read 40 bits
    for (j = 0; j < 5; j++) {
        for(i = 0; i < 8; i++) {
            while(!DHT_PIN); // start of bit (50us low)
            dht_delay_us(30);
            bits[j] <<= 1;
            if (DHT_PIN) bits[j] |= 1; // if high after 30us -> 1
            while(DHT_PIN); // wait end
        }
    }

    uint8_t chk = bits[0] + bits[1] + bits[2] + bits[3];
    if (chk != bits[4]) return 2;

    *hum  = ((uint16_t)bits[0] << 8) | bits[1];
    *temp = ((int16_t)bits[2] << 8) | bits[3];
    return 0;
}
