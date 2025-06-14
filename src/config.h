#ifndef CONFIG_H
#define CONFIG_H

// 1-Wire device ROM (family code + 6-byte serial + CRC)
// Default address: family 0x26 (DS2438) with example serial number
// Change these values to modify the address
#define OW_ROM_FAMILY 0x26
#define OW_ROM_SERIAL1 0x01
#define OW_ROM_SERIAL2 0x23
#define OW_ROM_SERIAL3 0x45
#define OW_ROM_SERIAL4 0x67
#define OW_ROM_SERIAL5 0x89
#define OW_ROM_SERIAL6 0xAB
#define OW_ROM_CRC     0xCD

// Pin assignments
#define OW_PIN   RA0
#define OW_TRIS  TRISA0
#define DHT_PIN  RA1
#define DHT_TRIS TRISA1

#endif // CONFIG_H
