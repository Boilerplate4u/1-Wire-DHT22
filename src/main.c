#include <xc.h>
#include <stdint.h>
#include "config.h"
#include "ow_slave.h"

#define _XTAL_FREQ 8000000UL

#pragma config FOSC = INTOSC
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = OFF
#pragma config CP = OFF
#pragma config BOREN = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config WRT = OFF
#pragma config PLLEN = OFF
#pragma config STVREN = OFF
#pragma config BORV = HI
#pragma config LVP = OFF

void main(void) {
    OSCCON = 0x70; // 8MHz internal
    ANSELA = 0; // all digital
    TRISA0 = 1; // OW pin input
    TRISA1 = 1; // DHT22 input

    ow_init();
    while(1) {
        ow_process();
    }
}
