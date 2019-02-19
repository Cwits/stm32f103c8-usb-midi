#ifndef __IRQ_H
#define __IRQ_H

#include "stm32f10x.h"

#define IRQ_USB_PRIORITY	8


void IRQ_Disable(void);
int32_t IRQ_Enable(void);

int32_t IRQ_Install(uint8_t IRQn, uint8_t priority);
void IRQ_DeInstall(uint8_t IRQn);

#endif 
