#ifndef DHT22_H
#define DHT22_H
#include <stdint.h>

// Reads temperature (0.1 C units) and humidity (0.1 % units)
// Returns 0 on success
uint8_t dht22_read(int16_t *temp, uint16_t *hum);

#endif
