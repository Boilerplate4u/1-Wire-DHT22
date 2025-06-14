#ifndef OW_SLAVE_H
#define OW_SLAVE_H
#include <stdint.h>

void ow_init(void);
void ow_process(void);

extern int16_t temperature; // in 0.1 C
extern uint16_t humidity;   // in 0.1 %

#endif
