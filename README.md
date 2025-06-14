# PIC12F1822 DHT22 to 1-Wire DS2438

This project converts a DHT22 temperature and humidity sensor to a Dallas 1‑Wire slave using a PIC12F1822 microcontroller. It emulates a DS2438 device so existing 1‑Wire software can read both temperature and humidity.

The implementation adapts the timing model from the open source [OneWireHub](https://github.com/orgua/OneWireHub) library.

## Changing the 1‑Wire address

Edit `src/config.h` and modify the `OW_ROM_*` defines. The family code should remain `0x26` for a DS2438 compatible device. Example:

```c
#define OW_ROM_SERIAL1 0x01
#define OW_ROM_SERIAL2 0x02
#define OW_ROM_SERIAL3 0x03
#define OW_ROM_SERIAL4 0x04
#define OW_ROM_SERIAL5 0x05
#define OW_ROM_SERIAL6 0x06
#define OW_ROM_CRC     0xA1
```

Recompile after changing these values.

## Building

1. Install MPLAB X and XC8 compiler from Microchip.
2. Create a new stand‑alone project for the PIC12F1822 and add all files from the `src` directory.
3. Set the processor frequency to 8 MHz and build the project. No extra libraries are required.

## Flashing

Use a PICkit or compatible programmer:

1. Connect Vdd, Vss, PGD (ICSPDAT), PGC (ICSPCLK) and MCLR to the programmer.
2. Program the generated `.hex` file using MPLAB X or the stand‑alone IPE tool.

## Testing

1. Connect the DHT22 data pin to RA1 of the PIC and a 10 kΩ pull‑up to Vdd.
2. Connect RA0 to the 1‑Wire bus with a pull‑up resistor (4.7 kΩ typical).
3. Power the PIC with 3.3 V or 5 V.
4. Use a 1‑Wire master (for example DS9490R with `owfs`) to detect the DS2438 device and read temperature and humidity.

## Home Assistant integration

1. Install the [OneWire](https://www.home-assistant.io/integrations/onewire/) integration and ensure the 1‑Wire bus is accessible (via OWFS or SysBus).
2. After adding the integration, Home Assistant will detect the DS2438 device automatically.
3. Two sensors will be created:
   - `temperature` mapped from DS2438 temperature register.
   - `humidity` mapped from the ADC register which this firmware stores as relative humidity.
4. Use these sensors in dashboards or automations as any other temperature and humidity readings.

